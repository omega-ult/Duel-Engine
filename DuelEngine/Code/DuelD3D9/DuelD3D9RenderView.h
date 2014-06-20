//  [6/10/2014 OMEGA] created

#ifndef _DUELD3D9RENDERVIEW_H_
#define _DUELD3D9RENDERVIEW_H_

#include "DuelD3D9Common.h"
#include "DuelD3D9VolatileResource.h"
#include "DuelRenderView.h"
#include "DuelD3D9Include.h"

namespace Duel
{
	class D3D9RenderColorView : public DRenderColorView, public D3D9VolatileResource
	{
		DUEL_DECLARE_RTTI(D3D9RenderColorView)
	public:
		D3D9RenderColorView(DRenderResourceFactory* creator, DPixelFormat fmt);
		virtual DGpuTextureConstantPtr getGpuTexutureConstant();

		// provided for GLFramBuffer.
		virtual void	resize(uint32 w, uint32 h);
		void			setAttachFrameBuffer(DFrameBuffer* fb);
		void			setAttachElement(ElementAttachment ea);

		virtual void onDeviceLost();

		virtual void onDeviceReset( IDirect3DDevice9* dev );

		IDirect3DSurface9*	getRenderSurface();

	protected:
		DFrameBuffer*	mAttachFB;
		ElementAttachment	mAttachPoint;
		DGpuTextureConstantPtr	mGpuConstant;

		IDirect3DTexture9* mTexture;
	};

	class D3D9RenderDepthStencilView : public DRenderDepthStencilView, public D3D9VolatileResource
	{
		DUEL_DECLARE_RTTI(D3D9RenderDepthStencilView)
	public:
		D3D9RenderDepthStencilView(DRenderResourceFactory* creator);

		virtual void	resize(uint32 w, uint32 h);
		void			setAttachFrameBuffer(DFrameBuffer* fb);

		virtual void onDeviceLost();

		virtual void onDeviceReset( IDirect3DDevice9* dev );

		IDirect3DSurface9*	getRenderSurface();

	protected:
		DFrameBuffer*	mAttachFB;

		IDirect3DSurface9* mSurface;
	};

}

#endif