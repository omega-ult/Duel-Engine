//  [10/30/2012 OMEGA] created

#ifndef _DUELRENDERRESOURCEFACTORY_H_
#define _DUELRENDERRESOURCEFACTORY_H_

#include "DuelCommon.h"
#include "DuelHardwareBuffer.h"
#include "DuelRenderState.h"
#include "DuelVertexBuffer.h"
#include "DuelIndexBuffer.h"

namespace Duel
{
	// this class provide some basic interface for HardwareBufferManager
	// to create vertex/index buffer, this class should be implemented
	// in specified render system. 
	class DUEL_API DRenderResourceFactory : public DObject
	{
	DUEL_DECLARE_RTTI(DRenderResourceFactory)
	public:
		// create an empty vertex buffer, can be filled with whatever data, use this buffer with
		// VertexDeclaration cooperated. this VertexBuffer differs on different render system.
		virtual	DVertexBufferPtr		createVetexBuffer(size_t vertexSize, size_t verticesCount,
			HardwareBufferUsage usage, bool useShadow, VertexBufferType type) = 0;
		// create an empty index buffer, can be filled with index data. this IndexBuffer differs on 
		// different render system, depending on the DHardwareBufferFactory
		virtual	DIndexBufferPtr			createIndexBuffer(IndexType itype, size_t indexCount, HardwareBufferUsage usage, bool useShadow) = 0;

		// vertexbinding.
		virtual	DRenderLayoutPtr		createRenderLayout() = 0;

		// create state object used in setting render state in graphic pipeline.
		//  [7/27/2013 OMEGA] added.
		virtual	DRasterizerStateObjectPtr	createRasterizerStateObject(const DRasterizerState& state) = 0;
		virtual	DDepthStencilStateObjectPtr	createDepthStencilStateObject(const DDepthStencilState& state) = 0;
		virtual	DBlendStateObjectPtr		createBlendStateObject(const DBlendState& state) = 0;

		// create shader object for a render pass.
		virtual	DShaderObjectPtr			createShaderObject(DRenderPass* pass) = 0;
		
		// create a window with name and setting, with a platform-specified handle.
		// this method is called by DRenderResourceManager, 
		virtual DRenderWindow*		createRenderWindow(const DString& name) = 0;
		// destory a render window. this method is called by DRenderResourceManager, 
		virtual void			destroyRenderWindow(DRenderWindow* wind) = 0;


		// use frame buffer to take place of render target,  this method is called by DRenderResourceManager, 
		virtual DFrameBuffer*	createFrameBuffer(uint32 w, uint32 h, uint32 colorbits) = 0;
		// destroy a buffer,  this method is called by DRenderResourceManager, 
		virtual void			destroyFrameBuffer(DFrameBuffer* buf) = 0;

		virtual	DRenderColorView*	createRenderColorView(DPixelFormat fmt) = 0;
		virtual	void			destroyRenderColorView(DRenderColorView* v) = 0;
		virtual	DRenderDepthStencilView*	createRenderDepthStencilView() = 0;
		virtual	void			destroyRenderDepthStencilView(DRenderDepthStencilView* v) = 0;
	};
}


#endif
