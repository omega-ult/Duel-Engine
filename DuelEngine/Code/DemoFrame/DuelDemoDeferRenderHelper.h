//  [4/7/2014 OMEGA] created

#ifndef _DUELDEMODEFERRENDERHELPER_H_
#define _DUELDEMODEFERRENDERHELPER_H_

#include "DuelCommon.h"
#include "DuelRenderable.h"

namespace Duel
{
	//////////////////////////////////////////////////////////////////////////
	// light accumulation map generation.
	// helper class to render ambient light to light accumulation map.
	class DDemoAmbientLightHelper : public DRenderable
	{
		DUEL_DECLARE_RTTI(DDemoAmbientLightHelper)
	public:
		DDemoAmbientLightHelper();
		virtual DRenderTechnique* getRenderTechnique(uint32 stage, DCamera* cam, LightIterator li);
	protected:
		DRenderTechniquePtr	mAmbientAccumTech;
	};



	// helper class to render point light to light accumulation map.
	class DDemoPointLightHelper : public DRenderable
	{
		DUEL_DECLARE_RTTI(DDemoPointLightHelper)
	public:
		DDemoPointLightHelper();
		virtual DRenderTechnique* getRenderTechnique(uint32 stage, DCamera* cam, LightIterator li);
	protected:
		DRenderTechniquePtr	mPointLightAccumTech;
	};


	// helper class to render directional light to light accumulation map.
	class DDemoDirectionalLightHelper : public DRenderable
	{
		DUEL_DECLARE_RTTI(DDemoDirectionalLightHelper)
	public:
		DDemoDirectionalLightHelper();
		virtual DRenderTechnique* getRenderTechnique(uint32 stage, DCamera* cam, LightIterator li);
	protected:
		DRenderTechniquePtr	mDirectionalLightAccumTech;
	};



	// helper class to render spot light to light accumulation map.
	class DDemoSpotLightHelper : public DRenderable
	{
		DUEL_DECLARE_RTTI(DDemoSpotLightHelper)
	public:
		DDemoSpotLightHelper();
		virtual DRenderTechnique* getRenderTechnique(uint32 stage, DCamera* cam, LightIterator li);
	protected:
		DRenderTechniquePtr	mSpotLightAccumTech;
	};

	// this class provide functionalities for DemoRenderWorkshop's defer rendering,
	// such as render a texture to a render target, clear GBuffer...
	// make sure the render target is in framebuffer color attachment_0.
	// render technique based on _BasicShaderPack/Demo_RenderWorkshop.dre
	class DDemoMergeHelper : public DRenderable
	{
		DUEL_DECLARE_RTTI(DDemoMergeHelper)
	public:
		enum 
		{
			RS_DeferMerge		= 0xc200,
			RS_ScreenTransfer	= 0xc300
		};
		DDemoMergeHelper();

		// for RS_DeferMerge only
		void	setInputAlbedo(DGpuTextureConstantPtr tex);
		void	setInputLightAccumulationMap(DGpuTextureConstantPtr tex);
		void	setInputDepth(DGpuTextureConstantPtr tex);

		// for RS_ScreenTransfer only
		void	setTransferSource(DGpuTextureConstantPtr tex);

		// override : DRenderable--------------------------
		// only RS_DeferMerge can return a valid render layout.
		virtual DRenderTechnique* getRenderTechnique(uint32 stage, DCamera* cam, LightIterator li);

		// override : DRenderable--------------------------
		void	updateCustomGpuParameter(DShaderObject* so);

	protected:
		DGpuTextureConstantPtr	mAlbedoTex;
		DGpuTextureConstantPtr	mLightAccumTex;
		DGpuTextureConstantPtr	mDepthTex;

		DGpuTextureConstantPtr	mTransSrc;

		DRenderTechniquePtr		mMergeTech;
		DRenderTechniquePtr		mScrnTransTech;
	};


}


#endif