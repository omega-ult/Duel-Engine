//  [4/13/2013 OMEGA] created

#include "DuelGLCommon.h"
#include <GL/glew.h>
#include "DuelGLRenderResourceFactory.h"
#include "DuelGLVertexBuffer.h"
#include "DuelGLIndexBuffer.h"
#include "DuelGLRenderLayout.h"
#include "DuelGLRenderState.h"
#include "DuelGLShaderObject.h"
#include "DuelGLRenderWindow.h"
#include "DuelGLFrameBuffer.h"
#include "DuelGLRenderView.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(GLRenderResourceFactory, DRenderResourceFactory);

#ifdef DUEL_PLATFORM_WINDOWS
	LRESULT CALLBACK __GL_RenderContextWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
#endif

	GLRenderResourceFactory::GLRenderResourceFactory() : 
		mMainHDC(NULL),
		mMainHWND(NULL),
		mMainHGLRC(NULL)
	{

	}

	GLRenderResourceFactory::~GLRenderResourceFactory()
	{
		shutdown();
	}

	Duel::DVertexBufferPtr GLRenderResourceFactory::createVetexBuffer( size_t vertexSize, size_t verticesCount, HardwareBufferUsage usage, bool useShadow, VertexBufferType type )
	{
		return DVertexBufferPtr(new GLVertexBuffer(vertexSize, verticesCount,  usage, useShadow, type));
	}

	Duel::DIndexBufferPtr GLRenderResourceFactory::createIndexBuffer( IndexType itype, size_t indexCount, HardwareBufferUsage usage, bool useShadow )
	{
		return DIndexBufferPtr(new GLIndexBuffer(itype, indexCount, usage, useShadow));
	}

	Duel::DRenderLayoutPtr GLRenderResourceFactory::createRenderLayout()
	{
		return DRenderLayoutPtr(new GLRenderLayout(this));
	}

	Duel::DRasterizerStateObjectPtr GLRenderResourceFactory::createRasterizerStateObject( const DRasterizerState& state )
	{
		return DRasterizerStateObjectPtr(new GLRasterizerStateObject(state));
	}

	Duel::DDepthStencilStateObjectPtr GLRenderResourceFactory::createDepthStencilStateObject( const DDepthStencilState& state )
	{
		return DDepthStencilStateObjectPtr(new GLDepthStencilStateObject(state));
	}

	Duel::DBlendStateObjectPtr GLRenderResourceFactory::createBlendStateObject( const DBlendState& state )
	{
		return DBlendStateObjectPtr(new GLBlendStateObject(state));
	}

	Duel::DShaderObjectPtr GLRenderResourceFactory::createShaderObject( DRenderPass* pass )
	{
		GLShaderObject* so = new GLShaderObject();
		so->build(pass);
		return DShaderObjectPtr(so);
	}

	DRenderWindow* GLRenderResourceFactory::createRenderWindow( const DString& name )
	{
		GLRenderWindow* newWnd = new GLRenderWindow(this, name);
		return newWnd;
	}

	void GLRenderResourceFactory::destroyRenderWindow( DRenderWindow* wind )
	{
		if (wind->getCreator() == this)
		{
			delete wind;
		}
	}

	DFrameBuffer* GLRenderResourceFactory::createFrameBuffer( uint32 w, uint32 h, uint32 colorBits )
	{
		GLFrameBuffer* ret = new GLFrameBuffer(this, w, h, colorBits);
		return ret;

	}

	void GLRenderResourceFactory::destroyFrameBuffer( DFrameBuffer* buf )
	{
		if (buf->getCreator() == this)
		{
			if (DCore::getSingleton().getRenderSystem() != NULL &&
				buf == DCore::getSingleton().getRenderSystem()->getCurrentFrameBuffer())
			{
				DCore::getSingleton().getRenderSystem()->bindFrameBuffer(NULL);
			}
			delete buf;
		}
	}

#ifdef DUEL_PLATFORM_WINDOWS

	void GLRenderResourceFactory::resetRenderContext()
	{
		wglMakeCurrent(mMainHDC, mMainHGLRC);
	}

	void GLRenderResourceFactory::initialize()
	{
		HINSTANCE hInst = GetModuleHandle(NULL);
		WNDCLASSEX wc;

		wc.cbSize			= sizeof(wc);
		wc.style			= CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc		= __GL_RenderContextWndProc;
		wc.cbClsExtra		= 0;
		wc.cbWndExtra		= 0;
		wc.hInstance		= hInst;
		wc.hIcon			= NULL;
		wc.hCursor			= NULL;
		wc.hbrBackground	= NULL;
		wc.lpszMenuName		= NULL;
		// TODO: from config
		wc.lpszClassName	= "__GLContextWindow";
		wc.hIconSm			= NULL;

		DWORD	styleWord;
		styleWord = WS_OVERLAPPED|WS_THICKFRAME|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX;
		RECT rc = { 0, 0, 50, 50 };
		AdjustWindowRectEx( &rc, styleWord, FALSE, 0 );
		RegisterClassEx(&wc);
		mMainHWND = CreateWindowEx(0, "__GLContextWindow", "__GLContextWindow", 
			styleWord, 0, 0,  rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInst, NULL);

		mMainHDC = GetDC(mMainHWND);
		uint32 sufaceFormatBit = 32;
		uint32 depthFormatBit = 24; // D24S8
		uint32 stencilFormatBit = 8;
		// there is no guarantee that the contents of the stack that become
		// the pfd are zeroed, therefore _make sure_ to clear these bits.
		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof(pfd));
		pfd.nSize		= sizeof(pfd);
		pfd.nVersion	= 1;
		pfd.dwFlags		= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType	= PFD_TYPE_RGBA;
		pfd.cColorBits	= static_cast<BYTE>(sufaceFormatBit);
		pfd.cDepthBits	= static_cast<BYTE>(depthFormatBit);
		pfd.cStencilBits = static_cast<BYTE>(stencilFormatBit);
		pfd.iLayerType	= PFD_MAIN_PLANE;

		// here is the trick, create a temp HWND to create glew evironment.

		int pixfmt = ChoosePixelFormat(mMainHDC, &pfd);
		assert(pixfmt != 0);

		SetPixelFormat(mMainHDC, pixfmt, &pfd);//每个窗口只能设置一次  

 		mMainHGLRC = wglCreateContext(mMainHDC);
 		wglMakeCurrent(mMainHDC, mMainHGLRC);
		glewInit();

		int flags = 0;//WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB;
#ifdef DUEL_DEBUG
		flags |= WGL_CONTEXT_DEBUG_BIT_ARB;
#endif
		int versions[9][2] =
		{
			{ 4, 4 },
			{ 4, 3 },
			{ 4, 2 },
			{ 4, 1 },
			{ 4, 0 },
			{ 3, 3 },
			{ 3, 2 },
			{ 3, 1 },
			{ 3, 0 },
		};
		int attribs[] = { WGL_CONTEXT_MAJOR_VERSION_ARB, 4, WGL_CONTEXT_MINOR_VERSION_ARB, 4, WGL_CONTEXT_FLAGS_ARB, flags,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB, 0 };
		for (int i = 0; i < 9; ++ i)
		{
			attribs[1] = versions[i][0];
			attribs[3] = versions[i][1];
			HGLRC hRC_new = wglCreateContextAttribsARB(mMainHDC,
				0, attribs);
			if (hRC_new != NULL)
			{
				wglMakeCurrent(mMainHDC, NULL);
				wglDeleteContext(mMainHGLRC);
				mMainHGLRC = hRC_new;
				wglMakeCurrent(mMainHDC, mMainHGLRC);
				break;
			}
		}

	}

	void GLRenderResourceFactory::shutdown()
	{
		if (mMainHGLRC != NULL)
		{
			wglMakeCurrent(mMainHDC, NULL);
			wglDeleteContext(mMainHGLRC);
		}
		if (mMainHWND != NULL)
		{
			DestroyWindow(mMainHWND);
		}
		mMainHWND = NULL;
		mMainHGLRC = NULL;
		mMainHDC = NULL;
	}

#endif // DUEL_PLATFORM_WINDOWS

	DRenderColorView* GLRenderResourceFactory::createRenderColorView( DPixelFormat fmt )
	{
		return new GLRenderColorView(this, fmt);
	}

	void GLRenderResourceFactory::destroyRenderColorView( DRenderColorView* v )
	{
		if (v->getAs<GLRenderColorView>(false) != NULL && v->getCreator() == this)
		{
			delete v;
		}
	}

	DRenderDepthStencilView* GLRenderResourceFactory::createRenderDepthStencilView()
	{
		return new GLRenderDepthStencilView(this);
	}

	void GLRenderResourceFactory::destroyRenderDepthStencilView( DRenderDepthStencilView* v )
	{
		if (v->getAs<GLRenderDepthStencilView>(false) != NULL && v->getCreator() == this)
		{
			delete v;
		}
	}


}