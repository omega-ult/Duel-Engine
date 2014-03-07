//  [12/23/2013 OMEGA] created

#ifndef _DUELINDEXDATA_H_
#define _DUELINDEXDATA_H_

#include "DuelCommon.h"
#include "DuelIndexBuffer.h"

namespace Duel
{

	// 只是一个包裹类.. 
	class DUEL_API DIndexData : public DObject
	{
		DUEL_DECLARE_RTTI(DIndexData)
	public:
		DIndexData();
		virtual ~DIndexData();
		DIndexData(DIndexBufferPtr buf, uint32 start, uint32 end);
		// this method will use 0, buf->getIndexCount() as its parameter.
		DIndexData(DIndexBufferPtr buf);

		void			setIndexStart(uint32 start);
		uint32			getIndexStart();

		void			setIndexEnd(uint32 end);
		uint32			getIndexEnd();

		DIndexBufferPtr	getIndexBuffer();
		void			setIndexBuffer(DIndexBufferPtr buf);


	protected:
		DIndexBufferPtr		mBuffer;
		uint32				mIdxStart;
		uint32				mIdxEnd;

	};

}


#endif