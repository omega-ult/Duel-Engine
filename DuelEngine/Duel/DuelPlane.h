//  [7/17/2012 OMEGA] created

#ifndef _DUELPLANE_H_
#define _DUELPLANE_H_

#include "DuelMath.h"
#include "DuelVector.h"

namespace Duel
{
	// The plane is represented as Dot(N,X) - c = 0, where N = (n0,n1,n2,0)
	// is a unit-length normal vector, c is the plane constant, and
	// X = (x0,x1,x2,1) is any point on the plane.  The user must ensure
	// that the normal vector is unit length.  The storage as a 4-tuple is
	// (n0,n1,n2,-d).
	//-------------------WildMagic5

	class DUEL_API DPlane
	{
	public:

		// indicate an object (such as point, box, or line) 
		// in which side of the plane
		enum Side
		{
			S_NoSide,
			S_OnPlane,
			S_Positive,
			S_Negative,
			S_Across
		};

		DPlane();
		DPlane(const DPlane& p);
		~DPlane() {};

		// specify N and c directly.
		DPlane(DReal n0, DReal n1, DReal n2, DReal c);
		DPlane(const DVector3& n, DReal c);

		// specify a DPlane use a (n)ormal and a (p)oint
		DPlane(const DVector3& n, const DVector3& p);

		// specify a DPlane use three points, see reset().
		DPlane(const DVector3& p0, const DVector3& p1, const DVector3& p2);

		// operator---------------------------
		DPlane&			operator = (const DPlane& rP) { normal = rP.normal; d = rP.d; return *this; }
		const DPlane&	operator + () { return *this; }
		const DPlane		operator - () { return DPlane(-normal,-d); }

		bool	operator == (const DPlane& rP) { return (normal == rP.normal) && (d == rP.d); }
		bool	operator != (const DPlane& rP) { return !operator==(rP); }

		// reset a plane using specified points.
		// notice that in our left-handed system, the contructed plane's normal is desided
		// clockwisely by the p0, p1, p2.
		void	reset(const DVector3& p0, const DVector3& p1, const DVector3& p2);
		// query a point at which side to the plane
		Side	getSide(const DVector3& p);
		Side	getSide(const DAxisAlignedBox& box);
		Side	getSide(const DSphere& sphere);

		// make the normal and the length of d become unit-length
		DReal	normalize();
		// get the distantce between specified point and normal
		DReal	getDistanceTo(const DVector3& p) const;

		// get the orthocenter of specified point.
		DVector3	getPointOrthocenter(const DVector3& p);

		DVector3 normal;
		DReal	d;
	};

	// a plane container, contains a chain of planes.
	typedef	std::vector<DPlane>	Planes;
	
}



#endif