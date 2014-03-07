//  [4/7/2013 OMEGA] created

#ifndef _DUELGLINDEXBUFFER_H_
#define _DUELGLINDEXBUFFER_H_

#include "Duel.h"
#include "DuelGLCommon.h"

namespace Duel
{

	class DUELGL_API GLIndexBuffer : public DIndexBuffer
	{
	DUEL_DECLARE_RTTI(GLIndexBuffer)
	public:
		GLIndexBuffer( IndexType type, size_t indexCount,	HardwareBufferUsage usage, bool useShadow );
		~GLIndexBuffer();
		// override : DIndexBuffer-------------------------
		virtual void	readData( size_t offset, size_t length, void* pDest );
		// override : DIndexBuffer-------------------------
		virtual void	writeData( size_t offset, size_t length, const void* pSource, bool discardWholeBuffer = false );
		// override : DIndexBuffer-------------------------
		virtual void*	lockImpl( size_t offset, size_t size, HardwareBufferLock lockType );
		// override : DIndexBuffer-------------------------
		virtual void	unlockImpl();

		virtual GLuint	getGLIndexBufferID() { return mBufferID; }
	protected:
		GLuint				mBufferID;
		char*				mShadowBuffer;
	};

}

#endif