//  [8/23/2013 OMEGA] created

#ifndef _DUELGLRENDERVIEW_H_
#define _DUELGLRENDERVIEW_H_

#include "DuelGLCommon.h"
#include "DuelGLGpuParameters.h"
#include "DuelRenderView.h"

namespace Duel
{
	class DUELGL_API GLRenderView : public DRenderView
	{
	DUEL_DECLARE_RTTI(GLRenderView)
	public:
		GLRenderView(DFrameBuffer* parent, ElementAttachment att, DPixelFormat fmt);
		virtual ~GLRenderView();


		// provided for GLFramBuffer.
		void	resize(uint32 w, uint32 h);
		GLuint	getTextureID() { return mTextureID; }

		// override : DRenderView--------------------------
		DGpuTextureConstant*	getGpuTexutureConstant() { return &mGpuConstant; }

	protected:
		GLuint	mTextureID;

		GLGpuTextureConstant	mGpuConstant;

	};


}

#endif
