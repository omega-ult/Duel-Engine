//  [8/12/2012 OMEGA] created

#ifndef _DUELINDEXBUFFER_H_
#define _DUELINDEXBUFFER_H_

#include "DuelHardwareBuffer.h"

namespace Duel
{
	enum IndexType
	{
		IT_16Bit = 0,
		IT_32Bit = 1
	};
	/** Specialisation of DHardwareBuffer for vertex index buffers, still abstract. */
	// shadow buffer means reserve a same buffer in the system memory, this method will
	// dupilcate memory usage, but for specified situations this method will increase
	// performance(reading data in system memory is faster than that in gpu memory)

	class DUEL_API DIndexBuffer : public DHardwareBuffer
	{
	DUEL_DECLARE_RTTI(DIndexBuffer)
	public:
		DIndexBuffer( IndexType type, size_t indexCount, HardwareBufferUsage usage, bool useShadow );
		virtual ~DIndexBuffer();
		
		IndexType	getType() const { return mIndexType; }
		size_t		getIndexCount() const { return mIndexCount; }
		size_t		getIndexSize() const { return mIndexSize; }

		bool		isShadowBufferUsed() { return mbUseShadow; }
		// NB subclasses should override lock, unlock, readData, writeData

	protected:
		IndexType	mIndexType;
		size_t		mIndexCount;
		size_t		mIndexSize;
		bool		mbUseShadow;
	};


}

#endif