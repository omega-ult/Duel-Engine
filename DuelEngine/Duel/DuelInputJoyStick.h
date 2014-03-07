//  [11/25/2013 OMEGA] created

#ifndef _DUELINPUTJOYSTICK_H_
#define _DUELINPUTJOYSTICK_H_

#include "DuelCommon.h"
#include "DuelInputComponent.h"
#include <ois-v1-3/includes/OIS.h>

namespace Duel
{
	/** @remarks default sensitivity for vector3 component of joystick */
#define DUEL_JOYSTICK_VECTOR3_DEFAULT 2.28f

	class DUEL_API DJoyStickState
	{
	public:
		//! Constructor
		DJoyStickState();

		//! Represents all the buttons (uses a bitset)
		std::vector<bool> mButtons;

		//! Represents all the single axes on the device
		std::vector<DInputAxis> mAxes;

		//! Represents the value of a POV. Maximum of 4
		DInputPOV	 mPOV[4];

		//! Represent the max sliders
		DInputSlider mSliders[4];

		//! Represents all Vector type controls the device exports
		std::vector<DInputVector3> mVectors;

		//! internal method to reset all variables to initial values
		void clear();
	};

	class DUEL_API DJoyStickEvent : public DEventBase
	{
		DUEL_DECLARE_RTTI(DJoyStickEvent);
	public:
		DJoyStickEvent();
		DJoyStickEvent(const OIS::JoyStickEvent& e, InputEventAction t, uint16 devI, int32 inputI);
		DJoyStickEvent& operator = (const DJoyStickEvent& event);
		DJoyStickState		state;
		int32				inputIndex;		// index for button/axis/pov/vec3.
		uint16				deviceIndex;	// multi device index.
		InputEventAction	action;
	};

}

#endif // !_DUELINPUTJOYSTICK_H_
