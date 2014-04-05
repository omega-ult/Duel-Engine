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
		// make public, allowing external lock.
		DUEL_AUTO_MUTEX
		//////////////////////////////////////////////////////////////////////////
		// signals block 
		// use this function to adjust rendering operation.
		// for example rendering gui may use another technique.
		DSignal<void(DRenderQueue* queue, DRenderGroup* group)>
			signalGroupStartRender;
		DSignal<void(DRenderQueue* queue, DRenderGroup* group)>
			signalGroupFinishRender;
		/////////////////////////////////////////////////////////////////////////
		DRenderWorkshop();
		virtual ~DRenderWorkshop(); // destroy all render queues and render targets.

		virtual	void		render(DRenderQueue* queue) = 0;

		virtual void		renderSingleObject(DFrameBuffer* target, DRenderable* rendObj, DRenderPass* pass) = 0;

		// set the target rendersystem which was used to render renderables.
		virtual void		setRenderSystem(DRenderSystem* rSys) { mRenderSystem = rSys; }



		// use this function to bind final render tareget.
		// 使用这个函数绑定最终的渲染目标:
		virtual void		setPresentTarget(DFrameBuffer* target) { mPresentTarget = target; }
		DFrameBuffer*		getPresentTarget() { return mPresentTarget; }

				/// Method for preparing shadow textures ready for use in a regular render
		/// Do not call manually unless before frame start or rendering is paused
		/// If lightList is not supplied, will render all lights in frustum
   //     virtual void prepareShadowTextures(DCamera* cam, Viewport* vp, const LightList* lightList = 0);




	protected:
		// the target render system used to render renderables.
		DRenderSystem*	mRenderSystem;
				//////////////////////////////////////////////////////////////////////////
		DFrameBuffer*	mPresentTarget;
	};
}

#endif