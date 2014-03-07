//  [8/23/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelViewport.h"
#include "DuelFrameBuffer.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DFrameBuffer, DObject);

	DFrameBuffer::DFrameBuffer( DRenderResourceFactory* parent, uint32 w, uint32 h, uint32 colorbits ) :
		mParent(parent),
		mWidth(w),
		mHeight(h),
		mColorBits(colorbits)
	{

	}

}