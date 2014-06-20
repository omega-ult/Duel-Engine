//  [1/15/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelInputManager.h"
#include "DuelInputEventQueue.h"
#include "DuelInputReceiver.h"
#include "DuelRenderWindow.h"
#ifdef DUEL_PLATFORM_WINDOWS_PHONE_8
#else
#include <OIS.h>
#endif

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DInputManager, DObject);
#ifdef DUEL_PLATFORM_WINDOWS_PHONE_8
#else
	DInputManager::DInputManager()
	{
#ifdef	DUEL_PLATFORM_WINDOWS
		mPlatform = "WINDOW";
#endif

	}


	DInputManager::~DInputManager()
	{

	}

	DInputEventQueue* DInputManager::registerWindow( DRenderWindow* wind, bool isExclusive )
	{
		EventContextMap::iterator i = mEventContextMap.find(wind);
		if (i != mEventContextMap.end())
		{
			return i->second.eventQueue;
		}
		// create a new OIS::InputManager
		OIS::ParamList pl;
		std::ostringstream wnd;
		wnd << (size_t)wind->getWindowHandle();
		pl.insert(std::make_pair(mPlatform, wnd.str()));
		if (!isExclusive)
		{
			// 非独占模式下鼠标可移出窗口之外
			pl.insert(std::make_pair(std::string("w32_mouse"), "DISCL_FOREGROUND"));    
			pl.insert(std::make_pair(std::string("w32_mouse"), "DISCL_NONEXCLUSIVE"));
		}

		// since no NULL will be returned, we do not check.
		// (OIS will throw exception)
		OIS::InputManager* mgr = OIS::InputManager::createInputSystem(pl);
		mgr->enableAddOnFactory(OIS::InputManager::AddOn_All);

		// since the ois's mouse event depend on its window size, so we must act
		// when the window size changed.
		wind->signalResized.connect(
			DBind(&DInputManager::onWindowResized, this, _1));

		DInputEventQueue* queue = new DInputEventQueue();
		DInputReceiver*	receiver = new DInputReceiver(mgr, queue);
		receiver->setWindowSize(wind->getWidth(), wind->getHeight());
		mEventContextMap.insert(std::make_pair(wind, EventContext(mgr, receiver, queue)));
		return queue;
	}

	DInputEventQueue* DInputManager::getEventQueue( DRenderWindow* wind )
	{
		EventContextMap::iterator i = mEventContextMap.find(wind);
		if (i != mEventContextMap.end())
		{
			return i->second.eventQueue;
		}
		return NULL;
	}


	void DInputManager::unregisterWindow( DRenderWindow* wind )
	{
		EventContextMap::iterator i = mEventContextMap.find(wind);
		if (i != mEventContextMap.end())
		{
			// remove listener.
			wind->signalResized.disconnect(
				DBind(&DInputManager::onWindowResized, this, _1));
			OIS::InputManager::destroyInputSystem(i->second.oisManager);
			delete i->second.eventQueue;
			delete i->second.eventReceiver;
			mEventContextMap.erase(i);
		}


	}

	void DInputManager::captureEvent()
	{
		EventContextMap::iterator i, iend = mEventContextMap.end();
		for (i = mEventContextMap.begin(); i != iend; ++i)
		{
			i->second.eventReceiver->captureEvent();
		}
	}

	INTERNAL void DInputManager::onWindowResized( DRenderWindow* wind )
	{
		EventContextMap::iterator i = mEventContextMap.find(wind);
		if (i != mEventContextMap.end())
		{
			i->second.eventReceiver->setWindowSize(wind->getWidth(), wind->getHeight());
		}
	}

#endif


}