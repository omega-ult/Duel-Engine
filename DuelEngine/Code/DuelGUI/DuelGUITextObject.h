//  [12/2/2013 OMEGA] created

#ifndef _DUELGUITEXTOBJECT_H_
#define _DUELGUITEXTOBJECT_H_

#include "DuelGUICommon.h"
#include "Duel.h"

namespace Duel
{
	class DUELGUI_API DGTextObject : public DObject
	{
	public:
		DGTextObject(DGTextDocument* document);
		~DGTextObject();
		INTERNAL void	setFormat(const DGTextFormat& format);
		// Returns the document this object belongs to.
		DGTextDocument*	getParentDocument() const;
		// Returns the text object's format.
		DGTextFormat	getFormat() const;
		// Returns the index of the object's format in the document's internal list of formats.
		int32			getFormatIndex() const;
		// Returns the object index of this object. This can be used together with QTextFormat::setObjectIndex().
		int32			getObjectIndex() const;
	protected:
		// not allowed.
		DGTextObject() {}

	};

	/*
	组成QTextDocument的列表结构。

		QTextBlock表示QTextDocument中的一个片断。
		它提供了其中的一个只读的block/paragraph. 它主要用来遍历text document来实现定制的layout或者重写一个新的document.
		可以通过text()和length()（包括了格式化的字符）得到文件内容的信息。
	*/
	class DUELGUI_API DGTextBlock
	{
	public:
		DGTextBlock(DGTextDocument* document);
		DGTextBlock();
		DGTextBlock(const DGTextBlock& b);
		inline DGTextBlock&	operator=(const DGTextBlock& b);
// 		friend class QSyntaxHighlighter;
// 	public:
// 		inline QTextBlock(QTextDocumentPrivate *priv, int b) : p(priv), n(b) {}
// 		inline QTextBlock() : p(0), n(0) {}
// 		inline QTextBlock(const QTextBlock &o) : p(o.p), n(o.n) {}
// 		inline QTextBlock &operator=(const QTextBlock &o) { p = o.p; n = o.n; return *this; }
// 
// 		inline bool isValid() const { return p != 0 && n != 0; }
// 
// 		inline bool operator==(const QTextBlock &o) const { return p == o.p && n == o.n; }
// 		inline bool operator!=(const QTextBlock &o) const { return p != o.p || n != o.n; }
// 		inline bool operator<(const QTextBlock &o) const { return position() < o.position(); }
// 
// 		int position() const;
// 		int length() const;
// 		bool contains(int position) const;
// 
// 		QTextLayout *layout() const;
// 		void clearLayout();
// 		QTextBlockFormat blockFormat() const;
// 		int blockFormatIndex() const;
// 		QTextCharFormat charFormat() const;
// 		int charFormatIndex() const;
// 
// 		Qt::LayoutDirection textDirection() const;
// 
// 		QString text() const;
// 
// 		const QTextDocument *document() const;
// 
// 		QTextList *textList() const;
// 
// 		QTextBlockUserData *userData() const;
// 		void setUserData(QTextBlockUserData *data);
// 
// 		int userState() const;
// 		void setUserState(int state);
// 
// 		int revision() const;
// 		void setRevision(int rev);
// 
// 		bool isVisible() const;
// 		void setVisible(bool visible);
// 
// 		int blockNumber() const;
// 		int firstLineNumber() const;
// 
// 		void setLineCount(int count);
// 		int lineCount() const;
// 
// 		class Q_GUI_EXPORT iterator {
// 			const QTextDocumentPrivate *p;
// 			int b;
// 			int e;
// 			int n;
// 			friend class QTextBlock;
// 			iterator(const QTextDocumentPrivate *priv, int begin, int end, int f) : p(priv), b(begin), e(end), n(f) {}
// 		public:
// 			iterator() : p(0), b(0), e(0), n(0) {}
// 			iterator(const iterator &o) : p(o.p), b(o.b), e(o.e), n(o.n) {}
// 
// 			QTextFragment fragment() const;
// 
// 			bool atEnd() const { return n == e; }
// 
// 			inline bool operator==(const iterator &o) const { return p == o.p && n == o.n; }
// 			inline bool operator!=(const iterator &o) const { return p != o.p || n != o.n; }
// 			iterator &operator++();
// 			inline iterator operator++(int) { iterator tmp = *this; operator++(); return tmp; }
// 			iterator &operator--();
// 			inline iterator operator--(int) { iterator tmp = *this; operator--(); return tmp; }
// 		};
// 
// 		// more Qt
// 		typedef iterator Iterator;
// 
// 		iterator begin() const;
// 		iterator end() const;
// 
// 		QTextBlock next() const;
// 		QTextBlock previous() const;
// 
// 		inline QTextDocumentPrivate *docHandle() const { return p; }
// 		inline int fragmentIndex() const { return n; }
// 
// 	private:
// 		QTextDocumentPrivate *p;
// 		int n;
// 		friend class QTextDocumentPrivate;
// 		friend class QTextLayout;
	};


	class DUELGUI_API DGTextBlockGroup : public DGTextObject
	{
	public:
		DGTextBlockGroup(DGTextDocument *doc);
		~DGTextBlockGroup();

		typedef	std::list<DGTextBlock>	BlockList;
		BlockList		getBlockList() const;

	protected:

		virtual void blockInserted(const DGTextBlock& block);
		virtual void blockRemoved(const DGTextBlock& block);
		virtual void blockFormatChanged(const DGTextBlock& block);


	protected:
		// not allowed.
		DGTextBlockGroup() {}
	};

	class DUELGUI_API DGTextList : public DGTextBlockGroup
	{

	};

	class DUELGUI_API DGTextFrame : public DGTextObject
	{

	};

}

#endif