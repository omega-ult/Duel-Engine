//  [8/12/2012 OMEGA] created

#ifndef _DUELVERTEXBUFFER_H_
#define _DUELVERTEXBUFFER_H_

#include "DuelHardwareBuffer.h"

namespace Duel
{
	// used in multi-stream drawing method,
	enum VertexBufferType
	{
		VBT_Geometry	= 0,
		VBT_Instance	= 1
	};

	class DUEL_API DVertexBuffer : public DHardwareBuffer
	{
	DUEL_DECLARE_RTTI(DVertexBuffer)
	public:
		// the vertexSize is the size of one vertex unit, including pos, normal, etc.
		// the vertexCount is always the count of the vertex, no matter 
		// what elements the vertex contains, for example, position, normal, etc.
		// shadow buffer means reserve a same buffer in the system memory, this method will
		// dupilcate memory usage, but for specified situations this method will increase
		// performance(reading data in system memory is faster than that in gpu memory)
		DVertexBuffer(size_t vertexSize, size_t verticesCount,
			HardwareBufferUsage usage, bool useShadow, VertexBufferType type = VBT_Geometry);
		virtual ~DVertexBuffer();

		size_t	getVertexSize() { return mVertexSize; }
		size_t	getVertexCount() { return mVertexCount; }

		void		setVertexBufferType(VertexBufferType type ) { mType = type; }
		VertexBufferType	getVertexBufferType() { return mType; }

		bool		isShadowBufferUsed() { return mbUseShadow; }

	protected:
		size_t	mVertexSize;
		size_t	mVertexCount;
		VertexBufferType	mType;
		bool	mbUseShadow;
	};
}

#endif