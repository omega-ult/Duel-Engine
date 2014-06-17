/*
	[7/15/2012 OMEGA] created
	this file contains the basic definitions for an inherited class that
	provides the rendering ability.
*/
//  [4/11/2013 OMEGA] 改用复杂参数去创建渲染窗口.

#ifndef _DUELIRENDERSYSTEM_H_
#define _DUELIRENDERSYSTEM_H_

#include "DuelCommon.h"
#include "DuelViewport.h"
#include "DuelGpuProgram.h"
#include "DuelGpuParameter.h"
#include "DuelRenderLayout.h"
#include "DuelRenderState.h"
#include "DuelRenderDeviceCaps.h"
#include "DuelRenderWindow.h"
#include "DuelFrameBuffer.h"
#include "DuelShaderObject.h"

namespace Duel {

	enum GpuConstantSlot
	{
		GCS_Int,
		GCS_Float,
		GCS_Bool
	};
	
	// base class for different rendersystems to inherite
	class DUEL_API DRenderSystem : public DObject
	{
	DUEL_DECLARE_RTTI(DRenderSystem)
	public:
		DRenderSystem();
		virtual ~DRenderSystem();
		
		// a name must be provided by subclass
		virtual const DString&  getName() const = 0;
		
		// this function give the rendersystem the ability of adapting different
		// application environment, for example, add <"WindowHandle", HWND*> into 
		// the options to intialize devices in Windows platform.
		virtual	void			initialize() = 0;
		// in this function do resource clearup
		virtual void			shutdown() = 0;

		//  [10/30/2013 OMEGA]
		// move to RenderResourceManager.
// 		// create a window with name and setting, with a platform-specified handle.
// 		virtual DRenderWindow*	createRenderWindow(const DString& name, const RenderWindowSetting& WndSetting, uint32 targetWnd) = 0;
// 		// destory a render window.
// 		virtual	void			destroyRenderWindow(DRenderWindow* wind) = 0;
// 
// 		// use frame buffer to take place of render target, 
// 		virtual	DFrameBuffer*	createFrameBuffer(uint32 w, uint32 h, DPixelFormat colorFmt) = 0;
// 		virtual	void			destroyFrameBuffer(DFrameBuffer* buf) = 0;

		// bind frame buffer for rendering.
		virtual	void			bindFrameBuffer(DFrameBuffer* buf) = 0;
		virtual	DFrameBuffer*	getCurrentFrameBuffer() = 0;


		// TODO:
		// 加入参数抽象化
		// sub-class should set state depends on render api.
		virtual	void			setRasterizerState(DRasterizerStateObject* rs) = 0;
		virtual void			setDepthStencilState(DDepthStencilStateObject* dss, uint32 frontStencilRef = 0, uint32 backStencilRef = 0) = 0;

		//  [3/25/2013 OMEGA] 新版本要加上的东西:
		// set blend state for specified render target, including main window(usually with index 0);
		virtual	void			setBlendState(DBlendStateObject* sbs, const DColor& blendFactor) = 0;

	
		//  [7/27/2013 OMEGA] removed. used state object instead.
		// get current state, use specified state to retrieve result
		virtual	DRasterizerStateObjectPtr	getCurrentRaseterizerState() = 0;
		virtual	DDepthStencilStateObjectPtr	getCurrentDepthStencilStateObject() = 0;
		virtual	DBlendStateObjectPtr		getCurrentBlendState() = 0;

		virtual	void						bindShaderObject(DShaderObject* so) = 0;

		// TODO: 添加参数
		// render an object
		virtual void		render(DRenderLayout* layout) = 0;
		


		// render system capbilities---------------------
		virtual	void		fillDeviceCaps() = 0;
		
		virtual	uint32		queryMaxTextureWidth() { return mCaps.MaxTextureWidth; }
		virtual	uint32		queryMaxTextureHeight() { return mCaps.MaxTextureHeight; }
		virtual	uint32		queryMaxTextureDepth() { return mCaps.MaxTextureDepth; }

		virtual	uint32		queryMaxTextureUnit() { return mCaps.MaxTextureUnits; }
		virtual	uint32		queryStencilBufferBitDepth() { return mCaps.StencilBufferBitDepth; }
		virtual	uint32		queryMaxVertexBlendMatrices() { return mCaps.MaxVertexBlendMatrices; }

		virtual	uint32		queryMaxPrimitiveCount() { return mCaps.MaxPrimitiveCount; }
		virtual	uint32		queryMaxVertexIndex() { return mCaps.MaxVertexIndex; }
		virtual	uint32		queryMaxStreams() { return mCaps.MaxStreams; }

		virtual	uint32		queryMaxMultiRenderTargets() { return mCaps.MaxMultiRenderTargets; }

		virtual	uint32		queryGpuConstantCaps(GpuProgramType gpt, GpuConstantSlot gcs);

		virtual	bool		isSupportShader(const DString& shader) { return mCaps.isSupportShader(shader); }


	protected:

		DReal				mGamma;

		// capability of the device.
		RenderDeviceCaps	mCaps;

	};

}



#endif