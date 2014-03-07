//  [11/25/2013 OMEGA] created

#ifndef _DUELINPUTMOUSE_H_
#define _DUELINPUTMOUSE_H_

#include "DuelCommon.h"
#include "DuelInputComponent.h"
#include <ois-v1-3/includes/OIS.h>


namespace Duel
{

	// same as OIS::MouseButtonID
	enum MouseButtonID
	{
		MBID_Left		= 0,
		MBID_Right		= 1,
		MBID_Middle		= 3,
		MBID_Button3	= 4,
		MBID_Button4	= 5,
		MBID_Button5	= 6,
		MBID_Button6	= 7,
		MBID_Button7	= 8
	};

	class DUEL_API DMouseState
	{
	public:
		DMouseState();

		DMouseState&	operator=(const DMouseState& s);
		/** Represents the height/width of your display area.. used if mouse clipping
		or mouse grabbed in case of X11 - defaults to 50.. Make sure to set this
		and change when your size changes.. */
		mutable int32 width, height;

		//! X Axis component
		DInputAxis X;

		//! Y Axis Component
		DInputAxis Y;

		//! Z Axis Component
		DInputAxis Z;

		//! represents all buttons - bit position indicates button down
		int32 buttonStates;

		//! Button down test
		bool isButtonDown( MouseButtonID button ) const;

		//! Clear all the values
		void clear();
	};

	class DUEL_API DMouseEvent : public DEventBase
	{
		DUEL_DECLARE_RTTI(DMouseEvent);
	public:
		DMouseEvent();
		DMouseEvent(const OIS::MouseEvent& e, InputEventAction t, MouseButtonID id);
		DMouseEvent& operator = (const DMouseEvent& event);
		DMouseState			state;
		InputEventAction	action;
		MouseButtonID		buttonID;
	};
}

#endif