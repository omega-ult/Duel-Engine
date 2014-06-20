//  [6/29/2013 OMEGA] created

#ifndef _DUELGUISIZEPOLICY_H_
#define _DUELGUISIZEPOLICY_H_

#include "DuelCommon.h"
#include "DuelGUICommon.h"

namespace Duel
{

	enum GUIOrientation {
		GO_None			= 0x0,
		GO_Horizontal	= 0x1,
		GO_Vertical		= 0x2,
		GO_Both			= GO_Horizontal | GO_Vertical
	};

	/*
		Constant		Value	Description
		SPF_GrowFlag	1		The widget can grow beyond its size hint if necessary.
		SPF_ExpandFlag	2		The widget should get as much space as possible.
		SPF_ShrinkFlag	4		The widget can shrink below its size hint if necessary.
		SPF_IgnoreFlag	8		The widget's size hint is ignored. The widget will get as much space as possible.
	
	*/
	enum SizePolicyFlag {
		SPF_Grow	= 0x01,
		SPF_Expanded	= 0x02,
		SPF_Shrink	= 0x04,
		SPF_Ignore	= 0x08
	};
	/*
		Constant				Value							Description
		SP_Fixed				0								The DGWidget::sizeHint() is the only acceptable alternative, so the widget can never grow or shrink (e.g. the vertical direction of a push button).
		SP_Minimum				GrowFlag						The sizeHint() is minimal, and sufficient. The widget can be expanded, but there is no advantage to it being larger (e.g. the horizontal direction of a push button). It cannot be smaller than the size provided by sizeHint().
		SP_Maximum				ShrinkFlag						The sizeHint() is a maximum. The widget can be shrunk any amount without detriment if other widgets need the space (e.g. a separator line). It cannot be larger than the size provided by sizeHint().
		SP_Preferred			GrowFlag | ShrinkFlag			The sizeHint() is best, but the widget can be shrunk and still be useful. The widget can be expanded, but there is no advantage to it being larger than sizeHint() (the default DGWidget policy).
		SP_Expanding			GrowFlag | ShrinkFlag |			The sizeHint() is a sensible size, but the widget can be shrunk and still be useful. The widget can make use of extra space, so it should get as much space as possible (e.g. the horizontal direction of a horizontal slider).
								ExpandFlag	
		SP_MinimumExpanding		GrowFlag | ExpandFlag			The sizeHint() is minimal, and sufficient. The widget can make use of extra space, so it should get as much space as possible (e.g. the horizontal direction of a horizontal slider).
		SP_Ignored				ShrinkFlag | GrowFlag | 
								IgnoreFlag
	*/

	enum SizePolicy {
		SP_Fixed				= 0,
		SP_Minimum				= SPF_Grow,
		SP_Maximum				= SPF_Shrink,
		SP_Preferred			= SPF_Grow | SPF_Shrink,
		SP_MinimumExpanding		= SPF_Grow | SPF_Expanded,
		SP_Expanding			= SPF_Grow | SPF_Shrink | SPF_Expanded,
		SP_Ignored				= SPF_Shrink | SPF_Grow | SPF_Ignore
	};

	// DGSizePolicy class is a layout attribute describing horizontal and vertical resizing policy.
	class DUELGUI_API DGSizePolicy
	{
	public:

		DGSizePolicy() : mPolicyWord(0) {}
		DGSizePolicy(uint32 word) : mPolicyWord(word) {}
		DGSizePolicy(SizePolicy horizontal, SizePolicy vertical) :
			mPolicyWord(horizontal | (vertical << SPM_HSize)) { }
		~DGSizePolicy() {}

		SizePolicy		getHorizontalPolicy() const { return static_cast<SizePolicy>(mPolicyWord & SPM_HMask); }
		SizePolicy		getVerticalPolicy() const { return static_cast<SizePolicy>((mPolicyWord & SPM_VMask) >> SPM_HSize); }

		void			setHorizontalPolicy(SizePolicy d) { mPolicyWord = (mPolicyWord & ~SPM_HMask) | d; }
		void			setVerticalPolicy(SizePolicy d) { mPolicyWord = (mPolicyWord & ~(SPM_HMask << SPM_HSize)) | (d << SPM_HSize); }

		GUIOrientation	getExpandingDirections() const {
			uint32	ret = 0;
			if (getVerticalPolicy() & (uint32)SPF_Expanded)
				ret |= GO_Vertical;
			if (getHorizontalPolicy() & (uint32)SPF_Expanded)
				ret |= GO_Horizontal;
			return (GUIOrientation)ret;
		}

		void		setHeightForWidth(bool b) { mPolicyWord = b ? (mPolicyWord | (1 << 2*SPM_HSize)) : (mPolicyWord & ~(1 << 2*SPM_HSize));  }
		bool		hasHeightForWidth() const { return (mPolicyWord & (1 << 2*SPM_HSize)) != 0; }
		void		setWidthForHeight(bool b) { mPolicyWord = b ? (mPolicyWord | (1 << (SPM_WFHShift))) : (mPolicyWord & ~(1 << (SPM_WFHShift)));  }
		bool		hasWidthForHeight() const { return (mPolicyWord & (1 << (SPM_WFHShift))) != 0; }

		bool		operator==(const DGSizePolicy& s) const { return mPolicyWord == s.mPolicyWord; }
		bool		operator!=(const DGSizePolicy& s) const { return mPolicyWord != s.mPolicyWord; }

		// sets the horizontal stretch factor of the size policy to the given stretchFactor. stretchFactor must be in the range [0,255].
		// when two widgets are adjacent to each other in a horizontal layout, 
		// setting the horizontal stretch factor of the widget on the left to 2 and the factor of widget on the right to 1
		// will ensure that the widget on the left will always be twice the size of the one on the right.
		uint8		getHorizontalStretch() const { return mPolicyWord >> 24; }
		uint8		getVerticalStretch() const { return (mPolicyWord >> 16) & 0xff; }
		void		setHorizontalStretch(uint8 stretchFactor) { mPolicyWord = (mPolicyWord&0x00ffffff) | (uint32(stretchFactor)<<24); }
		void		setVerticalStretch(uint8 stretchFactor) { mPolicyWord = (mPolicyWord&0xff00ffff) | (uint32(stretchFactor)<<16); }

		inline void transpose() {
			SizePolicy hData = getHorizontalPolicy();
			SizePolicy vData = getVerticalPolicy();
			uint8 hStretch = uint8(getHorizontalStretch());
			uint8 vStretch = uint8(getVerticalStretch());
			setHorizontalPolicy(vData);
			setVerticalPolicy(hData);
			setHorizontalStretch(vStretch);
			setVerticalStretch(hStretch);
		}


	private:
		enum SizePolicyMasks {
			SPM_HSize = 0x04,
			SPM_HMask = 0x0f,
			SPM_VMask = SPM_HMask << SPM_HSize,
			SPM_CTShift = 9,
			SPM_CTSize = 5,
			SPM_CTMask = ((0x1 << SPM_CTSize) - 1) << SPM_CTShift,
			SPM_WFHShift = SPM_CTShift + SPM_CTSize,
			SPM_UnusedShift = SPM_WFHShift + 1,
			SPM_UnusedSize = 1
		};
	private:
		uint32		mPolicyWord;
	};


}

#endif