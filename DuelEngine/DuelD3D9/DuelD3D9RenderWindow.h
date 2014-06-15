//  [6/12/2014 OMEGA] created

#ifndef _DUELD3D9RENDERWINDOW_H_
#define _DUELD3D9RENDERWINDOW_H_

#include "DuelD3D9Common.h"
#include "DuelRenderWindow.h"
#include "DuelD3D9VolatileResource.h"
#include "DuelD3D9Include.h"

namespace Duel
{
	class D3D9RenderWindow : public DRenderWindow, public D3D9VolatileResource
	{
		DUEL_DECLARE_RTTI(D3D9RenderWindow)

	public:
		D3D9RenderWindow(DRenderResourceFactory* creator, const DString& name);
		~D3D9RenderWindow();
		virtual const DString& getName();

		virtual void create( const DString& name, const RenderWindowSetting& setting, uint32 winHandle );

		virtual void setFullScreen( bool flag, const DisplayMode& fullScreenMode );

		virtual bool isFullScreen() const;

		virtual void reposition( uint32 top, uint32 left );

		virtual uint32 getTop();

		virtual uint32 getLeft();

		virtual void update();
		
		virtual void resize(uint32 width, uint32 height);

		virtual uint32 getWindowHandle();

		virtual void attachRenderColorView( ElementAttachment elem, DRenderColorView* v );

		virtual void detachRenderColorView( ElementAttachment elem );

		virtual void detachAllRenderColorViews();

		virtual void attachRenderDepthStencilView( DRenderDepthStencilView* v );

		virtual void detachRenderDepthStencilView();

		virtual DRenderColorView* getRenderColorView( ElementAttachment elem );

		virtual DRenderDepthStencilView* getRenderDepthStencilView();

		virtual void onDeviceLost();

		virtual void onDeviceReset(IDirect3DDevice9* dev);

	protected:
#ifdef DUEL_PLATFORM_WINDOWS
		HWND		mHWND;
		IDirect3DSwapChain9*	mSwapChain;
		D3DPRESENT_PARAMETERS	mPresentParam;

		// resources for transfering data from render target to 
		// the back buffer.
		IDirect3DVertexBuffer9*	mQuadVert;
		IDirect3DIndexBuffer9*	mQuadIndx;
		IDirect3DVertexDeclaration9*	mVDecl;
		IDirect3DVertexShader9*	mVShader;
		IDirect3DPixelShader9*	mPShader;
#endif
		DString		mName;
		// frame buffer part.
		typedef	std::vector<D3D9RenderColorView*>	RenderColorViewList;
		RenderColorViewList			mViewList;
		D3D9RenderDepthStencilView*	mCurDepthView;
		// the surface is defalt suface for 0 channel.
		DRenderColorView*		mMainSurface;
		DRenderDepthStencilView*	mMainDepthView;
	};
}


#endif