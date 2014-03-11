//  [8/23/2013 OMEGA] created

#ifndef _DUELGLRENDERVIEW_H_
#define _DUELGLRENDERVIEW_H_

#include "DuelGLCommon.h"
#include "DuelGLGpuParameters.h"
#include "DuelRenderView.h"

namespace Duel
{
	class DUELGL_API GLRenderColorView : public DRenderColorView
	{
	DUEL_DECLARE_RTTI(GLRenderColorView)
	public:
		GLRenderColorView(DFrameBuffer* parent, ElementAttachment att, DPixelFormat fmt);
		virtual ~GLRenderColorView();


		// provided for GLFramBuffer.
		virtual void	resize(uint32 w, uint32 h);
		GLuint			getTextureID() { return mTextureID; }

		// override : DRenderView--------------------------
		DGpuTextureConstant*	getGpuTexutureConstant() { return &mGpuConstant; }

	protected:
		GLuint	mTextureID;

		GLGpuTextureConstant	mGpuConstant;

	};
	
	class DUELGL_API GLRenderDepthView : public DRenderDepthView
	{
		DUEL_DECLARE_RTTI(GLRenderDepthView)
	public:
		GLRenderDepthView(DFrameBuffer* parent);
		virtual void	resize(uint32 w, uint32 h);
		GLuint	getTextureID() { return mTextureID; }
		// override : DRenderView--------------------------
		DGpuTextureConstant*	getGpuTexutureConstant() { return &mGpuConstant; }

	protected:
		GLuint	mTextureID;
		GLGpuTextureConstant	mGpuConstant;

	};

}

#endif
