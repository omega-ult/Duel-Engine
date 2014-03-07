//  [7/15/2012 OMEGA] created

#ifndef	_DUELANGLE_H_
#define _DUELANGLE_H_

#include "DuelMath.h"

namespace Duel {

	class DUEL_API DDegree 
	{
	public:
		DDegree() : mDegree(0) {}
		DDegree(DReal degree) : mDegree(degree) {}
		DDegree(const DRadian& radian);
		~DDegree(){}

		// operator overload
		inline DDegree& operator =	(const DReal f) { mDegree = f; return *this; }
		inline DDegree& operator =	(const DDegree& rD)  {mDegree = rD.mDegree; return *this; }
		DDegree& operator =	(const DRadian& rR);
		const DDegree& operator + () const { return *this; }
		inline DDegree	operator +	(const DDegree& rD) const { return DDegree( mDegree + rD.mDegree ); }
		DDegree	operator +	(const DRadian& rR) const;
		inline DDegree& operator += (const DDegree& rD) { mDegree += rD.mDegree; return *this; }
		DDegree& operator += (const DRadian& rR);
		DDegree	operator -	() const { return DDegree(-mDegree); }
		inline DDegree	operator -	(const DDegree& rD) const { return DDegree( mDegree - rD.mDegree ); }
		DDegree	operator -	(const DRadian& rR) const;
		inline DDegree& operator -= (const DDegree& rD) { mDegree -= rD.mDegree; return *this; }
		DDegree& operator -= (const DRadian& rR);
		inline DDegree	operator *	(DReal f) const { return DDegree( mDegree * f ); }
		inline DDegree& operator *= (DReal f) { mDegree *= f; return *this; }
		inline DDegree	operator /	(DReal f) const { return DDegree( mDegree / f ); }
		inline DDegree&	operator /= (DReal f) { mDegree /= f; return *this; }

		bool	operator <	(const DDegree& rD) const { return mDegree <		rD.mDegree; }
		bool	operator <=	(const DDegree& rD) const { return mDegree <=	rD.mDegree; }
		bool	operator ==	(const DDegree& rD) const { return mDegree ==	rD.mDegree; }
		bool	operator >	(const DDegree& rD) const { return mDegree >		rD.mDegree; }
		bool	operator >=	(const DDegree& rD) const { return mDegree >=	rD.mDegree; }
		bool	operator !=	(const DDegree& rD) const { return mDegree !=	rD.mDegree; }

		// get value
		DReal getDegree() const { return mDegree; };
		DReal getRadian() const;
		
	
	private:
		DReal	mDegree;
	};


	class DUEL_API DRadian
	{
	public:
		DRadian() : mRadian(0) {}
		DRadian(DReal radian) : mRadian(radian) {}
		DRadian(const DDegree& degree);
		~DRadian(){}

		// operator overload
		inline DRadian&	operator =	 (const DReal f) { mRadian = f; return *this; }
		inline DRadian&	operator =  (const DRadian& rR) { mRadian = rR.mRadian; return *this; }
		DRadian& operator =  (const DDegree& rD);
		const DRadian& operator + () const { return *this; }
		DRadian	operator +	(const DRadian& rR) const { return DRadian( mRadian + rR.mRadian ); }
		DRadian	operator +	(const DDegree& rD) const;
		inline DRadian&	operator +=	(const DRadian& rR) { mRadian += rR.mRadian; return *this; }
		DRadian& operator += (const DDegree& rD);
		inline DRadian	operator -	() const { return DRadian( -mRadian ); }
		inline DRadian	operator -	(const DRadian& rR) const { return DRadian( mRadian - rR.mRadian ); }
		DRadian	operator -	(const DDegree& rD) const;
		inline DRadian&	operator -=	(const DRadian& rR) { mRadian -= rR.mRadian; return *this; }
		DRadian& operator -= (const DDegree& rD);
		inline DRadian	operator *	(DReal f) const { return DRadian( mRadian * f ); }
		inline DRadian&	operator *=	(DReal f) { mRadian *= f; return *this; }
		inline DRadian	operator /	(DReal f) const { return DRadian( mRadian / f ); }
		inline DRadian&	operator /= (DReal f) { mRadian /= f; return *this; }

		bool	operator <	(const DRadian& rR) const { return mRadian <		rR.mRadian; }
		bool	operator <=	(const DRadian& rR) const { return mRadian <=	rR.mRadian; }
		bool	operator ==	(const DRadian& rR) const;
		bool	operator >	(const DRadian& rR) const { return mRadian >		rR.mRadian; }
		bool	operator >=	(const DRadian& rR) const { return mRadian >=	rR.mRadian; }
		bool	operator !=	(const DRadian& rR) const { return mRadian !=	rR.mRadian; }

		// get value
		DReal getDegree() const;
		DReal getRadian() const { return mRadian; };

		
	private:
		DReal	mRadian;
	};
}

#endif