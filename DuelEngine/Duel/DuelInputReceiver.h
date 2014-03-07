//  [1/15/2013 OMEGA] created

#ifndef _DUELINPUTRECEIVER_H_
#define _DUELINPUTRECEIVER_H_

#include "DuelCommon.h"
#include "DuelInputEventQueue.h"
#ifdef DUEL_PLATFORM_WINDOWS_PHONE_8
#else
#include <ois-v1-3/includes/OIS.h>
#endif


namespace Duel
{
#ifdef DUEL_PLATFORM_WINDOWS_PHONE_8
#else
	class DUEL_API DInputReceiver : public DObject, public OIS::MouseListener, public OIS::JoyStickListener, public OIS::KeyListener
	{
	DUEL_DECLARE_RTTI(DInputReceiver)
		// TODO:学习多个手柄的支持.
	public:
		// receiver will store the events into the queue.
		DInputReceiver(OIS::InputManager* inputMgr, DInputEventQueue* targetQueue);
		virtual ~DInputReceiver() {}
		bool	keyPressed( const OIS::KeyEvent &arg );
		bool	keyReleased( const OIS::KeyEvent &arg );
		
		bool	mouseMoved( const OIS::MouseEvent &arg );
		bool	mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
		bool	mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
		
		bool	buttonPressed( const OIS::JoyStickEvent &arg, int button );
		bool	buttonReleased( const OIS::JoyStickEvent &arg, int button );
		bool	axisMoved( const OIS::JoyStickEvent &arg, int axis );
		bool	povMoved( const OIS::JoyStickEvent &arg, int pov );
		bool	vector3Moved( const OIS::JoyStickEvent &arg, int index);
		
		// get the count of sticks.
		uint16	getJoyStickCount() const { return static_cast<uint16>(mJoySticks.size()); }

		// this method is called by InputSystem, used to retrieve input events.
		INTERNAL void	captureEvent();
		// since mouse event depends on window size, we provide this interface
		// for changing window size.
		void	setWindowSize(int32 width, int32 height);

	protected:
		DInputEventQueue*	mTargetQueue;
		// ois objects.
		OIS::Keyboard*		mKeyboard;
		OIS::Mouse*			mMouse;
		typedef std::vector<OIS::JoyStick*>	JoySticks;
		JoySticks			mJoySticks;
		// simple map recording joystick index.
		typedef	std::map<OIS::JoyStick*, uint16>	JoyStickIndexMap;
		JoyStickIndexMap	mJoyStickIndexMap;
	private:
		DInputReceiver() {} // not allowed.
	};

#endif
}

#endif