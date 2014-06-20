//  [7/15/2012 OMEGA] created

#ifndef	_DUELVECTOR_H_
#define _DUELVECTOR_H_

#include "DuelMath.h"

namespace Duel {

	//  ALL 3D VECTORS USE LEFT-HANDED COORDINATE



	class DUEL_API DVector2
	{
	public:
		DVector2() {}
		~DVector2() {}

		DVector2(DReal X, DReal Y) : x(X), y(Y) {}
		DVector2(DReal r[2]) : x(r[0]), y(r[1]) {}
		DVector2(DReal scaler) : x(scaler), y(scaler) {}
		DVector2(const DVector2& rV) : x(rV.x), y(rV.y) {}
		
		// operator overload
		DVector2&	operator =  (const DVector2& rV) { x = rV.x; y = rV.y; return *this; }
		DVector2&	operator =	 (DReal scaler) { x = y = scaler; return *this; }
		const DVector2&	operator + () const { return *this; }
		DVector2		operator +	(const DVector2& rV) const { return DVector2(x+rV.x, y+rV.y); }
		DVector2		operator +  (const DReal f) const { return DVector2(x+f, y+f); }
		DVector2&	operator += (const DVector2& rV) { x += rV.x; y += rV.y; return *this; }
		DVector2		operator -  () const { return DVector2(-x, -y); }
		DVector2		operator -  (const DVector2& rV) const { return DVector2(x-rV.x, y-rV.y); }
		DVector2		operator -	(const DReal f) const { return DVector2(x-f, y-f); }
		DVector2&	operator -= (const DVector2& rV) { x -= rV.x; y -= rV.y; return *this; }
		DVector2		operator *  (const DVector2& rV) const { return DVector2(x*rV.x, y*rV.y); }
		DVector2		operator *  (DReal scalar) const { return DVector2(x*scalar, y*scalar); }
		friend DVector2 operator * (const DReal scalar, const DVector2& rV)
		{ return DVector2(scalar * rV.x, scalar * rV.y); }
		DVector2&	operator *= (const DVector2& rV) { x *= rV.x; y *= rV.y; return *this; }
		DVector2&	operator *= (DReal scalar) { x *= scalar; y *= scalar; return *this; }
		DVector2		operator /  (const DVector2& rV) const { return DVector2(x/rV.x, y/rV.y); }
		DVector2		operator /  (DReal scalar) const { return DVector2(x/scalar, y/scalar); }
		friend DVector2 operator / (const DReal scalar, const DVector2& rV) 
		{ return DVector2(scalar / rV.x, scalar / rV.y); }
		DVector2&	operator /= (const DVector2& rV) { x /= rV.x; y /= rV.y; return *this; }
		DVector2&	operator /= (DReal scalar) { x /= scalar; y /= scalar; return *this; }

		bool		operator <	(const DVector2& rV) { return x < rV.x && y < rV.y; }
		bool		operator <=	(const DVector2& rV) { return x <= rV.x && y <= rV.y; }
		bool		operator ==	(const DVector2& rV);
		bool		operator >	(const DVector2& rV) { return x > rV.x && y > rV.y; }
		bool		operator >=	(const DVector2& rV) { return x >= rV.x && y >= rV.y; }
		bool		operator != (const DVector2& rV) { return !operator==(rV); }

		DReal		length() { return DMath::Sqrt(x*x + y*y); }
		DReal		squaredLength() { return x*x + y*y; }
		DReal		distance(const DVector2& rV) { return (*this - rV).length(); }
		DReal		squaredDistance(const DVector2& rV) { return (*this - rV).squaredLength(); }
		DReal		dotProduct(const DVector2& rV) { return x * rV.x + y * rV.y; }
		DReal		crossProduct(const DVector2& rV) { return x * rV.y - y * rV.x; }
        
		DVector2		midPoint( const DVector2& rV ) const
        { return DVector2(( x + rV.x ) * 0.5f, ( y + rV.y ) * 0.5f ); }

		// Returns a vector at a point half way between this and the passed in vector.
        DReal		normalize();

		// Sets this vector's components to the minimum of its own and the
        // ones of the passed in vector.
		void		floor(const DVector2& rV)
		{
			if( rV.x < x) x = rV.x; 
			if( rV.y < y) y = rV.y;
		}
		// Sets this vector's components to the maximum of its own and the
        // ones of the passed in vector.
		void		ceil(const DVector2& rV)
		{
			if( rV.x > x) x = rV.x; 
			if( rV.y > y) y = rV.y;
		}

		// A random vector which deviates from this vector by angle. This
        // vector will not be normalised, normalise it if you wish
        // afterwards.
		DVector2		randomDeviant(DReal r);
		DVector2		randomDeviant(const DDegree& rD);
		DVector2		randomDeviant(const DRadian& rR);

		bool		isZeroLength()
		{
			return (x*x + y*y) < (1e-06 * 1e-06);
			
		}
		// Calculates a reflection vector to the plane with the given normal .
        // @remarks NB assumes 'this' is pointing AWAY FROM the plane, invert if it is not.
		DVector2		reflect(const DVector2& normal)
        {
            return DVector2( *this - ( 2.0f * (this->dotProduct(normal)) * normal ) );
        }

		inline void	swap(DVector2& rV) { std::swap(x, rV.x); std::swap(y, rV.y); }

		inline DReal& operator [] (const size_t i) { assert( i < 2 ); return *(&x + i); }
		inline DReal operator [] (const size_t i) const { assert( i < 2 ); return *(&x + i); }

		// Pointer accessor for direct copying
		DReal*		ptr()	{ return &x; }

	public:
		static const DVector2 ZERO;
        static const DVector2 UNIT_X;
        static const DVector2 UNIT_Y;
        static const DVector2 NEGATIVE_UNIT_X;
        static const DVector2 NEGATIVE_UNIT_Y;
        static const DVector2 UNIT_SCALE;


	public:
		DReal x, y;
	};

	class DUEL_API DVector3
	{
	public:
		DVector3() {}
		~DVector3() {}

		DVector3(const DVector2& xy, DReal z) : x(xy.x), y(xy.y), z(z) {}
		DVector3(DReal X, DReal Y, DReal Z) : x(X), y(Y), z(Z) {}
		DVector3(DReal r[3]) : x(r[0]), y(r[1]), z(r[2]) {}
		DVector3(DReal scaler) : x(scaler), y(scaler), z(scaler) {}
		DVector3(const DVector3& rV) : x(rV.x), y(rV.y), z(rV.z) {}
		DVector3(const DVector4& rV);

		// operator overload
		DVector3&	operator =  (const DVector3& rV) { x = rV.x; y = rV.y; z = rV.z; return *this; }
		DVector3&	operator =  (const DVector4& rV);

		DVector3&	operator =	 (DReal scaler) { x = y = z = scaler; return *this; }
		const DVector3&	operator + () const { return *this; }
		DVector3		operator +	(const DVector3& rV) const { return DVector3(x+rV.x, y+rV.y, z+rV.z); }
		DVector3		operator +  (const DReal f) const { return DVector3(x+f, y+f, z+f); }
		DVector3&	operator += (const DVector3& rV) { x += rV.x; y += rV.y; z += rV.z; return *this; }
		DVector3		operator -  () const { return DVector3(-x, -y, -z); }
		DVector3		operator -  (const DVector3& rV) const { return DVector3(x-rV.x, y-rV.y, z-rV.z); }
		DVector3		operator -	(const DReal f) const { return DVector3(x-f, y-f, z-f); }
		DVector3&	operator -= (const DVector3& rV) { x -= rV.x; y -= rV.y; z -= rV.z; return *this; }
		DVector3		operator *  (const DVector3& rV) const { return DVector3(x*rV.x, y*rV.y, z*rV.z); }
		DVector3		operator *  (DReal scalar) const  { return DVector3(x*scalar, y*scalar, z*scalar); }
		friend DVector3 operator * (const DReal scalar, const DVector3& rV) 
		{ return DVector3(scalar * rV.x, scalar * rV.y, scalar * rV.z); }
		DVector3&	operator *= (const DVector3& rV) { x *= rV.x; y *= rV.y; z *= rV.z; return *this; }
		DVector3&	operator *= (DReal scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
		DVector3		operator /  (const DVector3& rV) const { return DVector3(x/rV.x, y/rV.y, z/rV.z); }
		DVector3		operator /  (DReal scalar) const { return DVector3(x/scalar, y/scalar, z/scalar); }
		friend DVector3 operator / (const DReal scalar, const DVector3& rV) 
		{ return DVector3(scalar / rV.x, scalar / rV.y, scalar / rV.z); }
		DVector3&	operator /= (const DVector3& rV) { x /= rV.x; y /= rV.y; z /= rV.z; return *this; }
		DVector3&	operator /= (DReal scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; }

		bool		operator <	(const DVector3& rV) const { return x < rV.x && y < rV.y && z < rV.z; }
		bool		operator <=	(const DVector3& rV) const { return x <= rV.x && y <= rV.y && z <= rV.z; }
		bool		operator ==	(const DVector3& rV) const { return rV.x == x && rV.y == y && z == rV.z; }
		bool		operator >	(const DVector3& rV) const { return x > rV.x && y > rV.y && z > rV.z; }
		bool		operator >=	(const DVector3& rV) const { return x >= rV.x && y >= rV.y && z >= rV.z; }
		bool		operator != (const DVector3& rV) const { return rV.x != x || rV.y != y || z != rV.z; }

		DReal		length() const { return DMath::Sqrt(x*x + y*y + z*z); }
		DReal		squaredLength() const { return x*x + y*y + z*z; }
		DReal		getDistanceTo(const DVector3& rV) const { return (*this - rV).length(); }
		DReal		getSquaredDistanceTo(const DVector3& rV) const { return (*this - rV).squaredLength(); }
		DReal		dotProduct(const DVector3& rV) const  { return x * rV.x + y * rV.y + z * rV.z; }
		DVector3		crossProduct(const DVector3& rV) const
		{
			return DVector3( y*rV.z - z*rV.y, z*rV.x - x*rV.z, x*rV.y - y*rV.x ); 
		}
        
		DVector3		midPoint( const DVector3& rV ) const
        { 
			return DVector3( (x+rV.x) * 0.5f, (y+rV.y) * 0.5f, (z+rV.z) * 0.5f ); 
		}

		// Returns a vector at a point half way between this and the passed in vector.
        DReal		normalize();

		// Sets this vector's components to the minimum of its own and the
        // ones of the passed in vector.
		void		floor(const DVector3& rV)
		{
			if( rV.x < x) x = rV.x; 
			if( rV.y < y) y = rV.y;
			if( rV.z < z) z = rV.z;
		}
		// Sets this vector's components to the maximum of its own and the
        // ones of the passed in vector.
		void		ceil(const DVector3& rV)
		{
			if( rV.x > x) x = rV.x; 
			if( rV.y > y) y = rV.y;
			if( rV.z > z) z = rV.z;
		}
		/** Gets the shortest arc quaternion to rotate this vector to the destination
            vector.
        @remarks
            If you call this with a dest vector that is close to the inverse
            of this vector, we will rotate 180 degrees around the 'fallbackAxis'
			(if specified, or a generated axis if not) since in this case
			ANY axis of rotation is valid.
        */
		DQuaternion	getRotationTo(const DVector3& dest, const DVector3& fallbackAxis = DVector3::ZERO) const;

		DVector3		perpendicular(void);

		// A random vector which deviates from this vector by angle. This
        // vector will not be normalised, normalise it if you wish
        // afterwards.

		DVector3		randomDeviant(DRadian& angle, DVector3 up = DVector3::ZERO);

		bool		isZeroLength()
		{
			return (x*x + y*y) < (1e-06 * 1e-06);
			
		}

		// Calculates a reflection vector to the plane with the given normal .
        // @remarks NB assumes 'this' is pointing AWAY FROM the plane, invert if it is not.
		DVector3		reflect(const DVector3& normal)
        {
            return DVector3( *this - ( 2.0f * (this->dotProduct(normal)) * normal ) );
        }

		void		swap(DVector3& rV);

		inline DReal& operator [] (const size_t i) { assert( i < 3 ); return *(&x + i); }
		inline DReal operator [] (const size_t i) const { assert( i < 3 ); return *(&x + i); }


		// Pointer accessor for direct copying
		DReal*	ptr()	{ return &x; }

	public:
		static const DVector3 ZERO;
        static const DVector3 UNIT_X;
        static const DVector3 UNIT_Y;
        static const DVector3 UNIT_Z;
        static const DVector3 NEGATIVE_UNIT_X;
        static const DVector3 NEGATIVE_UNIT_Y;
        static const DVector3 NEGATIVE_UNIT_Z;
        static const DVector3 UNIT_SCALE;

	public:
		DReal x, y, z;
	};


	class DVector4
	{
	public:
		DVector4() {}
		~DVector4() {}

		DVector4(const DVector3& rV, DReal w = 1.0f) : x(rV.x), y(rV.y), z(rV.z), w(w) {}
		DVector4(DReal X, DReal Y, DReal Z, DReal W) : x(X), y(Y), z(Z), w(W) {}
		DVector4(DReal r[4]) : x(r[0]), y(r[1]), z(r[2]), w(r[4]) {}
		DVector4(DReal scaler) : x(scaler), y(scaler), z(scaler), w(scaler) {}
		DVector4(const DVector4& rV) : x(rV.x), y(rV.y), z(rV.z), w(rV.w) {}
		
		// operator overload
		DVector4&	operator =  (const DVector4& rV);
		DVector4&	operator =	 (DReal scaler);
		const DVector4&	operator + () const;
		DVector4		operator +	(const DVector4& rV);
		DVector4		operator +  (const DReal f);
		DVector4&	operator += (const DVector4& rV);
		DVector4		operator -  ();
		DVector4		operator -  (const DVector4& rV);
		DVector4		operator -	(const DReal f);
		DVector4&	operator -= (const DVector4& rV);
		DVector4		operator *  (const DVector4& rV);
		DVector4		operator *  (DReal scalar);
		friend DVector4 operator * (const DReal scalar, const DVector4& rV);
		DVector4&	operator *= (const DVector4& rV);
		DVector4&	operator *= (DReal scalar);
		DVector4		operator /  (const DVector4& rV);
		DVector4		operator /  (DReal scalar);
		friend DVector4 operator / (const DReal scalar, const DVector4& rV);
		DVector4&	operator /= (const DVector4& rV);
		DVector4&	operator /= (DReal scalar);

		bool		operator <	(const DVector4& rV) { return x < rV.x && y < rV.y && z < rV.z && w < rV.w; }
		bool		operator <=	(const DVector4& rV) { return x <= rV.x && y <= rV.y && z <= rV.z && w <= rV.w; }
		bool		operator ==	(const DVector4& rV) { return rV.x == x && rV.y == y && z == rV.z && w == rV.w; }
		bool		operator >	(const DVector4& rV) { return x > rV.x && y > rV.y && z > rV.z && w > rV.w; }
		bool		operator >=	(const DVector4& rV) { return x >= rV.x && y >= rV.y && z >= rV.z && w >= rV.w; }
		bool		operator != (const DVector4& rV) { return rV.x != x || rV.y != y || z != rV.z || w != rV.w; }

		inline DReal& operator [] (const size_t i) { assert( i < 4 ); return *(&x + i); }
		inline DReal operator [] (const size_t i) const { assert( i < 4 ); return *(&x + i); }


		DReal	dotProduct(const DVector4& rV);

	public:
		DReal x, y, z, w;
	};

}


#endif