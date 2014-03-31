//  [6/23/2013 OMEGA] created

#ifndef _DUELDEMORENDERWORKSHOP_H_
#define _DUELDEMORENDERWORKSHOP_H_

#include "DuelCommon.h"
#include "DuelRenderWorkshop.h"

namespace Duel
{
	// this class provide functionalities for DemoRenderWorkshop's defer rendering,
	// such as render a texture to a render target, clear GBuffer...
	// make sure the render target is in framebuffer color attachment_0.
	// render technique based on _BasicShaderPack/Demo_RenderWorkshop.dre
	class DDemoDeferHelper : public DRenderable
	{
		DUEL_DECLARE_RTTI(DDemoDeferHelper)
	public:
		enum 
		{
			RS_Compose				= 0xc200,
			RS_ScreenQuadTransfer	= 0xc300
		};
		DDemoDeferHelper();

		// used in RS_ScreenQuadTransfer
		void	setInputTexture(DGpuTextureConstant* tex);

		// override : DRenderable--------------------------
		virtual DRenderLayout* getRenderLayout();
		// override : DRenderable--------------------------
		// only RS_ScreenQuadTransfer can return a valid render layout.
		virtual DRenderTechnique* getRenderTechnique( uint32 stage );

		// override : DRenderable--------------------------
		void	updateCustomGpuParameter(DShaderObject* so);

	protected:
		DGpuTextureConstant*	mInputTex;
		
		DRenderTechniquePtr	mCopyTexTech;
		DRenderTechniquePtr	mGBufferTech;	
		DRenderTechniquePtr	mComposeTech;
		DVertexData			mVData;
		DIndexData			mIData;
		DRenderLayoutPtr	mRenderLayout;
	};

	class DDemoRenderWorkshop : public DRenderWorkshop
	{
	DUEL_DECLARE_RTTI(DDemoRenderWorkshop)
	public:
		DDemoRenderWorkshop();
		~DDemoRenderWorkshop();

		struct DeferLayer
		{
			DeferLayer() : GBuffer(NULL), LightAccum(NULL) {}
			DFrameBuffer*	GBuffer;
			DFrameBuffer*	LightAccum;
		};
		// get current present target's defer layer.
		DeferLayer		getCurrentDeferlayer();

		// override : DRenderWorkshop---------------------
		void			renderSingleObject(DRenderable* rendObj, DRenderPass* pass);

		// override : DRenderWorkshop-----------------------
		// if you bind a framebuffer which is in one of our DeferLayer, this is generally
		// ok but you can not render() onto it using RS_Defer stage.
		virtual void	setPresentTarget(DFrameBuffer* target);
		// override : DRenderWorkshop-----------------------
		virtual void	render( DRenderQueue* queue );

		
	protected:
		void			populateRenderables(DRenderGroup* grp, uint32 renderStage);
		DeferLayer		createDeferLayer(DFrameBuffer* target);
		void			destryDeferLayer(DeferLayer lay);
		// because we are in a multi render target environment,
		// we need manage gbuffer for each present target, here we
		// can do some memory management to reduce the cost.
		typedef std::map<DFrameBuffer*, DeferLayer>	DeferLayerMap;
		DeferLayerMap		mDeferLayerMap;

		DDemoDeferHelper	mDeferHelper;


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