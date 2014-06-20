//  [8/23/2013 OMEGA] created

#include "DuelGLCommon.h"
#include "DuelGLTranslator.h"
#include "DuelGLRenderView.h"

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(GLRenderColorView, DRenderColorView);
	DUEL_IMPLEMENT_RTTI_1(GLRenderDepthStencilView, DRenderDepthStencilView);

	GLRenderColorView::GLRenderColorView( DRenderResourceFactory* creator, DPixelFormat fmt ) :
		DRenderColorView(creator, fmt),
		mTextureID(0)
	{
		// we wouldn't do resize here, parent frame buffer will call resize directly.
	}

	GLRenderColorView::~GLRenderColorView()
	{
		if (mTextureID != 0)
		{
			glDeleteTextures(1,&mTextureID);
		}
		if (mGpuConstant != NULL)
		{
			mGpuConstant->getAs<GLGpuTextureConstant>()->discard();
		}
	}

	void GLRenderColorView::resize( uint32 w, uint32 h )
	{
		if (w == mWidth && h == mHeight)
		{
			return;
		}
		mWidth = w;
		mHeight = h;

		if (mTextureID != 0)
		{
			glDeleteTextures(1, &mTextureID);
			mTextureID = 0;
		}
		glGenTextures(1, &mTextureID);
		glBindTexture(GL_TEXTURE_2D, mTextureID);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0 );

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		GLFormatGroup fg = GLTranslator::getGLFormat(mFormat);
		glTexImage2D(GL_TEXTURE_2D, 0, fg.internalFormat, mWidth, mHeight, 0, fg.glFormat, fg.glFormatType, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		if (mGpuConstant != NULL)
		{
			mGpuConstant->getAs<GLGpuTextureConstant>()->discard();
		}
		mGpuConstant = DGpuTextureConstantPtr(new GLGpuTextureConstant(GL_TEXTURE_2D, mTextureID));

	}

	void GLRenderColorView::setAttachFrameBuffer( DFrameBuffer* fb )
	{
		mAttachFB = fb;
	}

	Duel::DGpuTextureConstantPtr GLRenderColorView::getGpuTexutureConstant()
	{
		return mGpuConstant;
	}

	void GLRenderColorView::setAttachElement( ElementAttachment ea )
	{
		mAttachPoint = ea;
	}

	GLuint GLRenderColorView::getTextureID()
	{
		return mTextureID;
	}

	GLRenderDepthStencilView::GLRenderDepthStencilView( DRenderResourceFactory* creator ) :
		DRenderDepthStencilView(creator),
		mRBO(0)
	{
	}

	GLRenderDepthStencilView::~GLRenderDepthStencilView()
	{
		if (mRBO != 0)
		{
			glDeleteRenderbuffers(1, &mRBO);
		}
	}


	void GLRenderDepthStencilView::resize( uint32 w, uint32 h )
	{
		if (w == mWidth && h == mHeight)
		{
			return;
		}
		mWidth = w;
		mHeight = h;

		if (mRBO != 0)
		{
			glDeleteRenderbuffers(1, &mRBO);
		}
		glGenRenderbuffers(1, &mRBO);
		glBindRenderbuffer(GL_RENDERBUFFER, mRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mWidth, mHeight);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

	}


	void GLRenderDepthStencilView::setAttachFrameBuffer( DFrameBuffer* fb )
	{
		mAttachFB = fb;
	}

	GLuint GLRenderDepthStencilView::getRBO()
	{
		return mRBO;
	}


}