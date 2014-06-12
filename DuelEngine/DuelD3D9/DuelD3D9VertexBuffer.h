//  [6/7/2014 OMEGA] created

#ifndef _DUELD3D9VERTEXBUFFER_H_
#define _DUELD3D9VERTEXBUFFER_H_

#include "DuelD3D9Common.h"
#include "DuelVertexBuffer.h"

namespace Duel
{
	class D3D9VertexBuffer : public DVertexBuffer
	{
		DUEL_DECLARE_RTTI(D3D9VertexBuffer)
	public:
		D3D9VertexBuffer(size_t vertexSize, size_t verticesCount,
			HardwareBufferUsage usage, bool useShadow, VertexBufferType type = VBT_Geometry);
		virtual void readData( size_t offset, size_t length, void* pDest );

		virtual void writeData( size_t offset, size_t length, const void* pSource, bool discardWholeBuffer = false );

	protected:
		virtual void* lockImpl( size_t offset, size_t size, HardwareBufferLock locklockType );

		virtual void unlockImpl();

	};
}

#endif