//  [8/23/2013 OMEGA] created

#ifndef _DUELGLFRAMEBUFFER_H_
#define _DUELGLFRAMEBUFFER_H_


#include "DuelGLCommon.h"
#include "DuelFrameBuffer.h"


namespace Duel
{

	class DUELGL_API GLFrameBuffer : public DFrameBuffer
	{
	DUEL_DECLARE_RTTI(GLFrameBuffer)
	public:
		GLFrameBuffer(DRenderResourceFactory* parent,
			//GLRenderSystem* rSys,
			uint32 w, uint32 h, uint32 colorBits);
		virtual ~GLFrameBuffer();

		// override : DFrameBuffer-----------------------------
		virtual void	attachRenderColorView( ElementAttachment elem, DRenderColorView* v );
		virtual void	detachRenderColorView( ElementAttachment elem );
		virtual void	detachAllRenderColorViews();
		virtual void	attachRenderDepthStencilView( DRenderDepthStencilView* v );
		virtual void	detachRenderDepthStencilView();
		virtual DRenderColorView*	getRenderColorView( ElementAttachment elem );
		virtual DRenderDepthStencilView*	getRenderDepthStencilView();
		virtual void	resize( uint32 width, uint32 height );
		virtual void	clear( uint32 flags, const DColor& clr, DReal depth, int32 stencil );
		//////////////////////////////////////////////////////////////////////////
		GLuint			getFBO() { return mFBO; }
		//////////////////////////////////////////////////////////////////////////

	protected:

		//GLRenderSystem*	mTargetRSys;
		GLuint			mFBO;
		typedef	std::vector<GLRenderColorView*>	RenderColorViewList;
		RenderColorViewList	mViewList;
		GLRenderDepthStencilView*	mCurDepthView;
	};

}

#endif
