//  [7/15/2012 OMEGA] created

#ifndef	_DUELQUATERNION_H_
#define _DUELQUATERNION_H_

#include "DuelMath.h"


namespace Duel {

	// pre-declaration
	class DRotator;

	// The (x,y,z) coordinate system is assumed to be LEFT-HANDED.  Coordinate

	//             (+)
	//              y|  (+)|far|
	//               |  / z
	//               | /
	//               |/
	//      (-)------#--------(+)	
	//              /|       x
	//             / |
	//            /  |
	//    |near|(-)  |
	//              (-)
	

	class DUEL_API DQuaternion 
	{
	public:
		DQuaternion( DReal W = 1.0, DReal X = 0.0, DReal Y = 0.0, DReal Z = 0.0 )
		{
			w = W;
			x = X;
			y = Y;
			z = Z;
		}
		DQuaternion( DMatrix3 rM )
		{
			this->fromRotationMatrix(rM);
		}
		DQuaternion( DRadian rR, DVector3 rVaxis)
		{
			this->fromAngleAxis(rR,rVaxis);
		}


		DQuaternion( DReal r[4] )
		{
			memcpy(&w, r,sizeof(DReal)*4);
		}
		

		DQuaternion& operator =	(const DQuaternion& rQ) { w = rQ.w; x = rQ.x; y = rQ.y; z = rQ.z; return *this; }
		DQuaternion	operator +	(const DQuaternion& rQ) const { return DQuaternion(w + rQ.w, x + rQ.x, y + rQ.y, z + rQ.z); }
		DQuaternion	operator -	(const DQuaternion& rQ) const { return DQuaternion(w - rQ.w, x - rQ.x, y - rQ.y, z - rQ.z); }
		DQuaternion	operator -	(void) const { return DQuaternion(-w, -x, -y, -z); }
		
		// notice the order of the quaternion cross product, (i.e, mulutiplication of rotation)
		// we use a style different from the standard definition, which calculate accumulation of rotation
		// from right to left, we use a different style which let us multiply it from left to right,
		// so if we have rotation qa, qb, assume we rotate qa then qb, we can just do qa*qb.
		DQuaternion	operator *	(const DQuaternion& rQ) const;
		DQuaternion operator *= (const DQuaternion& rQ) { *this = operator*(rQ); return *this; }
		// rotation of a vector by a quaternion
		// use as q * v, always can get a correct result
		DVector3 operator *	(const DVector3& rV) const;
		DQuaternion	operator *	(DReal scalar) const { return DQuaternion(w*scalar, x*scalar, y*scalar, z*scalar); } 
		friend DQuaternion operator * (DReal scalar, const DQuaternion& rQ)  
				{ return DQuaternion(rQ.w*scalar, rQ.x*scalar, rQ.y*scalar, rQ.z*scalar); } 

		bool	operator ==	(const DQuaternion& rQ) const { return (w == rQ.w) && (x == rQ.x) && (y == rQ.y) && (z == rQ.z); }
		bool	operator != (const DQuaternion& rQ) const { return !operator == (rQ); }

		// dot product
		DReal		dotProduct(const DQuaternion& rQ) const;
		/// Normalises this quaternion, and returns the previous length
		DReal		normalize();
		// apply to non-zero quaternion, get a reversed rotation matrix
		DQuaternion	getConjugate() const;
		
		/// Equality with tolerance (tolerance is max angle difference)
		bool		equals(const DQuaternion& rQ, const DRadian& tolerance);
		
		DQuaternion	log() const;
		DQuaternion	exp() const;
		

		// Calculate the inertial-to-object Roll element of this quaternion.
		// along z axis
		DRadian		getRoll();
		// Calculate the inertial-to-object Pitch element of this quaternion.
		// along x axis
		DRadian		getPitch();
		// Calculate the inertial-to-object Yaw element of this quaternion.
		// along y axis
		DRadian		getYaw();	
		

		// spherical linear interpolation
		static DQuaternion	Slerp(DReal f, const DQuaternion& rSrcQ,
				const DQuaternion& rDstQ, bool shortestPath = false);

		static DQuaternion	SlerpExtraSpins(DReal f, const DQuaternion& rSrcQ,
				const DQuaternion& rDstQ, int iExtraSpins);

		// setup for spherical quadratic interpolation
		static void			Intermediate(const DQuaternion& rkQ0,
			const DQuaternion& rkQ1, const DQuaternion& rkQ2,
			DQuaternion& rkA, DQuaternion& rkB);

		// spherical quadratic interpolation
		static DQuaternion	Squad(DReal fT, const DQuaternion& rkP,
			const DQuaternion& rkA, const DQuaternion& rkB,
			const DQuaternion& rkQ, bool shortestPath = false);

		// normalised linear interpolation - faster but less accurate (non-constant rotation velocity)
		static DQuaternion	nLerp(DReal f, const DQuaternion& rSrcQ,
				const DQuaternion& rDstQ, bool shortestPath = false);

		void		fromAxes(const DVector3& xVec, const DVector3& yVec, const DVector3& zVec);
		void		toAxes(DVector3& xVec, DVector3& yVec, DVector3& zVec) const;
		void		fromRotationMatrix( const DMatrix3& rM );
		void		toRotationMatrix( DMatrix3& rM) const;
		void		fromAngleAxis(const DRadian& rR,const DVector3& rV );
		void		toAngleAxis( DRadian& rR, DVector3& rV ) const;
		void		toRotator(DRotator& rR) const;

		DReal operator [] ( const size_t i ) const
		{
			assert( i < 4 );
			return *(&w + i);
		}
		DReal* ptr() 
		{
			return &w;
		}

		void swap( DQuaternion& rQ );

		// cutoff for sine near zero
        static const DReal EPSILON;

        // special values
        static const DQuaternion ZERO;
        static const DQuaternion IDENTITY;


	public:
		DReal w, x, y, z;

	};

}


#endif