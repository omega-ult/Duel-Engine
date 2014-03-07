//  [7/16/2013 OMEGA] created

#include "DuelGUICommon.h"
#include "Duel.h"
#include "DuelCore.h"
#include "DuelGUIManager.h"
#include "DuelGUIMathTool.h"
#include "DuelInputManager.h"
#include "DuelRenderWindow.h"
#include "DuelGUIWidget.h"
#include "DuelGUIHotSpot.h"
#include "DuelGUICursor.h"

#include "DuelGUIStdCursor.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DGUIManager, DAutoGpuParameterDelegate);
	DUEL_IMPLEMENT_RTTI_1(DGUIMouseEvent, DMouseEvent);


	DGUIManager::DGUIManager( DRenderWindow* targetWindow) : 
		mTargetWindow(targetWindow),
		mEventQueue(NULL),
		mRenderQueue(NULL),
		mCurrentCursor(NULL),
		mDefaultCursor(NULL),
		mCursorAct(DGCursor::CA_Idle),
		mFocusWidgetList(NULL),
		mGUICamera(targetWindow->getName() + "_GUICamera"),
		mManagerName("StantardGUIManager"),
		mCacheGpuParamDele(NULL),
		mGlobalHotspot(NULL)
	{


		mGUICamera.setPorjectionType(PT_Perspective);
		mGUICamera.setFOV(DMath::HALF_PI/2);
		mGUICamera.setNearClipDistance(0.01f);
		mGUICamera.setFarClipDistance(10000.0f);
		mGUICamera.setAspectRatio((DReal)targetWindow->getWidth()/(DReal)targetWindow->getHeight());

		DReal zVal = ((DReal)targetWindow->getHeight()/2) / DMath::Tan(mGUICamera.getFOV()/2);
		mGUICamera.setEyePosition(0.0f, 0.0f, -zVal );
		mGUICamera.lookAt(0.0f, 0.0f, 0.0f);

		mCacheEventQueue = new DInputEventQueue();

		DRenderWorkshop* workshop = DCore::getSingleton().getRenderWorkshop();
		if (workshop)
		{
			mRenderQueue = workshop->createRenderQueue();
			workshop->signalGroupStartRender.connect(
				DBind(&DGUIManager::onStartRenderGUI, this, _1, _2));

			workshop->signalGroupFinishRender.connect(
				DBind(&DGUIManager::onFinishRenderGUI, this, _1, _2));
		}

		// TODO: 增加load基本gui资源的函数.
		DResourcePtr resT = DResourceGroupManager::getSingleton().
			getResouceManager("RenderEffect")->getResource("_BasicShaderPack", "UI_Basic.dre");
		if (resT != NULL)
		{
			resT->touch();
		}

		mDefaultCursor = new DGStdCursor();
		mDefaultCursor->setHostWindow(mTargetWindow);
		mCurrentCursor = mDefaultCursor;
	}

	DGUIManager::~DGUIManager()
	{
		if (mCacheEventQueue)
		{
			delete mCacheEventQueue;
		}
		WidgetList::iterator i, iend = mWidgetList.end();
		for (i = mWidgetList.begin(); i != iend; ++i)
		{
			(*i)->setManager(NULL);
		}

		DRenderWorkshop* workshop = DCore::getSingleton().getRenderWorkshop();
		if (workshop)
		{
			workshop->destroyRenderQueue(mRenderQueue);
			mRenderQueue = NULL;

			workshop->signalGroupStartRender.disconnect(
				DBind(&DGUIManager::onStartRenderGUI, this, _1, _2));

			workshop->signalGroupFinishRender.disconnect(
				DBind(&DGUIManager::onFinishRenderGUI, this, _1, _2));
		}

		if (mDefaultCursor)
		{
			delete mDefaultCursor;
		}
	}

	void DGUIManager::processInputEvent()
	{
		if ( mEventQueue == NULL)
		{
			return;
		}
		else if (mEventQueue->getEventCount() == 0)
		{
			return;
		}
		DInputEventQueue::EventInfo info;
		bool result;
		result = mEventQueue->popEventInfo(info);
		// here we filter the mouse event, catch the final position of the 
		// mouse an set it to the current mouse.
		bool hasMouseEvent = false;
		DMouseEvent	finalMouseEvent;
		// first push all events from main event queue, process them,
		while (result)
		{
			// we do not process time info
			bool processed = false;

			if (info.eventType == IDT_Mouse)
			{
				if (!hasMouseEvent)
				{
					// the first time we encountered a mouse event. reset the cursor action, then allow 
					// widgets to request other action.
					requestCursorAction(DGCursor::CA_Idle);
					hasMouseEvent = true;
				}
				DMouseEvent me;
				mEventQueue->popEvent(me);
				finalMouseEvent = me;
				DGUIMouseEvent guiME;
				guiME = me;
				// 我们的输入系统会根据窗口大小的改变而改变检测区域, 因此我们只要把窗口区域作为参数就可以了,
				// 不需要用到渲染窗口使用的viewport.
				DVector2 pos;
				pos = DGGUIMathTool::getScreenPoint(
					guiME.state.X.abs, guiME.state.Y.abs,
					DGSize((DReal)mTargetWindow->getWidth(), (DReal)mTargetWindow->getHeight()));

				DRay rayInCamera = mGUICamera.getCameraRayFromScreen(
						mTargetWindow->getWidth(), mTargetWindow->getHeight(), pos);

				guiME.ray = mGUICamera.getWorldRayFromCamera(rayInCamera);


				if (mGlobalHotspot != NULL)
				{
					processed = mGlobalHotspot->response(&guiME);
				}

				if (!processed)
				{
					// 如果当前有focuswidget, 则直接让其处理. 并且不往下派发消息.
					if (!mFocusWidgetList.empty())
					{
						(*mFocusWidgetList.begin())->processEvent(&guiME, info.timeStamp);
						processed = true;
					}
					else
					{
						// 这里再遍历widgetMap, 注意需要逆序.
						WidgetList::iterator i, iend = mWidgetList.end();
						for (i = mWidgetList.begin(); i != iend ; ++i)
						{
							if ((*i)->isEnable() || !(*i)->isVisible())
							{				
								processed = (*i)->processEvent(&guiME, info.timeStamp);
							}
							if (!processed)
							{
								// repush it.
								mCacheEventQueue->pushEvent(me, info.timeStamp);
							}
						}

					}
				}

			}
			else if (info.eventType == IDT_Keyboard)
			{
				DKeyEvent ke;
				mEventQueue->popEvent(ke);

				if (mGlobalHotspot != NULL)
				{
					processed = mGlobalHotspot->response(&ke);
				}
				if(!processed)
				{
					// 如果当前有focuswidget, 则直接让其处理. 并且不往下派发消息.
					if (!mFocusWidgetList.empty())
					{
						(*mFocusWidgetList.begin())->processEvent(&ke, info.timeStamp);
						processed = true;
					}
					else
					{
						// 非激活的控件无法收到.
						mCacheEventQueue->pushEvent(ke, info.timeStamp);
						processed = false;
					}
				}
			}
			else
			{
				DJoyStickEvent je;
				mEventQueue->popEvent(je);

				if (mGlobalHotspot != NULL)
				{
					processed = mGlobalHotspot->response(&je);
				}
				if (!processed)
				{
					// 如果当前有focuswidget, 则直接让其处理. 并且不往下派发消息.
					if (!mFocusWidgetList.empty())
					{
						(*mFocusWidgetList.begin())->processEvent(&je, info.timeStamp);
						processed = true;
					}
					else
					{
						// 非激活的控件无法收到.
						mCacheEventQueue->pushEvent(je, info.timeStamp);
						processed = false;
					}
				}
			}
			result = mEventQueue->popEventInfo(info);
		}
		// for now, the events in the event queue are all traversed.
		// second, push all unhandled event to the main event queue, leave them for other logic.
		result = mCacheEventQueue->popEventInfo(info);
		while (result)
		{
			if (info.eventType == IDT_Mouse)
			{
				DMouseEvent me;
				mCacheEventQueue->popEvent(me);
				mEventQueue->pushEvent(me, info.timeStamp);
			}
			else if (info.eventType == IDT_Keyboard)
			{
				DKeyEvent ke;
				mCacheEventQueue->popEvent(ke);
				mEventQueue->pushEvent(ke, info.timeStamp);
			}
			else
			{
				DJoyStickEvent je;
				mCacheEventQueue->popEvent(je);
				mEventQueue->pushEvent(je, info.timeStamp);
			}
			result = mCacheEventQueue->popEventInfo(info);
		}
		// process mouse cursor.
		if (hasMouseEvent)
		{
			if (mCurrentCursor)
			{
				// change the window corrdinate to the screen coordinate.
				DVector2 pos;
				pos = DGGUIMathTool::getScreenPoint(
					finalMouseEvent.state.X.abs, finalMouseEvent.state.Y.abs,
					DGSize((DReal)mTargetWindow->getWidth(), (DReal)mTargetWindow->getHeight()));
				mCurrentCursor->setPoint(pos);
				// debug.
				std::cout<<pos. x << "," <<pos.y << std::endl;
			}
		}
	}

	INTERNAL void DGUIManager::update()
	{
		WidgetList::iterator i, iend = mWidgetList.end();
		for (i = mWidgetList.begin(); i != iend; ++i)
		{
			(*i)->updateLayout();
		}
		for (i = mWidgetList.begin(); i != iend; ++i)
		{
			(*i)->updateComponent();
		}

		if (mCurrentCursor)
		{
			mCurrentCursor->update();
		}

		applyToRenderQueue();
	}


	void DGUIManager::addWidget( DGWidget* w )
	{
		w->setManager(this);
		mWidgetList.push_front(w);
	}

	void DGUIManager::removeWidget( DGWidget* w )
	{
		WidgetList::iterator i, iend = mWidgetList.end();
		for (i = mWidgetList.begin(); i != iend; ++i)
		{
			if ((*i) == w)
			{
				(*i)->setManager(NULL);
				mWidgetList.erase(i);
				return;
			}
		}

	}

	void DGUIManager::applyToRenderQueue()
	{
		// first we set depth for all widgets.
		WidgetList::reverse_iterator i, iend = mWidgetList.rend();

		for (i = mWidgetList.rbegin(); i != iend; ++i)
		{
			(*i)->applyToRenderQueue(mRenderQueue, RG_GuiFront);
		}
		if (mCurrentCursor)
		{
			mCurrentCursor->setCursorAction(mCursorAct);
			mCurrentCursor->applyToRenderQueue(mRenderQueue, RG_GuiFront);
		}
	}

	INTERNAL void DGUIManager::onStartRenderGUI(DRenderQueue* queue, DRenderGroup* group)
	{
		// todo : set the options for rendering 
		if (queue != mRenderQueue || group->getGroupID() != RG_GuiFront)
		{		
			return;
		}
		// set the parameter delegate.
		mCacheGpuParamDele = DAutoGpuParameter::getSingleton().getCurrentParameterDelegate();
		DAutoGpuParameter::getSingleton().setParameterDelegate(this);

		// rendering order should not be sort by the camera distance.
		group->setCameraSortingOrder(CDS_NoSort);
		group->sort(&mGUICamera);
		//DCamera* cam
		DRenderWorkshop* ws = DCore::getSingleton().getRenderWorkshop();
		ws->pushOption(mRenderOption);
	}

	INTERNAL void DGUIManager::onFinishRenderGUI(DRenderQueue* queue, DRenderGroup* group)
	{
		// todo : set the options for rendering 
		if (queue != mRenderQueue || group->getGroupID() != RG_GuiFront)
		{		
			return;
		}
		// pop the options we just pushed.
		DRenderWorkshop* ws = DCore::getSingleton().getRenderWorkshop();
		ws->popOption();
		group->clear();
		DAutoGpuParameter::getSingleton().setParameterDelegate(mCacheGpuParamDele);

	}
	
	void DGUIManager::setInputEventQueue( DInputEventQueue* eventQueue )
	{
		mEventQueue = eventQueue;
	}

	DCamera* DGUIManager::getGUICamera()
	{
		return &mGUICamera;
	}

	DGCursor* DGUIManager::getDefaultCursor()
	{
		return mDefaultCursor;
	}

	DGCursor* DGUIManager::getCurrentCursor()
	{
		return mCurrentCursor;
	}

	void DGUIManager::setCursor( DGCursor* cur )
	{
		if ( cur != NULL)
		{
			mCurrentCursor = cur;
		}
		else
		{
			cur = mDefaultCursor;
		}
		mCurrentCursor->setHostWindow(mTargetWindow);
	}

	void DGUIManager::pushFocusWidget( DGWidget* w )
	{
		if (w == NULL)
		{
			return;
		}
		WidgetList::iterator i, iend = mFocusWidgetList.end();
		for (i = mFocusWidgetList.begin(); i != iend; ++i)
		{
			if ((*i) == w)
			{
				mFocusWidgetList.erase(i);
				break;
			}
		}
		mFocusWidgetList.push_front(w);
	}

	DGWidget* DGUIManager::getFocusWidget()
	{
		if (mFocusWidgetList.empty())
		{
			return NULL;
		}
		return (*mFocusWidgetList.begin());
	}

	void DGUIManager::popFocusWidget()
	{
		if (mFocusWidgetList.empty())
		{
			return;
		}
		mFocusWidgetList.erase(mFocusWidgetList.begin());
	}


	void DGUIManager::promote( DGWidget* w )
	{
		if ((*mWidgetList.begin()) == w)
		{
			return;
		}
		WidgetList::iterator i, iend = mWidgetList.end();
		WidgetList::iterator preI;
		for (i = mWidgetList.begin(); i != iend; ++i)
		{
			preI = i;
			if ((*i) == w) // find!
			{
				std::swap(i, preI);
			}
		}
	}

	void DGUIManager::demote( DGWidget* w )
	{
		if ((*mWidgetList.rbegin()) == w)
		{
			return;
		}
		WidgetList::reverse_iterator i, iend = mWidgetList.rend();
		WidgetList::reverse_iterator preI;
		for (i = mWidgetList.rbegin(); i != iend; ++i)
		{
			preI = i;
			if ((*i) == w) // find!
			{
				std::swap(i, preI);
			}
		}
	}

	void DGUIManager::promoteToTop( DGWidget* w )
	{
		if ((*mWidgetList.begin()) == w)
		{
			return;
		}
		WidgetList::iterator i, iend = mWidgetList.end();
		WidgetList::iterator beginI = mWidgetList.begin();
		for (i = mWidgetList.begin(); i != iend; ++i)
		{
			if ((*i) == w) // find!
			{
				std::swap(i, beginI);
			}
		}
	}

	void DGUIManager::demoteToBottom( DGWidget* w )
	{
		if ((*mWidgetList.rbegin()) == w)
		{
			return;
		}
		WidgetList::reverse_iterator i, iend = mWidgetList.rend();
		WidgetList::reverse_iterator beginI = mWidgetList.rbegin();
		for (i = mWidgetList.rbegin(); i != iend; ++i)
		{
			if ((*i) == w) // find!
			{
				std::swap(i, beginI);
			}
		}
	}

	const DString& DGUIManager::getGUIManagerName()
	{
		return mManagerName;
	}

	DRenderWindow* DGUIManager::getTargetWindow()
	{
		return mTargetWindow;
	}

	void DGUIManager::requestCursorAction( uint32 act )
	{
		mCursorAct = act;
	}

	const DMatrix4& DGUIManager::getViewMatrix()
	{
		return mGUICamera.getViewMatrix();
	}

	const DMatrix4& DGUIManager::getProjectionMatrix()
	{
		return mGUICamera.getProjectionMatrix();
	}

	void DGUIManager::setGlobalHotspot( DGHotspot* hs )
	{
		mGlobalHotspot = hs;
	}

	DGHotspot* DGUIManager::getGlobalHotspot()
	{
		return mGlobalHotspot;
	}

	DRenderQueue* DGUIManager::getRenderQueue()
	{
		return mRenderQueue;
	}


	DGUIMouseEvent& DGUIMouseEvent::operator=( const DMouseEvent& e )
	{
		state = e.state;  
		buttonID = e.buttonID;  
		action = e.action; 
		return *this;
	}

	DGUIMouseEvent::DGUIMouseEvent( const OIS::MouseEvent& e, InputEventAction t, MouseButtonID id /*= MBID_None*/ ) :
		DMouseEvent(e, t, id)
	{

	}

	DGUIMouseEvent::DGUIMouseEvent()
	{

	}

}
