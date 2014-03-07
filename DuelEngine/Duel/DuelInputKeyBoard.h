//  [11/25/2013 OMEGA] created

#ifndef _DUELINPUTKEYBOARD_H_
#define _DUELINPUTKEYBOARD_H_

#include "DuelCommon.h"
#include "DuelInputComponent.h"
#include <ois-v1-3/includes/OIS.h>

namespace Duel
{

	enum KeyCode
	{
		KC_Unassigned  = 0x00,
		KC_Escape      = 0x01,
		KC_1           = 0x02,
		KC_2           = 0x03,
		KC_3           = 0x04,
		KC_4           = 0x05,
		KC_5           = 0x06,
		KC_6           = 0x07,
		KC_7           = 0x08,
		KC_8           = 0x09,
		KC_9           = 0x0A,
		KC_0           = 0x0B,
		KC_Minus       = 0x0C,    // - on main keyboard
		KC_Equals      = 0x0D,
		KC_Back        = 0x0E,    // backspace
		KC_Tab         = 0x0F,
		KC_Q           = 0x10,
		KC_W           = 0x11,
		KC_E           = 0x12,
		KC_R           = 0x13,
		KC_T           = 0x14,
		KC_Y           = 0x15,
		KC_U           = 0x16,
		KC_I           = 0x17,
		KC_O           = 0x18,
		KC_P           = 0x19,
		KC_LBracket    = 0x1A,
		KC_RBracket    = 0x1B,
		KC_Return      = 0x1C,    // Enter on main keyboard
		KC_LControl    = 0x1D,
		KC_A           = 0x1E,
		KC_S           = 0x1F,
		KC_D           = 0x20,
		KC_F           = 0x21,
		KC_G           = 0x22,
		KC_H           = 0x23,
		KC_J           = 0x24,
		KC_K           = 0x25,
		KC_L           = 0x26,
		KC_Semicolon   = 0x27,
		KC_Apostrophe  = 0x28,
		KC_Grave       = 0x29,    // accent
		KC_LShift      = 0x2A,
		KC_Backslash   = 0x2B,
		KC_Z           = 0x2C,
		KC_X           = 0x2D,
		KC_C           = 0x2E,
		KC_V           = 0x2F,
		KC_B           = 0x30,
		KC_N           = 0x31,
		KC_M           = 0x32,
		KC_Comma       = 0x33,
		KC_Period      = 0x34,    // . on main keyboard
		KC_Slash       = 0x35,    // / on main keyboard
		KC_RShift      = 0x36,
		KC_Multiply    = 0x37,    // * on numeric keypad
		KC_LMenu       = 0x38,    // left Alt
		KC_Space       = 0x39,
		KC_Capital     = 0x3A,
		KC_F1          = 0x3B,
		KC_F2          = 0x3C,
		KC_F3          = 0x3D,
		KC_F4          = 0x3E,
		KC_F5          = 0x3F,
		KC_F6          = 0x40,
		KC_F7          = 0x41,
		KC_F8          = 0x42,
		KC_F9          = 0x43,
		KC_F10         = 0x44,
		KC_NumLock     = 0x45,
		KC_Scroll      = 0x46,    // Scroll Lock
		KC_NumPad7     = 0x47,
		KC_NumPad8     = 0x48,
		KC_NumPad9     = 0x49,
		KC_SUBTRACT    = 0x4A,    // - on numeric keypad
		KC_NumPad4     = 0x4B,
		KC_NumPad5     = 0x4C,
		KC_NumPad6     = 0x4D,
		KC_Add         = 0x4E,    // + on numeric keypad
		KC_NumPad1     = 0x4F,
		KC_NumPad2     = 0x50,
		KC_NumPad3     = 0x51,
		KC_NumPad0     = 0x52,
		KC_Decimal     = 0x53,    // . on numeric keypad
		KC_Oem_102     = 0x56,    // < > | on UK/Germany keyboards
		KC_F11         = 0x57,
		KC_F12         = 0x58,
		KC_F13         = 0x64,    //                     (NEC PC98)
		KC_F14         = 0x65,    //                     (NEC PC98)
		KC_F15         = 0x66,    //                     (NEC PC98)
		KC_Kana        = 0x70,    // (Japanese keyboard)
		KC_ABNT_C1     = 0x73,    // / ? on Portugese (Brazilian) keyboards
		KC_Convert     = 0x79,    // (Japanese keyboard)
		KC_NoConvert   = 0x7B,    // (Japanese keyboard)
		KC_Yen         = 0x7D,    // (Japanese keyboard)
		KC_ABNT_C2     = 0x7E,    // Numpad . on Portugese (Brazilian) keyboards
		KC_NumPadEquals= 0x8D,    // = on numeric keypad (NEC PC98)
		KC_PrevTrack   = 0x90,    // Previous Track (KC_CIRCUMFLEX on Japanese keyboard)
		KC_At          = 0x91,    //                     (NEC PC98)
		KC_Colon       = 0x92,    //                     (NEC PC98)
		KC_Underline   = 0x93,    //                     (NEC PC98)
		KC_KanJi       = 0x94,    // (Japanese keyboard)
		KC_Stop        = 0x95,    //                     (NEC PC98)
		KC_Ax          = 0x96,    //                     (Japan AX)
		KC_UnLabeled   = 0x97,    //                        (J3100)
		KC_NextTrack   = 0x99,    // Next Track
		KC_NumPadEnter = 0x9C,    // Enter on numeric keypad
		KC_RControl    = 0x9D,
		KC_Mute        = 0xA0,    // Mute
		KC_Calculator  = 0xA1,    // Calculator
		KC_PlayPause   = 0xA2,    // Play / Pause
		KC_MediaStop   = 0xA4,    // Media Stop
		KC_VolumeDown  = 0xAE,    // Volume -
		KC_VolumeUp    = 0xB0,    // Volume +
		KC_WebHome     = 0xB2,    // Web home
		KC_NumPadComma = 0xB3,    // , on numeric keypad (NEC PC98)
		KC_Divide      = 0xB5,    // / on numeric keypad
		KC_SysRq       = 0xB7,
		KC_RMenu       = 0xB8,    // right Alt
		KC_Pause       = 0xC5,    // Pause
		KC_Home        = 0xC7,    // Home on arrow keypad
		KC_Up          = 0xC8,    // UpArrow on arrow keypad
		KC_PageUp      = 0xC9,    // PgUp on arrow keypad
		KC_Left        = 0xCB,    // LeftArrow on arrow keypad
		KC_Right       = 0xCD,    // RightArrow on arrow keypad
		KC_End         = 0xCF,    // End on arrow keypad
		KC_Down        = 0xD0,    // DownArrow on arrow keypad
		KC_PageDown    = 0xD1,    // PgDn on arrow keypad
		KC_Insert      = 0xD2,    // Insert on arrow keypad
		KC_Delete      = 0xD3,    // Delete on arrow keypad
		KC_LWin        = 0xDB,    // Left Windows key
		KC_RWin        = 0xDC,    // Right Windows key
		KC_AppMenu     = 0xDD,    // AppMenu key
		KC_Power       = 0xDE,    // System Power
		KC_Sleep       = 0xDF,    // System Sleep
		KC_Wake        = 0xE3,    // System Wake
		KC_WebSearch   = 0xE5,    // Web Search
		KC_WebFavorites= 0xE6,    // Web Favorites
		KC_WebRefresh  = 0xE7,    // Web Refresh
		KC_WebStop     = 0xE8,    // Web Stop
		KC_WebForward  = 0xE9,    // Web Forward
		KC_WebBack     = 0xEA,    // Web Back
		KC_MyComputer  = 0xEB,    // My Computer
		KC_Mail        = 0xEC,    // Mail
		KC_MediaSelect = 0xED     // Media Select
	};

	class DUEL_API DKeyEvent : public DEventBase
	{
		DUEL_DECLARE_RTTI(DKeyEvent);
	public:
		DKeyEvent();
		DKeyEvent(const OIS::KeyEvent& e, InputEventAction act);
		DKeyEvent& operator = (const DKeyEvent& event);
		//! KeyCode of event
		KeyCode				key;
		//! Text character, depends on current TextTranslationMode
		uint32				text;
		InputEventAction	action;
	};
}


#endif