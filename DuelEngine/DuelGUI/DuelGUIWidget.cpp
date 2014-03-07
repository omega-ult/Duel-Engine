//  [7/15/2013 OMEGA] created

#include "DuelGUICommon.h"
#include "DuelCommon.h"
#include "DuelRenderQueue.h"
#include "DuelGUIHotSpot.h"
#include "DuelGUIRenderDelegate.h"
#include "DuelGUIWidget.h"
#include "DuelGUISizePolicy.h"
#include "DuelGUIManager.h"
#include "DuelGUILayout.h"
#include "DuelNode.h"


namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DGWidget, DObject);

	DGWidget::DGWidget( DGWidget* parent ) :
		mbEnable(true),
		mbVisible(true),
		mManager(NULL),
		mLayout(NULL),
		mMinimumSize(0,0),
		mSizeHint(50,50),
		mMaximumSize(DGWIDGETSIZE_MAX,DGWIDGETSIZE_MAX),
		mCustomHotspot(NULL),
		mDefaultHotspot(NULL),
		mCustomRenderDelegate(NULL),
		mDefaultRenderDelegate(NULL),
		mbLayoutDirty(true),
		mParent(NULL)
	{
		mLocatorNode.addChild(&mTransFormNode);
		setParent(parent);
	}

	DGWidget::~DGWidget()
	{
		if (mManager)
		{
			mManager->removeWidget(this);
		}
		if (mParent)
		{
			// 解除关系.
			mParent->mChildren.remove(this);
			mParent->mTransFormNode.removeChild(&mLocatorNode);
			if (mParent->getLayout())
			{
				mParent->getLayout()->removeWidget(this);
			}
			ChildrenWidgetList::iterator i, iend = mChildren.end();
			for (i = mChildren.begin(); i != iend; ++i)
			{
				(*i)->setParent(NULL);
			}
		}
	}

	DGSizePolicy DGWidget::getSizePolicy() const
	{
		return mSizePolicy;
	}

	void DGWidget::setSizePolicy( DGSizePolicy policy )
	{
		mSizePolicy = policy;
	}

	DGSize DGWidget::getSizeHint() const
	{
		return mSizeHint;
	}
	DGSize DGWidget::getMinimumSize() const
	{
		// 考虑到layout的constrain功能, 这里尝试使用layout的计算方法.
		// see LayoutSizeConstraint
		if (mLayout)
		{
			switch (mLayout->getSizeConstrain())
			{
			case LSC_Default:
				{
					if (mMinimumSize.getWidth() > 0.0f && mMinimumSize.getHeight() > 0.0f)
					{
						return mMinimumSize;
					}
					return mLayout->getMinimumSize();
				}
				break;
			case LSC_NoConstrain:
				return mMinimumSize;
				break;
			case LSC_Minimum:
				return mLayout->getMinimumSize();
				break;
			case LSC_Fixed:
				return mLayout->getMinimumSize();
				break;
			case LSC_Maximum:
				return mMinimumSize;
				break;
			case LSC_MinimumMaximum:
				return mLayout->getMinimumSize();
				break;
			default:
				break;
			}
			// never reached.
			return mLayout->getMinimumSize();
		}
		return mMinimumSize;
	}

	DGSize DGWidget::getMaximumSize() const
	{
		// 考虑到layout的constrain功能, 这里尝试使用layout的计算方法.
		// see LayoutSizeConstraint
		if (mLayout)
		{
			switch (mLayout->getSizeConstrain())
			{
			case LSC_Default:
				return mMaximumSize;
				break;
			case LSC_NoConstrain:
				return mMaximumSize;
				break;
			case LSC_Minimum:
				return mMaximumSize;
				break;
			case LSC_Fixed:
				return mLayout->getMaximumSize();
				break;
			case LSC_Maximum:
				return mLayout->getMaximumSize();
				break;
			case LSC_MinimumMaximum:
				return mLayout->getMaximumSize();
				break;
			default:
				break;
			}
			// never reached.
			return mLayout->getMaximumSize();
		}
		return mMaximumSize;
	}


	bool DGWidget::processEvent( const DEventBase* e, uint64 timeStamp )
	{
		// 从这里分发消息给子窗口
		// 先交给子窗口处理.
		bool childProcessed = false;
		ChildrenWidgetList::iterator wi, wiend = mChildren.end();
		for (wi = mChildren.begin(); wi != wiend; ++wi)
		{
			childProcessed = (*wi)->processEvent(e, timeStamp);
		}
		// 如果子窗口已经处理了. 那么返回吧.
		if (childProcessed)
		{
			return childProcessed;
		}

			if (mCustomHotspot)
		{
			return mCustomHotspot->response(e);
			// TODO: 在这里加入事件逻辑:
		}
		else if(mDefaultHotspot)
		{
			return mDefaultHotspot->response(e);
		}
			return false;
	}


	void DGWidget::updateLayout()
	{
		if (mbLayoutDirty)
		{
			if (mLayout)
			{
				// layout会处理这个widget的大小.
				mLayout->update();
			}
			else
			{
				DGSize finalSize(mSizeHint.getWidth(), mSizeHint.getHeight());
				finalSize = finalSize.boundedTo(mMaximumSize);
				finalSize = finalSize.expandedTo(mMinimumSize);
				mGeometry.setSize(finalSize);
				setGeometry(mGeometry);
				mSizeHint = mGeometry.getSize();
			}
		}
		//  [11/5/2013 OMEGA]
		// 现在改为先由父对象计算框架, 然后将分配的空间交给子对象.
		ChildrenWidgetList::iterator i, iend = mChildren.end();
		for(i = mChildren.begin(); i != iend; ++i)
		{
			(*i)->updateLayout();	
		}
	}

	void DGWidget::setParent( DGWidget* p )
	{
		if (mParent == p)
		{
			return;
		}
		if (mParent)
		{
			mParent->mTransFormNode.removeChild(&mLocatorNode);
		}
		mParent = p;
		if (p)
		{
			// if this widget belongs to a gui manager directly,
			// remove it,
			if (mManager)
			{
				mManager->removeWidget(this);
			}
			p->mChildren.push_back(this);
			p->mTransFormNode.addChild(&mLocatorNode);
		}
	}

	void DGWidget::setLayout( DGLayout* layout )
	{
		mLayout = layout;
		requestLayoutUpdate();
	}

	DGLayout* DGWidget::getLayout()
	{
		return mLayout;
	}

	void DGWidget::move( DReal relativeX, DReal relativeY )
	{
		mGeometry.setCenter(mGeometry.getCenter() + DVector2(relativeX, relativeY));
		mLocatorNode.setRelativePosition(DVector3(mGeometry.getCenter(), 0.0f));
		signalMoved(this);
	}

	void DGWidget::moveTo( DReal absX, DReal absY )
	{
		DVector2 p(absX, absY);
		mGeometry.setCenter(DVector2(absX, absY));
		if (p != mGeometry.getCenter())
		{
			signalMoved(this);
		}
		mLocatorNode.setRelativePosition(DVector3(mGeometry.getCenter(), 0.0f));
	}


	void DGWidget::resize( DReal w, DReal h )
	{
		// send message to self.
		mSizeHint.setWidth(w);
		mSizeHint.setHeight(h);
		// 留给layout决定是否重新计算.
		requestLayoutUpdate();
	}

	bool DGWidget::isMinimized() const
	{
		return (mGeometry.getSize() == mMinimumSize);
	}

	bool DGWidget::isMaximized() const
	{
		return (mGeometry.getSize() == mMaximumSize);
	}
	
	DNode* DGWidget::getTransformNode()
	{
		return &mTransFormNode;
	}

	void DGWidget::setMinimumSize( const DGSize& s )
	{
		DReal minW;
		DReal minH;
		DGSize layoutSize(0.0f, 0.0f);
		if (mLayout)
		{
			layoutSize = mLayout->getMinimumSize();
		}
		mMinimumSize = s;
		mMinimumSize = mMinimumSize.expandedTo(layoutSize);
		mMaximumSize = mMaximumSize.expandedTo(mMinimumSize);
		if (getWidth() < mMinimumSize.getWidth() || getHeight() < mMinimumSize.getHeight())
		{
			minW = DMath::Max<DReal>(getWidth(), mMinimumSize.getWidth());
			minH = DMath::Max<DReal>(getHeight(), mMinimumSize.getHeight());
			resize(minW, minH);
		}
	}

	void DGWidget::setMaximumSize( const DGSize& s )
	{
		DGSize layoutSize(0, 0);
		if (mLayout)
		{
			layoutSize = mLayout->getMaximumSize();
		}
		mMaximumSize = s.expandedTo(layoutSize);
		mMaximumSize = mMaximumSize.expandedTo(mMinimumSize);

		if (getWidth() > mMaximumSize.getWidth() || getHeight() > mMaximumSize.getHeight())
		{
			DReal minW = DMath::Min<DReal>(getWidth(), mMinimumSize.getWidth());
			DReal minH = DMath::Min<DReal>(getHeight(), mMinimumSize.getHeight());
			resize(minW, minH);
		}
	}
	
	DGUIManager* DGWidget::getRootManager()
	{
		DGWidget* root = getRootWidget();
		if (root)
		{
			return root->mManager;
		}
		return NULL;
	}

	void DGWidget::applyToRenderQueue( DRenderQueue* queue, uint32 groupID )
	{
		if (mCustomRenderDelegate)
		{
			mCustomRenderDelegate->applyToRenderQueue(queue, groupID);
		}
		else if (mDefaultRenderDelegate)
		{
			mDefaultRenderDelegate->applyToRenderQueue(queue, groupID);
		}
		ChildrenWidgetList::iterator wi, wiend = mChildren.end();
		for (wi = mChildren.begin(); wi != wiend; ++wi)
		{
			(*wi)->applyToRenderQueue(queue, groupID);
		}
	}

	DGWidget* DGWidget::getParentWidget()
	{
		return mParent; 
	}

	DGWidget* DGWidget::getRootWidget()
	{
		if (mParent == NULL)
		{
			return this;
		}
		DGWidget* p = mParent;
		while (p)
		{
			if (p->mParent == NULL)
			{
				return p;
			}
			p = mParent->mParent;
		}
		return p;
	}

	bool DGWidget::isRootWidget()
	{
		return (getRootWidget() == this) ;
	}

	void DGWidget::requestLayoutUpdate()
	{
		mbLayoutDirty = true;
		DGWidget* parent = mParent;
		while (parent)
		{
			parent->requestLayoutUpdate();
			parent = parent->mParent;
		}
	}

	void DGWidget::updateComponent()
	{
		// update children firstly.
		ChildrenWidgetList::iterator i, iend = mChildren.end();
		for(i = mChildren.begin(); i != iend; ++i)
		{
			(*i)->updateComponent();	
		}
		if (mCustomHotspot)
		{
			mCustomHotspot->update();
		}
		else if (mDefaultHotspot)
		{
			mDefaultHotspot->update();
		}

		if (mCustomRenderDelegate)
		{
			mCustomRenderDelegate->update();
		}
		else if (mDefaultRenderDelegate)
		{
			mDefaultRenderDelegate->update();
		}
	}

	DVector2 DGWidget::getCenter()
	{
		return mGeometry.getCenter();
	}

	INTERNAL void DGWidget::setGeometry( const DGGeometry& g )
	{
		DGSize sz = mGeometry.getSize();
		mGeometry = g;
		if (sz != g.getSize())
		{
			signalResized(this);
		}
		DVector2 cent = mGeometry.getCenter();
		moveTo(cent.x, cent.y);
	}

	void DGWidget::getClippedDistance(DReal& L, DReal& R , DReal& T, DReal& B)
	{
		DGWidget* parent = getParentWidget();
		// 累计的最大的clip值.
		DReal maxL;
		DReal maxR;
		DReal maxT;
		DReal maxB;
		maxL = maxR = maxT = maxB = 0.0f;
		// 这里的算法是这样: 根据子空间在每个父空间中的相对位置, 计算出被每个父物件clip掉的距离, 求其最大值, 最后即为总clip距离.
		DVector2 offsetCent = mGeometry.getCenter();
		// halfSize即为宽高的"半径"
		DGSize halfSelfSize = DGSize(mGeometry.getSize().getWidth() * 0.5f, mGeometry.getSize().getHeight() * 0.5f);
		while (parent)
		{
			DGSize halfPSize = DGSize(parent->getGeometry().getSize().getWidth() * 0.5f, 
									parent->getGeometry().getSize().getHeight() * 0.5f);
			DReal tmpL = (-offsetCent.x + halfSelfSize.getWidth()) - halfPSize.getWidth();
			DReal tmpR = (offsetCent.x + halfSelfSize.getWidth()) - halfPSize.getWidth();
			DReal tmpT = (offsetCent.y + halfSelfSize.getHeight()) - halfPSize.getHeight();
			DReal tmpB = (-offsetCent.y + halfSelfSize.getHeight()) - halfPSize.getHeight();
			maxL = DMath::Max<DReal>( tmpL, maxL );
			maxR = DMath::Max<DReal>( tmpR, maxR );
			maxT = DMath::Max<DReal>( tmpT, maxT );
			maxB = DMath::Max<DReal>( tmpB, maxB );

			// 取得其在父空间中的绝对位置, 然后纳入下次计算.
			DVector2 pCent = parent->getGeometry().getCenter();
			offsetCent += pCent;
			parent = parent->getParentWidget();
		}
		T = maxT;
		B = maxB;
		L = maxL;
		R = maxR;
	}

	bool DGWidget::isFocused()
	{
		DGUIManager* mgr = getRootManager();
		if (mgr != NULL)
		{
			if (mgr->getFocusWidget() == this)
			{
				return true;
			}
		}
		return false;
	}

	DGSize DGWidget::getHostWindowSize()
	{
		DGSize ret(0.0f, 0.0f);
		DGUIManager* mgr = getRootManager();
		if (mgr)
		{
			ret.setWidth((DReal)mgr->getTargetWindow()->getWidth());
			ret.setHeight((DReal)mgr->getTargetWindow()->getHeight());
		}
		return ret;
	}

}