//  [11/28/2013 OMEGA] created

#ifndef _DUELGUISHOWCASELABEL_H_
#define _DUELGUISHOWCASELABEL_H_

#include "DuelGUICommon.h"
#include "DuelGUIWidget.h"
#include "DuelGUIShowcaseTextLayer.h"
#include "DuelGUIRenderDelegate.h"

namespace Duel
{
	class DGSLabel;

	// wtf.
	class DUELGUI_API DGSLabelRD : public DGRenderDelegate
	{
		DUEL_DECLARE_RTTI(DGSLabelRD)
	public:
	public:
		DGSLabelRD(DGSLabel* targetLabel);
		// override : DGRenderDelegate------------------
		virtual void	applyToRenderQueue( DRenderQueue* queue, uint32 groupID );
		void			update();
		virtual	DGWidget*	getParent();

	protected:
		// key 为texture的index, 在font里.
		typedef std::map<uint32, DGSTextLayer*>	TextLayermap;
		DGSLabel*			mTargetBox;
		DGSPictureLayer*	mPicLayer;
		bool				mbClipped;

	};

	class DUELGUI_API DGSLabel : public DGWidget
	{
		DUEL_DECLARE_RTTI(DGSLabel)
	public:
		DGSLabel(DGWidget* parent = NULL);
		~DGSLabel();
	
		// the text layer font.
		void			setTextFont(const DString& fontName, const DString& groupName);
		void			setTextFont(DResourcePtr font);
		DResourcePtr	getFont();

		void			setText(const DWString& text);
		const DWString&	getText();


		void			setTextAlignment(DGAlignment align);
		AlignmentFlag	getTextAlignment();


		// 好吧我现在就只是show看看. 什么都不说了
		void			setBackgroundAlignment(DGAlignment align);
		AlignmentFlag	getBackgroundAlignment();

		// whether to extend to the widget's geometry.
		void			setBackgroundScalable(bool flag);
		bool			isBackgroundScalable();

		// use group/texture name to set the texture.
		void			setBackgroundTexture(const DString& texName, const DString& grpName);
		// use texture to set the picture. this method will validate the type of the resource
		// only DTexture* is acceptable;
		void			setBackgroundTexture(DResourcePtr tex);

		DResourcePtr	getBackgroundTexture();

	protected:


		DWString		mText;
		DResourcePtr	mFont;
		DResourcePtr	mBgTex;

		DGAlignment		mTxtAlign;	
		DGAlignment		mBgAlign;


	};

}

#endif