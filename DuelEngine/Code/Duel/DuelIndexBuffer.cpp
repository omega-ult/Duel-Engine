//  [8/13/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelIndexBuffer.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DIndexBuffer, DHardwareBuffer)

	DIndexBuffer::DIndexBuffer( IndexType type, size_t indexCount, HardwareBufferUsage usage, bool useShadow ) :
		DHardwareBuffer(usage),
		mIndexCount(indexCount),
		mIndexType(type),
		mbUseShadow(useShadow)
	{
		switch(mIndexType)
		{
		case IT_16Bit:
			mIndexSize = sizeof( uint16 );
			break;
		case IT_32Bit:
			mIndexSize = sizeof( uint32 );
			break;
		}

		mByteSize = mIndexSize * mIndexCount;
	}

	DIndexBuffer::~DIndexBuffer()
	{

	}

}