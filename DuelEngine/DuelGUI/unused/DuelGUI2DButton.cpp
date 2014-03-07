//  [8/14/2013 OMEGA] created

#include "DuelGUICommon.h"
#include "DuelGUI.h"
#include "DuelGUI2DButton.h"
#include "DuelGUI2DButtonHS.h"
#include "DuelGUI2DButtonRD.h"


namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DG2DButton, DGWidget);

	DG2DButton::DG2DButton( DGWidget* parent ) :
		DGWidget(parent)
	{
		mCustomHotspot = new DG2DButtonHS(this);
		mCustomRenderDelegate = new DG2DButtonRD(this);
	}

	DG2DButton::~DG2DButton()
	{
		delete mCustomHotspot;
		delete mCustomRenderDelegate;
	}

}