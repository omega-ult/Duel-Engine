//  [12/7/2013 OMEGA] created

#ifndef _DUELGUITEXTFRAGMENT_H_
#define _DUELGUITEXTFRAGMENT_H_

#include "DuelGUICommon.h"
#include "Duel.h"
#include "DuelGUITextFormat.h"

namespace Duel
{

	// 最小的同一格式的字符单元.
	class DUELGUI_API DGTextFragment
	{
	public:
		DGTextFragment();

		DGTextCharFormat	getCharFormat() const;	
		int32				getCharFormatIndex() const;

		bool				contains(int32 pos);

		DWString			getText() const;
		
		// Returns the position of this text fragment in the document.
		int32				getPosition () const;
		uint32				getLength() const;
		// Returns true if this is a valid text fragment (i.e. has a valid position in a document);
		// otherwise returns false.
		bool				isValid() const;

		bool				operator != (const DGTextFragment& rh);
		bool				operator < (const DGTextFragment& rh);
		bool				operator == (const DGTextFragment& rh);
		DGTextFragment&		operator = (const DGTextFragment& rh);
	};
}

#endif