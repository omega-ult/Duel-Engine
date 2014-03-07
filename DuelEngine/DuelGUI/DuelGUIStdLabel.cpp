//  [11/1/2013 OMEGA] created

#include "DuelGUICommon.h"
#include "DuelGUIHotSpot.h"
#include "DuelGUIRenderDelegate.h"
#include "DuelGUIWidget.h"
#include "DuelGUIStdLabel.h"


namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DGStdLableHS, DGHotspot);
	DUEL_IMPLEMENT_RTTI_1(DGStdLabelRD, DGStdLabelRD);
	DUEL_IMPLEMENT_RTTI_1(DGStdLabel, DGWidget);

	DGStdLableHS::DGStdLableHS( DGStdLabel* parent ) :
		mTargetLabel(parent)
	{

	}

	DGWidget* DGStdLableHS::getParent()
	{
		return mTargetLabel;
	}


	DGStdLabelRD::DGStdLabelRD(DGStdLabel* parent ) :
		mTargetLabel(parent)
	{

	}

	DGWidget* DGStdLabelRD::getParent()
	{
		return mTargetLabel;
	}


	DGStdLabel::DGStdLabel( DGWidget* parent /*= NULL*/ )
	{
		mDefaultHotspot = new DGStdLableHS(this);
		mDefaultRenderDelegate = new DGStdLabelRD();
	}

}