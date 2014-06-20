//  [11/20/2013 OMEGA] created

#ifndef _DUELGUIALIGNMENT_H_
#define _DUELGUIALIGNMENT_H_

#include "DuelGUICommon.h"

namespace Duel
{
		// implementation from Qt 4.8.3
	/*
		The horizontal flags are:
		Constant			Value	Description
		AF_AlignLeft		0x0001	Aligns with the left edge.
		AF_AlignRight		0x0002	Aligns with the right edge.
		AF_AlignHCenter		0x0004	Centers horizontally in the available space.
		AF_AlignJustify		0x0008	Justifies the text in the available space.
		
		The vertical flags are:
		Constant			Value	Description
		AF_AlignTop			0x0020	Aligns with the top.
		AF_AlignBottom		0x0040	Aligns with the bottom.
		AF_AlignVCenter		0x0080	Centers vertically in the available space.

		You can use only one of the horizontal flags at a time. There is one two-dimensional flag:
		Constant			Value						Description
		AF_AlignCenter		AlignVCenter | AlignHCenter	Centers in both dimensions.
		
		You can use at most one horizontal and one vertical flag at a time. AF_AlignCenter counts as both horizontal and vertical.
		Three enum values are useful in applications that can be run in right-to-left mode:
		Constant			Value		Description
		AF_AlignAbsolute	0x0010		If the widget's layout direction is AF_RightToLeft (instead of AF_LeftToRight, the default), AF_AlignLeft refers to the right edge and AF_AlignRight to the left edge. This is normally the desired behavior. If you want AF_AlignLeft to always mean "left" and AF_AlignRight to always mean "right", combine the flag with AF_AlignAbsolute.
		AF_AlignLeading		AlignLeft	Synonym for AF_AlignLeft.
		AF_AlignTrailing	AlignRight	Synonym for AF_AlignRight.
	
	*/
	enum AlignmentFlag
	{
		AF_None					= 0X0000,
		AF_Left					= 0x0001,
		AF_Leading				= AF_Left,
		AF_Right				= 0x0002,
		AF_Trailing				= AF_Right,
		AF_HCenter				= 0x0004,
		AF_Justify				= 0x0008,
		AF_Absolute				= 0x0010,
		AF_HorizontalMask		= AF_Left | AF_Right | AF_HCenter | AF_Justify | AF_Absolute,

		AF_Top					= 0x0020,
		AF_Bottom				= 0x0040,
		AF_VCenter				= 0x0080,
		AF_VerticalMask			= AF_Top | AF_Bottom | AF_VCenter,

		AF_Center				= AF_VCenter | AF_HCenter
	};

	class DUELGUI_API DGAlignment
	{
	public:
		inline DGAlignment(const DGAlignment &f) : mAlignCode(f.mAlignCode) {}
		inline DGAlignment(AlignmentFlag f) : mAlignCode(f) {}
		inline DGAlignment() : mAlignCode(0) {}
		inline DGAlignment(uint32 f) : mAlignCode(f) {}

		inline DGAlignment &operator=(const DGAlignment &f) { mAlignCode = f.mAlignCode; return *this; }
		inline DGAlignment &operator&=(int32 mask) { mAlignCode &= mask; return *this; }
		inline DGAlignment &operator&=(uint32 mask) { mAlignCode &= mask; return *this; }
		inline DGAlignment &operator|=(DGAlignment f) { mAlignCode |= f.mAlignCode; return *this; }
		inline DGAlignment &operator|=(AlignmentFlag f) { mAlignCode |= f; return *this; }
		inline DGAlignment &operator^=(DGAlignment f) { mAlignCode ^= f.mAlignCode; return *this; }
		inline DGAlignment &operator^=(AlignmentFlag f) { mAlignCode ^= f; return *this; }

		inline operator uint32() const { return mAlignCode; }

		inline DGAlignment operator|(DGAlignment f) const { return DGAlignment(AlignmentFlag(mAlignCode | f.mAlignCode)); }
		inline DGAlignment operator|(AlignmentFlag f) const { return DGAlignment(AlignmentFlag(mAlignCode | f)); }
		inline DGAlignment operator^(DGAlignment f) const { return DGAlignment(AlignmentFlag(mAlignCode ^ f.mAlignCode)); }
		inline DGAlignment operator^(AlignmentFlag f) const { return DGAlignment(AlignmentFlag(mAlignCode ^ f)); }
		inline DGAlignment operator&(int32 mask) const { return DGAlignment(AlignmentFlag(mAlignCode & mask)); }
		inline DGAlignment operator&(uint32 mask) const { return DGAlignment(AlignmentFlag(mAlignCode & mask)); }
		inline DGAlignment operator&(AlignmentFlag f) const { return DGAlignment(AlignmentFlag(mAlignCode & f)); }
		inline DGAlignment operator~() const { return DGAlignment(AlignmentFlag(~mAlignCode)); }

		inline bool operator!() const { return !mAlignCode; }

		inline bool testFlag(AlignmentFlag f) const { return (mAlignCode & f) == f && (f != 0 || mAlignCode == int32(f) ); }
	private:
		uint32		mAlignCode;
	};
}

#endif