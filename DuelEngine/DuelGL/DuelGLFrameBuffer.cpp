//  [8/23/2013 OMEGA] created

#include "DuelGLCommon.h"
#include "DuelGLRenderSystem.h"
#include "DuelGLRenderResourceFactory.h"
#include "DuelGLRenderView.h"
#include "DuelGLFrameBuffer.h"

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(GLFrameBuffer, DFrameBuffer);

	GLFrameBuffer::GLFrameBuffer( DRenderResourceFactory* parent, /*GLRenderSystem* rSys, */uint32 w, uint32 h, uint32 colorbits ) :
		DFrameBuffer(parent, w, h, colorbits),
		//mTargetRSys(rSys),
		mFBO(0),
		mDepthView(NULL)
	{
		for (int32 i = 0; i < 8; i++)
		{
			mViewList.push_back(NULL);
		}
		mDepthView = new GLRenderDepthView(this);
		glGenFramebuffers(1, &mFBO);
		mCurViewport.reset(0,0, w, h);
		resize(w, h);
	}

	GLFrameBuffer::~GLFrameBuffer()
	{
		RenderColorViewList::iterator i, iend = mViewList.end();
		for (i = mViewList.begin(); i != iend; ++i)
		{
			if ((*i) != NULL)
			{
				delete (*i);
			}
		}
		if (mDepthView != NULL)
		{
			delete mDepthView;
		}
		if (mFBO != 0)
		{
			glDeleteRenderbuffers(1, &mFBO);
			mFBO = 0;
		}

	}


	void GLFrameBuffer::enableElement( ElementAttachment elem, DPixelFormat format )
	{
		if (DPixelFormatTool::getFormatBits(format) != mColorBits)
		{
			return;
		}
		GLuint	oldFbo = cacheFBO();
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
		if (mViewList[(uint32)(elem)] == NULL)
		{
			GLRenderColorView* rv = new GLRenderColorView(this, elem, format);
			mViewList[(uint32)(elem)] = rv;
			rv->resize(mWidth, mHeight);
			rv->setEnable(true);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (uint32)elem, GL_TEXTURE_2D, rv->getTextureID(), 0);
		}
		else
		{
			if (mViewList[(uint32)elem]->getFormat() != format)
			{
				delete mViewList[(uint32)elem];
				GLRenderColorView* rv = new GLRenderColorView(this, elem, format);
				rv->resize(mWidth, mHeight);
				mViewList[(uint32)(elem)] = rv;
			}
			mViewList[(uint32)(elem)]->setEnable(true);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (uint32)elem, GL_TEXTURE_2D, mViewList[(uint32)(elem)]->getTextureID(), 0);

		}
		glBindFramebuffer(GL_FRAMEBUFFER, oldFbo);
	}

	void GLFrameBuffer::disableElement( ElementAttachment elem )
	{
		GLuint	oldFbo = cacheFBO();
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
		if (mViewList[(uint32)(elem)] != NULL)
		{
			mViewList[(uint32)(elem)]->setEnable(false);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (uint32)elem, GL_TEXTURE_2D, 0, 0);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, oldFbo);
	}

	DRenderColorView* GLFrameBuffer::getRenderColorView( ElementAttachment elem )
	{
		assert((uint32)elem < mViewList.size());
		return mViewList[elem];
	}

	void GLFrameBuffer::resize( uint32 width, uint32 height )
	{
		// from wiki:
		/*
			EVER EVER MAKE A STENCIL buffer. All GPUs and all drivers do not support an independent stencil buffer. 
			If you need a stencil buffer, then you need to make a Depth=24, Stencil=8 buffer, also called D24S8. 
			Please search for the example about GL_EXT_packed_depth_stencil on this page.
		*/

		DFrameBuffer::resize(width, height);
		RenderColorViewList::iterator i, iend = mViewList.end();
		for (i = mViewList.begin(); i != iend; ++i)
		{
			if ((*i) != NULL)
			{
				(*i)->resize(width, height);
			}
		}

		mDepthView->resize(width, height);
		// cache the old fbo. so that we can make no effect on the pipe line.
		GLuint	oldFbo = cacheFBO();

		// set the rendering destination to FBO
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

		// bind valid and enabled render view.
		uint32 j; 
		for (i = mViewList.begin(), j = 0; i != iend; ++i, ++j)
		{
			if ((*i) != NULL)
			{
				if ((*i)->isEnabled())
				{			
					// attach a texture to FBO color attachement point
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + j, GL_TEXTURE_2D, (*i)->getTextureID(), 0);
				}
			}
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mDepthView->getTextureID(), 0);

// 		// for depth buffer and stencil buffer.
// 		if (mDepthStencilBuffer != 0)
// 		{
// 			glDeleteRenderbuffers(1, &mDepthStencilBuffer);
// 		}
// 		glGenRenderbuffers(1, &mDepthStencilBuffer);
// 		glBindRenderbuffer(GL_RENDERBUFFER, mDepthStencilBuffer);
// 		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
// 		glBindRenderbuffer(GL_RENDERBUFFER, 0);
// 
// 
// 
// 		// attach a renderbuffer to depth attachment point
// 		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthStencilBuffer);
// 
// 		// attach a renderbuffer to depth attachment point
// 		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mDepthStencilBuffer);

		glBindFramebuffer(GL_FRAMEBUFFER, oldFbo);

	}

	void GLFrameBuffer::clear( uint32 flags, const DColor& clr, DReal depth, int32 stencil )
	{
		GLuint	oldFbo = cacheFBO();


		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
		GLbitfield ogl_flags = 0;
		if (flags & CBM_Color)
		{
			ogl_flags |= GL_COLOR_BUFFER_BIT;
			glClearColor(clr.r, clr.g, clr.b, clr.a);
		}
		if (flags & CBM_Depth)
		{
			ogl_flags |= GL_DEPTH_BUFFER_BIT;
			glClearDepth(depth);
		}
		if (flags & CBM_Stencil)
		{
			ogl_flags |= GL_STENCIL_BUFFER_BIT;
			glClearStencil(stencil);
		}
		glClear(ogl_flags);

		glBindFramebuffer(GL_FRAMEBUFFER, oldFbo);

	}

	GLuint GLFrameBuffer::cacheFBO()
	{
		GLint	oldFbo = 0;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFbo);/*= mTargetRSys->getCurGLFBO();*/
		return (GLuint)oldFbo;
	}

	DRenderDepthView* GLFrameBuffer::getRenderDepthView()
	{
		return mDepthView;
	}


}