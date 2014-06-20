//  [4/20/2013 OMEGA] created

#ifndef _DUELGLPIXELBUFFER_H_
#define _DUELGLPIXELBUFFER_H_

#include "Duel.h"
#include "DuelGLCommon.h"
#include "DuelGLTranslator.h"

namespace Duel
{

	class DUELGL_API GLPixelBuffer : public DPixelBuffer
	{
	DUEL_DECLARE_RTTI(GLPixelBuffer)
	public:
		GLPixelBuffer(GLenum target, GLuint texid, GLint face, 
			GLint level, TextureType tt, DPixelFormat format, HardwareBufferUsage usage);
		~GLPixelBuffer();

		// override:DPixelBuffer-----------------------------------
		virtual LockedRect	lockRect(URect& rect, HardwareBufferLock lockType);
		// override:DPixelBuffer-----------------------------------
		virtual LockedBox	lockBox(UBox& box, HardwareBufferLock lockType);

		GLuint		getPBO() { return mPBO; }
	protected:
		// override:HardwareBuffer--------------------------------
		void		unlockImpl();

		// internal method used for transfering data.
		void		downloadToPBO();
		void		uploadToTexture(GLuint unpackPbo);

		// lock type.
		HardwareBufferLock	mLockType;
		
		// basic information about this buffer. 
		GLint		mFace;
		GLint		mLevel;
		//GLenum		mGLInternalFormat;
		// the binded target.
		GLenum		mFaceTarget;
		// store the texture object's id
		GLuint		mTextureID;
		GLenum		mGLusage;
		// use PBO to accelerate read/write
		GLuint		mPBO;

		// gl-style format.
		GLFormatGroup	mGLFormat;
	};


}


#endif