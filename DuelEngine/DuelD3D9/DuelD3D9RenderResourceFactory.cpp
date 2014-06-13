//  [6/6/2014 OMEGA] created

#include "DuelD3D9Common.h"
#include <d3d9.h>
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

#ifdef DUEL_PLATFORM_WINDOWS
	LRESULT CALLBACK __D3D_RenderContextWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
#endif

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
		HINSTANCE hInst = GetModuleHandle(NULL);
		WNDCLASSEX wc;

		wc.cbSize			= sizeof(wc);
		wc.style			= CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc		= __D3D_RenderContextWndProc;
		wc.cbClsExtra		= 0;
		wc.cbWndExtra		= 0;
		wc.hInstance		= hInst;
		wc.hIcon			= NULL;
		wc.hCursor			= NULL;
		wc.hbrBackground	= NULL;
		wc.lpszMenuName		= NULL;
		// TODO: from config
		wc.lpszClassName	= "__D3DContextWindow";
		wc.hIconSm			= NULL;

		DWORD	styleWord;
		styleWord = WS_OVERLAPPED|WS_THICKFRAME|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX;
		RECT rc = { 0, 0, 50, 50 };
		AdjustWindowRectEx( &rc, styleWord, FALSE, 0 );
		RegisterClassEx(&wc);
		mMainHWND = CreateWindowEx(0, "__D3DContextWindow", "__D3DContextWindow", 
			styleWord, 0, 0,  rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInst, NULL);


		LPDIRECT3D9 pd3d9 = NULL;
		pd3d9 = Direct3DCreate9(D3D_SDK_VERSION);
		// Set up the structure used to create the D3DDevice
		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory( &d3dpp, sizeof( d3dpp ) );
		d3dpp.Windowed = TRUE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

		pd3d9->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mMainHWND,
			D3DCREATE_HARDWARE_VERTEXPROCESSING,
			&d3dpp, &mMainDevice );

	}

	void D3D9RenderResourceFactory::shutdown()
	{

	}

}