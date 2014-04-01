//  [7/26/2013 OMEGA] created

#ifndef _DUELGUIPREDECLARE_H_
#define _DUELGUIPREDECLARE_H_


namespace Duel
{
	//////////////////////////////////////////////////////////////////////////
	// guibase
	class	DGSize;
	class	DGSizePolicy;
	class	DGCursor;
	class	DGWidget;
	class	DGHotspot;
	class	DGMargin;
	class	DGPadding;
	class	DGRenderDelegate;
	class	DGLayout;
	class	DGLayoutEngine;
	class	DGLayoutItem;
	class	DGUIManager;
	class	DGUIManagerFactory;
	class	DGUISystem;

	//////////////////////////////////////////////////////////////////////////
	// text
	class	DGTextDocument;
	class	DGTextDocumentLayout;
	class	DGTextObject;
	class	DGTextBlock;
	class	DGTextFragment;
	class	DGTextBlockGroup;
	class	DGTextList;
	class	DGTextFrame;
	class	DGTextCursor;

	class	DGTextFormat;
	class	DGTextCharFormat;
	class	DGTextImageFormat;
	class	DGTextFrameFormat;
	class	DGTextTableFormat;
	class	DGTextBlockFormat;
	class	DGTextListFormat;
	class	DGTextOption;

	class	DGTextEngine;
	class	DGTextLine;
	class	DGTextLayout;
	//////////////////////////////////////////////////////////////////////////
	// 2d suite
// 	class	DG2DButton;
// 	class	DG2DButtonHS;
// 	class	DG2DButtonRD;

	//////////////////////////////////////////////////////////////////////////
	// standard widgets
	class	DGStdCursor;
	class	DGStdCursorRD;
	class	DGStdCursorPictureLayer;

	class	DGStdHotspot;
	class	DGStdLabel;
	class	DGStdLabelRD;
	class	DGWindowLayout;
	class	DGWindow;
	class	DGWindowTitleBar;

	//////////////////////////////////////////////////////////////////////////
	// showcase
	class	DGSPictureBox;
	class	DGSPictureBoxRD;
	class	DGSPictureLayer;
	class	DGSPictureBoxHS;
}

#endif