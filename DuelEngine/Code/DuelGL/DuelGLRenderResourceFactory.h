//  [4/7/2013 OMEGA] created

#ifndef _DUELGLRENDERRESOURCEFACTORY_H_
#define _DUELGLRENDERRESOURCEFACTORY_H_

#include "Duel.h"
#include "DuelGLCommon.h"

namespace Duel
{
	class DUELGL_API GLRenderResourceFactory : public DRenderResourceFactory
	{
	DUEL_DECLARE_RTTI(GLRenderResourceFactory)
	public:
		GLRenderResourceFactory();
		~GLRenderResourceFactory();

		// called when loading GL plugin.
		void	initialize();
		void	shutdown();

#ifdef DUEL_PLATFORM_WINDOWS
		HGLRC			getRenderContext() { return mMainHGLRC; }
		PIXELFORMATDESCRIPTOR	getWindowPixelFormatDesc() { return mMainPfd; }
		int32			getWindowPixelFormat() { return mWindPixelFormat; }
		void			resetRenderContext();
#endif
		// override : DHardwareBufferFactory-------------------------
		virtual DVertexBufferPtr		createVetexBuffer(size_t vertexSize, size_t verticesCount,
			HardwareBufferUsage usage, bool useShadow, VertexBufferType type);
		// override : DHardwareBufferFactory-------------------------
		virtual DIndexBufferPtr			createIndexBuffer(IndexType itype, size_t indexCount, HardwareBufferUsage usage, bool useShadow);
		// override : DHardwareBufferFactory-------------------------
		virtual	DRenderLayoutPtr		createRenderLayout();
		// override : DHardwareBufferFactory-------------------------
		virtual DRasterizerStateObjectPtr createRasterizerStateObject( const DRasterizerState& state );
		// override : DHardwareBufferFactory-------------------------
		virtual DDepthStencilStateObjectPtr createDepthStencilStateObject( const DDepthStencilState& state );
		// override : DHardwareBufferFactory-------------------------
		virtual DBlendStateObjectPtr createBlendStateObject( const DBlendState& state );
		// override : DHardwareBufferFactory-------------------------
		virtual DTextureSamplerObjectPtr	createTextureSamplerObject(const DTextureSampler& state);
		// override : DHardwareBufferFactory-------------------------
		virtual DShaderObjectPtr createShaderObject( DRenderPass* pass );
		// override : DHardwareBufferFactory-------------------------
		virtual DRenderWindow* createRenderWindow( const DString& name );
		// override : DHardwareBufferFactory-------------------------
		virtual void destroyRenderWindow( DRenderWindow* wind );
		// override : DHardwareBufferFactory-------------------------
		virtual DFrameBuffer* createFrameBuffer( uint32 w, uint32 h, uint32 colorBits );
		// override : DHardwareBufferFactory-------------------------
		virtual void destroyFrameBuffer( DFrameBuffer* buf );
		virtual DRenderColorView* createRenderColorView( DPixelFormat fmt );
		// override : DHardwareBufferFactory-------------------------
		virtual void destroyRenderColorView( DRenderColorView* v );
		// override : DHardwareBufferFactory-------------------------
		virtual DRenderDepthStencilView* createRenderDepthStencilView();
		// override : DHardwareBufferFactory-------------------------
		virtual void destroyRenderDepthStencilView( DRenderDepthStencilView* v );


	protected:
#ifdef DUEL_PLATFORM_WINDOWS
		// we need a main context to manage the rendering pipeline, this is the key to 
		// the multi-window rendering, since our render window is only a frame buffer object,
		// which is not sharable with other rendering context(rc), so we must make sure
		// all frame buffer object is created by the main rc.
		HDC		mMainHDC;
		HGLRC	mMainHGLRC;
		HWND	mMainHWND;
		PIXELFORMATDESCRIPTOR	mMainPfd;
		int32	mWindPixelFormat;
#endif
	};

}


#endif