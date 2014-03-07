//  [11/25/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelInputJoyStick.h"
#include <ois-v1-3/includes/OIS.h>

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DJoyStickEvent, DEventBase);

	DJoyStickState::DJoyStickState()
	{
		clear();
	}

	void DJoyStickState::clear()
	{
		for( std::vector<bool>::iterator i = mButtons.begin(), e = mButtons.end(); i != e; ++i )
		{
			(*i) = false;
		}

		for( std::vector<DInputAxis>::iterator i = mAxes.begin(), e = mAxes.end(); i != e; ++i )
		{
			i->absOnly = true; //Currently, joysticks only report Absolute values
			i->clear();
		}

		for( std::vector<DInputVector3>::iterator i = mVectors.begin(), e = mVectors.end(); i != e; ++i )
		{
			i->clear();
		}

		for( int32 i = 0; i < 4; ++i )
		{
			mPOV[i].direction = DInputPOV::Centered;
			mSliders[i].abX = mSliders[i].abY = 0;
		}
	}



	DJoyStickEvent::DJoyStickEvent() :
		action(IEA_Pressed),
		inputIndex(0),
		deviceIndex(0)
	{

	}

	DJoyStickEvent::DJoyStickEvent( const OIS::JoyStickEvent& e, InputEventAction t, uint16 devI, int32 inputI ) : 
		deviceIndex(devI), 
		action(t),
		inputIndex(inputI)
	{
		state.mButtons = e.state.mButtons;
		state.mAxes.clear();
		std::vector<OIS::Axis>::const_iterator ai, aiend = e.state.mAxes.end();
		for (ai = e.state.mAxes.begin(); ai != aiend; ++ai)
		{
			DInputAxis axis;
			axis.rel = (*ai).rel;
			axis.abs = (*ai).abs;
			axis.absOnly = (*ai).absOnly;
			state.mAxes.push_back(axis);
		}
		for (uint32 povI = 0; povI < 4; ++povI)
		{
			state.mPOV[povI].direction = e.state.mPOV[povI].direction;
		}

		for (uint32 sliderI = 0; sliderI < 4; ++sliderI)
		{
			state.mSliders[sliderI].abX = e.state.mSliders[sliderI].abX;
			state.mSliders[sliderI].abY = e.state.mSliders[sliderI].abY;
		}
		state.mVectors.clear();
		std::vector<OIS::Vector3>::const_iterator vi, viend = e.state.mVectors.end();
		for (vi = e.state.mVectors.begin(); vi != viend; ++vi)
		{
			DInputVector3 vec3;
			vec3.x = (*vi).x;
			vec3.y = (*vi).y;
			vec3.z = (*vi).z;
			state.mVectors.push_back(vec3);
		}
	}

	DJoyStickEvent& DJoyStickEvent::operator=( const DJoyStickEvent& e )
	{
		state = e.state; 
		deviceIndex = e.deviceIndex; 
		action = e.action; 
		inputIndex = e.inputIndex; 
		return *this;
	}


}