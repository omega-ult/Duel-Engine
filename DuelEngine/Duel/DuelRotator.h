//  [7/21/2012 OMEGA] created

#ifndef _DUELROTATOR_H_
#define _DUELROTATOR_H_

#include "DuelMath.h"

namespace Duel
{

	// DRotator work as Euler-Angle system to represent a rotation
	// using yaw-pitch-roll style
	//              (+)
	//         y(yaw)|  (+)|far|
	//               |  / z(roll)
	//               | /
	//               |/
	//      (-)------#--------(+) 
	//              /|       x (pitch)
	//             / |
	//            /  |
	//    |near|(-)  |
	//              (-)

	class DRotator
	{
	public:
		DRotator(){};
		~DRotator(){};

		// the number must clip to [-1,1];
		DRotator(DReal y, DReal p, DReal r);
		DRotator(const DDegree& y, const DDegree& p, const DDegree& r);
		DRotator(const DRadian& y, const DRadian& p, const DRadian& r);
		// use quaternion to construct a rotator
		DRotator(const DQuaternion& rQ);
		// use a rotation matrix to construct a rotator
		DRotator(const DMatrix3& rM);


		void		fromQuaternion(const DQuaternion& rQ);
		void		fromMatrix(const DMatrix3& rM);
		DMatrix3		toMatrix() const;
		DQuaternion	toQuaternion() const ;
		
		void	rotateYaw(const DRadian& y);
		void	rotatePitch(const DRadian& p);
		void	rotateRoll(const DRadian& r);

		static const DRotator IDENTITY;

	public:
		DRadian yaw, pitch, roll;
	};


}

#endif