//  [6/6/2014 OMEGA] created

#include "DuelD3D9Common.h"
#include "DuelRenderResourceFactory.h"
#include "DuelD3D9RenderResourceFactory.h"

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(D3D9RenderResourceFactory, DRenderResourceFactory);




	Duel::DVertexBufferPtr D3D9RenderResourceFactory::createVetexBuffer( size_t vertexSize, size_t verticesCount, HardwareBufferUsage usage, bool useShadow, VertexBufferType type )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	Duel::DIndexBufferPtr D3D9RenderResourceFactory::createIndexBuffer( IndexType itype, size_t indexCount, HardwareBufferUsage usage, bool useShadow )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	Duel::DRenderLayoutPtr D3D9RenderResourceFactory::createRenderLayout()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	Duel::DRasterizerStateObjectPtr D3D9RenderResourceFactory::createRasterizerStateObject( const DRasterizerState& state )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	Duel::DDepthStencilStateObjectPtr D3D9RenderResourceFactory::createDepthStencilStateObject( const DDepthStencilState& state )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	Duel::DBlendStateObjectPtr D3D9RenderResourceFactory::createBlendStateObject( const DBlendState& state )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	Duel::DTextureSamplerObjectPtr D3D9RenderResourceFactory::createTextureSamplerObject( const DTextureSampler& state )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	Duel::DShaderObjectPtr D3D9RenderResourceFactory::createShaderObject( DRenderPass* pass )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	DRenderWindow* D3D9RenderResourceFactory::createRenderWindow( const DString& name )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void D3D9RenderResourceFactory::destroyRenderWindow( DRenderWindow* wind )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	DFrameBuffer* D3D9RenderResourceFactory::createFrameBuffer( uint32 w, uint32 h, uint32 colorbits )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void D3D9RenderResourceFactory::destroyFrameBuffer( DFrameBuffer* buf )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	DRenderColorView* D3D9RenderResourceFactory::createRenderColorView( DPixelFormat fmt )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void D3D9RenderResourceFactory::destroyRenderColorView( DRenderColorView* v )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	DRenderDepthStencilView* D3D9RenderResourceFactory::createRenderDepthStencilView()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void D3D9RenderResourceFactory::destroyRenderDepthStencilView( DRenderDepthStencilView* v )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void D3D9RenderResourceFactory::initialize()
	{

	}

}