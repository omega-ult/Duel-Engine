//  [6/7/2014 OMEGA] created

#ifndef _DUELD3D9INDEXBUFFER_H_
#define _DUELD3D9INDEXBUFFER_H_

#include "DuelD3D9Common.h"
#include "DuelIndexBuffer.h"

namespace Duel
{
	class D3D9IndexBuffer : public DIndexBuffer
	{
		DUEL_DECLARE_RTTI(D3D9IndexBuffer)
	public:
		D3D9IndexBuffer(IndexType type, size_t indexCount, HardwareBufferUsage usage, bool useShadow);

		virtual void readData( size_t offset, size_t length, void* pDest );

		virtual void writeData( size_t offset, size_t length, const void* pSource, bool discardWholeBuffer = false );

	protected:
		virtual void* lockImpl( size_t offset, size_t size, HardwareBufferLock locklockType );

		virtual void unlockImpl();

	};

}

#endif