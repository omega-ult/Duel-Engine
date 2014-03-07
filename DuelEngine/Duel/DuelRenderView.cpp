//  [8/23/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelRenderView.h"


namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(DRenderView, DObject);


	DRenderView::DRenderView( DFrameBuffer* parent, ElementAttachment att ) :
		mParent(parent),
		mAttachPoint(att),
		mbEnable(true)
	{
		mWidth = mParent->getWidth();
		mHeight = mParent->getHeight();
	}

}