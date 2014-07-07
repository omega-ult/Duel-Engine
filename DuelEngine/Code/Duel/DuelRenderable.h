//  [12/2/2012 OMEGA] created

#ifndef _DUELRENDERABLE_H_
#define _DUELRENDERABLE_H_

#include "DuelCommon.h"
#include "DuelRenderLayout.h"
#include "DuelGpuProgram.h"
#include "DuelRenderTechnique.h"

namespace Duel
{
// 	// stages that a renderable can use,
	enum RenderStage
	{
		RS_Defer_GBuffer,
		RS_LightMapGeneration,
		RS_Forward
	};

	// flag that indicate which type of shadow this renderable prefer using
	// dynamic shadow will be affected by dynamic light, so does the static shadow,
	// otherwise using SP_None to disable shadow rendering.
	enum ShadowProperty
	{
		SP_Dynamic,
		SP_Static,
		SP_None
	};

// 
// 	// defer shading techniques used in defer shading
// 	enum DeferRenderStage
// 	{
// 		DRS_OPAQUE_GBUFFER,
// 		DRS_TRANSPARENCY_BACK_GBUFFER,
// 		DRS_TRANSPARENCY_FRONT_GBUFFER,
// 		DRS_OPAQUE_MRTGBUFFER,
// 		DRS_TRANSPARENCY_BACK_MRTGBUFFER,
// 		DRS_TRANSPARENCY_FRONT_MRTGBUFFER,
// 		DRS_GEN_SHADOWMAP,
// 		DRS_GEN_REFLACTIVE_SHADOWMAP,
// 		DRS_LIGHTING,
// 		DRS_INDIRECT_LIGHTING,
// 		DRS_OPAQUE_SHADING,
// 		DRS_TRANSPARENCY_BACK_SHADING,
// 		DRS_TRANSPARENCY_FRONT_SHADING,
// 		DRS_OPAQUE_SPECIAL_SHADING,
// 		DRS_TRANSPARENCY_BACK_SPECIAL_SHADING,
// 		DRS_TRANSPARENCY_FRONT_SPECIAL_SHADING
// 	};

	// 每个renderable带有一个technique
	// common interface class designed for renderable objects.
	class DUEL_API DRenderable : public DObject
	{
	DUEL_DECLARE_RTTI(DRenderable)
	public:
		DRenderable();
		virtual ~DRenderable() {}

		// get the render layout of this renderable object.
		// param rs indicate which stage the layout belongs to
		//  [6/28/2013 OMEGA] no need for stage specification
		virtual	DRenderLayout*		getRenderLayout();

		typedef	SetIterator<std::set<DLightSource*>>	LightIterator;
		// get the technique of forward rendering of object,
		// pre-defined render stages are defined above, different renderworkshop may
		// have different render stage defined, read their specification before you
		// implement if you need to make it available, otherwise just return NULL.
		virtual DRenderTechnique*	getRenderTechnique(uint32 stage, DCamera* cam, LightIterator li) = 0;

		// get the depth to the camera,
		//virtual	DReal	getViewDepthToCamera(const DCamera* cam) const { return Math::Sqrt(getSquaredViewDepthToCamera(cam)); }
		virtual	DReal	getSquaredViewDepthToCamera(const DCamera* cam);

		// method used for retrieving world transform matrix of the renderable.
		virtual	void	getWorldTransform(DMatrix4& outMat) {}
		
		// update auto GpuParameter, sub-class could omit this.
		INTERNAL virtual void	updateAutoGpuParameter(DShaderObject* so);

		// update custom GpuParameter, sub-class should implement this
		INTERNAL virtual void	updateCustomGpuParameter(DShaderObject* so) {};

		// called before actual rendering 
		INTERNAL virtual void	preRender() { mCacheCamera = NULL; mCacheDepth = 0.0f; }
		// called after actual rendering
		INTERNAL virtual void	postRender() { mCacheCamera = NULL; mCacheDepth = 0.0f; }

	protected:
		const DCamera*			mCacheCamera;
		DReal					mCacheDepth;
		ShadowProperty			mShadowProperty;
		DRenderLayoutPtr		mRenderLayout;
	};

}

#endif