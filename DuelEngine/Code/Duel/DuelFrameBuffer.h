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
		DFrameBuffer(DRenderResourceFactory* creator, uint32 w, uint32 h, uint32 colorBits);
		virtual	~DFrameBuffer() {}

		DRenderResourceFactory*		getCreator() { return mCreator; }

		// notice: make sure the specified v is not currently attached to another frame buffer.
		// specified elem should not be occupied by other RenderView, otherwise you must detach it
		// firstly, if the v is attached to another elem in this frame buffer, this method will help you
		// change the element channel automatically. 
		// NULL pointer is unacceptable!
		virtual void		attachRenderColorView(ElementAttachment elem, DRenderColorView* v) = 0;
		virtual void		detachRenderColorView(ElementAttachment elem) = 0;
		virtual	void		detachAllRenderColorViews();
		// notice: make sure the specified v is not currently attached to another frame buffer.
		// if there is already a depth stencil view attached, detach it before you go ahead.
		// NULL pointer is unacceptable!
		virtual void		attachRenderDepthStencilView(DRenderDepthStencilView* v) = 0;
		virtual void		detachRenderDepthStencilView() = 0;

		// if a specified render view does not exist, it return NULL
		virtual	DRenderColorView*	getRenderColorView(ElementAttachment elem) = 0;
		// get the DepthView
		virtual	DRenderDepthStencilView*	getRenderDepthStencilView() = 0;

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
		DRenderResourceFactory*		mCreator;
		// basic property.
		uint32				mWidth;
		uint32				mHeight;
		uint32				mColorBits;
		DViewport			mCurViewport;
	};


}

#endif