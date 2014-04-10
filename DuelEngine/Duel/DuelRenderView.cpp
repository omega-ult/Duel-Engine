//  [8/23/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelRenderView.h"


namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(DRenderColorView, DObject);
	DUEL_IMPLEMENT_RTTI_1(DRenderDepthStencilView, DObject);


	DRenderColorView::DRenderColorView(DRenderResourceFactory* creator, DPixelFormat fmt) :
		mCreator(creator),
		mAttachFB(NULL),
		mAttachPoint(EA_Color0),
		mFormat(fmt),
		mWidth(0),
		mHeight(0)
	{

	}

	DRenderColorView::~DRenderColorView()
	{
		if (mAttachFB != NULL)
		{
			mAttachFB->detachRenderColorView(mAttachPoint);
		}
	}

	bool DRenderColorView::isAttached()
	{
		return getAttachedFrameBuffer() != NULL;
	}

	DFrameBuffer* DRenderColorView::getAttachedFrameBuffer()
	{
		return mAttachFB;
	}



	DRenderDepthStencilView::DRenderDepthStencilView(DRenderResourceFactory* creator) :
		mCreator(creator),
		mAttachFB(NULL)
	{

	}

	DRenderDepthStencilView::~DRenderDepthStencilView()
	{
		if (mAttachFB != NULL)
		{
			mAttachFB->detachRenderDepthStencilView();
		}
	}

	bool DRenderDepthStencilView::isAttached()
	{
		return getAttachedFrameBuffer() != NULL;
	}

	DFrameBuffer* DRenderDepthStencilView::getAttachedFrameBuffer()
	{
		return mAttachFB;
	}


}