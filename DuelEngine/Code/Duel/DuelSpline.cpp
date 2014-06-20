//  [11/4/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelSpline.h"

namespace Duel
{

	const DMatrix4 DSpline::Coefficient = DMatrix4( 2.0f, -2.0f,  1.0f,  1.0f,
												-3.0f,  3.0f, -2.0f, -1.0f,
												 0.0f,  0.0f,  1.0f,  0.0f,
												 1.0f,  0.0f,  0.0f,  0.0f);



	DSpline::DSpline() :
		mbAutoUpdate(true)
	{
		
	}

	DSpline::~DSpline()
	{

	}

	void DSpline::addPoint( const DVector3& p )
	{
		mPoints.push_back(p);
		mbDirty = true;
		if (mbAutoUpdate)
		{
			update();
		}
	}

	const DVector3& DSpline::getPoint( size_t index )
	{
		assert(index < mPoints.size());
		return	mPoints[index];
	}

	size_t DSpline::getPointCount()
	{
		return mPoints.size();
	}

	void DSpline::removePoint( size_t index )
	{
		if (index < mPoints.size())
		{
			std::vector<DVector3>::iterator iter = mPoints.begin(); 
			for (uint32 i = 0; i < index; ++iter)
			{
			}
			mPoints.erase(iter);
			mbDirty = true;
			if (mbAutoUpdate)
			{
				update();
			}
		}

	}

	void DSpline::clear()
	{
		mPoints.clear();
		mTangents.clear();
	}

	void DSpline::setPoint( size_t index, const DVector3& p )
	{
		if (index < mPoints.size())
		{
			std::vector<DVector3>::iterator iter = mPoints.begin(); 
			for (uint32 i = 0; i < index; ++iter)
			{
			}
			(*iter) = p;
			mbDirty = true;
			if (mbAutoUpdate)
			{
				update();
			}
		}
	}

	DVector3 DSpline::interpolate( DReal t ) const
	{
		// Currently assumes points are evenly spaced, will cause velocity
		// change where this is not the case
		// TODO: base on arclength?

		// Work out which segment this is in
		DReal fSeg = t * (mPoints.size() - 1);
		unsigned int segIdx = (unsigned int)fSeg;
		// Apportion t 
		t = fSeg - segIdx;

		return interpolateFrom(segIdx, t);
	}

	DVector3 DSpline::interpolateFrom( size_t index, DReal t ) const
	{
		// Bounds check
		assert(index < mPoints.size() &&
			"index out of bounds");

		if ((index + 1) == mPoints.size())
		{
			// Duff request, cannot blend to nothing
			// Just return source
			return mPoints[index];

		}

		// Fast special cases
		if (t == 0.0f)
		{
			return mPoints[index];
		}
		else if(t == 1.0f)
		{
			return mPoints[index + 1];
		}

		// DReal interpolation
		// Form a vector of powers of t
		DReal t2, t3;
		t2 = t * t;
		t3 = t2 * t;
		DVector4 powers(t3, t2, t, 1);


		// Algorithm is ret = powers * mCoeffs * DMatrix4(point1, point2, tangent1, tangent2)
		const DVector3& point1 = mPoints[index];
		const DVector3& point2 = mPoints[index+1];
		const DVector3& tan1 = mTangents[index];
		const DVector3& tan2 = mTangents[index+1];
		DMatrix4 pt;

		pt[0][0] = point1.x;
		pt[0][1] = point1.y;
		pt[0][2] = point1.z;
		pt[0][3] = 1.0f;
		pt[1][0] = point2.x;
		pt[1][1] = point2.y;
		pt[1][2] = point2.z;
		pt[1][3] = 1.0f;
		pt[2][0] = tan1.x;
		pt[2][1] = tan1.y;
		pt[2][2] = tan1.z;
		pt[2][3] = 1.0f;
		pt[3][0] = tan2.x;
		pt[3][1] = tan2.y;
		pt[3][2] = tan2.z;
		pt[3][3] = 1.0f;

		DVector4 ret =  pt * (Coefficient * powers); 


		return DVector3(ret.x, ret.y, ret.z);

	}

	void DSpline::setAutoUpdate( bool flag )
	{
		mbAutoUpdate = flag;
	}

	void DSpline::update()
	{
		if (!mbDirty)
		{
			return;
		}
		// Catmull-Rom approach
		// 
		// tangent[i] = 0.5 * (point[i+1] - point[i-1])
		//
		// Assume endpoint tangents are parallel with line with neighbour

		size_t i, numPoints;
		bool isClosed;

		numPoints = mPoints.size();
		if (numPoints < 2)
		{
			// Can't do anything yet
			return;
		}

		// Closed or open?
		if (mPoints[0] == mPoints[numPoints-1])
		{
			isClosed = true;
		}
		else
		{
			isClosed = false;
		}

		mTangents.resize(numPoints);



		for(i = 0; i < numPoints; ++i)
		{
			if (i ==0)
			{
				// Special case start
				if (isClosed)
				{
					// Use numPoints-2 since numPoints-1 is the last point and == [0]
					mTangents[i] = 0.5 * (mPoints[1] - mPoints[numPoints-2]);
				}
				else
				{
					mTangents[i] = 0.5 * (mPoints[1] - mPoints[0]);
				}
			}
			else if (i == numPoints-1)
			{
				// Special case end
				if (isClosed)
				{
					// Use same tangent as already calculated for [0]
					mTangents[i] = mTangents[0];
				}
				else
				{
					mTangents[i] = 0.5 * (mPoints[i] - mPoints[i-1]);
				}
			}
			else
			{
				mTangents[i] = 0.5 * (mPoints[i+1] - mPoints[i-1]);
			}

		}
		mbDirty = false;
	}


	RotationSpline::RotationSpline() :
		mbAutoUpdate(true)
	{

	}

	RotationSpline::~RotationSpline()
	{

	}

	void RotationSpline::addPoint( const DQuaternion& p )
	{
		mPoints.push_back(p);
		mbDirty = true;
		if (mbAutoUpdate)
		{
			update();
		}
	}

	const DQuaternion& RotationSpline::getPoint( uint16 index ) const
	{
		assert(index < mPoints.size());
		return mPoints[index];
	}

	uint16 RotationSpline::getPointCount() const
	{
		return mPoints.size();
	}

	void RotationSpline::removePoint( size_t index )
	{
		if (index < mPoints.size())
		{
			std::vector<DQuaternion>::iterator iter = mPoints.begin(); 
			for (uint32 i = 0; i < index; ++iter)
			{
			}
			mPoints.erase(iter);
			mbDirty = true;
			if (mbAutoUpdate)
			{
				update();
			}
		}
	}

	void RotationSpline::clear()
	{
		mPoints.clear();
		mTangents.clear();
	}

	void RotationSpline::setPoint( size_t index, const DQuaternion& p )
	{
		if (index < mPoints.size())
		{
			std::vector<DQuaternion>::iterator iter = mPoints.begin(); 
			for (uint32 i = 0; i < index; ++iter)
			{
			}
			(*iter) = p;
			mbDirty = true;
			if (mbAutoUpdate)
			{
				update();
			}
		}
	}

	DQuaternion RotationSpline::interpolate( DReal t, bool nearest /*= true*/ ) const
	{
		// Work out which segment this is in
		DReal fSeg = t * (mPoints.size() - 1);
		unsigned int segIdx = (unsigned int)fSeg;
		// Apportion t 
		t = fSeg - segIdx;

		return interpolateFrom(segIdx, t, nearest);

	}

	DQuaternion RotationSpline::interpolateFrom( size_t index, DReal t, bool nearest /*= true*/ ) const
	{
		// Bounds check
		assert (index >= 0 && index < mPoints.size() &&
			"index out of bounds");

		if ((index + 1) == mPoints.size())
		{
			// Duff request, cannot blend to nothing
			// Just return source
			return mPoints[index];

		}
		// Fast special cases
		if (t == 0.0f)
		{
			return mPoints[index];
		}
		else if(t == 1.0f)
		{
			return mPoints[index + 1];
		}

		// DReal interpolation
		// Use squad using tangents we've already set up
		const DQuaternion &p = mPoints[index];
		const DQuaternion &q = mPoints[index+1];
		const DQuaternion &a = mTangents[index];
		const DQuaternion &b = mTangents[index+1];

		// NB interpolate to nearest rotation
		return DQuaternion::Squad(t, p, a, b, q, nearest);

	}

	void RotationSpline::setAutoUpdate( bool flag )
	{
		mbAutoUpdate = flag;
	}

	void RotationSpline::update()
	{
		if (!mbDirty)
		{
			return;
		}
		// ShoeMake (1987) approach
		// Just like Catmull-Rom really, just more gnarly
		// And no, I don't understand how to derive this!
		//
		// let p = point[i], pInv = p.Inverse
		// tangent[i] = p * exp( -0.25 * ( log(pInv * point[i+1]) + log(pInv * point[i-1]) ) )
		//
		// Assume endpoint tangents are parallel with line with neighbour

		unsigned int i, numPoints;
		bool isClosed;

		numPoints = (unsigned int)mPoints.size();

		if (numPoints < 2)
		{
			// Can't do anything yet
			return;
		}

		mTangents.resize(numPoints);

		if (mPoints[0] == mPoints[numPoints-1])
		{
			isClosed = true;
		}
		else
		{
			isClosed = false;
		}

		DQuaternion invp, part1, part2, preExp;
		for(i = 0; i < numPoints; ++i)
		{
			DQuaternion &p = mPoints[i];
			invp = p.getConjugate();

			if (i ==0)
			{
				// special case start
				part1 = (invp * mPoints[i+1]).log();
				if (isClosed)
				{
					// Use numPoints-2 since numPoints-1 == end == start == this one
					part2 = (invp * mPoints[numPoints-2]).log();
				}
				else
				{
					part2 = (invp * p).log();
				}
			}
			else if (i == numPoints-1)
			{
				// special case end
				if (isClosed)
				{
					// Wrap to [1] (not [0], this is the same as end == this one)
					part1 = (invp * mPoints[1]).log();
				}
				else
				{
					part1 = (invp * p).log();
				}
				part2 = (invp * mPoints[i-1]).log();
			}
			else
			{
				part1 = (invp * mPoints[i+1]).log();
				part2 = (invp * mPoints[i-1]).log();
			}

			preExp = -0.25 * (part1 + part2);
			mTangents[i] = p * preExp.exp();

		}

		mbDirty = false;
	}

}