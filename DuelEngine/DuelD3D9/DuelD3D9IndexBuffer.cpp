//  [6/7/2014 OMEGA] created

#include "DuelD3D9Common.h"
#include "DuelD3D9IndexBuffer.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(D3D9IndexBuffer, DIndexBuffer);

	
	D3D9IndexBuffer::D3D9IndexBuffer( IndexType type, size_t indexCount, HardwareBufferUsage usage, bool useShadow ) :
		DIndexBuffer(type, indexCount, usage, useShadow)
	{

	}

	void D3D9IndexBuffer::readData( size_t offset, size_t length, void* pDest )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void D3D9IndexBuffer::writeData( size_t offset, size_t length, const void* pSource, bool discardWholeBuffer /*= false */ )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void* D3D9IndexBuffer::lockImpl( size_t offset, size_t size, HardwareBufferLock locklockType )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void D3D9IndexBuffer::unlockImpl()
	{
		throw std::exception("The method or operation is not implemented.");
	}

}