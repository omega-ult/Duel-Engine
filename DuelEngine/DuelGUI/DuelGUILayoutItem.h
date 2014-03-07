//  [6/29/2013 OMEGA] created


#ifndef _DUELGUILAYOUTITEM_H_
#define _DUELGUILAYOUTITEM_H_

#include "DuelCommon.h"
#include "DuelGUICommon.h"
#include "DuelGUISizePolicy.h"
#include "DuelGUIAlignment.h"
#include "DuelGUISize.h"
#include "DuelGUIGeometry.h"

namespace Duel
{
	class DGWidget;
	class DGLayout;
	class DGSpacerItem;

	// 4096
#define  DGLAYOUTSIZE_MAX (0x1000)

	class DGSpacerItem;
	class DGWidgetItem;
	// base class for layout.
	class DUELGUI_API DGLayoutItem : public DObject
	{
	DUEL_DECLARE_RTTI(DGLayoutItem)
	public:
		explicit DGLayoutItem(DGAlignment alignment = AF_Left) : mAlignment(alignment) {}
		virtual ~DGLayoutItem() {}
		// subclass must implements these methods
		// return the preferred size of this item. Implemented in subclasses.
		virtual	DGSize			getSizeHint() = 0;
		// return the minimum size of this item.
		virtual	DGSize			getMinimumSize() = 0;
		// return the maximum size of this item.
		virtual	DGSize			getMaximumSize() = 0;
		// returns whether this layout item can make use of more space than sizeHint().
		virtual GUIOrientation	getExpandingDirections()  = 0;
		// get/set the rectangle covered by this layout item.
		// NOTICE : you should not call it directly ! it will cause incorrect result.
		virtual void			setGeometry(const DGGeometry& r) = 0;
		virtual	const DGGeometry&	getGeometry() = 0;
		// implemented in subclasses to return whether this item is empty, i.e. whether it contains any widgets.
		virtual bool			isEmpty() const = 0;

		// returns true if this layout's preferred height depends on its width; otherwise returns false. The default implementation returns false.
		virtual bool			hasHeightForWidth() { return false; }
		// returns the preferred height for this layout item, given the width w.
		virtual DReal			getHeightForWidth(DReal w) { return 0.0f; }
		// returns the minimum height this widget needs for the given width, w. The default implementation simply returns heightForWidth(w).
		//virtual DReal			getMinimumHeightForWidth(DReal w) const { return getHeightForWidth(w); }
		// invalidates any cached information in this layout item.
		virtual void			invalidate() {}

		// get/set the alignment of this item to alignment
		DGAlignment				getAlignment() const { return mAlignment; }
		void					setAlignment(DGAlignment a) { mAlignment = a; }

		// If this item is a DGSpacerItem, it is returned as a DGSpacerItem; otherwise 0 is returned.
		// This function provides type-safe casting.
		virtual DGSpacerItem*	getSpacerItem() { return 0; }
		// If this item is a DGWidget, it is returned as a DGWidget; otherwise 0 is returned.
		// This function provides type-safe casting.
		virtual DGWidget*		getWidget() { return 0; }


	protected:
		DGAlignment				mAlignment;
	};

	//////////////////////////////////////////////////////////////////////////
	// normally you don't need to use this class directly, 
	// layout managers provide the following functions for manipulating empty space in layouts:
	// DGHBoxLayout: addSpacing(), addStretch(), insertSpacing(), insertStretch() 
	// DGGridLayout: setRowMinimumHeight(), setRowStretch(), setColumnMinimumWidth(), setColumnStretch() 
	class DUELGUI_API DGSpacerItem : public DGLayoutItem
	{
	DUEL_DECLARE_RTTI(DGSpacerItem)
	public:
		explicit DGSpacerItem(DReal w, DReal h,
			SizePolicy hData = SP_Minimum,
			SizePolicy vData = SP_Minimum) : mWidth(w), mHeight(h),mSizePolicy(hData, vData) {}
		virtual ~DGSpacerItem() {}

		void			changeSize(DReal w, DReal h,
									SizePolicy hData = SP_Minimum,
									SizePolicy vData = SP_Minimum);
		// override : DGLayoutItem-------------------------------
		DGSize			getSizeHint();
		// override : DGLayoutItem-------------------------------
		DGSize			getMinimumSize();
		// override : DGLayoutItem-------------------------------
		DGSize			getMaximumSize();
		// override : DGLayoutItem-------------------------------
		GUIOrientation	getExpandingDirections();
		// override : DGLayoutItem-------------------------------
		bool			isEmpty() const;
		// override : DGLayoutItem-------------------------------
		void			setGeometry(const DGGeometry& r);
		// override : DGLayoutItem-------------------------------
		const DGGeometry&	getGeometry();
		// override : DGLayoutItem-------------------------------
		virtual	DGSpacerItem*	getSpacerItem() { return this; }

	private:
		DReal			mWidth;
		DReal			mHeight;
		DGSizePolicy	mSizePolicy;
		DGGeometry		mGeometry;
	};
	//////////////////////////////////////////////////////////////////////////
	// The QWidgetItem class is a layout item that represents a widget.
	// Normally, you don't need to use this class directly.  layout managers provide
	// the following functions for manipulating widgets in layouts:
	// DGBoxLayout: addWidget(), insertWidget(), setStretchFactor() 
	// DGGridLayout: addWidget() 
	// DGStackedLayout: addWidget(), insertWidget(), currentWidget(), setCurrentWidget(), widget() 
	class DUELGUI_API DGWidgetItem : public DGLayoutItem
	{
	DUEL_DECLARE_RTTI(DGWidgetItem)
	public:
		explicit DGWidgetItem(DGWidget* w) : mWidget(w) {}
		virtual ~DGWidgetItem() {}
		// override : DGLayoutItem-------------------------------
		DGSize			getSizeHint();
		// override : DGLayoutItem-------------------------------
		DGSize			getMinimumSize();
		// override : DGLayoutItem-------------------------------
		DGSize			getMaximumSize();
		// override : DGLayoutItem-------------------------------
		GUIOrientation	getExpandingDirections() ;
		// override : DGLayoutItem-------------------------------
		bool			isEmpty() const;
		// override : DGLayoutItem-------------------------------
		void			setGeometry(const DGGeometry& r);
		// override : DGLayoutItem-------------------------------
		const DGGeometry&	getGeometry();
		// override : DGLayoutItem-------------------------------
		bool			hasHeightForWidth();
		// override : DGLayoutItem-------------------------------
		DReal			getHeightForWidth(DReal w);
		// override : DGLayoutItem-------------------------------
		virtual DGWidget*		getWidget() { return mWidget; }
	private:
		DGWidget*		mWidget;
	};


}

#endif