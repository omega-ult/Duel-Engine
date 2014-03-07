//  [12/3/2013 OMEGA] created

#ifndef _DUELGUITEXTDOCUMENTLAYOUT_H_
#define _DUELGUITEXTDOCUMENTLAYOUT_H_

#include "DuelGUICommon.h"
#include "Duel.h"

namespace Duel
{

	/*
	The standard layout provided by Qt can handle simple word processing including inline images, lists and tables.
	*/
	class DUELGUI_API DGTextDocumentLayout : public DObject
	{
		DUEL_DECLARE_RTTI(DGTextDocumentLayout)
	public:
		DGTextDocumentLayout(DGTextDocument* document);

		DGTextDocument* getDocument();


	};

}

#endif