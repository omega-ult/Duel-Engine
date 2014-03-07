//  [4/20/2013 OMEGA] created

#include "DuelGLCommon.h"
#include "DuelGLPixelBuffer.h"
#include "DuelGLTranslator.h"

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(GLPixelBuffer, DPixelBuffer);


	GLPixelBuffer::GLPixelBuffer( GLenum target, GLuint texid, GLint face, GLint level, TextureType tt, DPixelFormat format, HardwareBufferUsage usage ) :
		DPixelBuffer(0,0,0,tt, format,usage),
		mFaceTarget(target),
		mFace(face),
		mLevel(level),
		mTextureID(texid),
		mLockType(HBL_Discard),
		mPBO(0),
		mGLFormat(0,0,0)
	{
		// devise mWidth, mHeight and mDepth and mFormat
		GLint value = 0;

		glBindTexture( mFaceTarget, mTextureID );
		// Get face identifier
		if(mFaceTarget == GL_TEXTURE_CUBE_MAP)
		{
			mFaceTarget = GL_TEXTURE_CUBE_MAP_POSITIVE_X + face;
		}
		// Get width
		glGetTexLevelParameteriv(mFaceTarget, level, GL_TEXTURE_WIDTH, &value);
		mWidth = value;
		// Get height
		if(target == GL_TEXTURE_1D)
		{
			value = 1;	// Height always 1 for 1D textures
		}
		else
		{
			glGetTexLevelParameteriv(mFaceTarget, level, GL_TEXTURE_HEIGHT, &value);
		}
		mHeight = value;
		// Get depth
		if(target != GL_TEXTURE_3D)
		{
			value = 1; // Depth always 1 for non-3D textures
		}
		else
		{
			glGetTexLevelParameteriv(mFaceTarget, level, GL_TEXTURE_DEPTH, &value);
		}
		mDepth = value;
// 
// 		// Get format
// 		glGetTexLevelParameteriv(mFaceTarget, level, GL_TEXTURE_INTERNAL_FORMAT, &value);
// 		mGLInternalFormat = value;
		
		// Default
		mRowPitch = mWidth * DPixelFormatTool::getFormatBytes(mFormat);
		mSlicePitch = mHeight*mRowPitch;
		mByteSize = DPixelFormatTool::getSurfaceSize(mWidth, mHeight, mDepth, mFormat);

		mGLusage = GLTranslator::getGLUsage(usage);
		mGLFormat = GLTranslator::getGLFormat(mFormat);
		glBindTexture( mFaceTarget, 0 );

	}

	GLPixelBuffer::~GLPixelBuffer()
	{
		
	}

	Duel::LockedRect GLPixelBuffer::lockRect( URect& rect, HardwareBufferLock lockType )
	{
		assert(mPBO == 0);
		glGenBuffers(1, &mPBO);

		LockedRect ret;
		if (mType != TT_2D && mType != TT_1D && mType != TT_Cube)
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
				"Invalid texture type, failed to lock",
				"Duel::GLPixelBuffer::lockRect")
		}

		// bind the texture and PBO
		glBindTexture(mFaceTarget, mTextureID);
		mLockType = lockType;

		// since OpenGL only support downloading entire buffer.
		// we set the box to the max.
		rect.xBegin = 0; 
		rect.yBegin = 0;
		rect.xExtend = mWidth;
		rect.yExtend = mHeight;


		if (lockType == HBL_Discard)
		{
			// just allocate an empty space for writing.
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, mPBO);
			glBufferData(GL_PIXEL_UNPACK_BUFFER, mByteSize, 0, mGLusage);
			ret.dataPtr = (GLubyte*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
		}
		else
		{
			// download the image to the pbo, then get the buffer data pointer.
			downloadToPBO();

			glBindBuffer(GL_PIXEL_PACK_BUFFER, mPBO);
			switch (lockType)
			{
			case Duel::HBL_Normal:
			case Duel::HBL_NoOverwrite:
				ret.dataPtr = (GLubyte*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_WRITE);
				break;
			case Duel::HBL_ReadOnly:
				ret.dataPtr = (GLubyte*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
				break;
			default:
				ret.dataPtr = (GLubyte*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_WRITE);
			}
		}
		glBindTexture(mFaceTarget, 0);


		mbIsLocked = true;
		/////

		ret.rowPitch = mRowPitch;
		return  ret;
	}

	Duel::LockedBox GLPixelBuffer::lockBox( UBox& box, HardwareBufferLock lockType )
	{
		assert(mPBO == 0);
		glGenBuffers(1, &mPBO);

		LockedBox ret;		
		if (mType != TT_3D)
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
				"Invalid texture type, failed to lock",
				"Duel::GLPixelBuffer::lockBox")
		}

		// since OpenGL only support downloading entire buffer.
		// we set the box to the max.
		box.left = 0; 
		box.bottom = 0;
		box.back = 0;
		box.right = mWidth;
		box.top = mHeight;
		box.front = mDepth;


		// bind the texture and PBO
		glBindTexture(mFaceTarget, mTextureID);
		mLockType = lockType;

		if (lockType == HBL_Discard)
		{
			// just allocate an empty space for writing.
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, mPBO);
			glBufferData(GL_PIXEL_UNPACK_BUFFER, mByteSize, 0, mGLusage);
			ret.dataPtr = (GLubyte*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
		}
		else
		{
			// download the image to the pbo, then get the buffer data pointer.
			downloadToPBO();

			glBindBuffer(GL_PIXEL_PACK_BUFFER, mPBO);
			switch (lockType)
			{
			case Duel::HBL_Normal:
			case Duel::HBL_NoOverwrite:
				ret.dataPtr = (GLubyte*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_WRITE);
				break;
			case Duel::HBL_ReadOnly:
				ret.dataPtr = (GLubyte*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
				break;
			default:
				ret.dataPtr = (GLubyte*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_WRITE);
			}
		}

		glBindTexture(mFaceTarget, 0);
		////////
		// important mark as locked.
		mbIsLocked = true;
		/////

		ret.rowPitch = mRowPitch;
		ret.slicePitch = mSlicePitch;
		return  ret;

	}

	void GLPixelBuffer::unlockImpl()
	{
		// bind the texture and PBO
		glBindTexture(mFaceTarget, mTextureID);
		GLenum pboTarget = mLockType == HBL_Discard ? GL_PIXEL_UNPACK_BUFFER : GL_PIXEL_PACK_BUFFER;

		glBindBuffer(pboTarget, mPBO);
		GLboolean result = glUnmapBuffer(pboTarget); // release pointer to mapping buffer
		if (result == GL_FALSE)
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InternalError,
				"Failed to unlock buffer",
				"Duel::GLPixelBuffer::unlockImpl");
		}

		glBindBuffer(pboTarget, 0);
		glBindTexture(mFaceTarget, 0);

		if (mLockType != HBL_ReadOnly)
		{
			uploadToTexture(mPBO);
		}

		glDeleteBuffers(1, &mPBO);
		mPBO = 0;
	}

	void GLPixelBuffer::downloadToPBO()
	{
		// retrieve texture.
		glBindTexture(mFaceTarget, mTextureID);
		glBindBuffer(GL_PIXEL_PACK_BUFFER, mPBO);
		glBufferData(GL_PIXEL_PACK_BUFFER, mByteSize, 0, mGLusage);
		glGetTexImage(mFaceTarget, mLevel, mGLFormat.glFormat, mGLFormat.glFormatType, 0);
		glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
	}

	void GLPixelBuffer::uploadToTexture(GLuint unpackPbo)
	{
		glBindTexture(mFaceTarget, mTextureID);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, unpackPbo);

		if (mType == TT_1D)
		{
		 	if (DPixelFormatTool::isCompressed(mFormat))
		 	{
		 		glCompressedTexSubImage1D(mFaceTarget, mLevel, 0, mWidth, mGLFormat.glFormat, mGLFormat.glFormatType, 0 );
		 	}
		 	else
		 	{
		 		glTexSubImage1D(mFaceTarget, mLevel, 0, mWidth, mGLFormat.glFormat, mGLFormat.glFormatType, 0 );
		 	}
		}
		else if (mType == TT_2D)
		{
		 	if(DPixelFormatTool::isCompressed(mFormat))
		 	{				
		 		glCompressedTexSubImage2D(mFaceTarget, mLevel, 0, 0, mWidth, mHeight, mGLFormat.glFormat, mGLFormat.glFormatType, 0);
		 	}
		 	else
		 	{			
		 		glTexSubImage2D(mFaceTarget, mLevel, 0, 0, mWidth, mHeight, mGLFormat.glFormat, mGLFormat.glFormatType, 0);
		 	}
		}
		else if (mType == TT_Cube)
		{
		 	if (DPixelFormatTool::isCompressed(mFormat))
		 	{
		 		glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+mFace, mLevel, 0, 0, mWidth, mHeight, mGLFormat.glFormat, mGLFormat.glFormatType, 0);
		 	}
		 	else
		 	{
		 		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+mFace, mLevel, 0, 0, mWidth, mHeight,  mGLFormat.glFormat, mGLFormat.glFormatType, 0);
		 	}
		}
		else // TT_3D
		{
			if (DPixelFormatTool::isCompressed(mFormat))
			{
			 	glCompressedTexSubImage3D(GL_TEXTURE_3D, mLevel, 
			 		0, 0, 0, 
			 		mWidth, mHeight, mDepth, mGLFormat.glFormat, mGLFormat.glFormatType, 0);
			}
			else
			{
				glTexSubImage3D(GL_TEXTURE_3D, mLevel,0, 0, 0, 
					mWidth, mHeight, mDepth, mGLFormat.glFormat, mGLFormat.glFormatType, 0);
			}
		}

		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
		glBindTexture(mFaceTarget, 0);
	}

}