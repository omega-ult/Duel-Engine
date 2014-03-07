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
	class DUEL_API DRenderView : public DObject
	{
	DUEL_DECLARE_RTTI(DRenderView)
	public:
		// this constructor will initialize mParent, mAttachPoint, 
		// mWidth, mHeight, mFormat for sub-classes;
		DRenderView(DFrameBuffer* parent, ElementAttachment att);
		virtual	~DRenderView() {}

		DFrameBuffer*	getParent() { return mParent; }
		ElementAttachment	getAttachPoint() { return mAttachPoint; }
		// you should retrive its propertis after you attach it to a frame buffer.
		uint32			getWidth() { return mWidth; }
		uint32			getHeight() { return mHeight; }
		DPixelFormat	getFormat() { return mFormat; }

		void	setEnable(bool flag) { mbEnable = flag; }
		bool	isEnabled() { return mbEnable; }

		// save the current content into an image.
		virtual	void		saveToImage(DImage* dstImg) {}

		// copy the content to the texture, use main surface only.
		virtual	void		copyToTexture(DTexture* dstTexture) {}

		// this method is implemented by sub class, used in bingding texture 
		// resource to the gpu pipeline.
		virtual DGpuTextureConstant*	getGpuTexutureConstant() = 0;

	protected:
		DFrameBuffer*	mParent;
		ElementAttachment	mAttachPoint;
		uint32			mWidth;
		uint32			mHeight;
		DPixelFormat	mFormat;
		bool			mbEnable;
	};

}


#endif