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

		virtual void		renderSingleObject(DRenderable* rendObj, DRenderPass* pass) = 0;

		// create a render queue, used to fill renderables, which was to be rendered.
		virtual DRenderQueue*	createRenderQueue();

		// destroy a render queue.
		virtual	void			destroyRenderQueue(DRenderQueue* queue);
		// destroy all render queues.
		virtual void			destroyAllRenderQueues();

		// set the target rendersystem which was used to render renderables.
		virtual void			setRenderSystem(DRenderSystem* rSys) { mRenderSystem = rSys; }



		// use this function to bind final render tareget.
		// 使用这个函数绑定最终的渲染目标:
		virtual void		setPresentTarget(DFrameBuffer* target) { mPresentTarget = target; }
		DFrameBuffer*		getPresentTarget() { return mPresentTarget; }

				/// Method for preparing shadow textures ready for use in a regular render
		/// Do not call manually unless before frame start or rendering is paused
		/// If lightList is not supplied, will render all lights in frustum
   //     virtual void prepareShadowTextures(DCamera* cam, Viewport* vp, const LightList* lightList = 0);



		// [8/20/2013 OMEGA] add render option 
		// options for rendering, customized in sub-class.
		// pushOption will set option as current render option, 
		// you can use popOptions to restore previous options.
		// render workshop will always use the top option to render
		// a group.
		typedef	std::multimap<DString, DString>	RenderOption;
		virtual	void	pushOption(const RenderOption& option)
		{
			mOptionStack.push(option);
		}
		virtual	void	popOption()
		{
			mOptionStack.pop();
		}
		


	protected:
		// the target render system used to render renderables.
		DRenderSystem*	mRenderSystem;

		// container for render queues.
		typedef	std::set<DRenderQueue*>	RenderQueueSet;
		RenderQueueSet	mQueueSet;

		//////////////////////////////////////////////////////////////////////////
		DFrameBuffer*	mPresentTarget;

		//////////////////////////////////////////////////////////////////////////
		//TODO:
		// 加入light的shadowmap, 根据type来区别
		// RenderTarget* mSpotLightShadowMap. 等.


		//////////////////////////////////////////////////////////////////////////
		
		typedef	std::stack<RenderOption>	OptionStack;
		OptionStack		mOptionStack;
	};
}

#endif