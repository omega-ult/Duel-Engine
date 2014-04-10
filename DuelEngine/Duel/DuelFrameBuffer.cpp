//  [8/23/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelViewport.h"
#include "DuelFrameBuffer.h"
#include "DuelRenderView.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DFrameBuffer, DObject);

	DFrameBuffer::DFrameBuffer( DRenderResourceFactory* creator, uint32 w, uint32 h, uint32 colorbits ) :
		mCreator(creator),
		mWidth(w),
		mHeight(h),
		mColorBits(colorbits)
	{

	}

// 	void DFrameBuffer::attachRenderColorView( ElementAttachment elem, DRenderColorView* v )
// 	{
// 		if (v == NULL)
// 		{
// 			assert(false, "Can't attach an empty render color view");
// 			return;
// 		}
// 		if (v->getAttachedFrameBuffer() != NULL && v->getAttachedFrameBuffer() != this)
// 		{
// 			assert(false, "Specified view is attached to another frame buffer, you must detach it before reusing it");
// 			return;
// 		}
// 		// the case specified v is attached to another 
// 		if (getRenderColorView(elem) != NULL)
// 		{
// 			detachRenderColorView(elem);
// 		}
// 		if (v->getAttachedFrameBuffer() == this)
// 		{
// 			detachRenderColorView(v->getAttachPoint());
// 		}
// 	}
// 
// 	void DFrameBuffer::detachRenderColorView( ElementAttachment elem )
// 	{
// 		DRenderColorView* v = getRenderColorView(elem);
// 		if (v != NULL)
// 		{
// 			v->mAttachFB = NULL;
// 			v->mAttachPoint = EA_Color0;
// 		}
// 	}
// 
// 	void DFrameBuffer::attachRenderDepthStencilView( DRenderDepthStencilView* v )
// 	{
// 
// 	}
// 
// 	void DFrameBuffer::detachRenderDepthStencilView()
// 	{
// 
// 	}

}