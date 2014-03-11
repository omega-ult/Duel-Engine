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
		virtual void	enableElement( ElementAttachment elem, DPixelFormat format );
		virtual void	disableElement( ElementAttachment elem );
		virtual DRenderColorView*	getRenderColorView( ElementAttachment elem );
		virtual DRenderDepthView*	getRenderDepthView();
		virtual void	resize( uint32 width, uint32 height );
		virtual void	clear( uint32 flags, const DColor& clr, DReal depth, int32 stencil );
		//////////////////////////////////////////////////////////////////////////
		GLuint			getFBO() { return mFBO; }
		//////////////////////////////////////////////////////////////////////////

	protected:
		GLuint			cacheFBO();


		//GLRenderSystem*	mTargetRSys;
		GLuint			mFBO;
		typedef	std::vector<GLRenderColorView*>	RenderColorViewList;
		RenderColorViewList	mViewList;
		GLRenderDepthView*	mDepthView;
	};

}

#endif
