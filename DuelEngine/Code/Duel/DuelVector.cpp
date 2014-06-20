//  [7/15/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelVector.h"
#include "DuelQuaternion.h"


namespace Duel
{
#pragma region "DVector2"
	// DVector2----------------------
	const DVector2 DVector2::ZERO(0, 0);
	const DVector2 DVector2::UNIT_X(1, 0);
    const DVector2 DVector2::UNIT_Y(0, 1);
    const DVector2 DVector2::NEGATIVE_UNIT_X(-1,  0);
    const DVector2 DVector2::NEGATIVE_UNIT_Y( 0, -1);
    const DVector2 DVector2::UNIT_SCALE(1, 1);


	//--------------------------------
	DVector2 DVector2::randomDeviant(DReal r)
	{
		r *=  DMath::unitRandom() * DMath::TWO_PI;
        DReal cosa = cos(r);
        DReal sina = sin(r);
        return  DVector2( cosa * x - sina * y, sina * x + cosa * y);

	}

	DVector2 DVector2::randomDeviant(const DDegree& rD)
	{
		DReal r = rD.getRadian();
		r *=  DMath::unitRandom() * DMath::TWO_PI;
        DReal cosa = cos(r);
        DReal sina = sin(r);
        return  DVector2( cosa * x - sina * y, sina * x + cosa * y);
	}

	DVector2 DVector2::randomDeviant(const DRadian& rR)
	{
		DReal r = rR.getRadian();
		r *=  DMath::unitRandom() * DMath::TWO_PI;
        DReal cosa = cos(r);
        DReal sina = sin(r);
        return  DVector2( cosa * x - sina * y, sina * x + cosa * y);
	}

	DReal DVector2::normalize()
	{
		DReal l = length();
		// Will also work for zero-sized vectors, but will change nothing
		if ( l > 1e-08 )
		{
			x /= l;
			y /= l;
		}
		return l;
	}

	bool DVector2::operator==( const DVector2& rV )
	{
		return DMath::realEqual(rV.x, x) && DMath::realEqual(rV.y, y);
	}


#pragma endregion



#pragma region "DVector3"
	// DVector3-----------------------------------
	const DVector3 DVector3::ZERO(0,0,0);
    const DVector3 DVector3::UNIT_X(1,0,0);
    const DVector3 DVector3::UNIT_Y(0,1,0);
    const DVector3 DVector3::UNIT_Z(0,0,1);
    const DVector3 DVector3::NEGATIVE_UNIT_X(-1,  0,  0);
    const DVector3 DVector3::NEGATIVE_UNIT_Y( 0, -1,  0);
    const DVector3 DVector3::NEGATIVE_UNIT_Z( 0,  0, -1);
    const DVector3 DVector3::UNIT_SCALE(1,1,1);

	DVector3 DVector3::randomDeviant(DRadian& angle, DVector3 up)
    {
        DVector3 newUp;

        if (up == DVector3::ZERO)
        {
            // Generate an up vector
            newUp = this->perpendicular();
        }
        else
        {
            newUp = up;
        }

        // Rotate up vector by random amount around this
        DQuaternion q;
        q.fromAngleAxis( DRadian(DMath::unitRandom() * DMath::TWO_PI), *this );
        newUp = q * newUp;

        // Finally rotate this by given angle around randomised up
        q.fromAngleAxis( angle, newUp );
        return q * (*this);
    }

	void DVector3::swap( DVector3& rV )
	{
		DReal temp;
		temp = rV.x;
		rV.x = x;
		x = temp;

		temp = rV.y;
		rV.y = y;
		y = temp;

		temp = rV.z;
		rV.z = z;
		z = temp;
	}

	DReal DVector3::normalize()
	{
		DReal l = length();
		// Will also work for zero-sized vectors, but will change nothing
		if ( l > 1e-08 )
		{
			x /= l;
			y /= l;
			z /= l;
		}
		return l;
	}

	Duel::DVector3 DVector3::perpendicular( void )
	{
		static const DReal fSquareZero = (DReal)(1e-06 * 1e-06);

		DVector3 perp = this->crossProduct( DVector3::UNIT_X );

		// Check length
		if( perp.squaredLength() < fSquareZero )
		{
			/* This vector is the Y axis multiplied by a scalar, so we have
			to use another axis.
			*/
			perp = this->crossProduct( DVector3::UNIT_Y );
		}
		perp.normalize();

		return perp;
	}

	DQuaternion	DVector3::getRotationTo(const DVector3& dest, const DVector3& fallbackAxis/* = DVector3::ZERO*/) const
	{
		DQuaternion q;

		DVector3 v0 = *this;
		DVector3 v1 = dest;
		v0.normalize();
		v1.normalize();

		DReal d = v0.dotProduct(v1);

		if (d >= 1.0f)
		{
			return DQuaternion::IDENTITY;
		}
		if (d < (1e-6f-1.0f))
		{
			if (fallbackAxis != DVector3::ZERO)
			{
				// rotate 180 degrees about the fallback axis
				q.fromAngleAxis(DRadian(DMath::PI), fallbackAxis);
			}
			else
			{
				// Generate an axis
				DVector3 axis = DVector3::UNIT_X.crossProduct(*this);
				if (axis.isZeroLength()) // pick another if colinear
					axis = DVector3::UNIT_Y.crossProduct(*this);
				axis.normalize();
				q.fromAngleAxis(DRadian(DMath::PI), axis);
			}
		}
		else
		{
			DReal s = DMath::Sqrt((1+d)*2);
			DReal invs = 1/s;
			DVector3 c = v0.crossProduct(v1);

			q.x = c.x * invs;
			q.y = c.y * invs;
			q.z = c.z * invs;
			q.w = s * 0.5f;
			q.normalize();
		}
		return q;
	}

	DVector3::DVector3( const DVector4& rV ) :
		x(rV.x),
		y(rV.y),
		z(rV.z)
	{

	}

	DVector3& DVector3::operator=( const DVector4& rV )
	{
		x = rV.x; y = rV.y; z = rV.z;
		return *this;
	}

#pragma endregion

	DVector4& DVector4::operator=( const DVector4& rV )
	{
		x = rV.x; y = rV.y; z = rV.z; w = rV.w; return *this;
	}

	DVector4& DVector4::operator=( DReal scaler )
	{
		x = y = z = w = scaler; return *this;
	}

	const DVector4& DVector4::operator+() const
	{
		return *this;
	}

	Duel::DVector4 DVector4::operator+( const DVector4& rV )
	{
		return DVector4(x+rV.x, y+rV.y, z+rV.z, w+rV.w);
	}

	Duel::DVector4 DVector4::operator+( const DReal f )
	{
		return DVector4(x+f, y+f, z+f, w+f);
	}

	DVector4& DVector4::operator+=( const DVector4& rV )
	{
		x += rV.x; y += rV.y; z += rV.z; w += rV.w; return *this;
	}

	Duel::DVector4 DVector4::operator-()
	{
		return DVector4(-x, -y, -z, -w);
	}

	Duel::DVector4 DVector4::operator-( const DVector4& rV )
	{
		return DVector4(x-rV.x, y-rV.y, z-rV.z, w-rV.w);
	}

	Duel::DVector4 DVector4::operator-( const DReal f )
	{
		return DVector4(x-f, y-f, z-f, w-f);
	}

	DVector4& DVector4::operator-=( const DVector4& rV )
	{
		x -= rV.x; y -= rV.y; z -= rV.z; w -= rV.w; return *this;
	}

	Duel::DVector4 DVector4::operator*( const DVector4& rV )
	{
		return DVector4(x*rV.x, y*rV.y, z*rV.z, w*rV.w);
	}

	Duel::DVector4 DVector4::operator*( DReal scalar )
	{
		return DVector4(x*scalar, y*scalar, z*scalar, w*scalar);
	}

	DVector4 operator*( const DReal scalar, const DVector4& rV )
	{
		return DVector4(scalar * rV.x, scalar * rV.y, scalar * rV.z, scalar * rV.w);
	}

	DVector4& DVector4::operator*=( const DVector4& rV )
	{
		x *= rV.x; y *= rV.y; z *= rV.z; w *= rV.w; return *this;
	}

	DVector4& DVector4::operator*=( DReal scalar )
	{
		x *= scalar; y *= scalar; z *= scalar; w *= scalar; return *this;
	}

	Duel::DVector4 DVector4::operator/( const DVector4& rV )
	{
		return DVector4(x/rV.x, y/rV.y, z/rV.z, w/rV.w);
	}

	Duel::DVector4 DVector4::operator/( DReal scalar )
	{
		return DVector4(x/scalar, y/scalar, z/scalar, w/scalar);
	}

	DVector4 operator/( const DReal scalar, const DVector4& rV )
	{
		return DVector4(scalar / rV.x, scalar / rV.y, scalar / rV.z, scalar / rV.z);
	}

	DVector4& DVector4::operator/=( const DVector4& rV )
	{
		x /= rV.x; y /= rV.y; z /= rV.z; w /= rV.w; return *this;
	}

	DVector4& DVector4::operator/=( DReal scalar )
	{
		x /= scalar; y /= scalar; z /= scalar; w /= scalar; return *this;
	}

	DReal DVector4::dotProduct( const DVector4& rV )
	{
		return x * rV.x + y * rV.y + z * rV.z + w * rV.w;
	}

}