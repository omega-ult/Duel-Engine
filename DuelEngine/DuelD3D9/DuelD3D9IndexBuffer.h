//  [6/7/2014 OMEGA] created

#ifndef _DUELD3D9INDEXBUFFER_H_
#define _DUELD3D9INDEXBUFFER_H_

#include "DuelD3D9Common.h"
#include "DuelIndexBuffer.h"
#include "DuelD3D9Include.h"

namespace Duel
{
	class D3D9IndexBuffer : public DIndexBuffer
	{
		DUEL_DECLARE_RTTI(D3D9IndexBuffer)
	public:
		D3D9IndexBuffer(D3D9RenderResourceFactory* fact, IndexType type, size_t indexCount, HardwareBufferUsage usage, bool useShadow);
		~D3D9IndexBuffer();
		virtual void readData( size_t offset, size_t length, void* pDest );

		virtual void writeData( size_t offset, size_t length, const void* pSource, bool discardWholeBuffer = false );

		IDirect3DIndexBuffer9*	getIndexBuffer();;
	protected:
		virtual void* lockImpl( size_t offset, size_t size, HardwareBufferLock locklockType );

		virtual void unlockImpl();

		IDirect3DIndexBuffer9*	mIndxBuf;
		char*	mShadowBuf;
	};

}

#endif