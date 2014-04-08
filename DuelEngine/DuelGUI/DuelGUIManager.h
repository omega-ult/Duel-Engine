//  [11/9/2012 OMEGA] created

#ifndef _DUELGUIMANAGER_H_
#define _DUELGUIMANAGER_H_

#include "DuelCommon.h"
#include "DuelGUICommon.h"
#include "DuelRenderWorkshop.h"
#include "DuelGUICursor.h"

namespace Duel
{
	class DUELGUI_API DGUIMouseEvent : public DMouseEvent
	{
		DUEL_DECLARE_RTTI(DGUIMouseEvent)
	public:
		DGUIMouseEvent();
		DGUIMouseEvent(const OIS::MouseEvent& e, InputEventAction t, MouseButtonID id);
		DGUIMouseEvent& operator = (const DMouseEvent& e);
		// ray in gui world space. this event is only used in the gui system.
		// gui manager will calculate the ray when processing the mouse event, 
		DRay	ray;
	};
	// a manager class used to manage all ui elements, this class should not be created by 
	// client directly, you should use DGUISystem::registerRenderWindow() and then
	// use DGUIManager*	getGUIManager to retrieve it.
	// DG = Duel GUI
	class DUELGUI_API DGUIManager : public DObject
	{
		// this is standard gui manager,
	DUEL_DECLARE_RTTI(DGUIManager)
	public:
 		DGUIManager(DRenderWindow* targetWindow);
		virtual ~DGUIManager();

 		virtual DRenderWindow*	getTargetWindow();
		// used for identify the factroy that create this manager.
		virtual const DString&	getGUIManagerName();
		// set the event queue for retrieve input events.
		virtual void			setInputEventQueue(DInputEventQueue* eventQueue);
		// called every frame to handle input event.
		INTERNAL virtual void	processInputEvent();
		// update all widgets, and the render queue will be filled.
		INTERNAL virtual void	update();

		// this method will insert a widget to the first of the list.
		virtual void			addWidget(DGWidget* w);
		// remove the widget from widget map.
		virtual void			removeWidget(DGWidget* w);
		// make specified widget higher preference. the specified widget must be top-level widget.
		virtual	void			promote(DGWidget* w);
		// make specified widget lower preference. the specified widget must be top-level widget.
		virtual	void			demote(DGWidget* w);
		// give the specified widget the best priority for retrieving events. the specified widget must be top-level widget.
		virtual	void			promoteToTop(DGWidget* w);
		// give the specified widget the worst priority for retrieving events. the specified widget must be top-level widget.
		virtual	void			demoteToBottom(DGWidget* w);

		// global hot spot will receive the input events before any widget(with focus widget included),
		// this is used for hot-key or shortcut, set NULL to disable it.
		virtual void			setGlobalHotspot(DGHotspot* hs);
		virtual DGHotspot*		getGlobalHotspot();

		// focus on specified widget, all input event will be sent to it firstly.
		// if NULL is set, then nothing happend.
		virtual void			pushFocusWidget(DGWidget* w);
		// retrieve the last focused widget, if there is one.
		virtual DGWidget*		getFocusWidget();
		// pop the last focused widget, if there are more widgets in the list,
		// the most recently pushed widget will take place(FIFO queue);
		virtual void			popFocusWidget();

		// set user-defined cursor, if NULL, then the default cursor will be used.
		virtual void			setCursor(DGCursor* cur);
		virtual DGCursor*		getCurrentCursor();
		virtual DGCursor*		getDefaultCursor();
		// use this function to change the cursor action, this method will automatically
		// reset the action to CA_Idle properly.
		virtual	void			requestCursorAction(uint32 act);

		// the renderables will be fill in update procedure.
		DRenderQueue*			getRenderQueue();

		virtual DCamera*		getGUICamera();

		// override : DAutoGpuParameterDelegate---------------------
		virtual DMatrix4		getViewMatrix();
		virtual DMatrix4		getProjectionMatrix();

	protected:	
		// internal method used to fill render queue.
		virtual void			applyToRenderQueue();

		DString				mManagerName;

		DRenderWindow*		mTargetWindow;
		DInputEventQueue*	mEventQueue;
		DRenderQueue*		mRenderQueue;
		// used for unhandled messages' cache.
		DInputEventQueue*	mCacheEventQueue;


		// sort widget with z-index ascending, meaning that widget with lower z has
		// less priority.
		typedef std::list<DGWidget*>	WidgetList;
		WidgetList			mWidgetList;
		WidgetList			mFocusWidgetList;
		DGHotspot*			mGlobalHotspot;

		// current cursor.
		DGCursor*			mCurrentCursor;
		DGCursor*			mDefaultCursor;
		uint32				mCursorAct;
		
		DCamera				mGUICamera;
		DAutoGpuParameterDelegate*	mCacheGpuParamDele;
//	private:
//		DGUIManager() {}
	};
}

#endif