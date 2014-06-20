//  [7/1/2013 OMEGA] created

#ifndef _DUELGUILAYOUT_H_
#define _DUELGUILAYOUT_H_

#include "DuelCommon.h"
#include "DuelGUICommon.h"
#include "DuelGUIWidget.h"
#include "DuelGUILayoutItem.h"
#include "DuelGUISizeAdjustment.h"

namespace Duel
{
	/*
		here minimumSize() and maximumSize() refer to the layout's functions.
		Constant			value	Description
		LSC_Default			0		The main widget's minimum size is set to minimumSize(), unless the widget already has a minimum size(i.e, size with width and height bigger than 0.0f).
		LSC_NoConstrain		1		The widget is not constrained(i.e, main widget's geometry will no be modified by the layout).
		LSC_Minimum			2		The main widget's minimum size is set to minimumSize(); it cannot be smaller.
		LSC_Fixed			3		The main widget's size is set to sizeHint(); it cannot be resized at all.
		LSC_Maximum			4		The main widget's maximum size is set to maximumSize(); it cannot be larger.
		LSC_MinimumMaximum	5		The main widget's minimum size is set to minimumSize() and its maximum size is set to maximumSize().
	*/

	enum LayoutSizeConstraint {
		LSC_Default			= 0,	
		LSC_NoConstrain		= 1,	
		LSC_Minimum			= 2,	
		LSC_Fixed			= 3,	
		LSC_Maximum			= 4,	
		LSC_MinimumMaximum	= 5,	
	};

	class DUELGUI_API DGLayout : public DGLayoutItem
	{
	DUEL_DECLARE_RTTI(DGLayout)
	public:
		enum Direction
		{ 
			D_LeftToRight, 
			D_RightToLeft, 
			D_TopToBottom, 
			D_BottomToTop
		};
		// Constructs a new child QLayout.
		// This layout has to be inserted into another layout before geometry management will work.
		DGLayout();
		// Constructs a new top-level QLayout, with parent parent. parent may not be 0.
		// There can be only one top-level layout for a widget.
		DGLayout(DGWidget* w);
		virtual ~DGLayout() {}


		// Enables this layout if enable is true, otherwise disables it.
		// An enabled layout adjusts dynamically to changes; a disabled layout acts as if it did not exist.
		// By default all layouts are enabled.
		bool					isEnable();
		void					setEnable(bool flag);
		
		// Implemented in subclasses to add an item. How it is added is specific to each subclass.
		virtual	void			addItem(DGLayoutItem* item) = 0;
		// Removes the layout  item from the layout. It is the caller's responsibility to delete the item.
		virtual	void			removeItem(DGLayoutItem* item) = 0;
		// Adds widget w to this layout in a manner specific to the layout. This function uses addItem().
		virtual	void			addWidget(DGWidget* w) = 0;
		// Removes the widget item from the layout. It is the caller's responsibility to delete the item.
		virtual	void			removeWidget(DGWidget* w) = 0;
		// get/set the margin used around the layout.
		const DGMargin&			getContentMargin();
		void					setContentMargin(const DGMargin& m);
		// Searches for widget widget in this layout (not including child layouts).
		// Returns the index of widget, or -1 if widget is not found.
		virtual	int32			getIndexOf(DGWidget* w) = 0;
		


		// Must be implemented in subclasses to return the number of items in the layout.
		virtual	uint32			getItemCount() const = 0;
		// Must be implemented in subclasses to return the layout item at index. 
		// If there is no such item, the function must return 0. Items are numbered consecutively from 0. If an item is deleted, other items will be renumbered.
		virtual	DGLayoutItem*	getItemAt(uint32 index) const  = 0;
		// Must be implemented in subclasses to remove the layout item at index from the layout, and return the item.
		// If there is no such item, the function must do nothing and return 0. Items are numbered consecutively from 0. If an item is removed, other items will be renumbered.
		virtual	DGLayoutItem*	takeItemAt(uint32 index) = 0;

		// Returns the parent widget of this layout, or 0 if this layout is not installed on any widget.
		DGWidget*				getParentWidget();

		// get/set spacing between widgets inside the layout.
		virtual	void			setSpacing(uint32 s);
		virtual	uint32			getSpacing() const;

		// this property holds the resize mode of the layout.
		void					setSizeConstrain(LayoutSizeConstraint c);
		LayoutSizeConstraint	getSizeConstrain();
		// updates the layout for parentWidget().
		// you should generally not need to call this because it is automatically called at the most appropriate times.
		virtual	void			update() {}
		//////////////////////////////////////////////////////////////////////////
		// override : DGLayoutItem---------------------
		virtual GUIOrientation	getExpandingDirections();
		virtual	void			invalidate();
		virtual void			setGeometry(const DGGeometry& g);
		virtual	const DGGeometry&	getGeometry();
		virtual	bool			isEmpty() const;
		virtual	DGSize			getMinimumSize();
		virtual	DGSize			getMaximumSize();
		//////////////////////////////////////////////////////////////////////////

	protected:
		// This function is called from addLayout() or insertLayout() functions in subclasses to add layout l as a sub-layout.
		// The only scenario in which you need to call it directly is if you implement a custom layout that supports nested layouts.
		// NOTICE : the layout prepared to be added must have no parent.
		void					addChildLayout( DGLayout * l );
		// This function is called from addWidget() functions in subclasses to add w as a managed widget of a layout.
		void					addChildWidget( DGWidget * w );
		// Returns the rectangle that should be covered when the geometry of this layout is set to r, provided that this layout supports setAlignment().
		// The result is derived from sizeHint() and expanding(). It is never larger than r.
// 		IRect					getAlignmentRect( const IRect & r ) const;


		// parent widget of this layout.
		DGWidget*				mParentWidget;
		// indicate whether this layout has been redone.
		bool					mbDirty;
		DGGeometry				mGeometry;
		// An enabled layout adjusts dynamically to changes; a disabled layout acts as if it did not exist.
		bool					mbEnabled;
		// layout size mode.
		LayoutSizeConstraint	mConstrain;
		DGMargin				mMargin;

		uint32					mSpacing;
	};



}

#endif