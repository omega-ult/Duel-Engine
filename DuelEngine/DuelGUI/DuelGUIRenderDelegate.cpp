//  [7/16/2013 OMEGA] created

#include "DuelGUICommon.h"
#include "DuelCommon.h"
#include "DuelAutoGpuParameter.h"
#include "DuelGUIWidget.h"
#include "DuelGUIManager.h"
#include "DuelGUIRenderDelegate.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DGRenderDelegate, DObject);

	

	void DGRenderDelegate::preRender()
	{
		DGWidget* w = getParent();
		if (w)
		{
			DAutoGpuParameter::getSingleton().setParameterDelegate(w->getRootManager());
		}
	}

	void DGRenderDelegate::postRender()
	{
		DGWidget* w = getParent();
		if (w)
		{
			DAutoGpuParameter::getSingleton().setParameterDelegate(NULL);
		}
	}

}