//  [8/23/2013 OMEGA] created

#ifndef _DUELFRAMEBUFFER_H_
#define _DUELFRAMEBUFFER_H_

#include "DuelCommon.h"
#include "DuelPixelFormat.h"
#include "DuelViewport.h"

namespace Duel
{
	enum ElementAttachment
	{
		EA_Color0	= 0,
		EA_Color1	= 1,
		EA_Color2	= 2,
		EA_Color3	= 3,
		EA_Color4	= 4,
		EA_Color5	= 5,
		EA_Color6	= 6,
		EA_Color7	= 7
	};

	enum ClearBufferMask
	{
		CBM_Color   = 1UL << 0,
		CBM_Depth   = 1UL << 1,
		CBM_Stencil = 1UL << 2
	};

	
	class DUEL_API DFrameBuffer : public DObject
	{
	DUEL_DECLARE_RTTI(DFrameBuffer)
	public:
		// due to hardware limitation, we can only have the render views with same color bits.
		// when try to enable elements with pixel format which has different bits, that operation will fail.
		DFrameBuffer(DRenderResourceFactory* parent, uint32 w, uint32 h, uint32 colorBits);
		virtual	~DFrameBuffer() {}

		DRenderResourceFactory*		getParent() { return mParent; }

		// enable specified elements for this frame buffer, notice that
		// it may rely on rendersystem's capacity, if specified element are not available,
		// getRenderView() will return a NULL;
		virtual void		enableElement(ElementAttachment elem, DPixelFormat fmt) = 0;
		// diable specified element.
		virtual	void		disableElement(ElementAttachment elem) = 0;
		// if a specified render view does not exist, it return NULL
		virtual	DRenderColorView*	getRenderColorView(ElementAttachment elem) = 0;

		// get the DepthView
		virtual	DRenderDepthView*	getRenderDepthView() = 0;

		// set/get the properties of the render target.
		virtual	void		resize(uint32 width, uint32 height) { mWidth = width; mHeight = height; }
		virtual	uint32		getWidth() { return mWidth; }
		virtual	uint32		getHeight() { return mHeight; }

		// set rendering viewport.
		virtual	void		setViewport(const DViewport& vp) { mCurViewport = vp; }
		const DViewport&	getViewport() { return mCurViewport; }

		// color format for all elements.
		uint32				getColorBits() { return mColorBits; }

		// clear component
		virtual	void		clear(uint32 flags, const DColor& clr, DReal depth, int32 stencil) {}

	protected:
		DRenderResourceFactory*		mParent;
		// basic property.
		uint32				mWidth;
		uint32				mHeight;
		uint32				mColorBits;
		DViewport			mCurViewport;
	};


}

#endif