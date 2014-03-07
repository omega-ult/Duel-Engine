//  [6/29/2013 OMEGA] created

#ifndef _DUELGUIRENDERDELEGATE_H_
#define _DUELGUIRENDERDELEGATE_H_

#include "DuelCommon.h"
#include "DuelGUICommon.h"
#include "DuelRenderable.h"

namespace Duel
{
	// used in renderting gui.
	class DUELGUI_API DGRenderDelegate
	{
	DUEL_DECLARE_RTTI(DGRenderDelegate)
	public:
		DGRenderDelegate() {}
		virtual ~DGRenderDelegate() {}

		virtual DGWidget*		getParent() = 0;

		// interface for update content.
		virtual	void	update() {}
		// used to add renderables.
		virtual	void	applyToRenderQueue(DRenderQueue* queue, uint32 groupID) {}
		
	private:

	};

}


#endif