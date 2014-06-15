//  [6/7/2014 OMEGA] created

#ifndef _DUELD3D9FRAMEBUFFER_H_
#define _DUELD3D9FRAMEBUFFER_H_

#include "DuelD3D9Common.h"
#include "DuelFrameBuffer.h"

namespace Duel
{
	class D3D9FrameBuffer : public DFrameBuffer
	{
		DUEL_DECLARE_RTTI(D3D9FrameBuffer)
	public:
		D3D9FrameBuffer(DRenderResourceFactory* creator, uint32 w, uint32 h, uint32 colorBits);

		virtual void resize(uint32 width, uint32 height);

		virtual void attachRenderColorView( ElementAttachment elem, DRenderColorView* v );

		virtual void detachRenderColorView( ElementAttachment elem );

		virtual void detachAllRenderColorViews();

		virtual void attachRenderDepthStencilView( DRenderDepthStencilView* v );

		virtual void detachRenderDepthStencilView();

		virtual DRenderColorView* getRenderColorView( ElementAttachment elem );

		virtual DRenderDepthStencilView* getRenderDepthStencilView();
	protected:
		// frame buffer part.
		typedef	std::vector<D3D9RenderColorView*>	RenderColorViewList;
		RenderColorViewList			mViewList;
		D3D9RenderDepthStencilView*	mCurDepthView;
	};
}

#endif