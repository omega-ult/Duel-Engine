//  [8/10/2013 OMEGA] created

#ifndef _DUELGUI2DBUTTONHS_H_
#define _DUELGUI2DBUTTONHS_H_

#include "DuelGUICommon.h"
#include "DuelGUIPredeclare.h"
#include "DuelGUIHotSpot.h"

namespace Duel
{
	// a 2DHotSpot is a plain rect on the screen.
	class DUELGUI_API DG2DButtonHS : public DGHotspot
	{
	DUEL_DECLARE_RTTI(DG2DButtonHS)
	public:
		DG2DButtonHS(DG2DButton* parent);

		//////////////////////////////////////////////////////////////////////////
		// override : DGHotSpot---------------------------------
		virtual bool response( const DEventBase* e );
		virtual void update();
		//////////////////////////////////////////////////////////////////////////

	protected:
		// a rect that indicate the absolute rect to the window.
		IRect		mAbsRect;
	};

}

#endif