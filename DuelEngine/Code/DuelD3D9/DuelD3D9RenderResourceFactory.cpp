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

	D3D9RenderResourceFactory::D3D9RenderResourceFactory() :
		mD3D9(NULL),
		mMainDevice(NULL),
		mMainHWND(0)
	{
		ZeroMemory(&mPresentParam, sizeof(D3DPRESENT_PARAMETERS));
	}

	Duel::DVertexBufferPtr D3D9RenderResourceFactory::createVetexBuffer( size_t vertexSize, size_t verticesCount, HardwareBufferUsage usage, bool useShadow, VertexBufferType type )
	{
		return DVertexBufferPtr(new D3D9VertexBuffer(this, vertexSize, verticesCount,usage, useShadow));
	}

	Duel::DIndexBufferPtr D3D9RenderResourceFactory::createIndexBuffer( IndexType itype, size_t indexCount, HardwareBufferUsage usage, bool useShadow )
	{
		return DIndexBufferPtr(new D3D9IndexBuffer(this, itype, indexCount, usage, useShadow));
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
		DShaderObjectPtr ret = DShaderObjectPtr(new D3D9ShaderObject(this));
		ret->build(pass);
		return ret;
	}

	DRenderWindow* D3D9RenderResourceFactory::createRenderWindow( const DString& name )
	{
		D3D9RenderWindow* ret = new D3D9RenderWindow(this, name);
		mVolatileResList.push_back(ret);
		return ret;
	}

	void D3D9RenderResourceFactory::destroyRenderWindow( DRenderWindow* wind )
	{
		D3D9RenderWindow* d3dRendWind = wind->getAs<D3D9RenderWindow>();
		if (d3dRendWind != NULL && d3dRendWind->getCreator() == this)
		{
			VolatileResourceList::iterator i = mVolatileResList.begin();
			while (i != mVolatileResList.end())
			{
				if ((*i) == d3dRendWind)
				{
					mVolatileResList.erase(i);
					break;
				}
				++i;
			}
			delete wind;
		}
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
		wc.lpszClassName	= L"__D3DContextWindow";
		wc.hIconSm			= NULL;

		DWORD	styleWord;
		styleWord = WS_OVERLAPPED|WS_THICKFRAME|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX;
		RECT rc = { 0, 0, 50, 50 };
		AdjustWindowRectEx( &rc, styleWord, FALSE, 0 );
		RegisterClassEx(&wc);
		mMainHWND = CreateWindowEx(0, L"__D3DContextWindow", L"__D3DContextWindow", 
			styleWord, 0, 0,  rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInst, NULL);

		mD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
		// Set up the structure used to create the D3DDevice
		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory( &d3dpp, sizeof( d3dpp ) );
		d3dpp.Windowed = TRUE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

		mPresentParam.BackBufferCount			= 1; // 双倍缓冲
		mPresentParam.Windowed					= true;		
		mPresentParam.SwapEffect				= D3DSWAPEFFECT_DISCARD;
		mPresentParam.hDeviceWindow				= mMainHWND;	// 目标窗口, 不一定与device创建窗口相同.
		mPresentParam.EnableAutoDepthStencil	= false;
		mPresentParam.PresentationInterval		= D3DPRESENT_INTERVAL_ONE;	// swapbuffer数决定.
		mPresentParam.FullScreen_RefreshRateInHz= 0;
		mPresentParam.BackBufferFormat			= D3DFMT_X8R8G8B8;

		mD3D9->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mMainHWND,
			D3DCREATE_HARDWARE_VERTEXPROCESSING,
			&d3dpp, &mMainDevice );

		mMainDevice->Reset(&mPresentParam);
	}

	void D3D9RenderResourceFactory::shutdown()
	{
		ReleaseCOM(mMainDevice);
		ReleaseCOM(mD3D9);
		DestroyWindow(mMainHWND);
		mMainHWND = 0;
	}

	LPDIRECT3DDEVICE9 D3D9RenderResourceFactory::getMainDevice()
	{
		return mMainDevice;
	}

	void D3D9RenderResourceFactory::reset()
	{
		// release all volatile resources.
		VolatileResourceList::iterator i = mVolatileResList.begin();
		while (i != mVolatileResList.end())
		{
			(*i)->onDeviceLost();
			++i;
		}

		HRESULT hr = mMainDevice->Reset(&mPresentParam);
		while (hr != S_OK)
		{
			Sleep(100);
			hr = mMainDevice->Reset(&mPresentParam);
		}

		// recreate all volatile resources.
		i = mVolatileResList.begin();
		while (i != mVolatileResList.end())
		{
			(*i)->onDeviceReset(mMainDevice);
			++i;
		}

	}

}