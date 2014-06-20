//  [8/23/2013 OMEGA] created

#ifndef _DUELRENDERVIEW_H_
#define _DUELRENDERVIEW_H_

#include "DuelCommon.h"
#include "DuelPixelFormat.h"
#include "DuelFrameBuffer.h"

namespace Duel
{
	// specified render view may not be able to copy to texture or save to 
	// image especially for ET_DEPTH_STENCIL
	class DUEL_API DRenderColorView : public DObject
	{
	DUEL_DECLARE_RTTI(DRenderColorView)
	public:
		// this constructor will initialize mParent, mAttachPoint, 
		// mWidth, mHeight, mFormat for sub-classes;
		DRenderColorView(DRenderResourceFactory* creator, DPixelFormat fmt);
		~DRenderColorView();

		DRenderResourceFactory*	getCreator() { return mCreator; }
		// you should retrive its propertis after you attach it to a frame buffer.
		uint32			getWidth() { return mWidth; }
		uint32			getHeight() { return mHeight; }
		DPixelFormat	getFormat() { return mFormat; }

		// save the current content into an image.
		virtual	void		saveToImage(DImage* dstImg) {}

		// copy the content to the texture, use main surface only.
		virtual	void		copyToTexture(DTexture* dstTexture) {}

		// this method is implemented by sub class, used in bingding texture 
		// resource to the gpu pipeline.
		virtual DGpuTextureConstantPtr	getGpuTexutureConstant() = 0;

		// query whether this view is attached to a frame buffer.
		// only it is not attached, it can be bound to another frameBuffer.
		bool				isAttached();
		DFrameBuffer*		getAttachedFrameBuffer();
		ElementAttachment	getAttachPoint() { return mAttachPoint; }


	protected:
		DRenderResourceFactory*	mCreator;
		uint32					mWidth;
		uint32					mHeight;
		DPixelFormat			mFormat;
		DFrameBuffer*			mAttachFB;
		ElementAttachment		mAttachPoint;
	};

	class DUEL_API DRenderDepthStencilView : public DObject
	{
		DUEL_DECLARE_RTTI(DRenderDepthStencilView)
	public:
		DRenderDepthStencilView(DRenderResourceFactory* creator);
		~DRenderDepthStencilView();

		DRenderResourceFactory*	getCreator() { return mCreator; }
		// you should retrive its propertis after you attach it to a frame buffer.
		uint32			getWidth() { return mWidth; }
		uint32			getHeight() { return mHeight; }

		// query whether this view is attached to a frame buffer.
		bool			isAttached();
		DFrameBuffer*	getAttachedFrameBuffer();

	protected:
		DRenderResourceFactory*	mCreator;
		uint32					mWidth;
		uint32					mHeight;
		DFrameBuffer*			mAttachFB;
	};
}


#endif