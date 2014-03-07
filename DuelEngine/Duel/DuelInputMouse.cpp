//  [11/25/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelInputMouse.h"
#include <ois-v1-3/includes/OIS.h>

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(DMouseEvent, DEventBase);

	DMouseEvent::DMouseEvent( const OIS::MouseEvent& e, InputEventAction t, MouseButtonID id ) :
		buttonID(id), 
		action(t)
	{
		state.height = e.state.height;
		state.width = e.state.width;
		state.X.rel = e.state.X.rel;
		state.X.abs = e.state.X.abs;
		state.X.absOnly = e.state.X.absOnly;
		state.Y.rel = e.state.Y.rel;
		state.Y.abs = e.state.Y.abs;
		state.Y.absOnly = e.state.Y.absOnly;
		state.Z.rel = e.state.Z.rel;
		state.Z.abs = e.state.Z.abs;
		state.Z.absOnly = e.state.Z.absOnly;
		state.buttonStates = e.state.buttons;
	}

	DMouseEvent::DMouseEvent() :
		action(IEA_Moved),
		buttonID(MBID_Left)
	{

	}

	DMouseEvent& DMouseEvent::operator=( const DMouseEvent& e )
	{
		state = e.state;
		buttonID = e.buttonID;
		action = e.action;
		return *this;
	}


	DMouseState::DMouseState() : 
		width(50), 
		height(50), 
		buttonStates(0)
	{

	}

	DMouseState& DMouseState::operator=( const DMouseState& s )
	{
		X = s.X;
		Y = s.Y;
		Z = s.Z;
		buttonStates =s.buttonStates;
		return *this;
	}

	void DMouseState::clear()
	{
		X.clear();
		Y.clear();
		Z.clear();
		buttonStates = 0;
	}

	bool DMouseState::isButtonDown( MouseButtonID button ) const
	{
		return ((buttonStates & ( 1L << button )) == 0) ? false : true;
	}


}