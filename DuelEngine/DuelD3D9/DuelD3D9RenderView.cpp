//  [6/10/2014 OMEGA] created

#include "DuelD3D9Common.h"
#include "DuelD3D9RenderView.h"
#include "DuelD3D9RenderResourceFactory.h"
#include "DuelD3D9Translator.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(D3D9RenderColorView, DRenderColorView);
	DUEL_IMPLEMENT_RTTI_1(D3D9RenderDepthStencilView, DRenderDepthStencilView);


	D3D9RenderColorView::D3D9RenderColorView( DRenderResourceFactory* creator, DPixelFormat fmt ) :
		DRenderColorView(creator, fmt),
		mAttachFB(NULL),
		mAttachPoint(EA_Color0),
		mSurface(NULL)
	{

	}

	Duel::DGpuTextureConstantPtr D3D9RenderColorView::getGpuTexutureConstant()
	{
		return mGpuConstant;
	}

	void D3D9RenderColorView::resize( uint32 w, uint32 h )
	{
		if (mSurface != NULL)
		{
			ReleaseCOM(mSurface);
		}
		if (w == mWidth && h == mHeight)
		{
			return;
		}
		mWidth = w;
		mHeight = h;
		D3D9RenderResourceFactory* fact = mCreator->getAs<D3D9RenderResourceFactory>();
		onDeviceReset(fact->getMainDevice());
	}

	void D3D9RenderColorView::setAttachFrameBuffer( DFrameBuffer* fb )
	{
		mAttachFB = fb;
	}

	void D3D9RenderColorView::setAttachElement( ElementAttachment ea )
	{
		mAttachPoint = ea;
	}

	void D3D9RenderColorView::onDeviceLost()
	{
		if (mSurface != NULL)
		{
			ReleaseCOM(mSurface);
		}
	}

	void D3D9RenderColorView::onDeviceReset( IDirect3DDevice9* dev )
	{
		dev->CreateRenderTarget(mWidth, mHeight, D3D9Translator::getD3DFormat(mFormat),
			D3DMULTISAMPLE_NONE, 0, FALSE, &mSurface, NULL);
	}


	D3D9RenderDepthStencilView::D3D9RenderDepthStencilView( DRenderResourceFactory* creator ) : 
		DRenderDepthStencilView(creator),
		mAttachFB(NULL)
	{
		
	}


	void D3D9RenderDepthStencilView::resize( uint32 w, uint32 h )
	{

	}

	void D3D9RenderDepthStencilView::setAttachFrameBuffer( DFrameBuffer* fb )
	{
		mAttachFB = fb;
	}

}