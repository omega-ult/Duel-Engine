//  [7/16/2013 OMEGA] created

#ifndef _DUELGUISYSTEM_H_
#define _DUELGUISYSTEM_H_

#include "DuelCommon.h"
#include "DuelGUICommon.h"
#include "DuelGUIManager.h"

namespace Duel
{
	// guisystem is the top level class for accessing gui element, 
	// and it is designed to provide flexiblity for creating user-defined
	// gui, the gui component receives event as top-down order and 
	// use user-defined hotspot area to test mouse event, and draw with
	// user-defined renderdelegate.
	class DUELGUI_API DGUISystem : public DObject, public DSingleton<DGUISystem>
	{
	DUEL_DECLARE_RTTI(DGUISystem)
	public:
		DGUISystem();
		~DGUISystem();

		// load basic Resources for the standard widgets.
		void		initialize() {}

		// define pair for combining DRenderWindow and DGUIManager.
		typedef	std::map<DRenderWindow*, DGUIManager*>	GUIManagerMap;
		typedef	MapIterator<GUIManagerMap>				GUIManagerMapIterator;
		GUIManagerMapIterator		getGUIManagerIterator() { return GUIManagerMapIterator(mManagerMap.begin(), mManagerMap.end()); }

		// register a window and create a GUIManager which handles the 
		// events in that window
		DGUIManager*	createGUIManager(DRenderWindow* wind);
		// get specified guimanager for specified window, if the window
		// is not registered, a NULL will be returned.
		DGUIManager*	getGUIManager(DRenderWindow* wind);
		// unregister a window, and destroy the corresponding guimanager.
		void			destroyGUIManager(DRenderWindow* wind);

	protected:
		GUIManagerMap	mManagerMap;
	};


}
#endif