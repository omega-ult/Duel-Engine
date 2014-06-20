//  [12/2/2013 OMEGA] created

#ifndef _DUELGUITEXTCURSOR_H_
#define _DUELGUITEXTCURSOR_H_

#include "DuelGUICommon.h"
#include "Duel.h"

namespace Duel
{
	/*
	 QTextCursor contains information about both the cursor's position within a QTextDocument
	 and any selection that it has made.
	
	The cursor's current position() then is always either between two consecutive characters in the string,
	or else before the very first character or after the very last character in the string. 
	
	 from the developer's point of view, the document can be treated as one long string.

	 Some portions of that string can be considered to lie within particular blocks (e.g. paragraphs),
	 or within a table's cell, or a list's item, or other structural elements.
	 When we refer to "current character" we mean the character immediately before the cursor position() in the document.
	 Similarly, the "current block" is the block that contains the cursor position().

	 The cursor position can be changed programmatically using setPosition() and movePosition();
	 the latter can also be used to select text. For selections see selectionStart(), selectionEnd(), 
	 hasSelection(), clearSelection(), and removeSelectedText().

	 If the position() is at the start of a block atBlockStart() returns true; and if it is at the end of a block atBlockEnd() returns true. 
	 The format of the current character is returned by charFormat(), and the format of the current block is returned by blockFormat().

	 Formatting can be applied to the current text document using the setCharFormat(), mergeCharFormat(), 
	 setBlockFormat() and mergeBlockFormat() functions. 

	 Deletions can be achieved using deleteChar(), deletePreviousChar(), and removeSelectedText().

	 Text strings can be inserted into the document with the insertText() function,
	 blocks (representing new paragraphs) can be inserted with insertBlock().

	 Existing fragments of text can be inserted with insertFragment() but, if you want to insert pieces of text in various formats,
	 it is usually still easier to use insertText() and supply a character format.

	*/
	class DUELGUI_API DGTextCursor
	{
	public:
		enum MoveMode
		{
			MM_MoveAnchor,
			MM_KeepAnchor
		};
		enum MoveOperation
		{
			MO_NoMove,
			MO_Start,
			MO_StartOfLine,
			MO_StartOfWord,
			MO_PreviousBlock,
			MO_PreviousCharacter,
			MO_PreviousWord,	// Move to the beginning of the previous word.
			MO_Up,
			MO_Left,
			MO_WordLeft,
			MO_End,
			MO_EndOfLine,
			MO_EndOfWord,
			MO_EndOfBlock,
			MO_NextBlock,
			MO_NextCharacter,
			MO_NextWord,
			MO_Down,
			MO_Right,
			MO_WordRight
			// MO_NextCell, PreviousCell, NextRow, PreviousRow
		};

		enum SelectionType
		{
			ST_Document,
			ST_BlockUnderCursor,
			ST_LineUnderCursor,
			ST_WordUnderCursor
		};
		DGTextCursor();
		DGTextCursor(DGTextDocument* doc);
		DGTextCursor(DGTextFrame* frame);
		DGTextCursor(DGTextBlock* block);
		~DGTextCursor();

		DGTextDocument*	getParentDocument();
		/*
		Returns the anchor position; this is the same as position()
		unless there is a selection in which case position()
		marks one end of the selection and anchor() marks the other end.
		Just like the cursor position, the anchor position is between characters.
		*/
		int32			getAnchor();
		int32			getPosition();
		void			setPosition(int32 pos, MoveMode = MM_MoveAnchor);
		int32			getPositionInBlock();
		bool			atBlockEnd() const;
		bool			atBlockStart() const;
		bool			atEnd() const;
		bool			atStart() const;
		/*
		Indicates the start of a block of editing operations on the document
		that should appear as a single operation from an undo/redo point of view.

		For example:

		DGTextCursor cursor(textDocument);
		cursor.beginEditBlock();
		cursor.insertText("Hello");
		cursor.insertText("World");
		cursor.endEditBlock();

		textDocument->undo();


		The call to undo() will cause both insertions to be undone, causing both "World" and "Hello" to be removed.

		It is possible to nest calls to beginEditBlock and endEditBlock.
		The top-most pair will determine the scope of the undo/redo operation.
		*/
		void			beginEditBlock();
		void			endEditBlock();

		DGTextBlock			getBlock() const;
		/*
		The block char format is the format used when inserting text at the beginning of an empty block.
		*/
		DGTextCharFormat	getBlockCharFormat() const;
		void				setBlockCharFormat(const DGTextCharFormat& format);
		DGTextBlockFormat	getBlockFormat() const;
		void				setBlockFormat(const DGTextBlockFormat& format);
		
		/*
		Returns the format of the character immediately before the cursor position().
		If the cursor is positioned at the beginning of a text block that is not empty 
		then the format of the character immediately after the cursor is returned.
		*/
		DGTextCharFormat	getCharFormat() const;
		// ets the cursor's current character format to the given format. 
		// If the cursor has a selection, the given format is applied to the current selection.
		void				setCharFormat(const DGTextCharFormat& format);


		bool			hasSelection();
		void			clearSelection();

		DWString		getSelectedText();

		int32			getSelectionStart();
		int32			getSelectionEnd();
		/*

		Returns a pointer to the current frame. Returns 0 if the cursor is invalid.
		*/
		DGTextFrame*	getCurrentFrame() const;
		DGTextFrame*	insertFrame(const DGTextFrameFormat& format);
		/*
		If there is no selected text, deletes the character at the current cursor position;
		otherwise deletes the selected text.
		*/
		void			deleteChar();
		/*
		If there is no selected text, deletes the character before the current cursor position;
		otherwise deletes the selected text.
		*/
		void			deletePreviousChar(); 
		// if there is no seleciton, do nothing.
		void			removeSelectedText();

		void			insertBlock();
		void			insertBlock(const DGTextBlockFormat& format);
		void			insertBlock(const DGTextBlockFormat& blkFormat, const DGTextCharFormat& charFormat);

		void			insertImage(const DGTextImageFormat& format);

		void			insertText(const DWString& text);
		void			insertText(const DWString& text, const DGTextCharFormat& format);

		// return true if suceess
		bool			movePosition(MoveOperation operation, MoveMode mode = MM_MoveAnchor, int32 n = 1);
		//void			insertFragment(const QTextDocumentFragment & fragment);
		/*
		Returns true if the cursor is null; otherwise returns false. A null cursor is created by the default constructor.
		
		*/
		bool			isNull() const;


		//	Returns true if the other cursor is at a different position in the document as this cursor; otherwise returns false.
		bool			operator != (const DGTextCursor & other) const;
		//	Returns true if the other cursor is positioned later in the document than this cursor; otherwise returns false.
		bool			operator < (const DGTextCursor & other) const;

		//	Returns true if the other cursor is positioned later or at the same position in the document as this cursor; otherwise returns false.
		bool			operator <= (const DGTextCursor & other) const;

		//	Makes a copy of cursor and assigns it to this DGTextCursor. Note that DGTextCursor is an implicitly shared class.
		DGTextCursor&	operator = (const DGTextCursor & cursor);

		//	Returns true if the other cursor is at the same position in the document as this cursor; otherwise returns false.
		bool			operator == (const DGTextCursor & other) const;

		//	Returns true if the other cursor is positioned earlier in the document than this cursor; otherwise returns false.
		bool			operator > (const DGTextCursor & other) const;
		// Returns true if the other cursor is positioned earlier or at the same position in the document as this cursor; otherwise returns false.
		bool			operator >= (const DGTextCursor & other) const;

	};
}

#endif