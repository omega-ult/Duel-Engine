//  [2/8/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelVertexData.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DVertexStreams, DObject);
	DUEL_IMPLEMENT_RTTI_1(DVertexData, DObject);

	DVertexStreams::DVertexStreams()
	{

	}

	DVertexStreams::~DVertexStreams()
	{

	}

	void DVertexStreams::setStream( uint16 source, const DVertexBufferPtr& vertices )
	{
		mStreamMap[source] = vertices;
	}

	void DVertexStreams::clearStream( uint16 source )
	{
		VertexStreamMap::iterator i = mStreamMap.find(source);
		if (i != mStreamMap.end())
		{
			mStreamMap.erase(i);
		}
	}

	void DVertexStreams::clearAllStreams()
	{
		mStreamMap.clear();
	}

	DVertexBufferPtr DVertexStreams::getStream( uint16 source )
	{
		DVertexBufferPtr ret;
		VertexStreamMap::iterator i = mStreamMap.find(source);
		if (i != mStreamMap.end())
		{
			ret = i->second;
		}
		return ret;
	}



}