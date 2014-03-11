//  [8/23/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelRenderView.h"


namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(DRenderColorView, DObject);
	DUEL_IMPLEMENT_RTTI_1(DRenderDepthView, DObject);


	DRenderColorView::DRenderColorView( DFrameBuffer* parent, ElementAttachment att ) :
		mParent(parent),
		mAttachPoint(att),
		mbEnable(true)
	{
		mWidth = mParent->getWidth();
		mHeight = mParent->getHeight();
	}


	DRenderDepthView::DRenderDepthView( DFrameBuffer* parent ) :
		mParent(parent)
	{
		mWidth = mParent->getWidth();
		mHeight = mParent->getHeight();
	}

}