//  [8/23/2013 OMEGA] created

#include "DuelGLCommon.h"
#include "DuelGLRenderSystem.h"
#include "DuelGLRenderResourceFactory.h"
#include "DuelGLRenderView.h"
#include "DuelGLFrameBuffer.h"

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(GLFrameBuffer, DFrameBuffer);

	GLFrameBuffer::GLFrameBuffer( DRenderResourceFactory* parent, uint32 w, uint32 h, uint32 colorbits ) :
		DFrameBuffer(parent, w, h, colorbits),
		mFBO(0),
		mCurDepthView(NULL)
	{
		assert(w != 0 && h != 0);
		for (int32 i = 0; i < 8; i++)
		{
			mViewList.push_back(NULL);
		}
		glGenFramebuffers(1, &mFBO);
		mCurViewport.reset(0,0, w, h);
		resize(w, h);
	}

	GLFrameBuffer::~GLFrameBuffer()
	{
		detachAllRenderColorViews();
		detachRenderDepthStencilView();
		if (mFBO != 0)
		{
			glDeleteFramebuffers(1, &mFBO);
			mFBO = 0;
		}

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
		if (width == mWidth && height == mHeight)
		{
			return;
		}
		assert(width != 0 && height != 0);
		DFrameBuffer::resize(width, height);
		mCurViewport.reset(0,0, width, height);
		RenderColorViewList::iterator i, iend = mViewList.end();
		for (i = mViewList.begin(); i != iend; ++i)
		{
			if ((*i) != NULL)
			{
				(*i)->resize(width, height);
			}
		}

		if(mCurDepthView != NULL)
		{
			mCurDepthView->resize(width, height);
		}
		// cache the old fbo. so that we can make no effect on the pipe line.
		GLFrameBufferCache cacheFBO;

		// set the rendering destination to FBO
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

		// bind valid and enabled render view.
		uint32 j; 
		for (i = mViewList.begin(), j = 0; i != iend; ++i, ++j)
		{
			if ((*i) != NULL)
			{	
				// attach a texture to FBO color attachement point
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + j, GL_TEXTURE_2D, (*i)->getTextureID(), 0);
			}
			else
			{
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + j, GL_TEXTURE_2D, 0, 0);
			}
		}
		if (mCurDepthView != NULL)
		{
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mCurDepthView->getRBO());
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mCurDepthView->getRBO());
		}
		else
		{
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);
		}		

	}

	void GLFrameBuffer::clear( uint32 flags, const DColor& clr, DReal depth, int32 stencil )
	{
		GLFrameBufferCache cacheFBO;


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

	}


	DRenderDepthStencilView* GLFrameBuffer::getRenderDepthStencilView()
	{
		return mCurDepthView;
	}

	void GLFrameBuffer::attachRenderColorView( ElementAttachment elem, DRenderColorView* v )
	{
		if (v == NULL)
		{
			assert(false);// "Can't attach an empty render color view"
			return;
		}
		if (v->getAttachedFrameBuffer() != NULL && v->getAttachedFrameBuffer() != this)
		{
			assert(false);// "Specified view is attached to another frame buffer, you must detach it before reusing it"
			return;
		}
		// the case specified v is attached to another channel
		if (getRenderColorView(elem) != NULL)
		{
			assert(false);//"Specified attachement is already in use, you must detach it firstly"
			return;
		}
		// if this view has been attached to this frame buffer, change its channel.
		if (v->getAttachedFrameBuffer() == this)
		{
			detachRenderColorView(v->getAttachPoint());
		}

		// chech format compatibility
		if (DPixelFormatTool::getFormatBits(v->getFormat()) != mColorBits)
		{
			return;
		}
		GLRenderColorView* rv = v->getAs<GLRenderColorView>(false);
		// check type availability.
		if (rv == NULL)
		{
			return;
		}
		rv->resize(mWidth, mHeight);
		GLFrameBufferCache cacheFBO;
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);			
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (uint32)elem, GL_TEXTURE_2D, rv->getTextureID(), 0);
		mViewList[(uint32)(elem)] = rv;
		rv->setAttachFrameBuffer(this);
		rv->setAttachElement(elem);
	}

	void GLFrameBuffer::detachRenderColorView( ElementAttachment elem )
	{
		if (mViewList[(uint32)(elem)] != NULL)
		{
			GLFrameBufferCache cacheFBO;
			glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
			mViewList[(uint32)(elem)]->setAttachFrameBuffer(NULL);
			mViewList[(uint32)(elem)]->setAttachElement(EA_Color0);
			mViewList[(uint32)(elem)] = NULL;
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (uint32)elem, GL_TEXTURE_2D, 0, 0);
			
		}

	}

	void GLFrameBuffer::detachAllRenderColorViews()
	{
		GLFrameBufferCache cacheFBO;
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
		for (uint32 i = 0; i < 8; ++i)
		{
			mViewList[i] = NULL;
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, 0, 0);
		}
		
	}


	void GLFrameBuffer::attachRenderDepthStencilView( DRenderDepthStencilView* v )
	{		
		if (v == NULL)
		{
			assert(false);//"Can't attach an empty depth stencil view"
			return;
		}
		if (v->getAttachedFrameBuffer() != NULL && v->getAttachedFrameBuffer() != this)
		{
			assert(false);//"Specified view is attached to another frame buffer, you must detach it before reusing it"
			return;
		}
		GLRenderDepthStencilView* rv = v->getAs<GLRenderDepthStencilView>(false);
		// check type availability.
		if (rv == NULL)
		{
			return;
		}
		detachRenderDepthStencilView();
		rv->resize(mWidth, mHeight);
		mCurDepthView = rv;
		mCurDepthView->setAttachFrameBuffer(this);
		GLFrameBufferCache cacheFBO;
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rv->getRBO());
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rv->getRBO());
		
	}

	void GLFrameBuffer::detachRenderDepthStencilView()
	{
		if(mCurDepthView == NULL)
		{
			return;
		}
		mCurDepthView->setAttachFrameBuffer(NULL);
		mCurDepthView = NULL;
		GLFrameBufferCache cacheFBO;
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);

	}

}