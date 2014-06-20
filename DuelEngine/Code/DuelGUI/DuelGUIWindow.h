//  [10/31/2013 OMEGA] created

#ifndef _DUELGUIWINDOW_H_
#define _DUELGUIWINDOW_H_


#include "DuelGUICommon.h"
#include "DuelGUIWidget.h"
#include "DuelGUILayout.h"

namespace Duel
{
	class DUELGUI_API DGWindowLayout : public DGLayout
	{
		DUEL_DECLARE_RTTI(DGWindowLayout)
	public:
		DGWindowLayout();
		DGWindowLayout(DGWidget* w);

		// override : DGLayoug----------------------
		virtual void	update() {}
	};
	

	class DUELGUI_API DGWindowTitleBar : public DGWidget
	{
		DUEL_DECLARE_RTTI(DGWindowTitleBar)
	public:
		DGWindowTitleBar(DGWidget* parent = NULL) : DGWidget(parent) { }
	};

	// a standard window widget, with title bar and menu bar tool bar enabled.
	class DUELGUI_API DGWindow : public DGWidget
	{
		DUEL_DECLARE_RTTI(DGWindow)
	public:
		DGWindow(DGWidget* parent = NULL) : DGWidget(parent) {}

		void		setCentralWidget(DGWidget* wid);
		DGWidget*	getCentralWidget(); 

	protected:
		DGWidget*	mCentralWidget;

	};
}


#endif