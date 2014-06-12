//  [6/6/2014 OMEGA] created

#include "DuelD3D9Common.h"
#include "DuelRenderResourceFactory.h"
#include "DuelD3D9FrameBuffer.h"
#include "DuelD3D9RenderView.h"
#include "DuelD3D9RenderWindow.h"
#include "DuelD3D9RenderLayout.h"
#include "DuelD3D9RenderState.h"
#include "DuelD3D9ShaderObject.h"
#include "DuelD3D9IndexBuffer.h"
#include "DuelD3D9VertexBuffer.h"
#include "DuelD3D9RenderResourceFactory.h"

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(D3D9RenderResourceFactory, DRenderResourceFactory);

	Duel::DVertexBufferPtr D3D9RenderResourceFactory::createVetexBuffer( size_t vertexSize, size_t verticesCount, HardwareBufferUsage usage, bool useShadow, VertexBufferType type )
	{
		return DVertexBufferPtr(new D3D9VertexBuffer(vertexSize, verticesCount,usage, useShadow));
	}

	Duel::DIndexBufferPtr D3D9RenderResourceFactory::createIndexBuffer( IndexType itype, size_t indexCount, HardwareBufferUsage usage, bool useShadow )
	{
		return DIndexBufferPtr(new D3D9IndexBuffer(itype, indexCount, usage, useShadow));
	}

	Duel::DRenderLayoutPtr D3D9RenderResourceFactory::createRenderLayout()
	{
		return DRenderLayoutPtr(new D3D9RenderLayout(this));
	}

	Duel::DRasterizerStateObjectPtr D3D9RenderResourceFactory::createRasterizerStateObject( const DRasterizerState& state )
	{
		return DRasterizerStateObjectPtr(new D3D9RasterizerStateObject(state));
	}

	Duel::DDepthStencilStateObjectPtr D3D9RenderResourceFactory::createDepthStencilStateObject( const DDepthStencilState& state )
	{
		return DDepthStencilStateObjectPtr(new D3D9DepthStencilStateObject(state));
	}

	Duel::DBlendStateObjectPtr D3D9RenderResourceFactory::createBlendStateObject( const DBlendState& state )
	{
		return DBlendStateObjectPtr(new D3D9BlendStateObject(state));
	}

	Duel::DTextureSamplerObjectPtr D3D9RenderResourceFactory::createTextureSamplerObject( const DTextureSampler& state )
	{
		return DTextureSamplerObjectPtr(new D3D9TextureSamplerObject(state));
	}

	Duel::DShaderObjectPtr D3D9RenderResourceFactory::createShaderObject( DRenderPass* pass )
	{
		return DShaderObjectPtr(new D3D9ShaderObject());
	}

	DRenderWindow* D3D9RenderResourceFactory::createRenderWindow( const DString& name )
	{
		return NULL;
	}

	void D3D9RenderResourceFactory::destroyRenderWindow( DRenderWindow* wind )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	DFrameBuffer* D3D9RenderResourceFactory::createFrameBuffer( uint32 w, uint32 h, uint32 colorbits )
	{
		return new D3D9FrameBuffer(this, w, h, colorbits);
	}

	void D3D9RenderResourceFactory::destroyFrameBuffer( DFrameBuffer* buf )
	{
		if (buf != NULL && buf->getCreator() == this)
		{
			delete buf;
		}
	}

	DRenderColorView* D3D9RenderResourceFactory::createRenderColorView( DPixelFormat fmt )
	{
		return new D3D9RenderColorView(this, fmt);
	}

	void D3D9RenderResourceFactory::destroyRenderColorView( DRenderColorView* v )
	{
		if (v != NULL && v->getCreator() == this)
		{
			delete v;
		}
	}

	DRenderDepthStencilView* D3D9RenderResourceFactory::createRenderDepthStencilView()
	{
		return new D3D9RenderDepthStencilView(this);
	}

	void D3D9RenderResourceFactory::destroyRenderDepthStencilView( DRenderDepthStencilView* v )
	{
		if (v != NULL && v->getCreator() == this)
		{
			delete v;
		}
	}

	void D3D9RenderResourceFactory::initialize()
	{

	}

	void D3D9RenderResourceFactory::shutdown()
	{

	}

}