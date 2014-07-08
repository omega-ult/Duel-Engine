//  [1/4/2013 OMEGA] created

#ifndef _DUELRENDERWORKSHOP_H_
#define _DUELRENDERWORKSHOP_H_

#include "DuelCommon.h"
#include "DuelRenderSystem.h"
#include "DuelRenderQueue.h"
#include "DuelViewport.h"
#include "DuelFrameBuffer.h"


namespace Duel
{

	// this class is an interface, designed for rendering order and render operations,
	// it is designed as a plugin, 
	class DUEL_API DRenderWorkshop : public DObject
	{
	DUEL_DECLARE_RTTI(DRenderWorkshop)
	public:
		DRenderWorkshop();
		virtual ~DRenderWorkshop(); // destroy all render queues and render targets.

		virtual	void		render(DRenderQueue* queue) = 0;

		virtual void		renderSingleObject(DFrameBuffer* target, DRenderable* rendObj, DRenderPass* pass) = 0;

	
		// use this function to bind final render tareget.
		virtual void		setPresentTarget(DFrameBuffer* target) { mPresentTarget = target; }
		DFrameBuffer*		getPresentTarget() { return mPresentTarget; }



	protected:
				//////////////////////////////////////////////////////////////////////////
		DFrameBuffer*	mPresentTarget;
	};
}

#endif