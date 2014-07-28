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
		enum 
		{
			RS_DeferAmbientLight	= 0xc100
		};
		// only accept RS_DeferAmbientLight as valid stage parameter
		// override : DRenderable--------------------------
		virtual DRenderTechnique* getRenderTechnique(uint32 stage, DCamera* cam, LightIterator li);
		// override : DRenderable--------------------------
		void	updateCustomGpuParameter(DShaderObject* so);

		void	setAmbientLightParameter(DLightSource* light);
	protected:
		DRenderTechniquePtr	mAmbientLightTech;
		DColor	mAmbientColor;
	};



	// helper class to render point light to light accumulation map.
	class DDemoPointLightHelper : public DRenderable
	{
		DUEL_DECLARE_RTTI(DDemoPointLightHelper)
	public:
		DDemoPointLightHelper();
		enum 
		{
			RS_DeferPointLight	= 0xc110
		};
		// only accept RS_DeferPointLight as valid stage parameter
		// override : DRenderable--------------------------
		virtual DRenderTechnique* getRenderTechnique(uint32 stage, DCamera* cam, LightIterator li);
		// override : DRenderable--------------------------
		void	updateCustomGpuParameter(DShaderObject* so);

		// use specified point light to upate parameter.
		void	setPointLightParameter(DLightSource* light);

		// these texture constant will be reset as NULL after
		// updateCustomGpuParameter() finished
		void	setViewSpaceNormaTexture(DGpuTextureConstantPtr tex);
		void	setDepthTexture(DGpuTextureConstantPtr tex);

	protected:
		DRenderTechniquePtr	mPointLightTech;

		DVector3	mLightPos;
		DReal		mLightRadius;
		DColor		mDiffuseColor;
		DColor		mSpecularColor;

		DGpuTextureConstantPtr	mVNormalTexture;
		DTextureSamplerObjectPtr	mVNormalSampler;
		DGpuTextureConstantPtr	mDepthTexture;
		DTextureSamplerObjectPtr	mDepthSampler;
	};


	// helper class to render directional light to light accumulation map.
	class DDemoDirectionalLightHelper : public DRenderable
	{
		DUEL_DECLARE_RTTI(DDemoDirectionalLightHelper)
	public:
		DDemoDirectionalLightHelper();
		enum 
		{
			RS_DeferDirectionalLight	= 0xc120
		};
		// only accept RS_DeferDirectionalLight as valid stage parameter
		// override : DRenderable--------------------------
		virtual DRenderTechnique* getRenderTechnique(uint32 stage, DCamera* cam, LightIterator li);
		// override : DRenderable--------------------------
		void	updateCustomGpuParameter(DShaderObject* so);

		// use specified directional light to upate parameter.
		void	setDirectionalLightParameter(DLightSource* light);
	protected:
		DRenderTechniquePtr	mDirectionalLightTech;
	};



	// helper class to render spot light to light accumulation map.
	class DDemoSpotlightHelper : public DRenderable
	{
		DUEL_DECLARE_RTTI(DDemoSpotlightHelper)
	public:
		DDemoSpotlightHelper();
		enum 
		{
			RS_DeferSpotlight	= 0xc130
		};
		// only accept RS_DeferSpotlight as valid stage parameter
		// override : DRenderable--------------------------
		virtual DRenderTechnique* getRenderTechnique(uint32 stage, DCamera* cam, LightIterator li);
		// override : DRenderable--------------------------
		void	updateCustomGpuParameter(DShaderObject* so);

		// use specified spot light to upate parameter.
		void	setSpotlightParameter(DLightSource* light);
	protected:
		DRenderTechniquePtr	mSpotlightTech;
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

		DTextureSamplerObjectPtr	mTextureSampler;
	};

	class DDemoCopyTextureHelper : public DRenderable
	{
		DUEL_DECLARE_RTTI(DDemoCopyTextureHelper)
	public:
		DDemoCopyTextureHelper();
		// for RS_ScreenTransfer only
		void	setTransferSource(DGpuTextureConstantPtr tex);

		// override : DRenderable--------------------------
		// only RS_DeferMerge can return a valid render layout.
		virtual DRenderTechnique* getRenderTechnique(uint32 stage, DCamera* cam, LightIterator li);

		// override : DRenderable--------------------------
		void	updateCustomGpuParameter(DShaderObject* so);
	protected:

		DGpuTextureConstantPtr	mSrcTex;
		DRenderTechniquePtr		mCopyTech;
		DTextureSamplerObjectPtr	mTextureSampler;

	};

}


#endif