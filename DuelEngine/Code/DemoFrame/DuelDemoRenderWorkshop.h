//  [6/23/2013 OMEGA] created

#ifndef _DUELDEMORENDERWORKSHOP_H_
#define _DUELDEMORENDERWORKSHOP_H_

#include "DuelCommon.h"
#include "DuelRenderWorkshop.h"
#include "DuelDemoDeferRenderHelper.h"

namespace Duel
{

	class DDemoRenderWorkshop : public DRenderWorkshop
	{
	DUEL_DECLARE_RTTI(DDemoRenderWorkshop)
	public:
		DDemoRenderWorkshop();
		~DDemoRenderWorkshop();

		class DeferLayer
		{
		public:
			DeferLayer();
			~DeferLayer();
			void	initialize(uint32 w, uint32 h);
			void	prepareGBufferStage();
			void	prepareLightingStage();
			void	prepareMergeStage();
			DFrameBuffer*		getFrameBuffer();
			DRenderColorView*	getAlbedoMap();
			DRenderColorView*	getDepthMap();
			DRenderColorView*	getViewSpaceNormalMap();
			DRenderColorView*	getLightAccumulationMap();
			DRenderColorView*	getMergedColorMap();
			void	shutdown();
		private:
			DFrameBuffer*		mFrameBuffer;
			DRenderColorView*	mAlbedo;
			DRenderColorView*	mDepth;
			DRenderColorView*	mViewSpaceNormal;
			DRenderColorView*	mLightAccum;
			DRenderColorView*	mMergeView;
		};
		// get current present target's defer layer.
		DeferLayer*		getCurrentDeferlayer();

		// override : DRenderWorkshop---------------------
		void			renderSingleObject(DFrameBuffer* target, DRenderable* rendObj, DRenderPass* pass);

		// override : DRenderWorkshop-----------------------
		// if you bind a framebuffer which is in one of our DeferLayer, this is generally
		// ok but you can not render() onto it using RS_Defer stage.
		virtual void	setPresentTarget(DFrameBuffer* target);
		// override : DRenderWorkshop-----------------------
		virtual void	render( DRenderQueue* queue );

		
	protected:
		void			populateRenderables(DRenderGroup* grp, uint32 renderStage);
		DeferLayer*		createDeferLayer(DFrameBuffer* target);
		void			destroyDeferLayer(DeferLayer* lay);
		// because we are in a multi render target environment,
		// we need manage gbuffer for each present target, here we
		// can do some memory management to reduce the cost.
		typedef std::map<DFrameBuffer*, DeferLayer*>	DeferLayerMap;
		DeferLayerMap		mDeferLayerMap;


		DDemoMergeHelper	mMergeHelper;


		struct RenderElement
		{
			DRenderable* renderable;
			DRenderPass* renderPass;
		};
		typedef std::vector<RenderElement>	RenderElementList;
		RenderElementList	mRenderList;
	};
}

#endif