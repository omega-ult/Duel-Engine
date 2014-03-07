//  [12/5/2013 OMEGA] created

#ifndef _DUELGUITEXTFONT_H_
#define _DUELGUITEXTFONT_H_

#include "DuelGUICommon.h"
#include "Duel.h"

namespace Duel
{

	// 用来指示文字的字体, 并非"字体资源"
	class DUELGUI_API DGTextFont
	{
	public:
// 		enum StyleHint {
// 			Helvetica,  SansSerif = Helvetica,
// 			Times,      Serif = Times,
// 			Courier,    TypeWriter = Courier,
// 			OldEnglish, Decorative = OldEnglish,
// 			System,
// 			AnyStyle,
// 			Cursive,
// 			Monospace,
// 			Fantasy
// 		};

// 		enum StyleStrategy {
// 			PreferDefault       = 0x0001,
// 			PreferBitmap        = 0x0002,
// 			PreferDevice        = 0x0004,
// 			PreferOutline       = 0x0008,
// 			ForceOutline        = 0x0010,
// 			PreferMatch         = 0x0020,
// 			PreferQuality       = 0x0040,
// 			PreferAntialias     = 0x0080,
// 			NoAntialias         = 0x0100,
// 			OpenGLCompatible    = 0x0200,
// 			ForceIntegerMetrics = 0x0400,
// 			NoFontMerging       = 0x8000
// 		};
// 
// 		enum HintingPreference {
// 			PreferDefaultHinting        = 0,
// 			PreferNoHinting             = 1,
// 			PreferVerticalHinting       = 2,
// 			PreferFullHinting           = 3
// 		};

		enum FontWeight 
		{
			FW_Light    = 25,
			FW_Normal   = 50,
			FW_DemiBold = 63,
			FW_Bold     = 75,
			FW_Black    = 87
		};

		enum FontStyle 
		{
			FS_StyleNormal,
			FS_StyleItalic,
			FS_StyleOblique
		};

		enum FontStretch 
		{
			FS_UltraCondensed =  50,
			FS_ExtraCondensed =  62,
			FS_Condensed      =  75,
			FS_SemiCondensed  =  87,
			FS_Unstretched    = 100,
			FS_SemiExpanded   = 112,
			FS_Expanded       = 125,
			FS_ExtraExpanded  = 150,
			FS_UltraExpanded  = 200
		};

		enum FontCapitalization 
		{
			FC_MixedCase,
			FC_AllUppercase,
			FC_AllLowercase,
			FC_SmallCaps,
			FC_Capitalize
		};

		enum FontSpacingType 
		{
			FS_PercentageSpacing,
			FS_AbsoluteSpacing
		};
// 
// 		enum FontResolveProperties {
// 			FamilyResolved              = 0x0001,
// 			SizeResolved                = 0x0002,
// 			StyleHintResolved           = 0x0004,
// 			StyleStrategyResolved       = 0x0008,
// 			WeightResolved              = 0x0010,
// 			StyleResolved               = 0x0020,
// 			UnderlineResolved           = 0x0040,
// 			OverlineResolved            = 0x0080,
// 			StrikeOutResolved           = 0x0100,
// 			FixedPitchResolved          = 0x0200,
// 			StretchResolved             = 0x0400,
// 			KerningResolved             = 0x0800,
// 			CapitalizationResolved      = 0x1000,
// 			LetterSpacingResolved       = 0x2000,
// 			WordSpacingResolved         = 0x4000,
// 			HintingPreferenceResolved   = 0x8000,
// 			StyleNameResolved           = 0x10000,
// 			AllPropertiesResolved       = 0x1ffff
// 		};

		// location of font resource.
		void			setFont(const DString& fontName, const DString& groupName);
		const DString&	getFontName() const;
		const DString&	getFontGroupName() const;

		void			setCapitalization();
		FontCapitalization	getCapitalization() const;

		bool			isBold() const;
		void			setBold(bool flag);

		// you can use enum FontWeight to set this font, weight should range form 0 to 10.
		void			setWeight(uint32 weight);
		uint32			getWeight() const;

		void			setPointSize(uint32 size);
		uint32			getPointSize() const;

		// use enum FontStretch to set the stretch. or you can set a specified value, ranges from 50 to 200.
		void			setStretch(uint32 stretch);
		uint32			getStretch() const;

		bool			setOverlineEnable(bool flag);
		bool			isOverlineEnable() const;

		void			setStrikeOutEnable(bool flag);
		bool			isStrikeOutEnable();
		
		void			setStyle(FontStyle style);
		FontStyle		getStyle() const; 


	};
}

#endif