//  [7/15/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelAngle.h"
#include "DuelMath.h"

namespace Duel {




	/////////////////////////////////////////////////
	DDegree::DDegree(const DRadian& radian) : mDegree(radian.getDegree()){}

	DDegree& DDegree::operator = (const DRadian& rR)
	{
		mDegree = rR.getDegree();
		return *this;
	}

	DDegree DDegree::operator + (const DRadian& rR) const
	{
		return DDegree( mDegree + rR.getDegree());
	}

	DDegree& DDegree::operator += (const DRadian& rR)
	{
		mDegree += rR.getDegree();
		return *this;
	}

	DDegree DDegree::operator - (const DRadian& rR) const
	{
		return DDegree( mDegree - rR.getDegree() );
	}

	DDegree& DDegree::operator -= (const DRadian& rR)
	{
		mDegree -= rR.getDegree();
		return *this;
	}

	DReal DDegree::getRadian() const
	{
		return DMath::DegreeToRadian(mDegree);
	}



	/////////////////////////////////////////////////
	DRadian::DRadian (const DDegree& degree) : mRadian(degree.getRadian()){}
	
	DRadian& DRadian::operator = (const DDegree& rD) 
	{ 
		mRadian = rD.getRadian(); 
		return *this; 
	}
	
	DRadian DRadian::operator + (const DDegree& rD) const
	{ 
		return DRadian( mRadian + rD.getRadian() ); 
	}
	
	DRadian& DRadian::operator += (const DDegree& rD)
	{ 
		mRadian += rD.getRadian(); 
		return *this; 
	}

	DRadian DRadian::operator - (const DDegree& rD) const 
	{ 
		return DRadian( mRadian - rD.getRadian() ); 
	}

	DRadian& DRadian::operator -= (const DDegree& rD) 
	{
		mRadian -= rD.getRadian();
		return *this;
	}

	DReal DRadian::getDegree() const
	{
		return DMath::RadianToDegree(mRadian);
	}

	bool DRadian::operator==( const DRadian& rR ) const
	{
		return DMath::realEqual(mRadian, rR.mRadian);
	}



}