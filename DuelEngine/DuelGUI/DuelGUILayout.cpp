//  [7/1/2013 OMEGA] created

#include "DuelGUICommon.h"
#include "DuelCommon.h"
#include "DuelGUILayout.h"
#include "DuelGUILayoutItem.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DGLayout, DGLayoutItem);

	DGLayout::DGLayout() :
		mParentWidget(NULL),
		mbEnabled(true),
		mbDirty(true),
		mMargin(0,0,0,0),
		mConstrain(LSC_Default),
		mSpacing(0)
	{

	}
	 
	DGLayout::DGLayout( DGWidget* w ) :
		mParentWidget(w),
		mbEnabled(true),
		mbDirty(true),
		mMargin(0,0,0,0),
		mConstrain(LSC_Default),
		mSpacing(0)
	{
		if (w)
		{
			w->setLayout(this);
		}
	}

	bool DGLayout::isEnable()
	{
		return mbEnabled;
	}

	void DGLayout::setEnable( bool flag )
	{
		mbEnabled = flag;
	}


	const DGMargin& DGLayout::getContentMargin()
	{
		return mMargin;
	}

	void DGLayout::setContentMargin(const DGMargin& m)
	{
		mMargin = m;
	}

	/*!
	Returns whether this layout can make use of more space than
	sizeHint(). A value of Vertical or Horizontal means that
	it wants to grow in only one dimension, whereas Vertical |
	Horizontal means that it wants to grow in both dimensions.

	The default implementation returns Horizontal | Vertical.
	Subclasses reimplement it to return a meaningful value based on
	their child widgets's \l{SizePolicy}{size policies}.
	*/
	Duel::GUIOrientation DGLayout::getExpandingDirections()
	{
		return GO_Both;
	}
	

	DGWidget* DGLayout::getParentWidget()
	{
		return mParentWidget;
	}

	void DGLayout::setSpacing( uint32 s )
	{
		mSpacing = s;
	}

	Duel::uint32 DGLayout::getSpacing() const
	{
		return mSpacing;
	}

	void DGLayout::setSizeConstrain( LayoutSizeConstraint c )
	{
		mConstrain = c;
	}

	Duel::LayoutSizeConstraint DGLayout::getSizeConstrain()
	{
		return mConstrain;
	}

	void DGLayout::invalidate()
	{
		mGeometry.setCenter(DVector2(0.0f, 0.0f));
		mGeometry.setSize(DGSize(0.0f, 0.0f));
		mbDirty = true;
	}

	void DGLayout::setGeometry( const DGGeometry& r )
	{
		mGeometry = r;
	}

	const DGGeometry& DGLayout::getGeometry()
	{
		return mGeometry;
	}

	bool DGLayout::isEmpty() const
	{
		int i = 0;
		DGLayoutItem* item = getItemAt(i);
		while (item) {
			if (!item->isEmpty())
				return false;
			++i;
			item = getItemAt(i);
		}
		return true;
	}
	
	/*!
		Returns the minimum size of this layout. This is the smallest
		size that the layout can have while still respecting the
		specifications.

		The default implementation allows unlimited resizing.
	*/
	DGSize DGLayout::getMinimumSize() 
	{		
		return DGSize(0.0f, 0.0f);
	}

	/*!
	Returns the maximum size of this layout. This is the largest size
	that the layout can have while still respecting the
	specifications.


	The default implementation allows unlimited resizing.
	*/
	Duel::DGSize DGLayout::getMaximumSize() 
	{
		return DGSize(DGLAYOUTSIZE_MAX, DGLAYOUTSIZE_MAX);
	}

	void DGLayout::addChildLayout( DGLayout * l )
	{
		l->mParentWidget = mParentWidget;
	}

	void DGLayout::addChildWidget( DGWidget * w )
	{
		w->setParent(mParentWidget);
	}

	/*!
    Returns the rectangle that should be covered when the geometry of
    this layout is set to \a r, provided that this layout supports
    setAlignment().

    The result is derived from sizeHint() and expanding(). It is never
    larger than \a r.
	*/
// 	Duel::IRect DGLayout::getAlignmentRect( const IRect & r ) const
// 	{
// 		DGSize s = getSizeHint();
// 		AlignmentFlag a = AF_None;
// 
// 		/*
// 		  This is a hack to obtain the real maximum size, not
// 		  DGSize(DGLayoutSIZE_MAX, DGLayoutSIZE_MAX), the value consistently
// 		  returned by DGLayoutItems that have an alignment.
// 		*/
// 		DGLayout *that = const_cast<DGLayout *>(this);
// 		that->setAlignment(AF_None);
// 		DGSize ms = that->getMaximumSize();
// 		that->setAlignment(a);
// 		GUIOrientation	orientation = getExpandingDirections();
// 		if ((orientation & GO_Horizontal) ||
// 			!(a & AF_HorizontalMask)) 
// 		{
// 			s.setWidth(DMath::Min(r.width, ms.getWidth()));
// 		}
// 		if ((orientation & GO_Vertical) ||
// 			!(a & AF_VerticalMask)) 
// 		{
// 			s.setHeight(DMath::Min(r.height, ms.getHeight()));
// 		}
// 		else if (hasHeightForWidth()) 
// 		{
// 			int32 hfw = getHeightForWidth(s.getWidth());
// 			if (hfw < s.getHeight())
// 			{
// 				s.setHeight(DMath::Min(hfw, ms.getHeight()));
// 			}
// 		}
// 		
// 		s = s.boundedTo(DGSize(r.width, r.height));
// 		int32 x = r.left;
// 		int32 y = r.top;
// 		
// 		if (a & AF_Bottom)
// 		{
// 			y += (r.height - s.getHeight());
// 		}
// 		else if (!(a & AF_Top))
// 		{
// 			y += (r.height - s.getHeight()) / 2;
// 		}
// 
// 		if (a & AF_Right)
// 		{
// 			x += (r.width - s.getWidth());
// 		}
// 		else if (!(a & AF_Left))
// 		{
// 			x += (r.width - s.getWidth()) / 2;
// 		}
// 		
// 		return IRect(x, y, s.getWidth(), s.getHeight());
// 	}

}