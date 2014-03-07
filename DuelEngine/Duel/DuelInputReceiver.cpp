//  [1/16/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelInputReceiver.h"
#include "DuelInputEventQueue.h"
#ifdef DUEL_PLATFORM_WINDOWS_PHONE_8
#else
#include <ois-v1-3/includes/OIS.h>
#endif

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DInputReceiver, DObject);
#ifdef DUEL_PLATFORM_WINDOWS_PHONE_8
#else
	DInputReceiver::DInputReceiver( OIS::InputManager* inputMgr, DInputEventQueue* targetQueue ) : 
		mTargetQueue(targetQueue)
	{
		mKeyboard = (OIS::Keyboard*)inputMgr->createInputObject(OIS::OISKeyboard, true);
		mKeyboard->setEventCallback(this);
		mMouse = (OIS::Mouse*)inputMgr->createInputObject(OIS::OISMouse, true);
		mMouse->setEventCallback(this);
		
		uint16 devCount = inputMgr->getNumberOfDevices(OIS::OISJoyStick);
		for(uint16 i = 0; i < devCount; ++i)
		{
			OIS::JoyStick* tmp;
			tmp = (OIS::JoyStick*)inputMgr->createInputObject(OIS::OISJoyStick, true);
			tmp->setEventCallback(this);
			mJoyStickIndexMap[tmp] = i;
			mJoySticks.push_back(tmp);
		}
	}

	bool DInputReceiver::keyPressed( const OIS::KeyEvent& arg )
	{
		mTargetQueue->pushEvent(DKeyEvent(arg, IEA_Pressed));
// 		std::cout << " KeyPressed {" << arg.key
// 			<< ", " << ((Keyboard*)(arg.device))->getAsString(arg.key)
// 			<< "} || Character (" << (char)arg.text << ")" << std::endl;
		return true;
	}

	bool DInputReceiver::keyReleased( const OIS::KeyEvent &arg )
	{
		mTargetQueue->pushEvent(DKeyEvent(arg, IEA_Pressed));
// 		if( arg.key == KC_ESCAPE || arg.key == KC_Q )
// 			appRunning = false;
// 		std::cout << "KeyReleased {" << ((Keyboard*)(arg.device))->getAsString(arg.key) << "}\n";
		return true;
	}

	bool DInputReceiver::mouseMoved( const OIS::MouseEvent &arg )
	{
		mTargetQueue->pushEvent(DMouseEvent(arg, IEA_Moved, MBID_Left)) ;
// 		const OIS::MouseState& s = arg.state;
// 		std::cout << "\nMouseMoved: Abs("
// 			<< s.X.abs << ", " << s.Y.abs << ", " << s.Z.abs << ") Rel("
//			<< s.X.rel << ", " << s.Y.rel << ", " << s.Z.rel << ")";
		return true;
	}

	bool DInputReceiver::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		mTargetQueue->pushEvent(DMouseEvent(arg, IEA_Pressed, (Duel::MouseButtonID)id));
// 		const OIS::MouseState& s = arg.state;
// 		std::cout << "\nMouse button #" << id << " pressed. Abs("
// 			<< s.X.abs << ", " << s.Y.abs << ", " << s.Z.abs << ") Rel("
// 			<< s.X.rel << ", " << s.Y.rel << ", " << s.Z.rel << ")";
		return true;
	}

	bool DInputReceiver::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		mTargetQueue->pushEvent(DMouseEvent(arg, IEA_Released, (Duel::MouseButtonID)id));
// 		const OIS::MouseState& s = arg.state;
// 		std::cout << "\nMouse button #" << id << " released. Abs("
// 			<< s.X.abs << ", " << s.Y.abs << ", " << s.Z.abs << ") Rel("
// 			<< s.X.rel << ", " << s.Y.rel << ", " << s.Z.rel << ")";
		return true;
	}

	bool DInputReceiver::buttonPressed( const OIS::JoyStickEvent &arg, int button )
	{
		JoyStickIndexMap::iterator i = mJoyStickIndexMap.find((OIS::JoyStick*)arg.device);
		mTargetQueue->pushEvent(DJoyStickEvent(arg, IEA_Pressed, i->second, button));
// 		std::cout << std::endl << arg.device->vendor() << ". Button Pressed # " << button;
		return true;
	}

	bool DInputReceiver::buttonReleased( const OIS::JoyStickEvent &arg, int button )
	{
		JoyStickIndexMap::iterator i = mJoyStickIndexMap.find((OIS::JoyStick*)arg.device);
		mTargetQueue->pushEvent(DJoyStickEvent(arg, IEA_Released, i->second, button));
// 		std::cout << std::endl << arg.device->vendor() << ". Button Released # " << button;
		return true;
	}

	bool DInputReceiver::axisMoved( const OIS::JoyStickEvent &arg, int axis )
	{
		JoyStickIndexMap::iterator i = mJoyStickIndexMap.find((OIS::JoyStick*)arg.device);
		mTargetQueue->pushEvent(DJoyStickEvent(arg, IEA_JAxisMove, i->second, axis));
		//Provide a little dead zone
// 		if( arg.state.mAxes[axis].abs > 2500 || arg.state.mAxes[axis].abs < -2500 )
// 			std::cout << std::endl << arg.device->vendor() << ". Axis # " << axis << " Value: " << arg.state.mAxes[axis].abs;
		return true;
	}

	bool DInputReceiver::povMoved( const OIS::JoyStickEvent &arg, int pov )
	{
		JoyStickIndexMap::iterator i = mJoyStickIndexMap.find((OIS::JoyStick*)arg.device);
		mTargetQueue->pushEvent(DJoyStickEvent(arg, IEA_JPOVMove, i->second, pov));
// 		std::cout << std::endl << arg.device->vendor() << ". POV" << pov << " ";
// 
// 		if( arg.state.mPOV[pov].direction & Pov::North ) //Going up
// 			std::cout << "North";
// 		else if( arg.state.mPOV[pov].direction & Pov::South ) //Going down
// 			std::cout << "South";
// 
// 		if( arg.state.mPOV[pov].direction & Pov::East ) //Going right
// 			std::cout << "East";
// 		else if( arg.state.mPOV[pov].direction & Pov::West ) //Going left
// 			std::cout << "West";
// 
// 		if( arg.state.mPOV[pov].direction == Pov::Centered ) //stopped/centered out
// 			std::cout << "Centered";
		return true;
	}

	bool DInputReceiver::vector3Moved( const OIS::JoyStickEvent &arg, int index )
	{
		JoyStickIndexMap::iterator i = mJoyStickIndexMap.find((OIS::JoyStick*)arg.device);
		mTargetQueue->pushEvent(DJoyStickEvent(arg,IEA_JVec3Move, i->second, index));
// 		std::cout.precision(2);
// 		std::cout.flags(std::ios::fixed | std::ios::right);
// 		std::cout << std::endl << arg.device->vendor() << ". Orientation # " << index 
// 			<< " X Value: " << arg.state.mVectors[index].x
// 			<< " Y Value: " << arg.state.mVectors[index].y
// 			<< " Z Value: " << arg.state.mVectors[index].z;
// 		std::cout.precision();
// 		std::cout.flags();
		return true;
	}

	void DInputReceiver::captureEvent()
	{
		mKeyboard->capture();
		mMouse->capture();
		JoySticks::iterator i, iend = mJoySticks.end();
		for(i = mJoySticks.begin(); i != iend; ++i)
		{
			(*i)->capture();
		}
	}

	void DInputReceiver::setWindowSize( int32 width, int32 height )
	{
		const OIS::MouseState &ms = mMouse->getMouseState();
		ms.width = width;
		ms.height = height;
	}

#endif

}