//  [7/15/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelMath.h"
#include "DuelVector.h"
#include "DuelRay.h"
#include "DuelSphere.h"
#include "DuelPlane.h"
#include "DuelBoundingBox.h"
#include "DuelMatrix.h"


namespace Duel {


	const DReal DMath::REAL_POS_INFINITY = std::numeric_limits<DReal>::infinity();
    const DReal DMath::REAL_NEG_INFINITY = -std::numeric_limits<DReal>::infinity();
    const DReal DMath::PI = DReal( 4.0 * atan( 1.0 ) );
    const DReal DMath::TWO_PI = DReal( 2.0 * PI );
    const DReal DMath::HALF_PI = DReal( 0.5 * PI );
	const DReal DMath::LOG2 = log(DReal(2.0));

	uint32	DMath::mTableSize = 0;
	DReal	*DMath::mSinTable = NULL;
	DReal	*DMath::mTanTable = NULL;


	DMath::DMath(uint32 tableSize)
	{
		// get cpu count since OS' starting. very excellent for setting seed.
		srand((unsigned int)time(NULL));
		if(tableSize != 0)
		{
			initTriangleTable(tableSize);
		}
		else
		{
			initTriangleTable(4096);
		}
	}


	DRadian	DMath::arcSin(DReal f)
	{
		if ( -1.0 < f )
        {
            if ( f < 1.0 )
                return DRadian(asin(f));
            else
                return DRadian(0.0);
        }
        else
        {
            return DRadian(PI);
        }
	}
	DRadian	DMath::arcCos(DReal f)
	{
		if ( -1.0 < f )
        {
            if ( f < 1.0 )
                return DRadian(acos(f));
            else
                return DRadian(HALF_PI);
        }
        else
        {
            return DRadian(-HALF_PI);
        }
	}
	DRadian	DMath::arcTan(DReal f)
	{
		return DRadian(atan(f));
	}
	DRadian	DMath::arcTan2(DReal y, DReal x)
	{
		return DRadian(atan2(y,x));
	}



#pragma region "Triangle function overload"
	
	//////////////////////////////////////////
	DReal DMath::Sin(DReal f, bool useTable)
	{
		return (!useTable) ?  sin(f) : getTabledSin(f); 
	}

	DReal DMath::Sin(const DDegree& rD, bool useTable)
	{
		return (!useTable) ?  sin(rD.getRadian()) : getTabledSin(rD.getRadian()); 
	}

	DReal DMath::Sin(const DRadian& rR, bool useTable)
	{
		return (!useTable) ?  sin(rR.getRadian()) : getTabledSin(rR.getRadian());
	}

	//////////////////////////////////////////
	DReal DMath::Cos(DReal f, bool useTable)
	{
		return (!useTable) ?  cos(f) : getTabledCos(f);
	}

	DReal DMath::Cos(const DDegree& rD, bool useTable)
	{
		return (!useTable) ?  cos(rD.getRadian()) : getTabledCos(rD.getRadian());
	}

	DReal DMath::Cos(const DRadian& rR, bool useTable)
	{
		return (!useTable) ?  cos(rR.getRadian()) : getTabledCos(rR.getRadian());
	}

	//////////////////////////////////////////
	DReal DMath::Tan(DReal f, bool useTable)
	{
		return (!useTable) ?  tan(f) : getTabledTan(f);
	}

	DReal DMath::Tan(const DDegree& rD, bool useTable)
	{
		return (!useTable) ?  tan(rD.getRadian()) : getTabledTan(rD.getRadian());
	}

	DReal DMath::Tan(const DRadian& rR, bool useTable)
	{
		return (!useTable) ?  tan(rR.getRadian()) : getTabledTan(rR.getRadian());
	}
	//////////////////////////////////////////

#pragma endregion


	void DMath::initTriangleTable(uint32 size)
	{
		if(!mSinTable)
			delete[] mSinTable;

		mSinTable = new DReal[size];
		mTanTable = new DReal[size];
		mTableSize = size;

		DReal factor = DMath::TWO_PI / size;

		for(uint32 i = 0; i < size; ++i )
		{
			mTanTable[i] = tan( i*factor );
			mSinTable[i] = sin( i*factor );
		}
	}

	DReal DMath::getTabledSin(DReal radian)
	{
		int i;
		if(radian > 0)
		{
			i = int(radian * mTableSize / DMath::TWO_PI) % mTableSize;
		}
		else
		{
			i = mTableSize - (int(-radian * (mTableSize / DMath::TWO_PI)) % mTableSize) -1;
		}
		return mSinTable[i];
	}

	DReal DMath::getTabledCos(DReal radian)
	{
		return getTabledSin(radian + DMath::HALF_PI);
	}

	DReal DMath::getTabledTan(DReal radian)
	{
		int i = int(radian * (mTableSize / DMath::TWO_PI)) % mTableSize;
		return mTanTable[i];
	}

	DReal DMath::RadianToDegree( DReal r )
	{
		return r / DMath::PI * 180;
	}

	DReal DMath::DegreeToRadian( DReal r )
	{
		return r / 180 * DMath::PI;
	}

	Duel::DDegree DMath::Abs( const DDegree& rD )
	{
		return DDegree(abs(rD.getDegree()));
	}

	Duel::DRadian DMath::Abs( const DRadian& rR )
	{
		return DRadian(abs(rR.getRadian()));
	}

	Duel::DMatrix4 DMath::makeViewMatrix( const DVector3& position, const DQuaternion& orientation, const DMatrix4* reflectMatrix /*= 0*/ )
	{
		// View matrix is:
		//
		//  [ Rx  Uy  Dz  0  ]
		//  [ Rx  Uy  Dz  0  ]
		//  [ Rx  Uy  Dz  0  ]
		//  [ Tx  Ty  Tz  1  ]
		//
		// Where T = -(Transposed(Rot) * Pos)

		// This is most efficiently done using 3x3 Matrices
// 		Matrix3 rot;
// 		orientation.toRotationMatrix(rot);

		DVector3 R, U, D;
		DMatrix3 mat;
		// get Conjutate means we need a rotation on Inertial space, not on object.
		orientation.getConjugate().toRotationMatrix(mat);
		R.x = mat[0][0]; R.y = mat[1][0]; R.z = mat[2][0];
		U.x = mat[0][1]; U.y = mat[1][1]; U.z = mat[2][1];
		D.x = mat[0][2]; D.y = mat[1][2]; D.z = mat[2][2];

		// Make final matrix
		DMatrix4 viewMatrix(mat);
		
		viewMatrix[3][0] = -R.dotProduct(position);
		viewMatrix[3][1] = -U.dotProduct(position);
		viewMatrix[3][2] = -D.dotProduct(position);

		// Deal with reflections
		if (reflectMatrix)
		{
			viewMatrix = viewMatrix * (*reflectMatrix);
		}

		return viewMatrix;
	}

	DVector3 DMath::getNormal( const DVector3& a, const DVector3& b, const DVector3& c )
	{
		return (b - a).crossProduct(c - a);
	}


	bool DMath::intersect( const DRay& ray, const DPlane& p, DReal* dist )
	{
		DReal denom = p.normal.dotProduct(ray.getDirection());
		DReal distance = 0;
		bool hit = false;
		// parallel detecting
		if (!realEqual(denom, 0.0f))
		{
			DReal nom = p.normal.dotProduct(ray.getOrigin()) + p.d;
			distance = -(nom/denom);
			hit = distance >= 0? true : false;
		}
		if (dist != NULL)
		{
			*dist = distance;
		}
		return hit;
	}

	bool DMath::intersect( const DRay& ray, const DSphere& s, DReal* dist1, DReal* dist2 )
	{
		DReal d1 = 0;
		DReal d2 = 0;
		bool hit = false;
		const DVector3& raydir = ray.getDirection();
		// Adjust ray origin relative to sphere center
		const DVector3& rayorig = ray.getOrigin() - s.getCenter();
		DReal radius = s.getRadius();

		// Build coeffs which can be used with std quadratic solver
		// ie t = (-b +/- sqrt(b*b + 4ac)) / 2a
		DReal a = raydir.dotProduct(raydir);
		DReal b = 2 * rayorig.dotProduct(raydir);
		DReal c = rayorig.dotProduct(rayorig) - radius*radius;

		// Calc determinant
		DReal d = (b*b) - (4 * a * c);
		if (d >=0)
		{
			// BTW, if d=0 there is one intersection, if d > 0 there are 2
			DReal sqrtD = DMath::Sqrt(d);
			d1 = ( -b - sqrtD ) / (2 * a);
			d2 = ( -b + sqrtD ) / (2 * a);
			// if the nearest distance is a negative value, then
			// the point is in the sphere.
			if (d1 < d2)
			{
				std::swap(d1, d2);
			}
			hit = true;
		}

		if (dist1 != NULL)
		{
			*dist1 = d1;
		}
		if (dist2 != NULL)
		{
			*dist2 = d2;
		}
		return hit;
	}

	bool DMath::intersect( const DRay& ray, const DAxisAlignedBox& b, DReal* dist)
	{
		// the distance along axis
		DReal t; 
		// nearest hit point's distance
		DReal lowt = 0;
		DVector3 hitpoint;
		const DVector3& min = b.getMinimum();
		const DVector3& max = b.getMaximum();
		const DVector3& rayorig = ray.getOrigin();
		const DVector3& raydir = ray.getDirection();		

		bool hit = false;

		if (b.isInfinite())
		{
			hit = true;
		}
		else if (b.isFinite())
		{

			// check inside.
			if(rayorig > min && rayorig < max)
			{
				*dist = lowt;
				return true;
			}
			// Min x
			if (rayorig.x <= min.x && raydir.x > 0)
			{
				t = (min.x - rayorig.x) / raydir.x;
				if (t >= 0)
				{
					// Substitute t back into ray and check bounds and dist
					hitpoint = rayorig + raydir * t;
					if (hitpoint.y >= min.y && hitpoint.y <= max.y &&
						hitpoint.z >= min.z && hitpoint.z <= max.z &&
						(!hit || t < lowt))
					{
						hit = true;
						lowt = t;
					}
				}
			}
			// Max x
			if (rayorig.x >= max.x && raydir.x < 0)
			{
				t = (max.x - rayorig.x) / raydir.x;
				if (t >= 0)
				{
					// Substitute t back into ray and check bounds and dist
					hitpoint = rayorig + raydir * t;
					if (hitpoint.y >= min.y && hitpoint.y <= max.y &&
						hitpoint.z >= min.z && hitpoint.z <= max.z &&
						(!hit || t < lowt))
					{
						hit = true;
						lowt = t;
					}
				}
			
			}
			// Min y
			if (rayorig.y <= min.y && raydir.y > 0)
			{
				t = (min.y - rayorig.y) / raydir.y;
				if (t >= 0)
				{
					// Substitute t back into ray and check bounds and dist
					hitpoint = rayorig + raydir * t;
					if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
						hitpoint.z >= min.z && hitpoint.z <= max.z &&
						(!hit || t < lowt))
					{
						hit = true;
						lowt = t;
					}
				}
			}
			// Max y
			if (rayorig.y >= max.y && raydir.y < 0)
			{
				t = (max.y - rayorig.y) / raydir.y;
				if (t >= 0)
				{
					// Substitute t back into ray and check bounds and dist
					hitpoint = rayorig + raydir * t;
					if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
						hitpoint.z >= min.z && hitpoint.z <= max.z &&
						(!hit || t < lowt))
					{
						hit = true;
						lowt = t;
					}
				}
			}
			// Min z
			if (rayorig.z <= min.z && raydir.z > 0)
			{
				t = (min.z - rayorig.z) / raydir.z;
				if (t >= 0)
				{
					// Substitute t back into ray and check bounds and dist
					hitpoint = rayorig + raydir * t;
					if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
						hitpoint.y >= min.y && hitpoint.y <= max.y &&
						(!hit || t < lowt))
					{
						hit = true;
						lowt = t;
					}
				}
			}
			// Max z
			if (rayorig.z >= max.z && raydir.z < 0)
			{
				t = (max.z - rayorig.z) / raydir.z;
				if (t >= 0)
				{
					// Substitute t back into ray and check bounds and dist
					hitpoint = rayorig + raydir * t;
					if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
						hitpoint.y >= min.y && hitpoint.y <= max.y &&
						(!hit || t < lowt))
					{
						hit = true;
						lowt = t;
					}
				}
			}
		}
		// else NULL
		if (dist != NULL)
		{
			*dist = lowt;
		}
		return hit;
	}

	bool DMath::intersect( const DRay& ray, DOrientedBox* ob, DReal* dist )
	{
		// the distance along axis
		DReal t; 
		// nearest hit point's distance
		DReal lowt = 0;
		DVector3 hitpoint;
		const DAxisAlignedBox& b = ob->getBoundingBox();
		// create a ray model in the bounding box's space.
		DMatrix4 bLToW = ob->getLocalTransform();
		const DVector3& min = b.getMinimum();
		const DVector3& max = b.getMaximum();
		
		DVector3 localTarget = ray.getPoint(1.0f);
		// build local ray in the box's space.
		DRay localRay(bLToW * ray.getOrigin(), DVector3::UNIT_Z);
		localTarget = bLToW * localTarget;
		localRay.pointTo(localTarget);

		DVector3 raydir = localRay.getDirection();
		DVector3 rayorig = localRay.getOrigin();		

		bool hit = false;

		if (b.isInfinite())
		{
			hit = true;
		}
		else if (b.isFinite())
		{

			// check inside.
			if(rayorig > min && rayorig < max)
			{
				*dist = lowt;
				return true;
			}
			// Min x
			if (rayorig.x <= min.x && raydir.x > 0)
			{
				t = (min.x - rayorig.x) / raydir.x;
				if (t >= 0)
				{
					// Substitute t back into ray and check bounds and dist
					hitpoint = rayorig + raydir * t;
					if (hitpoint.y >= min.y && hitpoint.y <= max.y &&
						hitpoint.z >= min.z && hitpoint.z <= max.z &&
						(!hit || t < lowt))
					{
						hit = true;
						lowt = t;
					}
				}
			}
			// Max x
			if (rayorig.x >= max.x && raydir.x < 0)
			{
				t = (max.x - rayorig.x) / raydir.x;
				if (t >= 0)
				{
					// Substitute t back into ray and check bounds and dist
					hitpoint = rayorig + raydir * t;
					if (hitpoint.y >= min.y && hitpoint.y <= max.y &&
						hitpoint.z >= min.z && hitpoint.z <= max.z &&
						(!hit || t < lowt))
					{
						hit = true;
						lowt = t;
					}
				}

			}
			// Min y
			if (rayorig.y <= min.y && raydir.y > 0)
			{
				t = (min.y - rayorig.y) / raydir.y;
				if (t >= 0)
				{
					// Substitute t back into ray and check bounds and dist
					hitpoint = rayorig + raydir * t;
					if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
						hitpoint.z >= min.z && hitpoint.z <= max.z &&
						(!hit || t < lowt))
					{
						hit = true;
						lowt = t;
					}
				}
			}
			// Max y
			if (rayorig.y >= max.y && raydir.y < 0)
			{
				t = (max.y - rayorig.y) / raydir.y;
				if (t >= 0)
				{
					// Substitute t back into ray and check bounds and dist
					hitpoint = rayorig + raydir * t;
					if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
						hitpoint.z >= min.z && hitpoint.z <= max.z &&
						(!hit || t < lowt))
					{
						hit = true;
						lowt = t;
					}
				}
			}
			// Min z
			if (rayorig.z <= min.z && raydir.z > 0)
			{
				t = (min.z - rayorig.z) / raydir.z;
				if (t >= 0)
				{
					// Substitute t back into ray and check bounds and dist
					hitpoint = rayorig + raydir * t;
					if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
						hitpoint.y >= min.y && hitpoint.y <= max.y &&
						(!hit || t < lowt))
					{
						hit = true;
						lowt = t;
					}
				}
			}
			// Max z
			if (rayorig.z >= max.z && raydir.z < 0)
			{
				t = (max.z - rayorig.z) / raydir.z;
				if (t >= 0)
				{
					// Substitute t back into ray and check bounds and dist
					hitpoint = rayorig + raydir * t;
					if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
						hitpoint.y >= min.y && hitpoint.y <= max.y &&
						(!hit || t < lowt))
					{
						hit = true;
						lowt = t;
					}
				}
			}
		}
		// else NULL
		if (dist != NULL)
		{
			*dist = lowt;
		}
		return hit;
	}

	bool DMath::intersect( const DOrientedBox& a, const DOrientedBox& b )
	{
		DOrientedBox abox = a;
		DOrientedBox bbox = b;

		DMatrix4 bLocalT = bbox.getLocalTransform();
		DAxisAlignedBox bAAB = bbox.getBoundingBox();
		bAAB.translate(bbox.getOrigin());
		for (uint32 i = 0; i < 8; ++i)
		{
			DVector3 p = abox.getCorner((BoundingBoxCorner)i);
			if (bAAB.contains(p))
			{
				return true;
			}
		}
		return false;
	}

	bool DMath::intersect( const DRay& ray, const DVector3& a, const DVector3& b, const DVector3& c, DReal* dist )
	{
		DReal distance = 0;
		bool hit = false;
		// algorithm: Geometric Tools for Computer Graphics. Philip J.Schneider & David H.Eberly
		DVector3 e1 = b-a;
		DVector3 e2 = c-a;
		DVector3 p = ray.getDirection().crossProduct(e2);
		DReal tmp = p.dotProduct(e1);

		// tolerance 
		DReal tolerance = 1e-6f;
		if (DMath::Abs(tmp) < tolerance)
		{
			*dist = distance;
			return hit;
		}
		tmp = 1/tmp;
		DVector3 s = ray.getOrigin() - a;
		DReal u = tmp * s.dotProduct(p);
		if (u < 0.0f || u > 1.0f)
		{
			*dist = distance;
			return hit;
		}
		DVector3 q = s.crossProduct(e1);
		DReal v = tmp * s.dotProduct(q);
		if (v < 0.0f || v > 1.0f)
		{
			*dist = distance;
			return hit;
		}
		hit = true;
		distance = e2.dotProduct(q);
		if (dist != NULL)
		{
			*dist = distance;
		}
		return hit;
	}

	bool DMath::intersect( const DSphere& s, const DAxisAlignedBox& b )
	{
		// the distance along axis
		bool hit = false;
		const DVector3& sCent = s.getCenter();
		const DVector3& min = b.getMinimum();
		const DVector3& max = b.getMaximum();
		DReal sqrDist = 0;

		if (b.isInfinite())
		{
			hit = true;
		}
		else if (b.isFinite())
		{

			// algorithm: Geometric Tools for Computer Graphics. Philip J.Schneider & David H.Eberly
			// x
			if (sCent.x < min.x)
			{
				sqrDist += DMath::Sqr(sCent.x - min.x);
			}
			else if (sCent.x > max.x)
			{
				sqrDist += DMath::Sqr(sCent.x - max.x);
			}
			// y 
			if (sCent.y < min.y)
			{
				sqrDist += DMath::Sqr(sCent.y - min.y);
			}
			else if (sCent.y > max.y)
			{
				sqrDist += DMath::Sqr(sCent.y - max.y);
			}
			// z 
			if (sCent.z < min.z)
			{
				sqrDist += DMath::Sqr(sCent.z - min.z);
			}
			else if (sCent.z > max.z)
			{
				sqrDist += DMath::Sqr(sCent.z - max.z);
			}

			if (sqrDist <= DMath::Sqr(s.getRadius()))
			{
				hit = true;
			}
		}
		// else NULL
		return hit;
	}

	bool DMath::intersect( const DSphere& s, const DPlane& p )
	{
		return s.getRadius() >= DMath::Abs(p.getDistanceTo(s.getCenter()));
	}

	bool DMath::intersect( const DPlane& p, const DAxisAlignedBox& b )
	{
		// the distance along axis
		bool hit = false;

		if (b.isInfinite())
		{
			hit = true;
		}
		else if (b.isFinite())
		{
			DVector3& halfSize = (b.getMaximum()-b.getMinimum()) * 0.5f;
			DReal maxDist = DMath::Abs(p.normal.dotProduct(halfSize));
			if(p.getDistanceTo(b.getCenter()) <= maxDist)
			{
				hit = true;
			}
		}
		// else NULL
		return hit;
	}

}