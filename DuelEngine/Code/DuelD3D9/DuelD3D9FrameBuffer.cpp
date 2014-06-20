//  [6/7/2014 OMEGA] created

#include "DuelD3D9Common.h"
#include "DuelFrameBuffer.h"
#include "DuelD3D9RenderView.h"
#include "DuelD3D9FrameBuffer.h"
#include "DuelD3D9RenderSystem.h"
#include "DuelCore.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(D3D9FrameBuffer, DFrameBuffer);

	D3D9FrameBuffer::D3D9FrameBuffer( DRenderResourceFactory* creator, uint32 w, uint32 h, uint32 colorBits ) :
		DFrameBuffer(creator, w, h, colorBits),
		mCurDepthView(NULL)
	{
		for (int32 i = 0; i < 8; i++)
		{
			mViewList.push_back(NULL);
		}
	}

	void D3D9FrameBuffer::resize( uint32 width, uint32 height )
	{
		if (width == mWidth && height == mHeight)
		{
			return;
		}
		assert(width != 0 && height != 0);
		DFrameBuffer::resize(width, height);
		mCurViewport.reset(0,0, width, height);
		RenderColorViewList::iterator i, iend = mViewList.end();
		for (i = mViewList.begin(); i != iend; ++i)
		{
			if ((*i) != NULL)
			{
				(*i)->resize(width, height);
			}
		}

		if(mCurDepthView != NULL)
		{
			mCurDepthView->resize(width, height);
		}
	}

	void D3D9FrameBuffer::attachRenderColorView( ElementAttachment elem, DRenderColorView* v )
	{
		if (v == NULL)
		{
			assert(false);// "Can't attach an empty render color view"
			return;
		}
		if (v->getAttachedFrameBuffer() != NULL && v->getAttachedFrameBuffer() != this)
		{
			assert(false);// "Specified view is attached to another frame buffer, you must detach it before reusing it"
			return;
		}
		// the case specified v is attached to another channel
		if (getRenderColorView(elem) != NULL)
		{
			assert(false);//"Specified attachement is already in use, you must detach it firstly"
			return;
		}
		// if this view has been attached to this frame buffer, change its channel.
		if (v->getAttachedFrameBuffer() == this)
		{
			detachRenderColorView(v->getAttachPoint());
		}

		// chech format compatibility
		if (DPixelFormatTool::getFormatBits(v->getFormat()) != mColorBits)
		{
			return;
		}
		D3D9RenderColorView* rv = v->getAs<D3D9RenderColorView>(false);
		// check type availability.
		if (rv == NULL)
		{
			return;
		}
		rv->resize(mWidth, mHeight);
		mViewList[(uint32)(elem)] = rv;
		rv->setAttachFrameBuffer(this);
		rv->setAttachElement(elem);	
	}

	void D3D9FrameBuffer::detachRenderColorView( ElementAttachment elem )
	{
		if (mViewList[(uint32)(elem)] != NULL)
		{
			mViewList[(uint32)(elem)]->setAttachFrameBuffer(NULL);
			mViewList[(uint32)(elem)]->setAttachElement(EA_Color0);
			mViewList[(uint32)(elem)] = NULL;
		}
	}

	void D3D9FrameBuffer::detachAllRenderColorViews()
	{
		for (uint32 i = 0; i < 8; ++i)
		{
			mViewList[i] = NULL;
		}
	}

	void D3D9FrameBuffer::attachRenderDepthStencilView( DRenderDepthStencilView* v )
	{
		if (v == NULL)
		{
			assert(false);//"Can't attach an empty depth stencil view"
			return;
		}
		if (v->getAttachedFrameBuffer() != NULL && v->getAttachedFrameBuffer() != this)
		{
			assert(false);//"Specified view is attached to another frame buffer, you must detach it before reusing it"
			return;
		}
		D3D9RenderDepthStencilView* rv = v->getAs<D3D9RenderDepthStencilView>(false);
		// check type availability.
		if (rv == NULL)
		{
			return;
		}
		detachRenderDepthStencilView();
		rv->resize(mWidth, mHeight);
		mCurDepthView = rv;
		mCurDepthView->setAttachFrameBuffer(this);
	}

	void D3D9FrameBuffer::detachRenderDepthStencilView()
	{
		if(mCurDepthView == NULL)
		{
			return;
		}
		mCurDepthView->setAttachFrameBuffer(NULL);
		mCurDepthView = NULL;
	}

	DRenderColorView* D3D9FrameBuffer::getRenderColorView( ElementAttachment elem )
	{
		assert((uint32)elem < mViewList.size());
		return mViewList[elem];
	}

	DRenderDepthStencilView* D3D9FrameBuffer::getRenderDepthStencilView()
	{
		return mCurDepthView;
	}


	void D3D9FrameBuffer::clear( uint32 flags, const DColor& clr, DReal depth, int32 stencil )
	{

		D3D9RenderSystem* d3dRSys = DCore::getSingleton().getRenderSystem()->getAs<D3D9RenderSystem>();
		
		d3dRSys->clearFrameBuffer(this, flags, clr, depth, stencil);

	}

}