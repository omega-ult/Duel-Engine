//  [6/10/2014 OMEGA] created

#include "DuelD3D9Common.h"
#include "DuelD3D9RenderView.h"
#include "DuelD3D9RenderResourceFactory.h"
#include "DuelD3D9Translator.h"
#include "DuelD3D9ShaderObject.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(D3D9RenderColorView, DRenderColorView);
	DUEL_IMPLEMENT_RTTI_1(D3D9RenderDepthStencilView, DRenderDepthStencilView);


	D3D9RenderColorView::D3D9RenderColorView( DRenderResourceFactory* creator, DPixelFormat fmt ) :
		DRenderColorView(creator, fmt),
		mAttachFB(NULL),
		mAttachPoint(EA_Color0),
		mTexture(NULL)
	{

	}

	Duel::DGpuTextureConstantPtr D3D9RenderColorView::getGpuTexutureConstant()
	{
		return mGpuConstant;
	}

	void D3D9RenderColorView::resize( uint32 w, uint32 h )
	{
		if (w == mWidth && h == mHeight)
		{
			return;
		}
		onDeviceLost();
		mWidth = w;
		mHeight = h;
		D3D9RenderResourceFactory* fact = mCreator->getAs<D3D9RenderResourceFactory>();
		IDirect3DDevice9* dev = fact->getMainDevice();
		onDeviceReset(dev);
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
		ReleaseCOM(mTexture);
		if (mGpuConstant != NULL)
		{
			mGpuConstant->getAs<D3D9GpuTextureConstant>()->discard();
			mGpuConstant = DGpuTextureConstantPtr();
		}
	}

	void D3D9RenderColorView::onDeviceReset( IDirect3DDevice9* dev )
	{
		dev->CreateTexture(mWidth, mHeight, 1, D3DUSAGE_RENDERTARGET, D3D9Translator::getD3DFormat(mFormat), D3DPOOL_DEFAULT,
			&mTexture, NULL);
		mGpuConstant = DGpuTextureConstantPtr(new D3D9GpuTextureConstant(mTexture));
	}

	IDirect3DSurface9* D3D9RenderColorView::getRenderSurface()
	{
		if (mTexture)
		{
			IDirect3DSurface9* suf = NULL;
			mTexture->GetSurfaceLevel(0, &suf);
			return suf;
		}
		return NULL;
	}

	D3D9RenderDepthStencilView::D3D9RenderDepthStencilView( DRenderResourceFactory* creator ) : 
		DRenderDepthStencilView(creator),
		mAttachFB(NULL),
		mSurface(NULL)
	{
		
	}


	void D3D9RenderDepthStencilView::resize( uint32 w, uint32 h )
	{
		if (w == mWidth && h == mHeight)
		{
			return;
		}

		ReleaseCOM(mSurface);

		mWidth = w;
		mHeight = h;
		D3D9RenderResourceFactory* fact = mCreator->getAs<D3D9RenderResourceFactory>();
		onDeviceReset(fact->getMainDevice());
	}

	void D3D9RenderDepthStencilView::setAttachFrameBuffer( DFrameBuffer* fb )
	{
		mAttachFB = fb;
	}

	void D3D9RenderDepthStencilView::onDeviceLost()
	{
		ReleaseCOM(mSurface);
	}

	void D3D9RenderDepthStencilView::onDeviceReset( IDirect3DDevice9* dev )
	{
		dev->CreateDepthStencilSurface(mWidth, mHeight, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0,
			FALSE, &mSurface, NULL);
	}

	IDirect3DSurface9* D3D9RenderDepthStencilView::getRenderSurface()
	{
		return mSurface;
	}

}