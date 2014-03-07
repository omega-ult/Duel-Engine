//  [10/30/2012 OMEGA] created

#ifndef _DUELRENDERRESOURCEMANAGER_H_
#define _DUELRENDERRESOURCEMANAGER_H_

#include "DuelCommon.h"
#include "DuelSingleton.h"
#include "DuelHardwareBuffer.h"
#include "DuelRenderWindow.h"
#include "DuelRenderState.h"
#include "DuelVertexBuffer.h"
#include "DuelIndexBuffer.h"
#include "DuelRenderResourceFactory.h"

namespace Duel
{

	// this class is designed to provide hardware vertex buffer/index buffer 
	// without knowing which render system the system is based on. sub-system
	// should implement a specified 'factory', and register it into the 
	// HardwareBufferManager in the initialization process.
	class DUEL_API DRenderResourceManager : public DObject, public DSingleton<DRenderResourceManager>
	{
	DUEL_DECLARE_RTTI(DRenderResourceManager)
	public:
		// notice that this class has no responsibility for the lifetime of the
		// factory, but it act as a singleton, it will keep living as long as the 
		// program exist, so DO NOT delete the factory outside, it may cause serious 
		// problem!
		DRenderResourceManager(DRenderResourceFactory* factory);
		
		// these two methods are always available.
		DRawBufferPtr			createRawBuffer(DHardwareBuffer* src);
		DRawBufferPtr			createRawBuffer(size_t size);

		// create an empty vertex buffer, can be filled with whatever data, use this buffer with
		// VertexDeclaration cooperated. this VertexBuffer differs on different render system.
		DVertexBufferPtr		createVetexBuffer(size_t vertexSize, size_t verticesCount,
			HardwareBufferUsage usage, bool useShadow, VertexBufferType type = VBT_Geometry);
		// create an empty index buffer, can be filled with index data. this IndexBuffer differs on 
		// different render system, depending on the DHardwareBufferFactory
		DIndexBufferPtr			createIndexBuffer(IndexType itype, size_t indexCount, HardwareBufferUsage usage, bool useShadow);

		DRenderLayoutPtr		createRenderLayout();


		// create state object used in setting render state in graphic pipeline.
		//  [7/27/2013 OMEGA] added.
		//  [10/29/2013 OMEGA] moved from DRenderSystem.
		DRasterizerStateObjectPtr	createRasterizerStateObject(const DRasterizerState& state);
		DDepthStencilStateObjectPtr	createDepthStencilStateObject(const DDepthStencilState& state);
		DBlendStateObjectPtr		createBlendStateObject(const DBlendState& state);

		// create shader object for a render pass.
		DShaderObjectPtr			createShaderObject(DRenderPass* pass);


		// create a window with name and setting, with a platform-specified handle.
		// if specified render window is created, the current one will be returned.
		DRenderWindow*	createRenderWindow(const DString& name, const RenderWindowSetting& WndSetting, uint32 targetWnd);
		// destory a render window.
		void			destroyRenderWindow(DRenderWindow* wind);


		// use frame buffer to take place of render target, 
		DFrameBuffer*	createFrameBuffer(uint32 w, uint32 h, uint32 colorBits);
		void			destroyFrameBuffer(DFrameBuffer* buf);

	protected:


		// hold a reference to the factory.
		DRenderResourceFactory*	mFactory;
		// store render windows.
		typedef	std::set<DRenderWindow*>	RenderWindowSet;
		RenderWindowSet			mRenderWindowSet;
	private:
		// forbid default constructor
		DRenderResourceManager() {}
	};


}

#endif
