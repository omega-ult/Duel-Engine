//  [11/10/2012 OMEGA] created

#ifndef _DUELGUIHOTSPOT_H_
#define _DUELGUIHOTSPOT_H_

#include "DuelCommon.h"
#include "DuelGUICommon.h"
#include "DuelGUIPredeclare.h"

namespace Duel
{

	// a HotSpot is a class used to response whether the cursor is in it
	// it is designed to be owned by GUIFrame, and tested by GUIManager
	class DUELGUI_API DGHotspot : public DObject
	{
	DUEL_DECLARE_RTTI(DGHotspot)
	public:
		// constructor, should only be created by the GUIComponent.
		DGHotspot(){}
		virtual ~DGHotspot() {}
		virtual DGWidget*		getParent() = 0; 

		//  [8/6/2013 OMEGA] ¸ü»»½Ó¿Ú.
		virtual	bool			response(const DEventBase* e) { return false; }
		// used to update properties.
		virtual void			update() {}
	protected:
	};


}

#endif