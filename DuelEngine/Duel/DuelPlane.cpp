//  [7/17/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelPlane.h"
#include "DuelBoundingBox.h"
#include "DuelSphere.h"

namespace Duel
{
	DPlane::DPlane()
	{
		normal.x = 0;
		normal.y = 0;
		normal.z = 0;
		d = 0;
	}

	DPlane::DPlane( const DPlane& p )
	{
		normal = p.normal;
		d = p.d;
	}

	DPlane::DPlane( DReal n0, DReal n1, DReal n2, DReal c )
	{
		normal.x = n0;
		normal.y = n1;
		normal.z = n2;
		d = c;
	}

	DPlane::DPlane( const DVector3& n, DReal c )
	{
		normal = n;
		d = c;
	}

	DPlane::DPlane( const DVector3& n, const DVector3& p )
	{
		normal = n;
		normal.normalize();
		d = (-n).dotProduct(p);
	}

	DPlane::DPlane( const DVector3& p0, const DVector3& p1, const DVector3& p2 )
	{
		reset(p0, p1, p2);
	}

	DReal DPlane::normalize()
	{
		DReal l = normal.length();

		// avoid zero length
		if(l > 1e-08f)
		{
			normal /= l;
			d /= l;
		}
		return l;
	}

	DReal DPlane::getDistanceTo( const DVector3& p ) const
	{
		return normal.dotProduct(p) + d;
	}

	DPlane::Side DPlane::getSide( const DVector3& p )
	{
		// if current plane's normal is a zero vector
		// means no correct answer can be given
		if(normal == DVector3::ZERO)
		{
			return DPlane::S_NoSide;
		}

		DReal dist = getDistanceTo(p);

		if(dist > 0.0f)
		{
			return DPlane::S_Positive;
		}
		if(dist < -0.0f)
		{
			return DPlane::S_Negative;
		}

		return DPlane::S_OnPlane;
	}

	DPlane::Side DPlane::getSide( const DAxisAlignedBox& box )
	{
		if (box.isInfinite())
		{
			return S_Across;
		}
		if (box.isNull())
		{
			return S_NoSide;
		}

		//////////////////////////////////////////////////////////////////////////
		// from 3D Math Primer for Graphics	and Game Development
		// page 310
		DVector3 bMin = box.getMinimum();
		DVector3 bMax = box.getMaximum();
		float minD, maxD;
		if (normal.x > 0.0f) {
			minD = normal.x*bMin.x; maxD = normal.x*bMax.x;
		} else {
			minD = normal.x*bMax.x; maxD = normal.x*bMin.x;
		}

		if (normal.y > 0.0f) {
			minD += normal.y*bMin.y; maxD += normal.y*bMax.y;
		} else {
			minD += normal.y*bMax.y; maxD += normal.y*bMin.y;
		}

		if (normal.z > 0.0f) {
			minD += normal.z*bMin.z; maxD += normal.z*bMax.z;
		} else {
			minD += normal.z*bMax.z; maxD += normal.z*bMin.z;
		}

		// Check if completely on the front side of the plane
		if (minD >= -d) {
			return S_Positive;
		}
		// Check if completely on the back side of the plane
		if (maxD <= -d) {
			return S_Negative;
		}
		// We straddle the plane
		return S_Across;


// 		DReal dist = getDistanceTo(box.getCenter());
// 		DReal maxAbsDist = Math::Abs(normal.dotProduct(box.getHalfSize()));
// 
// 		if (dist < -maxAbsDist)
// 		{
// 			return S_NAGATIVE;
// 		}
// 		if (dist > maxAbsDist)
// 		{
// 			return DPlane::S_POSITIVE;
// 		}
// 		return DPlane::S_ACROSS;
	}

	DPlane::Side DPlane::getSide( const DSphere& sphere )
	{
		DReal dist = getDistanceTo(sphere.getCenter());
		if (dist < sphere.getRadius())
		{
			return DPlane::S_Across;
		}
		else
		{
			return getSide(sphere.getCenter());
		}
	}

	void DPlane::reset( const DVector3& p0, const DVector3& p1, const DVector3& p2 )
	{
		DVector3 edge1 = p1 - p0;
		DVector3 edge2 = p2 - p0;


		normal = edge1.crossProduct(edge2);
		normal.normalize();
		d = -normal.dotProduct(p0);
	}

}