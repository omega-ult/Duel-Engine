//  [11/1/2013 OMEGA] created

#ifndef _DUELGUISTDLABEL_H_
#define _DUELGUISTDLABEL_H_

#include "DuelGUICommon.h"
#include "DuelGUIHotSpot.h"
#include "DuelGUIWidget.h"

namespace Duel
{
	class DGStdLabel;

	class DUELGUI_API DGStdLableHS : public DGHotspot
	{
		DUEL_DECLARE_RTTI(DGStdLableHS)
	public:
		DGStdLableHS(DGStdLabel* parent = NULL);

		virtual DGWidget* getParent();
	protected:
		DGStdLabel*		mTargetLabel;

	};

	class DUELGUI_API DGStdLabelRD : public DGRenderDelegate
	{
		DUEL_DECLARE_RTTI(DGStdLabelRD)
	public:
		DGStdLabelRD(DGStdLabel* parent = NULL);

		virtual DGWidget* getParent();
	protected:
		DGStdLabel*		mTargetLabel;
	};

	class DUELGUI_API DGStdLabel : public DGWidget
	{
		DUEL_DECLARE_RTTI(DGStdLabel)
	public:
		DGStdLabel(DGWidget* parent = NULL);


	protected:

	};


}

#endif