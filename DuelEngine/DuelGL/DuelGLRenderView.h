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
		GLRenderColorView(DRenderResourceFactory* creator, DPixelFormat fmt);
		~GLRenderColorView();


		// provided for GLFramBuffer.
		virtual void	resize(uint32 w, uint32 h);
		GLuint			getTextureID();
		void			setAttachFrameBuffer(DFrameBuffer* fb);
		void			setAttachElement(ElementAttachment ea);

		// override : DRenderView--------------------------
		DGpuTextureConstantPtr	getGpuTexutureConstant();

	protected:
		GLuint	mTextureID;

		DGpuTextureConstantPtr	mGpuConstant;

	};
	
	class DUELGL_API GLRenderDepthStencilView : public DRenderDepthStencilView
	{
		DUEL_DECLARE_RTTI(GLRenderDepthStencilView)
	public:
		GLRenderDepthStencilView(DRenderResourceFactory* creator);
		~GLRenderDepthStencilView();
		virtual void	resize(uint32 w, uint32 h);
		GLuint			getTextureID();
		void			setAttachFrameBuffer(DFrameBuffer* fb);
		// override : DRenderView--------------------------
		DGpuTextureConstantPtr	getGpuTexutureConstant();

	protected:
		GLuint	mTextureID;
		DGpuTextureConstantPtr	mGpuConstant;

	};

}

#endif
