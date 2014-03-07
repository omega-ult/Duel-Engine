//  [8/7/2013 OMEGA] created

#ifndef _DUELGUIBOXLAYOUT_H_
#define _DUELGUIBOXLAYOUT_H_

#include "DuelGUICommon.h"
#include "DuelGUILayout.h"
#include "DuelGUILayoutEngine.h"

namespace Duel
{
	struct DGLayoutStruct;

	///////////////////////////////////////////////////////////////
	// a container struct used for wraping objects in box layout.///////////
	struct DGBoxLayoutItem
	{
		DGBoxLayoutItem(DGLayoutItem* i, uint32 _stretch = 0) :
			item(i), stretch(_stretch), magic(false) {}
		~DGBoxLayoutItem() { delete item; }

		DReal hfw(DReal w)
		{
			if (item->hasHeightForWidth()) 
			{
				return item->getHeightForWidth(w);
			} 
			else 
			{
				return item->getSizeHint().getHeight();
			}
		}
		DReal mhfw(DReal w) 
		{
			if (item->hasHeightForWidth()) 
			{
				return item->getHeightForWidth(w);
			}
			else 
			{
				return item->getMinimumSize().getHeight();
			}
		}
		uint32 hStretch()
		{
			if (stretch == 0 && item->getWidget()) 
			{
				return item->getWidget()->getSizePolicy().getHorizontalStretch();
			}
			else 
			{
				return stretch;
			}
		}
		uint32 vStretch() 
		{
			if (stretch == 0 && item->getWidget())
			{
				return item->getWidget()->getSizePolicy().getVerticalStretch();
			} 
			else 
			{
				return stretch;
			}
		}

		DGLayoutItem*	item;
		uint32			stretch;
		bool			magic;
	};
	// this class lines up child widgets horizontally or vertically.
	class DUELGUI_API DGBoxLayout : public DGLayout
	{
	DUEL_DECLARE_RTTI(DGBoxLayout)
	public:
		DGBoxLayout(Direction dir, DGWidget* w = NULL);
		// set/get the direction of the box. addWidget() and addSpacing() work in this direction; 
		// the stretch stretches in this direction.
		void			setDirection(Direction dir);
		Direction		getDirection() const;

		// add a layout/spacerItem at the last.
		void			addLayout(DGLayout* layout, uint32 stretch);
		// Adds spacerItem to the end of this box layout.
		void			addSpacerItem(DGSpacerItem* item);
		// Limits the perpendicular dimension of the box (e.g. height if the box is LeftToRight)
		// to a minimum of size. Other constraints may increase the limit.
		void			addStrut(DReal size);
		// Adds widget to the end of this box layout, with a stretch factor of stretch and alignment alignment.
		// The stretch factor applies only in the direction of the DGBoxLayout, 
		// and is relative to the other boxes and widgets in this DGBoxLayout. Widgets and boxes with higher stretch factors grow more.
		// If the stretch factor is 0 and nothing else in the DGBoxLayout has a stretch factor greater than zero, 
		// the space is distributed according to the DGWidget:getSizePolicy() of each widget that's involved.
		// The alignment is specified by alignment. The default alignment is 0, which means that the widget fills the entire cell.
		void			addWidget(DGWidget* w, int32 stretch = 0, DGAlignment align = AF_None);

		// Inserts layout at position index, with stretch factor stretch. If index is negative, the layout is added at the end.
		// layout becomes a child of the box layout.
		void			insertLayout(int32 index, DGLayout* layout, uint32 stretch = 0);
		// Inserts spacerItem at position index, with zero minimum size and stretch factor. If index is negative the space is added at the end.
		void			insertSpacerItem(int32 index, DGSpacerItem* item);

		// Inserts widget at position index, with stretch factor stretch and alignment alignment. If index is negative, the widget is added at the end.
		// for more information about stretch and alignment, see addWidget();
		void			insertWidget(int32 index, DGWidget* w, uint32 stretch = 0, DGAlignment align = AF_None);



		//////////////////////////////////////////////////////////////////////////
		// override : DGLayout-------------------------
		virtual	void			addItem(DGLayoutItem* item);
		virtual	DGLayoutItem*	getItemAt(uint32 index) const;
		virtual	DGLayoutItem*	takeItemAt(uint32 index); 
		virtual	uint32			getItemCount() const;
		virtual	void			removeItem(DGLayoutItem* item);
		virtual void			removeWidget(DGWidget* w);
		virtual	void			update();
		// override : DGLayoutItem---------------------
		virtual void			setGeometry(const DGGeometry& g);
		virtual	GUIOrientation	getExpandingDirections();
		virtual	bool			hasHeightForWidth(); 
		virtual	void			invalidate();
		virtual	DGSize			getMinimumSize();
		virtual	DGSize			getMaximumSize();
		virtual	DGSize			getSizeHint();
		//////////////////////////////////////////////////////////////////////////

	protected:	
		// internal functions for supporting public actions.
		void			deleteAllItems();
		void			setupGeometry();
		void			effectiveMargins(int32 *left, int32 *top, int32 *right, int32 *bottom) const;

		// used in storing items and their geometry properties.
		typedef	std::list<DGBoxLayoutItem*>	ItemList;
		typedef	std::vector<DGLayoutStruct>	GeometryArray;
		ItemList		mItemList;
		GeometryArray	mGeoArray;

		// only indicate the geometry of this layout is invalidate, it can be
		// reset when setupGeometry() is called.
		bool			mbGeoDirty;
		int32			mHFWWidth;
		int32			mHFWHeight;
		int32			mHFWMinHeight;
		DGSize			mSizeHint;
		DGSize			mMinSize;
		DGSize			mMaxSize;
		DGMargin		mMargin;
		GUIOrientation	mExpanding;
		bool			mbHasHFW;
		Direction		mDirection;
	};
	
	class DGVBoxLayout : public DGBoxLayout
	{
	DUEL_DECLARE_RTTI(DGVBoxLayout)
	public:
		DGVBoxLayout(DGWidget* w = NULL);
	};

	class DGHBoxLayout : public DGBoxLayout
	{
	DUEL_DECLARE_RTTI(DGHBoxLayout)
	public:
		DGHBoxLayout(DGWidget* w = NULL);
	};

}


#endif