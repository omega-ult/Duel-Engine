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
		~GLRenderColorView();


		// provided for GLFramBuffer.
		virtual void	resize(uint32 w, uint32 h);
		GLuint			getTextureID() { return mTextureID; }

		// override : DRenderView--------------------------
		DGpuTextureConstantPtr	getGpuTexutureConstant() { return mGpuConstant; }

	protected:
		GLuint	mTextureID;

		DGpuTextureConstantPtr	mGpuConstant;

	};
	
	class DUELGL_API GLRenderDepthView : public DRenderDepthView
	{
		DUEL_DECLARE_RTTI(GLRenderDepthView)
	public:
		GLRenderDepthView(DFrameBuffer* parent);
		~GLRenderDepthView();
		virtual void	resize(uint32 w, uint32 h);
		GLuint	getTextureID() { return mTextureID; }
		// override : DRenderView--------------------------
		DGpuTextureConstantPtr	getGpuTexutureConstant() { return mGpuConstant; }

	protected:
		GLuint	mTextureID;
		DGpuTextureConstantPtr	mGpuConstant;

	};

}

#endif
