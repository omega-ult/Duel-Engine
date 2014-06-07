//  [6/6/2014 OMEGA] created

#ifndef _DUELD3D9RENDERRESOURCEFACTORY_H_
#define _DUELD3D9RENDERRESOURCEFACTORY_H_

#include "DuelD3D9Common.h"
#include "DuelRenderResourceFactory.h"

namespace Duel
{
	class DUELD3D9_API D3D9RenderResourceFactory : public DRenderResourceFactory
	{
		DUEL_DECLARE_RTTI(D3D9RenderResourceFactory)
	public:

		virtual DVertexBufferPtr createVetexBuffer( size_t vertexSize, size_t verticesCount, HardwareBufferUsage usage, bool useShadow, VertexBufferType type );

		virtual DIndexBufferPtr createIndexBuffer( IndexType itype, size_t indexCount, HardwareBufferUsage usage, bool useShadow );

		virtual DRenderLayoutPtr createRenderLayout();

		virtual DRasterizerStateObjectPtr createRasterizerStateObject( const DRasterizerState& state );

		virtual DDepthStencilStateObjectPtr createDepthStencilStateObject( const DDepthStencilState& state );

		virtual DBlendStateObjectPtr createBlendStateObject( const DBlendState& state );

		virtual DTextureSamplerObjectPtr createTextureSamplerObject( const DTextureSampler& state );

		virtual DShaderObjectPtr createShaderObject( DRenderPass* pass );

		virtual DRenderWindow* createRenderWindow( const DString& name );

		virtual void destroyRenderWindow( DRenderWindow* wind );

		virtual DFrameBuffer* createFrameBuffer( uint32 w, uint32 h, uint32 colorbits );

		virtual void destroyFrameBuffer( DFrameBuffer* buf );

		virtual DRenderColorView* createRenderColorView( DPixelFormat fmt );

		virtual void destroyRenderColorView( DRenderColorView* v );

		virtual DRenderDepthStencilView* createRenderDepthStencilView();

		virtual void destroyRenderDepthStencilView( DRenderDepthStencilView* v );

		void initialize();
	};

}

#endif