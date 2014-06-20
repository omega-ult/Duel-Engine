//  [6/29/2013 OMEGA] created

#include "DuelGUICommon.h"
#include "DuelCommon.h"
#include "DuelGUIWidget.h"
#include "DuelGUISizePolicy.h"
#include "DuelGUISize.h"
#include "DuelGUILayoutItem.h"
#include "DuelGUILayoutEngine.h"


namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DGLayoutItem, DObject);
	DUEL_IMPLEMENT_RTTI_1(DGSpacerItem, DGLayoutItem);
	DUEL_IMPLEMENT_RTTI_1(DGWidgetItem, DGLayoutItem);

	//inline	static DGGeometry	fromLayoutItemRect()

	void DGSpacerItem::changeSize( DReal w, DReal h, SizePolicy hData /*= SP_MINIMUM*/, SizePolicy vData /*= SP_MINIMUM*/ )
	{
		mWidth = w;
		mHeight = h;
		mSizePolicy = DGSizePolicy(hData, vData);
	}

	DGSize DGSpacerItem::getSizeHint() 
	{
		return DGSize(mWidth, mHeight);
	}

	DGSize DGSpacerItem::getMinimumSize() 
	{
		return DGSize(	mSizePolicy.getHorizontalPolicy() & SPF_Shrink ? 0.0f : mWidth,
						mSizePolicy.getVerticalPolicy() & SPF_Shrink ? 0.0f : mHeight);
	}

	DGSize DGSpacerItem::getMaximumSize() 
	{
		return DGSize(	mSizePolicy.getHorizontalPolicy() & SPF_Grow ? DGLAYOUTSIZE_MAX : mWidth,
						mSizePolicy.getVerticalPolicy() & SPF_Grow ? DGLAYOUTSIZE_MAX : mHeight);
	}

	GUIOrientation DGSpacerItem::getExpandingDirections()
	{
		return mSizePolicy.getExpandingDirections();
	}

	bool DGSpacerItem::isEmpty() const
	{
		return true;
	}

	void DGSpacerItem::setGeometry( const DGGeometry& r)
	{
		mGeometry = r;
		mWidth = r.getSize().getWidth();
		mHeight = r.getSize().getHeight();
	}

	const DGGeometry& DGSpacerItem::getGeometry() 
	{
		return mGeometry;
	}


	//////////////////////////////////////////////////////////////////////////
	DGSize DGWidgetItem::getSizeHint() 
	{
		DGSize s(0.0f, 0.0f);
		if (!isEmpty())
		{
			s = mWidget->getSizeHint().expandedTo(mWidget->getMinimumSize());
			s = s.boundedTo(mWidget->getMaximumSize()).expandedTo(mWidget->getMinimumSize());


			if (mWidget->getSizePolicy().getHorizontalPolicy() == SP_Ignored)
				s.setWidth(0.0f);
			if (mWidget->getSizePolicy().getVerticalPolicy() == SP_Ignored)
				s.setHeight(0.0f);
		}
		return s;
	}

	DGSize DGWidgetItem::getMinimumSize() 
	{
		if (isEmpty())
		{
			return DGSize(0.0f, 0.0f);
		}
		DGSize s = DGLayoutEngine::smartMinSize((DGWidgetItem*)this);
		return s;
	}

	DGSize DGWidgetItem::getMaximumSize() 
	{
		if (isEmpty())
		{
			return DGSize(0.0f, 0.0f);
		}
		DGSize s = DGLayoutEngine::smartMaxSize((DGWidgetItem*)this, mAlignment);

		return s;
	}

	GUIOrientation DGWidgetItem::getExpandingDirections()
	{
		if (isEmpty())
			return GO_None;
		GUIOrientation	e = mWidget->getSizePolicy().getExpandingDirections();

		    /*
			### Qt 4.0:
			If the layout is expanding, we make the widget expanding, even if
			its own size policy isn't expanding. This behavior should be
			reconsidered.
			*/
		if (mWidget->getLayout()) {
			if (mWidget->getSizePolicy().getHorizontalPolicy() & SPF_Grow
				&& (mWidget->getLayout()->getExpandingDirections() & GO_Horizontal))
			{
				e = (e == GO_Vertical ? GO_Both : GO_Horizontal);
			}
			if (mWidget->getSizePolicy().getVerticalPolicy() & SPF_Grow
				&& (mWidget->getLayout()->getExpandingDirections() & GO_Vertical))
			{
				e = (e == GO_Horizontal ? GO_Both : GO_Vertical);
			}
		}
		return e;
	}

	bool DGWidgetItem::isEmpty() const
	{
		return !mWidget->isVisible();
	}

	void DGWidgetItem::setGeometry( const DGGeometry& r )
	{
		if (isEmpty())
		{
			return;
		}
		mWidget->setGeometry(r);
	}

	const DGGeometry& DGWidgetItem::getGeometry() 
	{
		return mWidget->getGeometry();
	}

	bool DGWidgetItem::hasHeightForWidth() 
	{
		return false;
	}

	DReal DGWidgetItem::getHeightForWidth( DReal w ) 
	{
		return 0.0f;
	}

}