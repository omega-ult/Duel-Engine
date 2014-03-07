//  [4/12/2013 OMEGA] created

#include "DuelGLCommon.h"
#include "DuelGLVertexBuffer.h"
#include "DuelGLTranslator.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(GLVertexBuffer, DVertexBuffer);


	GLVertexBuffer::GLVertexBuffer( size_t vertexSize, size_t verticesCount, HardwareBufferUsage usage, bool useShadow, VertexBufferType type /*= VBT_GEOMETRY*/ ) :
		DVertexBuffer(vertexSize, verticesCount,usage, useShadow, type),
		mShadowBuffer(NULL),
		mBufferID(0)
	{
		glGenBuffersARB( 1, &mBufferID );

		if (!mBufferID)
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InternalError, 
				"Cannot create GL vertex buffer", 
				"Duel::GLVertexBuffer::GLVertexBuffer");
		}

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, mBufferID);

		if (mbUseShadow)
		{
			mShadowBuffer = new char[mByteSize];
		}

		// Initialise mapped buffer and set usage
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, mByteSize, NULL, 
			GLTranslator::getGLUsage(usage));
	}

	GLVertexBuffer::~GLVertexBuffer()
	{
		glDeleteBuffersARB(1, &mBufferID);
		if(mShadowBuffer != NULL)
		{
			delete mShadowBuffer;
		}
	}

	void GLVertexBuffer::writeData( size_t offset, size_t length, const void* pSource, bool discardWholeBuffer /*= false */ )
	{
		HardwareBufferLock hblock = discardWholeBuffer? HBL_Discard : HBL_Normal;
		void* pDest = lock(offset, length, hblock);
		memcpy(pDest, pSource, length);
		this->unlock();
	}

	void GLVertexBuffer::readData( size_t offset, size_t length, void* pDest )
	{
		void* pSrc = lock(offset, length, HBL_ReadOnly);
		memcpy(pDest, pSrc, length);
		this->unlock();
	}

	void* GLVertexBuffer::lockImpl( size_t offset, size_t size, HardwareBufferLock lockType )
	{
		GLenum access = 0;

		if(mbIsLocked)
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InternalError,
				"Invalid attempt to lock an vertex buffer that has already been locked",
				"Duel::GLVertexBuffer::lockImpl");
		}

		mLockType = lockType;

		if (mbUseShadow)
		{
			return mShadowBuffer + offset;
		}

		// Use glMapBuffer
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, mBufferID );
		// Use glMapBuffer
		if(lockType == HBL_Discard)
		{
			// Discard the buffer
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, mByteSize, NULL, 
				GLTranslator::getGLUsage(mUsage));

		}
		if (mUsage & HBU_WriteOnly)
			access = GL_WRITE_ONLY_ARB;
		else if (lockType == HBL_ReadOnly || lockType == HBL_NoOverwrite)
			access = GL_READ_ONLY_ARB;
		else
			access = GL_READ_WRITE_ARB;

		void* pBuffer = glMapBufferARB( GL_ARRAY_BUFFER_ARB, access);

		if(pBuffer == 0)
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InternalError, 
				"Vertex Buffer: Out of memory",
				"Duel::GLVertexBuffer::lockImpl");
		}

		void* retPtr = 0;
		// return offsetted
		retPtr = static_cast<void*>(
			static_cast<unsigned char*>(pBuffer) + offset);
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
		return retPtr;
	}

	void GLVertexBuffer::unlockImpl()
	{
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, mBufferID );
		if (mLockType == HBL_ReadOnly || mLockType == HBL_NoOverwrite)
		{
			// if we are using shadow buffer, for read_only lock, skip it.
			if (!mbUseShadow)
			{

				if(!glUnmapBufferARB( GL_ARRAY_BUFFER_ARB ))
				{
					DUEL_EXCEPT_BRIEF(DException::ET_InternalError, 
						"Buffer data corrupted, please reload", 
						"Duel::GLVertexBuffer::unlockImpl");
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
				glBufferDataARB(GL_ARRAY_BUFFER_ARB, mByteSize, NULL, 
					GLTranslator::getGLUsage(mUsage));
				void* pBuffer = glMapBufferARB( GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY_ARB);
				
				if(pBuffer == 0)
				{
					DUEL_EXCEPT_BRIEF(DException::ET_InternalError, 
						"Vertex Buffer: Out of memory",
						"Duel::GLVertexBuffer::unlockImpl");
				}
				memcpy(pBuffer, mShadowBuffer, mByteSize);
			}

			if(!glUnmapBufferARB( GL_ARRAY_BUFFER_ARB ))
			{
				DUEL_EXCEPT_BRIEF(DException::ET_InternalError, 
					"Buffer data corrupted, please reload", 
					"Duel::GLVertexBuffer::unlockImpl");
			}
		}
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0);
	}

}
