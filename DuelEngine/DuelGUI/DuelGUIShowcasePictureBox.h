//  [11/20/2013 OMEGA] created

#ifndef _DUELGUISHOWCASEPICTUREBOX_H_
#define _DUELGUISHOWCASEPICTUREBOX_H_

#include "DuelGUICommon.h"
#include "Duel.h"
#include "DuelGUIWidget.h"
#include "DuelGUIHotSpot.h"
#include "DuelGUIShowcasePictureLayer.h"
#include "DuelGUIRenderDelegate.h"

namespace Duel
{
	class	DGSPictureBox;

	class DGSPictureBoxHS : public DGHotspot
	{
		DUEL_DECLARE_RTTI(DGSPictureBoxHS)
	public:
		DGSPictureBoxHS(DGSPictureBox* targetBox);
		// 好吧记录下思路, 这里使用widget的 定位点, 法向量, 乘上widget的transformNode的矩阵, 获得其在gui
		// 空间中的绝对值, 然后再和事件中的ray进行相交检测.

		virtual bool	response( const DEventBase* e );

		virtual void	update();

		virtual DGWidget*	getParent();

	protected:
		DGSPictureBox*	mTargetBox;
		DPlane			mWidgetPlane;
	};


	class DGSPictureBoxRD : public DGRenderDelegate
	{
		DUEL_DECLARE_RTTI(DGSPictureBoxRD)
	public:
		DGSPictureBoxRD(DGSPictureBox* targetBox);
		// override : DGRenderDelegate------------------
		virtual void	applyToRenderQueue( DRenderQueue* queue, uint32 groupID );
		void			update();
		
		virtual	DGWidget*	getParent();
	protected:
		DGSPictureBox*			mTargetBox;
		DGSPictureLayer*	mPicLayer;
		bool				mbClipped;
	};

	class DUELGUI_API DGSPictureBox : public DGWidget
	{
		DUEL_DECLARE_RTTI(DGSPictureBox)
	public:
		DGSPictureBox(DGWidget* parent = NULL);
		~DGSPictureBox();

		
		// 好吧我现在就只是show看看. 什么都不说了
		void		setAlignment(DGAlignment align);
		DGAlignment	getAlignment();

		// whether to extend to the widget's geometry.
		void		setScalable(bool flag);
		bool		isScalable();

		// use group/texture name to set the texture.
		void		setTexture(const DString& texName, const DString& grpName);
		// use texture to set the picture. this method will validate the type of the resource
		// only DTexture* is acceptable;
		void		setTexture(DResourcePtr tex);

		DResourcePtr	getTexture();

	protected:
		DGSPictureBoxRD*	mRD;
		DGSPictureBoxHS*	mHS;
		DResourcePtr		mTex;

		DGAlignment		mAlign;

		bool			mbScalable;
	};
}


#endif