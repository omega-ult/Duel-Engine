//  [6/23/2013 OMEGA] created

#ifndef _DUELBASICRENDERWORKSHOP_H_
#define _DUELBASICRENDERWORKSHOP_H_

#include "DuelCommon.h"

namespace Duel
{


	class DUEL_API DABasicRenderWorkshop : public DRenderWorkshop
	{
		DUEL_DECLARE_RTTI(DABasicRenderWorkshop)
	public:
		DABasicRenderWorkshop();

		// override : DRenderWorkshop-----------------------
		virtual	void	setRenderTarget(DRenderTarget* target);
		// override : DRenderWorkshop---------------------
		virtual void	renderDeferStage();
		// override : DRenderWorkshop---------------------
		virtual void	renderForwardStage();
		// override : DRenderWorkshop---------------------
		virtual void	setOptions( const RenderOption& option );

	protected:
		DRenderTarget*	mDefferTexture[4];

	};
}

#endif