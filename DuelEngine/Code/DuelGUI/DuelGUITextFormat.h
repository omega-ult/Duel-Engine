//  [12/3/2013 OMEGA] created

#ifndef _DUELGUITEXTFORMAT_H_
#define _DUELGUITEXTFORMAT_H_

#include "DuelGUICommon.h"
#include "Duel.h"
#include "DuelGUITextFont.h"
#include "DuelGUITextOption.h"


namespace Duel
{
	/*
	The DGTextLength class encapsulates the different types of length used in a QTextDocument.

	When we specify a value for the length of an element in a text document,
	we often need to provide some other information so that the length is used in the way we expect. 
	For example, when we specify a table width, the value can represent a fixed number of pixels, 
	or it can be a percentage value. This information changes both the meaning of the value and the way it is used.

	Generally, this class is used to specify table widths. 
	These can be specified either as a fixed amount of pixels, as a percentage of the containing frame's width, 
	or by a variable width that allows it to take up just the space it requires.

	*/
	class DUELGUI_API DGTextLength
	{
	public:
		enum LengthType { 
			LT_VariableLength,
			LT_FixedLength,
			LT_PercentageLength
		};

		DGTextLength() : mLengthType(LT_VariableLength), mRawValue(0) {}

		DGTextLength(LengthType type, DReal value) : mLengthType(type), mRawValue(value) {}

		inline LengthType	getType() const { return mLengthType; }
		inline DReal		getValue(DReal maximumLength) const
		{
			switch (mLengthType) {
			case LT_FixedLength:
				return mRawValue;
			case LT_VariableLength:
				return maximumLength;
			case LT_PercentageLength:
				return mRawValue * maximumLength / DReal(100.0f);
			}
			return -1.0f;
		}

		inline DReal		getRawValue() const
		{
			return mRawValue; 
		}

		inline bool operator==(const DGTextLength &other) const
		{ 
			return mLengthType == other.mLengthType
				&& DMath::realEqual(mRawValue, other.mRawValue); 
		}
		inline bool operator!=(const DGTextLength &other) const
		{ 
			return mLengthType != other.mLengthType
				|| DMath::realEqual(mRawValue, other.mRawValue); 
		}

	private:
		LengthType	mLengthType;
		DReal		mRawValue;
	};

	/*
	QTextFormat
	|
	|<--- QTextCharFormat <---QTextImageFormat
	|
	|<--- QTextFrameFormat <--- QTextTableFormat
	|
	|<--- QTextBlockFormat
	|
	|<--- QTextListFormat


	*/

	class DUELGUI_API DGTextFormat : public DObject
	{
		DUEL_DECLARE_RTTI(DGTextFormat)
	public:
// 		enum FormatType {
// 			FT_InvalidFormat = -1,
// 			FT_BlockFormat = 1,
// 			FT_CharFormat = 2,
// 			FT_ListFormat = 3,
// 			FT_TableFormat = 4,
// 			FT_FrameFormat = 5,
// 
// 			FT_UserFormat = 100
// 		};

		enum FormatProperty {
// 			FP_ObjectIndex = 0x0,
// 
// 			// paragraph and char
// 			FP_CssFloat = 0x0800,
// 			FP_LayoutDirection = 0x0801,
// 
// 			FP_OutlinePen = 0x810,
// 			FP_BackgroundBrush = 0x820,
// 			FP_ForegroundBrush = 0x821,
// 			// Internal to qtextlayout.cpp: ObjectSelectionBrush = 0x822
// 			FP_BackgroundImageUrl = 0x823,
// 
// 			// paragraph
// 			FP_BlockAlignment = 0x1010,
// 			FP_BlockTopMargin = 0x1030,
// 			FP_BlockBottomMargin = 0x1031,
// 			FP_BlockLeftMargin = 0x1032,
// 			FP_BlockRightMargin = 0x1033,
// 			FP_TextIndent = 0x1034,
// 			FP_TabPositions = 0x1035,
// 			FP_BlockIndent = 0x1040,
// 			FP_LineHeight = 0x1048,
// 			FP_LineHeightType = 0x1049,
// 			FP_BlockNonBreakableLines = 0x1050,
// 			FP_BlockTrailingHorizontalRulerWidth = 0x1060,

			// character properties
// 			FP_FirstFontProperty = 0x1FE0,
// 			FP_FontCapitalization = FP_FirstFontProperty,
// 			FP_FontLetterSpacing = 0x1FE1,
// 			FP_FontWordSpacing = 0x1FE2,
// 			FP_FontStyleHint = 0x1FE3,
// 			FP_FontStyleStrategy = 0x1FE4,
// 			FP_FontKerning = 0x1FE5,
// 			FP_FontHintingPreference = 0x1FE6,
// 			FP_FontFamily = 0x2000,
// 			FP_FontPointSize = 0x2001,
// 			FP_FontSizeAdjustment = 0x2002,
// 			FP_FontSizeIncrement = FP_FontSizeAdjustment, // old name, compat
// 			FP_FontWeight = 0x2003,
// 			FP_FontItalic = 0x2004,
// 			FP_FontUnderline = 0x2005, // deprecated, use TextUnderlineStyle instead
// 			FP_FontOverline = 0x2006,
// 			FP_FontStrikeOut = 0x2007,
// 			FP_FontFixedPitch = 0x2008,
// 			FP_FontPixelSize = 0x2009,
// 			FP_LastFontProperty = FP_FontPixelSize,
// 
// 			FP_TextUnderlineColor = 0x2010,
// 			FP_TextVerticalAlignment = 0x2021,
// 			FP_TextOutline = 0x2022,
// 			FP_TextUnderlineStyle = 0x2023,
// 			FP_TextToolTip = 0x2024,
// 
// 			FP_IsAnchor = 0x2030,
// 			FP_AnchorHref = 0x2031,
// 			FP_AnchorName = 0x2032,
// 			FP_ObjectType = 0x2f00,
// 
// 			// list properties
// 			FP_ListStyle = 0x3000,
// 			FP_ListIndent = 0x3001,
// 			FP_ListNumberPrefix = 0x3002,
// 			FP_ListNumberSuffix = 0x3003,
// 
// 			// table and frame properties
// 			FP_FrameBorder = 0x4000,
// 			FP_FrameMargin = 0x4001,
// 			FP_FramePadding = 0x4002,
// 			FP_FrameWidth = 0x4003,
// 			FP_FrameHeight = 0x4004,
// 			FP_FrameTopMargin    = 0x4005,
// 			FP_FrameBottomMargin = 0x4006,
// 			FP_FrameLeftMargin   = 0x4007,
// 			FP_FrameRightMargin  = 0x4008,
// 			FP_FrameBorderBrush = 0x4009,
// 			FP_FrameBorderStyle = 0x4010,
// 
// 			FP_TableColumns = 0x4100,
// 			FP_TableColumnWidthConstraints = 0x4101,
// 			FP_TableCellSpacing = 0x4102,
// 			FP_TableCellPadding = 0x4103,
// 			FP_TableHeaderRowCount = 0x4104,
// 
// 			// table cell properties
// 			FP_TableCellRowSpan = 0x4810,
// 			FP_TableCellColumnSpan = 0x4811,
// 
// 			FP_TableCellTopPadding = 0x4812,
// 			FP_TableCellBottomPadding = 0x4813,
// 			FP_TableCellLeftPadding = 0x4814,
// 			FP_TableCellRightPadding = 0x4815,

// 			// image properties
// 			FP_ImageName = 0x5000,
// 			FP_ImageWidth = 0x5010,
// 			FP_ImageHeight = 0x5011,
// 
// 			// internal
// 			/*
// 			SuppressText = 0x5012,
// 			SuppressBackground = 0x513
// 			*/
// 
// 			// selection properties
// 			FP_FullWidthSelection = 0x06000,
// 
// 			// page break properties
// 			FP_PageBreakPolicy = 0x7000,
// 
// 			// --
// 			FP_UserProperty = 0x100000
// 		};
// 
// 		enum ObjectTypes {
// 			OT_NoObject,
// 			OT_ImageObject,
// 			OT_TableObject,
// 			OT_TableCellObject,
// 
// 			OT_UserObject = 0x1000
// 		};
// 
// 		enum PageBreakFlag {
// 			PBF_PageBreak_Auto = 0,
// 			PBF_PageBreak_AlwaysBefore = 0x001,
// 			PBF_PageBreak_AlwaysAfter  = 0x010
// 			// PageBreak_AlwaysInside = 0x100
 		};

		// background / foreground color.
		/*
		void	setBackgroundColor(const DColor& c);
		DColor	getBackgroundColor() const; 

		void	setForegroundColor(const DColor& c);
		DColor	getForegroundColor() const;

		*/
		int32	getObjectIndex();	// FP_ObjectIndex

	};


	class DUELGUI_API DGTextCharFormat : public DGTextFormat
	{
		DUEL_DECLARE_RTTI(DGTextCharFormat)
	public:
		/*

		Constant	Value	Description
		QTextCharFormat::NoUnderline	0	Text is draw without any underlining decoration.
		QTextCharFormat::SingleUnderline	1	A line is drawn using Qt::SolidLine.
		QTextCharFormat::DashUnderline	2	Dashes are drawn using Qt::DashLine.
		QTextCharFormat::DotLine	3	Dots are drawn using Qt::DotLine;
		QTextCharFormat::DashDotLine	4	Dashs and dots are drawn using Qt::DashDotLine.
		QTextCharFormat::DashDotDotLine	5	Underlines draw drawn using Qt::DashDotDotLine.
		QTextCharFormat::WaveUnderline	6	The text is underlined using a wave shaped line.
		QTextCharFormat::SpellCheckUnderline	7	The underline is drawn depending on the QStyle::SH_SpellCeckUnderlineStyle style hint of the QApplication style. By default this is mapped to WaveUnderline, on Mac OS X it is mapped to DashDotLine.
		
		*/
// 		enum UnderlineStyle
// 		{
// 			US_NoUnderline,
// 			US_SingleUnderline,
// 			US_DashUnderline,
// 			US_DotLine,
// 			US_DashDotLine,
// 			US_DashDotDotLine,
// 			US_WaveUnderline,
// 			US_SpellCheckUnderline
// 		};

		/*

		Constant	Value	Description
		QTextCharFormat::AlignNormal	0	Adjacent characters are positioned in the standard way for text in the writing system in use.
		QTextCharFormat::AlignSuperScript	1	Characters are placed above the baseline for normal text.
		QTextCharFormat::AlignSubScript	2	Characters are placed below the baseline for normal text.
		QTextCharFormat::AlignMiddle	3	The center of the object is vertically aligned with the base line. Currently, this is only implemented for inline objects.
		QTextCharFormat::AlignBottom	5	The bottom edge of the object is vertically aligned with the base line.
		QTextCharFormat::AlignTop	4	The top edge of the object is vertically aligned with the base line.
		
		*/
		enum VerticalAlignment
		{
			VA_AlignNormal,
			VA_AlignSuperScript,
			VA_AlignSubScript,
			VA_AlignMiddle,
			VA_AlignBottom,
			VA_AlignTop
		};

		/*
		void		setFont(const DString& fontName, groupname);
		*/
		void			setTextFont(const DGTextFont& font);
		const DGTextFont&	getTextFont() const;
// 
// 		void			setUnderlineStyle(UnderlineStyle style);
// 		UnderlineStyle	getUnderlineStyle();

		void			setUnderlineColor(const DColor& c);
		const DColor&	getUnderlineColor() const;

		void			setVerticalAlignment(VerticalAlignment a);
		VerticalAlignment	getVerticalAlignment() const;

		void			setAnchorHref(const DString& value);
		const DString&	getAnchorHref() const;

		void			setAnchorEnable(bool flag);
		void			isAnchorEnabled() const;


	private:
		DGTextFont	mFont;
	};


	class DUELGUI_API DGTextImageFormat : public DGTextCharFormat
	{
		DUEL_DECLARE_RTTI(DGTextImageFormat)
	public:

		// should be a texture.
		void			setImage(const DString& resName, const DString& groupName);

		void			setSize(const DGSize& size);
		const DGSize&	getSize() const;

	};
// 
// 	class DUELGUI_API DGTableCellFormat : public DGTextCharFormat
// 	{
// 		DUEL_DECLARE_RTTI(DGTableCellFormat)
// 	public:
// 		/*
// 		上下左右padding的 set/get. 自己写.
// 		*/
// 	};

	class DUELGUI_API DGTextFrameFormat : public DGTextFormat
	{
		DUEL_DECLARE_RTTI(DGTextFrameFormat)
	public:
// 		enum BorderStyle
// 		{
// 			BS_None,
// 			BS_Dotted,
// 			BS_Dashed,
// 			BS_Solid,
// 			BS_Double,
// 			BS_DotDash,
// 			BS_DotDotDash,
// 			BS_Groove,
// 			BS_Ridge,
// 			BS_Inset
// 		};

		void			setMargin(const DGMargin& margin);
		const DGMargin&	getMargin() const;
		void			setPadding(const DGPadding& padding);
		const DGPadding&	getPadding() const;

		void			setBorderColor(const DColor& c);
		const DColor&	getBorderColor() const;

		void			setBorderWidth(DReal w);
		DReal			getBorderWidth() const;

		void			setPosition(const DVector2& p);
		// relative position to the parent frame.
		DVector2		getPosition() const;

		// use text length to specified special parameter such as percentage.
		void			setWidth(const DGTextLength& w);
		DGTextLength	getWidth() const;
		void			setHeight(const DGTextLength& h);
		DGTextLength	getHeight() const;

	};
// 
// 	class DUELGUI_API DGTextTableFormat : public DGTextFrameFormat
// 	{
// 		 
// 	};

	class DUELGUI_API DGTextBlockFormat : public DGTextFormat
	{
		DUEL_DECLARE_RTTI(DGTextBlockFormat)
	public:

		void			setAlignment(DGAlignment align);
		DGAlignment		getAlignment() const;

		void			setMargin(const DGMargin& margin);
		const DGMargin&	getMargin() const;

		void			setIndent(uint32 indent);
		uint32			getIndent() const;

		void			setLineHeight(DReal h);
		DReal			getLineHeight();

		void			setLineBreakable(bool flag);
		bool			isLineBreakable() const;


	};
// 
// 	class DUELGUI_API DGTextListFormat : public DGTextFormat
// 	{
// 
// 	};


}

#endif // !_DUELGUITEXTFORMAT_H_
