//  [12/5/2013 OMEGA] created

#ifndef _DUELGUITEXTOPTION_H_
#define _DUELGUITEXTOPTION_H_

#include "DuelGUICommon.h"
#include "Duel.h"
#include "DuelGUIAlignment.h"
#include "DuelGUILayout.h"

namespace Duel
{
	/*
		The QTextOption class provides a description of general rich text properties.
		QTextOption is used to encapsulate common rich text properties in a single object. 
		It contains information about text alignment, layout direction, word wrapping, 
		and other standard properties associated with text rendering and layout.
	*/

	class DUELGUI_API DGTextOption
	{
	public:

		enum TabType {
			TT_LeftTab,		// A left-tab
			TT_RightTab,	// A right-tab
			TT_CenterTab,	// A centered-tab
			TT_DelimiterTab	// A tab stopping at a certain delimiter-character
		};

		struct DUELGUI_API Tab {
			inline Tab() : position(80), type(DGTextOption::TT_LeftTab) { }
			inline Tab(DReal pos, TabType tabType, char delim = 0)
				: position(pos), type(tabType), delimiter(delim) {}

			inline bool operator==(const Tab &other) const {
				return type == other.type
					&& DMath::realEqual(position, other.position)
					&& delimiter == other.delimiter;
			}

			inline bool operator!=(const Tab &other) const {
				return !operator==(other);
			}

			DReal	position;
			TabType	type;
			DReal	delimiter;
		};

		DGTextOption();
		DGTextOption(DGAlignment alignment);
		~DGTextOption();

		DGTextOption(const DGTextOption& o);
		DGTextOption&	operator=(const DGTextOption& o);


		inline void			setAlignment(DGAlignment alignment) { mAlign = alignment; }
		inline DGAlignment	getAlignment() const { return mAlign; }

		inline void			setTextDirection(DGLayout::Direction aDirection) { mDirection = aDirection; }
		inline DGLayout::Direction		getTextDirection() const { return mDirection; }

		// This enum describes how text is wrapped in a document.
		enum WrapMode {
			WM_NoWrap,						// Text is not wrapped at all.
			WM_WordWrap,					// Text is wrapped at word boundaries.
			WM_WrapAnywhere,				// Text can be wrapped at any point on a line, even if it occurs in the middle of a word.
			WM_WrapAtWordBoundaryOrAnywhere	// If possible, wrapping occurs at a word boundary; 
											// otherwise it will occur at the appropriate point on the line, even in the middle of a word.
		};

		inline void			setWrapMode(WrapMode wrap) { mWrapMode = wrap; }
		inline WrapMode		getWrapMode() const { return mWrapMode; }

// 
// 		inline void setTabStop(qreal tabStop);
// 		inline qreal tabStop() const { return tab; }
// 
// 		void setTabArray(QList<qreal> tabStops);
// 		QList<qreal> tabArray() const;
// 
// 		void setTabs(QList<Tab> tabStops);
// 		QList<Tab> tabs() const;
// 
// 		void setUseDesignMetrics(bool b) { design = b; }
// 		bool useDesignMetrics() const { return design; }

	private:
		DGAlignment			mAlign;
		DGLayout::Direction	mDirection;
		WrapMode			mWrapMode;
	};
}

#endif