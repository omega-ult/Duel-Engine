//  [7/15/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelQuaternion.h"
#include "DuelMath.h"
#include "DuelVector.h"
#include "DuelAngle.h"

#pragma warning(push)
// double to float casting warning
#pragma warning(disable : 4244)

namespace Duel{

	const DReal DQuaternion::EPSILON = (DReal)1e-03;
	const DQuaternion DQuaternion::ZERO(0.0f, 0.0f, 0.0f, 0.0f);
	const DQuaternion DQuaternion::IDENTITY(1.0f, 0.0f, 0.0f, 0.0f);

	DQuaternion DQuaternion::operator * (const DQuaternion& rQ) const
	{
		//////////////////////////////////////////////////////////////////////////
		// 使用了从左到右的乘法, 原始算法见下注释 [11/23/2013 OMEGA]
		return DQuaternion
		(
			w * rQ.w - x * rQ.x - y * rQ.y - z * rQ.z,
            w * rQ.x + x * rQ.w + z * rQ.y - y * rQ.z,	// diff 使用从左到右的乘法//  [11/23/2013 OMEGA]
            w * rQ.y + y * rQ.w + x * rQ.z - z * rQ.x,	// diff 使用从左到右的乘法//  [11/23/2013 OMEGA]
            w * rQ.z + z * rQ.w + y * rQ.x - x * rQ.y	// diff 使用从左到右的乘法//  [11/23/2013 OMEGA]
		);
// 		return DQuaternion
// 		(
// 			w * rQ.w - x * rQ.x - y * rQ.y - z * rQ.z,
//             w * rQ.x + x * rQ.w + y * rQ.z - z * rQ.y,
//             w * rQ.y + y * rQ.w + z * rQ.x - x * rQ.z,
//             w * rQ.z + z * rQ.w + x * rQ.y - y * rQ.x
// 		);
}

	DReal DQuaternion::dotProduct(const DQuaternion& rQ) const
	{
		return w*rQ.w + x*rQ.x + y*rQ.y + z*rQ.z;
	}

	DReal DQuaternion::normalize()
	{
		DReal length = DMath::Sqrt(w*w + x*x + y*y + z*z);
		w /= length;
		x /= length;
		y /= length;
		z /= length;

		return length;

	}

	DQuaternion DQuaternion::getConjugate() const
	{
		DReal fNorm = w*w + x*x + y*y + z*z;
		if( fNorm > 0.0 )
		{
			return DQuaternion(w/fNorm, -x/fNorm, -y/fNorm, -z/fNorm);
		}
		else
		{
			// invalid result
			return ZERO;
		}
	}

	DVector3 DQuaternion::operator * (const DVector3& rV) const
	{
		// nVidia SDK implementation
		DVector3 v(rV);
		DVector3 uv, uuv;
		DVector3 qvec(x, y, z);
		uv = qvec.crossProduct(v);
		uuv = qvec.crossProduct(uv);
		uv *= (2.0f * w);
		uuv *= 2.0f;

		return v + uv + uuv;
	}

	bool DQuaternion::equals(const DQuaternion& rQ, const DRadian& tolerance)
	{
		DReal fCos = dotProduct(rQ);
		DRadian a = DMath::arcCos(fCos);

		return (DMath::Abs(a.getRadian()) <= tolerance.getRadian()) 
			|| (DMath::realEqual(a.getRadian(), DMath::PI, tolerance.getRadian()));
	}

	DQuaternion DQuaternion::log() const
	{
		// If q = cos(A)+sin(A)*(x*i+y*j+z*k) where (x,y,z) is unit length, then
		// log(q) = A*(x*i+y*j+z*k).  If sin(A) is near zero, use log(q) =
		// sin(A)*(x*i+y*j+z*k) since sin(A)/A has limit 1.

		DQuaternion kResult;
		kResult.w = 0.0;

		if ( DMath::Abs(w) < 1.0 )
		{
			DRadian fAngle ( DMath::arcCos(w) );
			DReal fSin = DMath::Sin(fAngle);
			if ( DMath::Abs(fSin) >= 1e-03 )
			{
				DReal fCoeff = fAngle.getRadian()/fSin;
				kResult.x = fCoeff*x;
				kResult.y = fCoeff*y;
				kResult.z = fCoeff*z;
				return kResult;
			}
		}

		kResult.x = x;
		kResult.y = y;
		kResult.z = z;

		return kResult;
	}

	DQuaternion DQuaternion::exp() const
	{
		// If q = A*(x*i+y*j+z*k) where (x,y,z) is unit length, then
		// exp(q) = cos(A)+sin(A)*(x*i+y*j+z*k).  If sin(A) is near zero,
		// use exp(q) = cos(A)+A*(x*i+y*j+z*k) since A/sin(A) has limit 1.

		DRadian fAngle ( DMath::Sqrt(x*x+y*y+z*z) );
		DReal fSin = DMath::Sin(fAngle);

		DQuaternion kResult;
		kResult.w = DMath::Cos(fAngle);

		if ( DMath::Abs(fSin) >= 1e-03 )
		{
			DReal fCoeff = fSin/(fAngle.getRadian());
			kResult.x = fCoeff*x;
			kResult.y = fCoeff*y;
			kResult.z = fCoeff*z;
		}
		else
		{
			kResult.x = x;
			kResult.y = y;
			kResult.z = z;
		}

		return kResult;
	}

	DQuaternion DQuaternion::Slerp(DReal f, const DQuaternion& rSrcQ,
				const DQuaternion& rDstQ, bool shortestPath)
	{
		DReal fCos = rSrcQ.dotProduct(rDstQ);
		DQuaternion t;

		// Do we need to invert rotation?
		if(fCos <= 0.0f && shortestPath)
		{
			fCos = -fCos;
			t = -rDstQ;
		}
		else
		{
			t = rDstQ;
		}

		if(DMath::Abs(fCos) < 1-EPSILON)
		{
			DReal fSin = DMath::Sqrt(1 - DMath::Sqr(fCos));
			DRadian a = DMath::arcTan2(fSin, fCos);
			DReal fCoeff0 = DMath::Sin(a * (1-f))/fSin;
			DReal fCoeff1 = DMath::Sin(a * f)/fSin;

			return rSrcQ*fCoeff0 + t*fCoeff1;
		}
		else
		{
			// There are two situations:
            // 1. "rSrcQ" and "rDstQ" are very close (fCos ~= +1), so we can do a linear
            //    interpolation safely.
            // 2. "rSrcP" and "rDstQ" are almost inverse of each other (fCos ~= -1), there
            //    are an infinite number of possibilities interpolation. but we haven't
            //    have method to fix this case, so just use linear interpolation here.
            DQuaternion d = rSrcQ*(1.0f - f) + t * f;
            // taking the complement requires renormalisation
			d.normalize();
            return d;
		}

	}

	DQuaternion DQuaternion::SlerpExtraSpins(DReal f, const DQuaternion& rSrcQ,
				const DQuaternion& rDstQ, int iExtraSpins)
	{
		DReal fCos = rSrcQ.dotProduct(rDstQ);
		DRadian a = (DMath::arcCos(fCos));

		if(DMath::Abs(a.getRadian() < EPSILON))
		{
			return rDstQ;
		}
		else
		{
			DReal fSin = DMath::Sin(a);
			DRadian fPhase = DMath::PI * iExtraSpins * f;
			DReal fCoeff0 = DMath::Sin(a*(1.0f - f) - fPhase)/fSin;
			DReal fCoeff1 = DMath::Sin(a*f + fPhase)/fSin;
			return rSrcQ*fCoeff0 + rDstQ*fCoeff1;
		}
	}

	void DQuaternion::Intermediate( const DQuaternion& rkQ0, const DQuaternion& rkQ1, const DQuaternion& rkQ2, DQuaternion& rkA, DQuaternion& rkB )
	{
		// assert:  q0, q1, q2 are unit quaternions
		DQuaternion tempQ0 = rkQ0;
		DQuaternion tempQ1 = rkQ1;
		tempQ0.normalize();
		tempQ1.normalize();
		DQuaternion kQ0inv = tempQ0.getConjugate();
		DQuaternion kQ1inv = tempQ1.getConjugate();
		DQuaternion rkP0 = kQ0inv*rkQ1;
		DQuaternion rkP1 = kQ1inv*rkQ2;
		DQuaternion kArg = 0.25*(rkP0.log()-rkP1.log());
		DQuaternion kMinusArg = -kArg;

		rkA = rkQ1*kArg.exp();
		rkB = rkQ1*kMinusArg.exp();
	}

	DQuaternion DQuaternion::Squad( DReal fT, const DQuaternion& rkP, const DQuaternion& rkA, const DQuaternion& rkB, const DQuaternion& rkQ, bool shortestPath /*= false*/ )
	{
		DReal fSlerpT = 2.0f*fT*(1.0f-fT);
		DQuaternion kSlerpP = Slerp(fT, rkP, rkQ, shortestPath);
		DQuaternion kSlerpQ = Slerp(fT, rkA, rkB);
		return Slerp(fSlerpT, kSlerpP ,kSlerpQ);
	}

	DQuaternion DQuaternion::nLerp(DReal f, const DQuaternion& rSrcQ,
				const DQuaternion& rDstQ, bool shortestPath)
	{
		DQuaternion a;
		DReal fCos = rSrcQ.dotProduct(rDstQ);
		if( fCos < 0.0f && shortestPath )
		{
			a = rSrcQ + ((-rDstQ) - rSrcQ) * f;
		}
		else
		{
			a = rSrcQ + ((rDstQ) - rSrcQ) * f;
		}
		a.normalize();
		return a;
	}

	DRadian DQuaternion::getRoll()
	{
		// extrack sin(pitch)
		DReal sp = -2.0f * (y*z + w*x);

		// check for gimbal lock
		if(DMath::Abs(sp) > 0.9999f)
		{
			// slam bank to zero
			return DRadian(0.0f);
		}
		else
		{
			return DRadian(DMath::arcTan2(x*y - w*z, 0.5f - x*x - z*z));
		}

	}

	DRadian DQuaternion::getPitch()
	{
		// extrack sin(pitch)
		DReal sp = -2.0f * (y*z + w*x);

		// check for gimbal lock
		if(DMath::Abs(sp) > 0.9999f)
		{
			// looking strait up or down
			return DRadian( DMath::HALF_PI * sp );
		}
		else
		{
			return DRadian(DMath::arcSin(sp));
		}


	}

	DRadian DQuaternion::getYaw()
	{

		// extrack sin(pitch)
		DReal sp = -2.0f * (y*z + w*x);

		// check for gimbal lock
		if(DMath::Abs(sp) > 0.9999f)
		{
			return DRadian(DMath::arcTan2( -x*z - w*y, 0.5f - y*y - z*z));
		}
		else
		{
			return DRadian(DMath::arcTan2(x*z - w*y, 0.5f - x*x - y*y));
		}
		

	}

	void DQuaternion::fromRotationMatrix(const DMatrix3& rM)
	{
		// algorithm from OSG
		//DReal s;
		int  i, j;

		// use tor store the largest trace
		DReal tq[4];
		tq[0] = 1 + rM[0][0] + rM[1][1] + rM[2][2];
		tq[1] = 1 + rM[0][0] - rM[1][1] - rM[2][2];
		tq[2] = 1 - rM[0][0] + rM[1][1] - rM[2][2];
		tq[3] = 1 - rM[0][0] - rM[1][1] + rM[2][2];
		
		// find the maximum index (could also use stacked if's later)
		for(i = 1, j = 0; i < 4; ++i)
		{
			j = (tq[i] > tq[j]) ? i : j;
		}
		
		float biggestVal = sqrt(tq[j]) * 0.5f;
		float mult = 0.25f / biggestVal;

		switch(j)
		{
		//the diagonal
		case 0:
			// instant calculation
			w = biggestVal;
			x = mult*(rM[2][1] - rM[1][2]);
			y = mult*(rM[0][2] - rM[2][0]);
			z = mult*(rM[1][0] - rM[0][1]);
			break;
		case 1:
			w = mult*(rM[2][1] - rM[1][2]);
			x = biggestVal;
			y = mult*(rM[1][0] + rM[0][1]);
			z = mult*(rM[0][2] + rM[2][0]);
			break;
		case 2:
			w = mult*(rM[0][2] - rM[2][0]);
			x = mult*(rM[1][0] + rM[0][1]);
			y = biggestVal;
			z = mult*(rM[2][1] + rM[1][2]);
			break;
		case 3:
			w = mult*(rM[1][0] - rM[0][1]);
			x = mult*(rM[0][2] + rM[2][0]);
			y = mult*(rM[2][1] + rM[1][2]);
			z = biggestVal;
			break;
		}
		
	}

	void DQuaternion::toRotationMatrix( DMatrix3& rM) const
	{

		DReal x2  = x+x;
		DReal y2  = y+y;
		DReal z2  = z+z;

		DReal wx = x2*w;
		DReal wy = y2*w;
		DReal wz = z2*w;

		DReal xx = x2*x;
		DReal xy = y2*x;
		DReal xz = z2*x;

		DReal yy = y2*y;
		DReal yz = z2*y;

		DReal zz = z2*z;
// 		if (rt == RT_Inertial)
// 		{
// 			// from Inertial To Object
// 			rM[0][0] = 1.0f-(yy+zz);
// 			rM[1][0] = xy+wz;
// 			rM[2][0] = xz-wy;
// 
// 			rM[0][1] = xy-wz;
// 			rM[1][1] = 1.0f-(xx+zz);
// 			rM[2][1] = yz+wx;
// 
// 			rM[0][2] = xz+wy;
// 			rM[1][2] = yz-wx;
// 			rM[2][2] = 1.0f-(xx+yy);
// 
// 		}
// 		else // RT_Object
// 		{
			// from Object To Inertial	
			rM[0][0] = 1.0f-(yy+zz);
			rM[1][0] = xy-wz;
			rM[2][0] = xz+wy;

			rM[0][1] = xy+wz;
			rM[1][1] = 1.0f-(xx+zz);
			rM[2][1] = yz-wx;

			rM[0][2] = xz-wy;
			rM[1][2] = yz+wx;
			rM[2][2] = 1.0f-(xx+yy);
// 
// 		}
	}

	void DQuaternion::fromAngleAxis (const DRadian& rR, const DVector3& rV)
    {
        // assert:  axis[] is unit length
        //
        // The quaternion representing the rotation is
        //   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

        DRadian fHalfAngle ( rR*0.5 );
        DReal fSin = DMath::Sin(fHalfAngle);
        w = DMath::Cos(fHalfAngle);
        x = fSin*rV.x;
        y = fSin*rV.y;
        z = fSin*rV.z;
    }

	void DQuaternion::toAngleAxis(DRadian& rR, DVector3& rV) const
    {
        // The quaternion representing the rotation is
        //   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

        DReal fSqrLength = x*x+y*y+z*z;
        if ( fSqrLength > 0.0 )
        {
            rR = DMath::arcCos(w)*2.0;
            DReal fInvLength = DMath::invSqrt(fSqrLength);
            rV.x = x*fInvLength;
            rV.y = y*fInvLength;
            rV.z = z*fInvLength;
        }
        else
        {
            // angle is 0 (mod 2*pi), so any axis will do
            rR = DRadian(0.0);
            rV.x = 1.0;
            rV.y = 0.0;
            rV.z = 0.0;
        }
	}



	void DQuaternion::toRotator( DRotator& rR ) const
	{
		// extrack sin(pitch)
		DReal sp = -2.0f * (y*z + w*x);

		// check for gimbal lock
		if(DMath::Abs(sp) > 0.9999f)
		{
			rR.yaw = DRadian(DMath::arcTan2( -x*z - w*y, 0.5f - y*y - z*z));
			rR.pitch = DRadian( DMath::HALF_PI * sp );
			rR.roll = DRadian(0.0f);
		}
		else
		{
			rR.yaw = DRadian(DMath::arcTan2(x*z - w*y, 0.5f - x*x - y*y));
			rR.pitch = DRadian(DMath::arcTan2(x*y - w*z, 0.5f - x*x - z*z));
			rR.roll = DRadian(DMath::arcTan2(x*y - w*z, 0.5f - x*x - z*z));
		}

	}


	void DQuaternion::swap( DQuaternion& rQ )
	{
		DReal temp;
		temp = rQ.w;
		rQ.w = w;
		w = temp;

		temp = rQ.x;
		rQ.x = x;
		x = temp;

		temp = rQ.y;
		rQ.y = y;
		y = temp;

		temp = rQ.z;
		rQ.z = z;
		z = temp;
	}

	void DQuaternion::toAxes( DVector3& xVec, DVector3& yVec, DVector3& zVec ) const
	{
		DMatrix3 mat;
		toRotationMatrix(mat);
		xVec.x = mat[0][0]; xVec.y = mat[1][0]; xVec.z = mat[2][0];
		yVec.x = mat[0][1]; yVec.y = mat[1][1]; yVec.z = mat[2][1];
		zVec.x = mat[0][2]; zVec.y = mat[1][2]; zVec.z = mat[2][2];
	}

	void DQuaternion::fromAxes( const DVector3& xVec, const DVector3& yVec, const DVector3& zVec )
	{
		DMatrix3 rot;

		rot[0][0] = xVec.x; rot[1][0] = xVec.y;	rot[2][0] = xVec.z;
		rot[0][1] = yVec.x;	rot[1][1] = yVec.y;	rot[2][1] = yVec.z;
		rot[0][2] = zVec.x;	rot[1][2] = zVec.y;	rot[2][2] = zVec.z;

		fromRotationMatrix(rot);
	}

}

#pragma warning(pop)