//  [1/16/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelTimer.h"
#include "DuelInputComponent.h"
#include "DuelInputMouse.h"
#include "DuelInputKeyboard.h"
#include "DuelInputJoyStick.h"
#include "DuelInputEventQueue.h"
#include <OIS.h>

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(DInputEventQueue, DObject);

	uint16	DInputEventQueue::maxEventCount = 1024;


	void DInputEventQueue::setMaxEventCount( uint16 size /*= 1024*/ )
	{
		maxEventCount = size;
	}

	DInputEventQueue::DInputEventQueue() :
		mEventCount(0)
	{
		
	}

	void DInputEventQueue::pushEvent( const DKeyEvent& e )
	{
		if (mEventInforQueue.size() < maxEventCount)
		{
			pushEvent(e, mTimer.getMilliseconds());
		}
	}

	void DInputEventQueue::pushEvent( const DMouseEvent& e )
	{
		if (mEventInforQueue.size() < maxEventCount)
		{
			pushEvent(e, mTimer.getMilliseconds());
		}
	}

	void DInputEventQueue::pushEvent( const DJoyStickEvent& e)
	{
		if (mEventInforQueue.size() < maxEventCount)
		{
			pushEvent(e, mTimer.getMilliseconds());
		}
	}

	void DInputEventQueue::pushEvent( const DKeyEvent& e, uint64 time )
	{
		if (mEventInforQueue.size() < maxEventCount)
		{
			mEventInforQueue.push(EventInfo(IDT_Keyboard, time));
			mKeyEventQueue.push(e);
			mEventCount++;
		}
	}

	void DInputEventQueue::pushEvent( const DMouseEvent& e, uint64 time )
	{
		if (mEventInforQueue.size() < maxEventCount)
		{
			mEventInforQueue.push(EventInfo(IDT_Mouse, time));
			mMouseEventQueue.push(e);
			mEventCount++;
		}
	}

	void DInputEventQueue::pushEvent( const DJoyStickEvent& e, uint64 time )
	{		
		if (mEventInforQueue.size() < maxEventCount)
		{
			mEventInforQueue.push(EventInfo(IDT_JoyStick, time));
			mJoyStickEventQueue.push(e);
			mEventCount++;
		}
	}

	bool DInputEventQueue::popEventInfo( EventInfo& info )
	{
		if (!mEventInforQueue.empty())
		{
			info = mEventInforQueue.front();
			mEventInforQueue.pop();
			mEventCount--;
			return true;
		}
		return false;
	}

	bool DInputEventQueue::popEvent( DKeyEvent& e )
	{
		if (!mKeyEventQueue.empty())
		{
			e = mKeyEventQueue.front();
			mKeyEventQueue.pop();
			mEventCount--;
			return true;
		}
		return false;
	}

	bool DInputEventQueue::popEvent( DMouseEvent& e )
	{
		if (!mMouseEventQueue.empty())
		{
			e = mMouseEventQueue.front();
			mMouseEventQueue.pop();
			mEventCount--;
			return true;
		}
		return false;
	}

	bool DInputEventQueue::popEvent( DJoyStickEvent& e )
	{
		if (!mJoyStickEventQueue.empty())
		{
			e = mJoyStickEventQueue.front();
			mJoyStickEventQueue.pop();
			mEventCount--;
			return true;
		}
		return false;
	}

	bool DInputEventQueue::popEvent( EventInfo& info, DKeyEvent& ke, DMouseEvent& me, DJoyStickEvent& je )
	{
		if (popEventInfo(info))
		{
			switch (info.eventType)
			{
			case IDT_Mouse:
				popEvent(me);
				break;
			case IDT_Keyboard:
				popEvent(ke);
				break;
			case IDT_JoyStick:
				popEvent(je);
				break;
			default:
				break;
			}
			return true;
		}
		return false;
	}

	void DInputEventQueue::clearEvents()
	{
		while (!mEventInforQueue.empty())
		{
			mEventInforQueue.pop();
		}
		while (!mKeyEventQueue.empty())
		{
			mKeyEventQueue.pop();
		}	
		while (!mMouseEventQueue.empty())
		{
			mMouseEventQueue.pop();
		}
		while (!mJoyStickEventQueue.empty())
		{
			mJoyStickEventQueue.pop();
		}
		mEventCount = 0;
	}



}