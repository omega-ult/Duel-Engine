//  [12/23/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelIndexData.h"


namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(DIndexData, DObject);


	DIndexData::DIndexData() :
		mIdxStart(0),
		mIdxEnd(0)
	{

	}

	DIndexData::DIndexData( DIndexBufferPtr buf, uint32 start, uint32 end ) :
		mBuffer(buf),
		mIdxStart(0),
		mIdxEnd(0)
	{

	}

	DIndexData::DIndexData( DIndexBufferPtr buf ) :
		mBuffer(buf),
		mIdxStart(0),
		mIdxEnd(0)
	{
		if (buf)
		{
			mIdxEnd = buf->getIndexCount();
		}
	}

	DIndexData::~DIndexData()
	{

	}

	void DIndexData::setIndexStart( uint32 start )
	{
		mIdxStart = start;
	}

	Duel::uint32 DIndexData::getIndexStart()
	{
		return mIdxStart;
	}

	void DIndexData::setIndexEnd( uint32 end )
	{
		mIdxEnd = end;
	}

	Duel::uint32 DIndexData::getIndexEnd()
	{
		return mIdxEnd;
	}

	Duel::DIndexBufferPtr DIndexData::getIndexBuffer()
	{
		return mBuffer;
	}

	void DIndexData::setIndexBuffer( DIndexBufferPtr buf )
	{
		mBuffer = buf;
		mIdxStart = 0;
		mIdxEnd = 0;
		if(mBuffer)
		{
			mIdxEnd = mBuffer->getIndexCount();
		}
	}

}