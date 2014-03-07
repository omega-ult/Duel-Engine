//  [4/12/2013 OMEGA] created

#include "DuelGLCommon.h"
#include "DuelGLIndexBuffer.h"
#include "DuelGLTranslator.h"

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(GLIndexBuffer, DIndexBuffer);



	GLIndexBuffer::GLIndexBuffer( IndexType type, size_t indexCount, HardwareBufferUsage usage, bool useShadow ) :
		DIndexBuffer(type, indexCount, usage, useShadow),
		mShadowBuffer(NULL),
		mBufferID(0)
	{
		glGenBuffersARB( 1, &mBufferID );

		if (!mBufferID)
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InternalError, 
				"Cannot create GL vertex buffer", 
				"Duel::GLIndexBuffer::GLVertexBuffer");
		}

		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, mBufferID);

		if (mbUseShadow)
		{
			mShadowBuffer = new char[mByteSize];
		}

		// Initialise mapped buffer and set usage
		glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, mByteSize, NULL, 
			GLTranslator::getGLUsage(usage));

	}

	GLIndexBuffer::~GLIndexBuffer()
	{
		glDeleteBuffersARB(1, &mBufferID);
		if(mShadowBuffer != NULL)
		{
			delete mShadowBuffer;
		}

	}

	void GLIndexBuffer::readData( size_t offset, size_t length, void* pDest )
	{
		void* pSrc = lock(offset, length, HBL_ReadOnly);
		memcpy(pDest, pSrc, length);
		this->unlock();
	}

	void GLIndexBuffer::writeData( size_t offset, size_t length, const void* pSource, bool discardWholeBuffer /*= false */ )
	{
		HardwareBufferLock hblock = discardWholeBuffer? HBL_Discard : HBL_Normal;
		void* pDest = lock(offset, length, hblock);
		memcpy(pDest, pSource, length);
		this->unlock();
	}

	void* GLIndexBuffer::lockImpl( size_t offset, size_t size, HardwareBufferLock lockType )
	{
		GLenum access = 0;

		if(mbIsLocked)
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InternalError,
				"Invalid attempt to lock an index buffer that has already been locked",
				"Duel::GLIndexBuffer::lockImpl");
		}

		mLockType = lockType;

		if (mbUseShadow)
		{
			return mShadowBuffer + offset;
		}

		// Use glMapBuffer
		glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER, mBufferID );
		// Use glMapBuffer
		if(lockType == HBL_Discard)
		{
			// Discard the buffer
			glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, mByteSize, NULL, 
				GLTranslator::getGLUsage(mUsage));

		}
		if (mUsage & HBU_WriteOnly)
			access = GL_WRITE_ONLY_ARB;
		else if (lockType == HBL_ReadOnly || lockType == HBL_NoOverwrite)
			access = GL_READ_ONLY_ARB;
		else
			access = GL_READ_WRITE_ARB;

		void* pBuffer = glMapBufferARB( GL_ELEMENT_ARRAY_BUFFER, access);

		if(pBuffer == 0)
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InternalError, 
				"Index Buffer: Out of memory",
				"Duel::GLIndexBuffer::lockImpl");
		}

		void* retPtr = 0;
		// return offsetted
		retPtr = static_cast<void*>(
			static_cast<unsigned char*>(pBuffer) + offset);
		glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER, 0 );
		return retPtr;
	}

	void GLIndexBuffer::unlockImpl()
	{	
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, mBufferID);

		if (mLockType == HBL_ReadOnly || mLockType == HBL_NoOverwrite)
		{
			// if we are using shadow buffer, for read_only lock, skip it.
			if (!mbUseShadow)
			{

				if(!glUnmapBufferARB( GL_ELEMENT_ARRAY_BUFFER ))
				{
					DUEL_EXCEPT_BRIEF(DException::ET_InternalError, 
						"Buffer data corrupted, please reload", 
						"Duel::GLIndexBuffer::unlockImpl");
				}
			}
		}
		else
		{

			if (mbUseShadow)
			{
				if (mLockType == HBL_NoOverwrite)
				{
					return;
				}
				// Discard the buffer
				glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, mByteSize, NULL, 
					GLTranslator::getGLUsage(mUsage));
				void* pBuffer = glMapBufferARB( GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY_ARB);

				if(pBuffer == 0)
				{
					DUEL_EXCEPT_BRIEF(DException::ET_InternalError, 
						"Index Buffer: Out of memory",
						"Duel::GLIndexBuffer::unlockImpl");
				}
				memcpy(pBuffer, mShadowBuffer, mByteSize);
			}

			if(!glUnmapBufferARB( GL_ELEMENT_ARRAY_BUFFER ))
			{
				DUEL_EXCEPT_BRIEF(DException::ET_InternalError, 
					"Buffer data corrupted, please reload", 
					"Duel::GLIndexBuffer::unlockImpl");
			}

		}
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);

	}

}