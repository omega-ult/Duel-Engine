//  [4/7/2013 OMEGA] created

#ifndef _DUELGLVERTEXBUFFER_H_
#define _DUELGLVERTEXBUFFER_H_

#include "Duel.h"
#include "DuelGLCommon.h"


namespace Duel
{

	class DUELGL_API GLVertexBuffer : public DVertexBuffer
	{
	DUEL_DECLARE_RTTI(GLVertexBuffer)
	public:
		GLVertexBuffer(size_t vertexSize, size_t verticesCount,
			HardwareBufferUsage usage, bool useShadow, VertexBufferType type = VBT_Geometry);
		~GLVertexBuffer();

		// override : DVertexBuffer-------------------------
		virtual void	readData( size_t offset, size_t length, void* pDest );
		// override : DVertexBuffer-------------------------
		virtual void	writeData( size_t offset, size_t length, const void* pSource, bool discardWholeBuffer = false );
		// override : DVertexBuffer-------------------------
		virtual void*	lockImpl( size_t offset, size_t size, HardwareBufferLock lockType );
		// override : DVertexBuffer-------------------------
		virtual void	unlockImpl();

		virtual GLuint	getGLVertexBufferID() { return mBufferID; }
	protected:
		GLuint				mBufferID;
		char*				mShadowBuffer;
	};

}

#endif