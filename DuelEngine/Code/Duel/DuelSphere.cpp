//  [11/10/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelMath.h"
#include "DuelSphere.h"

namespace Duel
{


	DSphere::DSphere() :
		mRadius(1.0f),
		mCenter(DVector3::ZERO)
	{

	}

	DSphere::DSphere( const DVector3& center, DReal radius ) :
		mRadius(radius),
		mCenter(center)
	{

	}

	bool DSphere::intersect( const DSphere& rSph ) const
	{
		return mCenter.getDistanceTo(rSph.mCenter) <= mRadius + rSph.mRadius;
	}

	bool DSphere::intersect( const DAxisAlignedBox& aabb ) const
	{
		return DMath::intersect(*this,aabb);
	}

	bool DSphere::intersect( const DPlane& p ) const
	{
		return mRadius <= p.getDistanceTo(mCenter);
	}

	bool DSphere::intersect( const DRay& ray ) const
	{
		return DMath::intersect(ray, *this, NULL, NULL);
	}

	bool DSphere::intersect( const DVector3& pos ) const
	{
		return mRadius <= mCenter.getDistanceTo(pos);
	}

}