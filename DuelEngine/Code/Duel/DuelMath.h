//  [7/15/2012 OMEGA] created

#ifndef	_DUELMATH_H_
#define _DUELMATH_H_

#include "DuelType.h"

namespace Duel {
	// type defines
	
	//////////////////////////////////////////////////////////////////////////
	// math
	class	DMath;
	class	DRadian;
	class	DDegree;

	template<typename T>
	class	TBox;


	template<typename T>
	class	TRectangle;

	class	DMatrix3;
	class	DMatrix4;

	class	DPlane;
	class	DQuaternion;
	class	DRotator;
	class	DVector2;
	class	DVector3;
	class	DVector4;

	class	DRay;
	class	DSphere;
	class	DAxisAlignedBox;
	class	DOrientedBox;

	class DUEL_API DMath
	{
	public:
		// use  'new DMath(int)' before using triangleTable
		DMath(uint32 tableSize = 4096);
		~DMath() {}

		static DReal		RadianToDegree(DReal r);
		static DReal		DegreeToRadian(DReal r);
		
		static DReal		Sin(DReal f, bool useTable = false);
		static DReal		Sin(const DDegree& rD, bool useTable = false);
		static DReal		Sin(const DRadian& r, bool useTable = false);

		static DReal		Cos(DReal f, bool useTable = false);
		static DReal		Cos(const DDegree& rD, bool useTable = false);
		static DReal		Cos(const DRadian& r, bool useTable = false);
		
		static DReal		Tan(DReal f, bool useTable = false);
		static DReal		Tan(const DDegree& rD, bool useTable = false);
		static DReal		Tan(const DRadian& r, bool useTable = false);

		static DRadian		arcSin(DReal f);
		static DRadian		arcCos(DReal f);
		static DRadian		arcTan(DReal f);
		static DRadian		arcTan2(DReal y, DReal x);
			
		static int			Abs(int i) { return i > 0 ? i : -i; }
		static DReal		Abs(DReal f) { return abs(f); }
		static DDegree		Abs(const DDegree& rD);
		static DRadian		Abs(const DRadian& rR);
		static int			ICeil(DReal f) { return int(ceil(f)); }
		static DReal		Ceil(DReal f) { return ceil(f); }
		static int			IFloor(DReal f) { return int(floor(f)); }
		static DReal		Floor(DReal f) { return floor(f); }
		
		static DReal		Exp(DReal f) { return exp(f); }
		static DReal		Log(DReal f) { return log(f); }
		static DReal		Log2(DReal f) { return (log(f) / DMath::LOG2); }
		static DReal		LogN(DReal base, DReal f) { return (log(f) / log(base)); }
		static DReal		Pow(DReal base, DReal exp) { return pow(base, exp); }
		static DReal		Sqr(DReal f) { return f*f; }
		static DReal		Sqrt(DReal f) { return sqrt(f); }
		static DReal		invSqrt(DReal f) { return 1/sqrt(f); }

		template<typename T>
		inline static T		Max(T a, T b) { return a > b ? a : b; }
		template<typename T>
		inline static T		Min(T a, T b) { return a < b ? a : b; }

		inline	static int32	Round(DReal d)
		{
			return d >= 0.0f ? int(d + 0.5f) : int32(d - int32(d-1) + 0.5f) + int32(d-1); 
		}
		inline	static int64	Round64(DReal d)
		{
			return d >= 0.0f ? int(d + 0.5f) : int64(d - int64(d-1) + 0.5f) + int64(d-1); 
		}

		inline static bool		realEqual(DReal a, DReal b, DReal tolerance = std::numeric_limits<DReal>::epsilon())
		{
			if (fabs(b-a) <= tolerance)
				return true;
			else
				return false;
		}
		// calculate the normal of a triangle, we assume that using clock-wise as the point order.
		// result will not be normalized.
		static DVector3		getNormal(const DVector3& a, const DVector3& b, const DVector3& c);

		static DReal		unitRandom() { return static_cast<DReal>(rand()) / static_cast<DReal>(RAND_MAX); }
		static DReal		rangeRandom(DReal low, DReal up) { return low + (up - low)*unitRandom(); }
		static DReal		symmetricRandom() { return 2.0f * unitRandom() - 1.0f; }
		
		static DMatrix4		makeViewMatrix(const DVector3& position, const DQuaternion& orientation, 
			const DMatrix4* reflectMatrix = 0);
		// intersection test, result can be true of false.
		// the param dist is an out parameter which will retrieve the 
		// distance between two testing object, if it is NULL, then no distance
		// information can be provided
		// dist is the distance from the origin of the ray to the joint
		static bool		intersect(const DRay& ray, const DPlane& p, DReal* dist);
		// dist1 is the distance of the nearest joint, dist2 is the farthest joint, we do not take
		// the case that origin of the ray is in the sphere into consideration, to test this ,use DSphere::intersect(DVector3)instead.
		// if the nearest distance is a negative value, then
		// the point is in the sphere.
		static bool		intersect(const DRay& ray, const DSphere& s, DReal* dist1, DReal* dist2);
		// dist is the nearest joint 
		static bool		intersect(const DRay& ray, const DAxisAlignedBox& b, DReal* dist);
		// intersection between two oriented box.
		static bool		intersect(const DOrientedBox& a, const DOrientedBox& b);
		// intersection between ray and oriented box.
		static bool		intersect(const DRay& ray, DOrientedBox* b, DReal* dist);
		// triangle intersection test, point a, b, c is the pointer of the three point.
		// dist is the distance from the center of ray to the triangle.
		static bool		intersect(const DRay& ray, const DVector3& a, const DVector3& b, const DVector3& c, DReal* dist);
		// intersection between sphere and aabb.
		static bool		intersect(const DSphere& s, const DAxisAlignedBox& b);
		// intersection between sphere and plane
		static bool		intersect(const DSphere& s, const DPlane& p);
		// intersection between plane and aabb.
		static bool		intersect(const DPlane& p, const DAxisAlignedBox& b);

		static const DReal REAL_POS_INFINITY;
		static const DReal REAL_NEG_INFINITY;
		static const DReal PI;
		static const DReal TWO_PI;
		static const DReal HALF_PI;
		static const DReal LOG2;

	protected:
		static DReal *mSinTable;
		static DReal *mTanTable;
		static uint32	 mTableSize;
		// construct lookup table
		static void			initTriangleTable(uint32 size);
		static DReal		getTabledSin(DReal radian);
		static DReal		getTabledCos(DReal radian);
		static DReal		getTabledTan(DReal radian);
	
	};

}

#endif