//  [11/10/2012 OMEGA] created

#ifndef _DUELRAY_H_
#define _DUELRAY_H_

#include "DuelMath.h"

namespace Duel
{

	// a ray is usualy a light ray, which has a origin and and a
	// direction, along which it extends infinitely.
	class DUEL_API DRay
	{
	public:
		// initial origin is zero, and direction is UNIT_X
		DRay() : mOrigin(DVector3::ZERO), mDirection(DVector3::UNIT_X) {}
		DRay(const DVector3& origin, const DVector3& direction) : mOrigin(origin),	mDirection(direction) {}
		
		// set/get the properties of the ray
		const DVector3&	getOrigin() const { return mOrigin; }
		void			setOrigin(const DVector3& origin) { mOrigin = origin; }
		const DVector3&	getDirection() const { return mDirection; }
		void			setDirection(const DVector3& dir) { mDirection = dir; }
		// point the ray to the target point, if the p is identical to the origin, then this method do nothing.
		void			pointTo(const DVector3& p);

		// get a point with spcified distance to the origin.
		DVector3			getPoint(DReal distance) const { return mOrigin + mDirection*distance; }
		DVector3			operator*(DReal distance) const { return getPoint(distance); }

		// get the common perpendicular to specified ray, if it does not exist,
		// a ray with zero direction will be returned.
		// resulting ray will be from the perpendicular foot of this ray to the specified
		// ray b, pass a length parameter to retrieve the length between two ray, or a NULL to ignore it.
		DRay				getCommonPerpendicularTo(const DRay& b, DReal* length = NULL);

		// refer DMath::Intersection.
		bool			intersect(const DPlane& p, DReal* distance) const
		{
			return DMath::intersect(*this, p, distance);
		}
		bool			intersect(const DSphere& s, DReal* dist1, DReal* dist2) const 
		{
			return DMath::intersect(*this, s, dist1, dist2);
		}
		bool			intersect(const DAxisAlignedBox& b, DReal* dist) const 
		{
			return DMath::intersect(*this, b, dist);
		}
		bool			intersect(const DVector3& a, const DVector3& b, const DVector3& c, DReal* dist) const 
		{
			return DMath::intersect(*this, a, b, c, dist);
		}

	protected:
		DVector3	mOrigin;
		DVector3	mDirection;
	};

}

#endif