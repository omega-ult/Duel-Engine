//  [4/7/2014 OMEGA] created

#ifndef _DUELDEMODEFERRENDERHELPER_H_
#define _DUELDEMODEFERRENDERHELPER_H_

#include "DuelCommon.h"

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
		void	setInputTexture(DGpuTextureConstantPtr tex);

		// override : DRenderable--------------------------
		// only RS_ScreenQuadTransfer can return a valid render layout.
		virtual DRenderTechnique* getRenderTechnique( uint32 stage );

		// override : DRenderable--------------------------
		void	updateCustomGpuParameter(DShaderObject* so);

	protected:
		DGpuTextureConstantPtr	mInputTex;

		DRenderTechniquePtr	mCopyTexTech;
		DRenderTechniquePtr	mGBufferTech;	
		DRenderTechniquePtr	mComposeTech;
	};

	//////////////////////////////////////////////////////////////////////////
	// light accumulation map generation.
	// helper class to render ambient light to light accumulation map.
	class DDemoAmbientLightHelper : public DRenderable
	{
		DUEL_DECLARE_RTTI(DDemoAmbientLightHelper)
	public:
		DDemoAmbientLightHelper();
		virtual DRenderTechnique* getRenderTechnique( uint32 stage );
	protected:
		DRenderTechniquePtr	mAmbientAccumTech;
	};



	// helper class to render point light to light accumulation map.
	class DDemoPointLightHelper : public DRenderable
	{
		DUEL_DECLARE_RTTI(DDemoPointLightHelper)
	public:
		DDemoPointLightHelper();
		virtual DRenderTechnique* getRenderTechnique( uint32 stage );
	protected:
		DRenderTechniquePtr	mPointLightAccumTech;
	};


	// helper class to render directional light to light accumulation map.
	class DDemoDirectionalLightHelper : public DRenderable
	{
		DUEL_DECLARE_RTTI(DDemoDirectionalLightHelper)
	public:
		DDemoDirectionalLightHelper();
		virtual DRenderTechnique* getRenderTechnique( uint32 stage );
	protected:
		DRenderTechniquePtr	mDirectionalLightAccumTech;
	};



	// helper class to render spot light to light accumulation map.
	class DDemoSpotLightHelper : public DRenderable
	{
		DUEL_DECLARE_RTTI(DDemoSpotLightHelper)
	public:
		DDemoSpotLightHelper();
		virtual DRenderTechnique* getRenderTechnique( uint32 stage );
	protected:
		DRenderTechniquePtr	mSpotLightAccumTech;
	};


}


#endif