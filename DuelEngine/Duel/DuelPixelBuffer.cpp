//  [9/16/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelException.h"
#include "DuelPixelBuffer.h"
#include "DuelImageDesc.h"

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(DPixelBuffer, DHardwareBuffer)
	DPixelBuffer::DPixelBuffer( size_t width, size_t height, size_t depth, TextureType type, DPixelFormat format, HardwareBufferUsage usage ) :
		DHardwareBuffer(usage),
		mWidth(width),
		mHeight(height),
		mDepth(depth),
		mType(type),
		mFormat(format)
	{

		uint32 mipDepth	= (mType == TT_3D)? depth : 1;
		size_t	totalSize	= 0;

		if(mType == TT_Cube)
		{
			totalSize += 6 * DPixelFormatTool::getSurfaceSize(mWidth, mHeight, 1, mFormat);
		}
		else
		{
			totalSize += DPixelFormatTool::getSurfaceSize(mWidth, mHeight, mDepth, mFormat);
		} 
		mByteSize = totalSize;
	}



	DPixelBuffer::~DPixelBuffer(void)
	{
	}

	void* DPixelBuffer::lockImpl( size_t offset, size_t size, HardwareBufferLock locklockType )
	{
		DUEL_EXCEPT_BRIEF(DException::ET_NotImplemented,
			"DPixelBuffer do not support lock() function.",
			"Duel::DPixelBuffer::lockImpl")
	}

	void DPixelBuffer::readData( size_t offset, size_t length, void* pDest )
	{
		DUEL_EXCEPT_BRIEF(DException::ET_NotImplemented,
			"DPixelBuffer do not support readData() function.",
			"Duel::DPixelBuffer::readData")
	}

	void DPixelBuffer::writeData( size_t offset, size_t length, const void* pSource, bool discardWholeBuffer /*= false*/ )
	{
		DUEL_EXCEPT_BRIEF(DException::ET_NotImplemented,
			"DPixelBuffer do not support writeData() function.",
			"Duel::DPixelBuffer::writeData")
	}


}
