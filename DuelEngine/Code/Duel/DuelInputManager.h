//  [11/13/2012 OMEGA] created

#ifndef _DUELINPUTMANAGER_H_
#define _DUELINPUTMANAGER_H_

#include "DuelCommon.h"
#include "DuelInputReceiver.h"
#include "DuelInputEventQueue.h"

#ifdef DUEL_PLATFORM_WINDOWS_PHONE_8
#else
#include <OIS.h>
#endif

namespace Duel
{
#ifdef DUEL_PLATFORM_WINDOWS_PHONE_8

#else
	// input manager holds event receivers and corresponding window,
	class DUEL_API DInputManager : public DObject, public DSingleton<DInputManager>
	{
	DUEL_DECLARE_RTTI(DInputManager)
	public:
		DInputManager();
		virtual ~DInputManager();

		// register a window, catch its input events, an DInputEventQueue will be returned
		// for processing events this window received.
		// isExclusive means the window will own the mouse pointer.
		DInputEventQueue*	registerWindow(DRenderWindow* wind, bool isExclusive );
		// get event queue for specified window.
		DInputEventQueue*	getEventQueue(DRenderWindow* wind);
		// unregister a window, no longer listen to its input event, at the same time,
		// the corresponding event queue will also be destroyed.
		void				unregisterWindow(DRenderWindow* wind);

		// capture input events, must be called explicitly in main loop;
		void				captureEvent();

	protected:
		// on window resized, we do this:
		INTERNAL void		onWindowResized(DRenderWindow* wind);
		// a simple description used to initialize OIS::InputManager.
		DString				mPlatform;
		// a map storage that stores the binded InputManager.
		struct EventContext
		{
			EventContext(OIS::InputManager* manager, DInputReceiver* receiver, DInputEventQueue* queue) : 
				oisManager(manager), 
				eventReceiver(receiver), 
				eventQueue(queue) {}
			OIS::InputManager*	oisManager;
			DInputReceiver*		eventReceiver;
			DInputEventQueue*	eventQueue;
		};
		typedef std::map<DRenderWindow*, EventContext>	EventContextMap;
		EventContextMap	mEventContextMap;
	};

#endif

}

#endif