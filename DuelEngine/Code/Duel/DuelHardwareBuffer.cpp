//  [8/12/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelHardwareBuffer.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DHardwareBuffer, DObject);
	DUEL_IMPLEMENT_RTTI_1(DRawBuffer, DHardwareBuffer);

	DHardwareBuffer::DHardwareBuffer( HardwareBufferUsage usage) :
		mUsage(usage),
		mLockType(HBL_Normal),
		mbIsLocked(false)
	{

	}

	DHardwareBuffer::DHardwareBuffer()
	{

	}

	DHardwareBuffer::~DHardwareBuffer()
	{

	}

	bool DHardwareBuffer::isLocked()
	{
		return mbIsLocked;
	}

	Duel::HardwareBufferUsage DHardwareBuffer::getUsage()
	{
		return mUsage;
	}

	size_t DHardwareBuffer::getSize()
	{
		return mByteSize;
	}

	void DHardwareBuffer::unlock()
	{
		assert(mbIsLocked);
		unlockImpl();
		mbIsLocked = false;
	}

	void* DHardwareBuffer::lock( HardwareBufferLock lockType )
	{
		return lock(0, mByteSize, lockType);
	}

	void* DHardwareBuffer::lock( size_t offset, size_t size, HardwareBufferLock lockType )
	{
		assert(!mbIsLocked);
		assert(offset < mByteSize);
		assert(offset + size <= mByteSize);
		void* ret = lockImpl(offset, size, lockType);
		mbIsLocked = true;
		mLockStart = offset;
		mLockSize = size;
		return ret;
	}

	DRawBuffer::DRawBuffer(size_t byteSize) : 
		DHardwareBuffer(HBU_Dynamic)
	{
		mByteSize = byteSize;
		mData = new uint8[byteSize];
	}

	DRawBuffer::DRawBuffer( DHardwareBuffer* src ) : 
		DHardwareBuffer(HBU_Dynamic)
	{
		mByteSize = src->getSize();
		mData = new uint8[mByteSize];
		if (!src->isLocked())
		{
			char* data = (char*)src->lock(HBL_ReadOnly);
			memcpy(mData, src, mByteSize);
			src->unlock();
		}
	}

	DRawBuffer::~DRawBuffer()
	{
		delete mData;
	}

	void DRawBuffer::readData( size_t offset, size_t length, void* pDest )
	{
		assert(offset + length <= mByteSize);
		memcpy(pDest, mData + offset, length);
	}

	void DRawBuffer::writeData( size_t offset, size_t length, const void* pSource, bool discardWholeBuffer /*= false */ )
	{
		assert(offset + length <= mByteSize);
		memcpy(mData + offset, pSource, length);
	}

	void* DRawBuffer::lockImpl( size_t offset, size_t size, HardwareBufferLock locklockType )
	{
		return (mData + offset);
	}

	void DRawBuffer::unlockImpl()
	{
		
	}


}