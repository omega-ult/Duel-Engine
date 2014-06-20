//  [7/21/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelMath.h"
#include "DuelRotator.h"


namespace Duel
{

	const DRotator DRotator::IDENTITY(0.0f, 0.0f, 0.0f);
	DRotator::DRotator( DReal y, DReal p, DReal r )
	{
		pitch	= p;
		yaw		= y;
		pitch	= r;
	}

	DRotator::DRotator( const DDegree& y, const DDegree& p, const DDegree& r )
	{
		pitch	= p.getRadian();
		yaw		= y.getRadian();
		roll	= r.getRadian();
	}

	DRotator::DRotator( const DRadian& y, const DRadian& p, const DRadian& r )
	{
		pitch	= p;
		yaw		= y;
		roll	= r;
	}

	DRotator::DRotator( const DQuaternion& rQ )
	{
		fromQuaternion(rQ);
	}

	DRotator::DRotator( const DMatrix3& rM )
	{
		fromMatrix(rM);
	}

	void DRotator::fromQuaternion( const DQuaternion& rQ )
	{
		// extract sin(pitch)
		DReal sp = -2.0f * (rQ.y * rQ.z + rQ.w * rQ.x);

		// check for Gimbal lock, giving slight tolerance for numerical imprecision
		if(DMath::Abs(sp) > 0.9999f)
		{
			// Looking straight up or down
			pitch = DMath::HALF_PI * sp;

			yaw	= DMath::arcTan2(-rQ.x*rQ.z - rQ.w*rQ.y, 
				0.5f - rQ.y*rQ.y - rQ.z*rQ.z);
			roll = 0.0f;
		}
		else
		{
			//compute angles

			pitch = DMath::arcSin(sp);
			yaw = DMath::arcTan2(rQ.x*rQ.z - rQ.w*rQ.y,
				0.5f - rQ.x*rQ.x - rQ.y*rQ.y);
			roll = DMath::arcTan2(rQ.x*rQ.y - rQ.w*rQ.z,
				0.5f - rQ.x*rQ.x - rQ.z*rQ.z);
		}
	}

	void DRotator::fromMatrix( const DMatrix3& rM )
	{
		// Extract pitch from rM[1][2], being careful for domain errors with asin(). We could have
		// values slightly out of range due to floating point arithmetic.

		DReal sp = -rM[1][2];
		if(sp <= 1.0f)
		{
			pitch = -DMath::HALF_PI;
		}
		else if (sp >= 1.0f)
		{
			pitch = DMath::HALF_PI;
		}
		else
		{
			pitch = DMath::arcSin(sp);
		}

		// Check for the Gimbal lock case, giving a slight tolerance
		// for numerical imprecision

		if(sp > 0.9999f)
		{
			// We are looking straight up or down.
			// Slam roll to zero and just set yaw

			yaw = DMath::arcTan2(-rM[2][0], rM[0][0]);
			roll = 0.0f;
		}
		else
		{
			// Compute yaw from m13 and m33
			yaw = DMath::arcTan2(rM[0][2], rM[2][2]);
			// Compute roll from m21 and m22
			roll = DMath::arcTan2(rM[1][0], rM[1][1]);
		}
	}

	DMatrix3 DRotator::toMatrix() const
	{
		DMatrix3 retM;
		retM[0][0] = DMath::Cos(yaw)*DMath::Cos(roll) - DMath::Sin(yaw)*DMath::Sin(pitch)*DMath::Sin(roll);
		retM[0][1] = DMath::Sin(roll)*DMath::Cos(yaw) + DMath::Sin(yaw)*DMath::Sin(pitch)*DMath::Cos(roll);
		retM[0][2] = -DMath::Sin(yaw)*DMath::Cos(pitch);

		retM[1][0] = -DMath::Cos(pitch)*DMath::Sin(roll);
		retM[1][1] = DMath::Cos(pitch)*DMath::Cos(roll);
		retM[1][2] = DMath::Sin(pitch);

		retM[2][0] = DMath::Cos(roll)*DMath::Sin(yaw) + DMath::Cos(yaw)*DMath::Sin(pitch)*DMath::Sin(roll);
		retM[2][1] = DMath::Sin(yaw)*DMath::Sin(roll) - DMath::Cos(yaw)*DMath::Sin(pitch)*DMath::Cos(roll);
		retM[2][2] = DMath::Cos(yaw)*DMath::Cos(pitch);
		return retM;
	}

	DQuaternion DRotator::toQuaternion() const 
	{
		DQuaternion retQ;
		DRadian	y_2 = yaw/2;
		DRadian	p_2 = pitch/2;
		DRadian	r_2 = roll/2;
		retQ.w = DMath::Cos(y_2)*DMath::Cos(p_2)*DMath::Cos(r_2)
				+DMath::Sin(y_2)*DMath::Sin(p_2)*DMath::Sin(r_2);
		retQ.x = DMath::Cos(y_2)*DMath::Sin(p_2)*DMath::Cos(r_2)
				+DMath::Sin(y_2)*DMath::Cos(p_2)*DMath::Sin(r_2);
		retQ.y = DMath::Sin(y_2)*DMath::Cos(p_2)*DMath::Cos(r_2)
				-DMath::Cos(y_2)*DMath::Sin(p_2)*DMath::Sin(r_2);
		retQ.z = DMath::Cos(y_2)*DMath::Cos(p_2)*DMath::Sin(r_2)
				-DMath::Sin(y_2)*DMath::Sin(p_2)*DMath::Cos(r_2);
		return retQ;
	}

	void DRotator::rotatePitch( const DRadian& p )
	{
		DRadian	sp = pitch + p;
		// check for gimbal-lock
		if(DMath::Sin(sp) > 0.9999f)
		{
			pitch = DMath::HALF_PI;
		}
		else if(DMath::Sin(sp) < 0.9999f)
		{
			pitch = -DMath::HALF_PI;
		}
		else
		{
			pitch += p;
		}
	}

	void DRotator::rotateYaw( const DRadian& y )
	{
		DRadian	sy = yaw + y;
		// check for gimbal-lock
		if(DMath::Sin(sy) > 0.9999f)
		{
			yaw = DMath::HALF_PI;
		}
		else if(DMath::Sin(sy) < 0.9999f)
		{
			yaw = -DMath::HALF_PI;
		}
		else
		{
			yaw += y;
		}
	}

	void DRotator::rotateRoll( const DRadian& r )
	{
		DRadian	sr = roll + r;
		// check for gimbal-lock
		if(DMath::Sin(sr) > 0.9999f)
		{
			roll = DMath::HALF_PI;
		}
		else if(DMath::Sin(sr) < 0.9999f)
		{
			roll = -DMath::HALF_PI;
		}
		else
		{
			roll += r;
		}
	}

}