//  [11/25/2013 OMEGA] created

#ifndef _DUELINPUTCOMPONENT_H_
#define _DUELINPUTCOMPONENT_H_

#include "DuelCommon.h"

// 使用一个adapter类型抽象OIS的这一套东西.
namespace Duel
{

	enum InputDeviceType
	{
		IDT_Unknown       = 0,
		IDT_Keyboard      = 1,
		IDT_Mouse         = 2,
		IDT_JoyStick      = 3,
		IDT_Tablet        = 4,
		IDT_MultiTouch    = 5
	};

	enum InputEventAction
	{
		IEA_Moved		= 0,
		IEA_Pressed		= 1,
		IEA_Released	= 2,

		IEA_JAxisMove	= 3,
		IEA_JPOVMove	= 4,
		IEA_JVec3Move	= 5
	};



	//! Base type for all device components (button, axis, etc)
	enum InputComponentType
	{
		ICT_Unknown = 0,
		ICT_Button  = 1, //ie. Key, mouse button, joy button, etc
		ICT_Axis    = 2, //ie. A joystick or mouse axis
		ICT_Slider  = 3, //
		ICT_POV     = 4, //ie. Arrow direction keys
		ICT_Vector3 = 5  //ie. WiiMote orientation
	};

	//! Base of all device components (button, axis, etc)
	class DUEL_API DInputComponent : public DObject
	{
		DUEL_DECLARE_RTTI(DInputComponent)
	public:
		DInputComponent() : cType(ICT_Unknown) {};
		DInputComponent(InputComponentType type) : cType(type) {};
		//! Indicates what type of coponent this is
		InputComponentType cType;
	};

	//! Button can be a keyboard key, mouse button, etc
	class DUEL_API DInputButton : public DInputComponent
	{
		DUEL_DECLARE_RTTI(DInputButton)
	public:
		DInputButton() : DInputComponent(ICT_Button), pushed(false) {}
		DInputButton(bool bPushed) : DInputComponent(ICT_Button), pushed(bPushed) {}
		//! true if pushed, false otherwise
		bool pushed;
	};

	//! Axis component
	class DUEL_API DInputAxis : public DInputComponent
	{
		DUEL_DECLARE_RTTI(DInputAxis)
	public:
		DInputAxis() : DInputComponent(ICT_Axis), abs(0), rel(0), absOnly(false) {};

		DInputAxis&		operator = (const DInputAxis& ia)
		{
			abs = ia.abs;
			rel = ia.rel;
			absOnly = ia.absOnly;
			return *this;
		}

		//! Absoulte and Relative value components
		int32 abs, rel;

		//! Indicates if this Axis only supports Absoulte (ie JoyStick)
		bool absOnly;

		//! Used internally by OIS
		void clear()
		{
			abs = rel = 0;
		}
	};

	//! A 3D Vector component (perhaps an orientation, as in the WiiMote)
	class DUEL_API DInputVector3 : public DInputComponent
	{
		DUEL_DECLARE_RTTI(DInputVector3)
	public:
		DInputVector3() {}
		DInputVector3(DReal _x, DReal _y, DReal _z) : DInputComponent(ICT_Vector3), x(_x), y(_y), z(_z) {};

		//! X component of vector
		DReal x;

		//! Y component of vector
		DReal y;

		//! Z component of vector
		DReal z;

		void clear()
		{
			x = y = z = 0.0f;
		}
	};

	//! POV / HAT Joystick component
	class DUEL_API DInputPOV : public DInputComponent
	{
		DUEL_DECLARE_RTTI(DInputPOV)
	public:
		DInputPOV() : DInputComponent(ICT_POV), direction(0) {}

		static const int32 Centered  = 0x00000000;
		static const int32 North     = 0x00000001;
		static const int32 South     = 0x00000010;
		static const int32 East      = 0x00000100;
		static const int32 West      = 0x00001000;
		static const int32 NorthEast = 0x00000101;
		static const int32 SouthEast = 0x00000110;
		static const int32 NorthWest = 0x00001001;
		static const int32 SouthWest = 0x00001010;

		int32 direction;
	};

	//! A sliding axis - only used in Win32 Right Now
	class DUEL_API DInputSlider : public DInputComponent
	{
		DUEL_DECLARE_RTTI(DInputSlider)
	public:
		DInputSlider() : DInputComponent(ICT_Slider), abX(0), abY(0) {};
		//! true if pushed, false otherwise
		int32 abX, abY;
	};

	// just a base class.
	class DUEL_API DEventBase : public DObject
	{
		DUEL_DECLARE_RTTI(DEventBase);
	public:
		DEventBase() {}
	};

}

#endif