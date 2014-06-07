//  [6/7/2014 OMEGA] created

#include "DuelD3D9Common.h"
#include "DuelD3D9VertexBuffer.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(D3D9VertexBuffer, DVertexBuffer);


	void D3D9VertexBuffer::readData( size_t offset, size_t length, void* pDest )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void D3D9VertexBuffer::writeData( size_t offset, size_t length, const void* pSource, bool discardWholeBuffer /*= false */ )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void* D3D9VertexBuffer::lockImpl( size_t offset, size_t size, HardwareBufferLock locklockType )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void D3D9VertexBuffer::unlockImpl()
	{
		throw std::exception("The method or operation is not implemented.");
	}

}