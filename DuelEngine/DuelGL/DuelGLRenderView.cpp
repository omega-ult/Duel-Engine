//  [8/23/2013 OMEGA] created

#include "DuelGLCommon.h"
#include "DuelGLTranslator.h"
#include "DuelGLRenderView.h"

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(GLRenderView, DRenderView);

	GLRenderView::GLRenderView( DFrameBuffer* parent, ElementAttachment att, DPixelFormat fmt ) :
		DRenderView(parent, att),
		mTextureID(0),
		mGpuConstant(this)
	{
		mFormat = fmt;
		// we wouldn't do resize here, parent frame buffer will call resize directly.
	}

	GLRenderView::~GLRenderView()
	{
		if (mTextureID != 0)
		{
			glDeleteTextures(1,&mTextureID);
		}
	}

	void GLRenderView::resize( uint32 w, uint32 h )
	{
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
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap generation included in OpenGL v1.4
		GLFormatGroup fg = GLTranslator::getGLFormat(mFormat);
		glTexImage2D(GL_TEXTURE_2D, 0, fg.internalFormat, mWidth, mHeight, 0, fg.glFormat, fg.glFormatType, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

	}

}