//  [8/12/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelVertexBuffer.h"

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(DVertexBuffer, DHardwareBuffer);

	DVertexBuffer::DVertexBuffer( size_t vertexSize, size_t verticesCount, 
		HardwareBufferUsage usage, bool useShadow, VertexBufferType type/* = VBT_GEOMETRY */) :
		DHardwareBuffer(usage),
		mVertexSize(vertexSize),
		mVerticesCount(verticesCount),
		mbUseShadow(useShadow),
		mType(type)
	{
		mByteSize = mVertexSize*mVerticesCount;
	}

	DVertexBuffer::~DVertexBuffer()
	{

	}

}