//  [11/7/2013 OMEGA] created

#ifndef _DUELGUICURSOR_H_
#define _DUELGUICURSOR_H_

#include "DuelGUICommon.h"
#include "Duel.h"
#include "DuelGUIRenderDelegate.h"
#include "DuelGUISize.h"

namespace Duel
{
	class DUELGUI_API DGCursor : public DObject
	{
		DUEL_DECLARE_RTTI(DGCursor)
	public:
		// user can use self-defined cursor action, here the enum is the pre-defined
		// action code, it should not be used as self action code
		enum CursorAction
		{
			CA_Idle					= 0x0000,
			CA_Busy					= 0x0001,		
			CA_Pressed				= 0x0002,
			CA_Drag					= 0x0003,
			CA_ScaleHorizontal		= 0x0004,
			CA_ScaleVertical		= 0x0005,
			CA_Scale_LeftCorner		= 0x0006,
			CA_Scale_RightCorner	= 0x0007,
			CA_Link					= 0x0008,
			CA_Scroll				= 0x0009,
			CA_Edit					= 0x000a,
			CA_Invalid				= 0x000b
		};
		DGCursor();
		virtual ~DGCursor();


		// use CursorAction as parameter is valid, this can be parsed as standard action.
		void			setCursorAction(uint32 m);
		uint32			getCursorAction();

		// point to the screen space(use middle point of the screen as origin).
		// the pos will be clipped ty the host window.
		void			pointToScreen(const DVector2& pos);
		// point to the window space(use top-left corner as origin).
		void			pointToWindow(uint32 left, uint32 top);

		// get/set the size of the cursor. the default cursor is 20x20;
		void			setSize(const DGSize& size);
		const DGSize&	getSize();

		// query the current point in Screen
		const DVector2&	getPointInScreen();

		// set/get visiblity of this cursor.
		void			setVisible(bool flag);
		bool			isVisible();

		// set/get the opacity property of this cursor.
		void			setOpacity(DReal val);
		DReal			getOpacity();

		// set the remder delegate for this cursor, but do not own it, client should know how and 
		// when to delete it.
		void					setRenderDelegate(DGRenderDelegate* dele);
		// used to render this widget.
		DGRenderDelegate*		getRenderDelegate();

		// set the hosting window. should only be called by DGManager.
		INTERNAL void	setHostWindow(DRenderWindow* wind);
		// set the hosting window. should only be called by DGManager.
		INTERNAL DRenderWindow*	getHostWindow();

		// draw self, should only be called by DGManager.
		INTERNAL void	applyToRenderQueue(DRenderQueue* queue, uint32 groupID);

		// should be called by the gui manager.
		INTERNAL void	update();
		// this should only be called by DGManager, this function will not send 
		// mouse event to the OS. using screen space.
		INTERNAL void	setPoint(const DVector2& p);
	protected:
		void			setOSCursorImpl(const DVector2& p);
		// the size hold the cursor.
		DRenderWindow*	mHostWindow;
		DVector2		mPoint;
		uint32			mCurAction;
		bool			mbVisible;
		DReal			mOpacity;
		DGSize			mSize;
		DGRenderDelegate*	mRenderDelegate;
	};
}

#endif