//  [8/14/2013 OMEGA] created

#ifndef _DUELGUI2DBUTTON_H_
#define _DUELGUI2DBUTTON_H_


#include "DuelGUICommon.h"
#include "DuelGUIWidget.h"
#include "DuelGUI2DButtonHS.h"
#include "DuelGUI2DButtonRD.h"

namespace Duel
{

	class DUELGUI_API DG2DButton : public DGWidget
	{
	DUEL_DECLARE_RTTI(DG2DButton)
	public:
		DG2DButton(DGWidget* parent);
		virtual	~DG2DButton();
	protected:

	};

}


#endif