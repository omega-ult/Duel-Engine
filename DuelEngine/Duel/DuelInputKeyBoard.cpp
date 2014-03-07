//  [11/25/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelInputComponent.h"
#include "DuelInputKeyboard.h"
#include <ois-v1-3/includes/OIS.h>

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(DKeyEvent, DEventBase);

	DKeyEvent::DKeyEvent() :
		action(IEA_Pressed),
		key(KC_Unassigned),
		text(0)
	{

	}

	DKeyEvent::DKeyEvent( const OIS::KeyEvent& e, InputEventAction act ) :
		text(e.text), 
		action(act)
	{
		key = (KeyCode)(e.key);
	}


	DKeyEvent& DKeyEvent::operator=( const DKeyEvent& e )
	{
		key = e.key;
		text = e.text; 
		action = e.action;
		return *this;
	}



}