//  [7/15/2012 OMEGA] created

#ifndef	_DUELSPHERE_H_
#define _DUELSPHERE_H_

#include "DuelMath.h"

namespace Duel
{

	class DUEL_API DSphere
	{
	public:
		// initial value is radius 1.0f, center zero.
		DSphere();
		DSphere(const DVector3& center, DReal radius);

		// set/get properties
		DReal			getRadius() const { return mRadius; }
		void			setRadius(DReal r) { mRadius = r; }
		const DVector3&	getCenter() const { return mCenter;}
		void			setCenter(const DVector3& c) { mCenter = c; }

		// query whether the sphere intersect with other math object
		bool			intersect(const DSphere& rSph) const;
		bool			intersect(const DAxisAlignedBox& aabb) const;
		bool			intersect(const DPlane& p) const;
		bool			intersect(const DRay& ray) const;
		bool			intersect(const DVector3& pos) const; // whether the point located in the sphere.



	protected:
		DReal	mRadius;
		DVector3	mCenter;
	};

}

#endif