//  [9/2/2013 OMEGA] created

#include "DuelGUICommon.h"
#include "Duel.h"
#include "DuelMatrix.h"
#include "DuelGUISizeAdjustment.h"
#include "DuelGUIMathTool.h"


namespace Duel
{
// 
// 	DMatrix4 DGGUIMathTool::getScreenSpaceTransform( const IRect& rectInWindow, const DGSize& windSize )
// 	{
// 		DMatrix4 ret = DMatrix4::IDENTITY;
// 		DReal xScale = (DReal)rectInWindow.width / (DReal)windSize.getWidth();
// 		DReal yScale = (DReal)rectInWindow.height / (DReal)windSize.getHeight();
// 
// 		DVector2 spacePos;
// 		spacePos.x = 2*(DReal)rectInWindow.left/(DReal)windSize.getWidth()-1.0f + xScale;
// 		spacePos.y = 1.0f-2*(DReal)rectInWindow.top/(DReal)windSize.getHeight() - yScale;
// 
// 		ret.setScale(DVector3(xScale, yScale, 1.0f));
// 		ret.setTranslate(DVector3(spacePos.x, spacePos.y, 0.0f));
// 		return ret;
// 	}
// 
// 	DMatrix4 DGGUIMathTool::getWorldSpaceTransform( const IRect& rectInWindow, const DGSize& windSize )
// 	{
// 		DMatrix4 ret = DMatrix4::IDENTITY;
// 		DReal xScale = (DReal)rectInWindow.width / 2;
// 		DReal yScale = (DReal)rectInWindow.height / 2;
// 
// 		DVector2 windPos;
// 		windPos.x =   (DReal)rectInWindow.left - (DReal)windSize.getWidth()/2 + xScale;
// 		windPos.y = -((DReal)rectInWindow.height - (DReal)windSize.getHeight()/2 + yScale);
// 
// 		ret.setScale(DVector3(xScale, yScale, 1.0f));
// 		ret.setTranslate(DVector3(windPos.x, windPos.y, 0.0f));
// 
// 		return ret; 
// 	}

	DMatrix4 DGGUIMathTool::getScreenSpaceTransform( const DVector2 posInScreen, const DGSize& quadSize, const DGSize& winSize )
	{
		DMatrix4 ret = DMatrix4::IDENTITY;
		DReal xScale = (DReal)quadSize.getWidth() / (DReal)winSize.getWidth();
		DReal yScale = (DReal)quadSize.getHeight() / (DReal)winSize.getHeight();
		
		DVector2 spacePos;
		spacePos.x = (DReal)posInScreen.x/(DReal)winSize.getWidth() * 2;
		spacePos.y = (DReal)posInScreen.y/(DReal)winSize.getHeight() * 2;

		ret.setScale(DVector3(xScale, yScale, 1.0f));
		ret.setTranslate(DVector3(spacePos.x, spacePos.y, 0.0f));
		return ret;
	}

	DMatrix4 DGGUIMathTool::getScreenSpaceTransform( const DGGeometry& geo, const DGSize& winSize )
	{
		return getScreenSpaceTransform(geo.getCenter(), geo.getSize(), winSize);
	}


	DMatrix4 DGGUIMathTool::getClippedTransform( const DGSize& size, const DGMargin& clipMargin )
	{
		return getClippedTransform(size,
			(DReal)clipMargin.getLeft(),
			(DReal)clipMargin.getRight(),
			(DReal)clipMargin.getTop(),
			(DReal)clipMargin.getBottom());
	}

	DMatrix4 DGGUIMathTool::getClippedTransform( const DGSize& size, DReal clipL, DReal clipR, DReal clipT, DReal clipB )
	{
		DMatrix4 ret = DMatrix4::IDENTITY;
		DReal geoW = size.getWidth();
		DReal geoH = size.getHeight();
		DReal xScale = (geoW - (clipL + clipR)) / geoW;
		DReal yScale = (geoH - (clipT + clipB)) / geoH;

		// translator mah ....
		// 公式就不做注释了
		/*
			|<      originWidth         >|
			|---[------v--^---]----------|
			    | clippedWidth|
			           v
			    calculated Center
		*/
		DVector2 spacePos;
		spacePos.x = ((geoW/2 - clipR) - (geoW/2 - clipL))/2;
		spacePos.y = ((geoH/2 - clipT) - (geoH/2 - clipB))/2;

		ret.setScale(DVector3(xScale, yScale, 1.0f));
		ret.setTranslate(DVector3(spacePos.x, spacePos.y, 0.0f));
		return ret;
	}

	DVector2 DGGUIMathTool::getScreenPoint( uint32 left, uint32 top, const DGSize& clipSize )
	{
		DVector2 ret; 
		DReal w = clipSize.getWidth();
		DReal h = clipSize.getHeight();

		ret.x = (DReal)left - w * 0.5f;
		ret.y = h * 0.5f - (DReal)top;

		ret.x = DMath::Min<DReal>(ret.x, w * 0.5f);
		ret.x = DMath::Max<DReal>(ret.x, -w * 0.5f);
		ret.y = DMath::Min<DReal>(ret.y, h * 0.5f);
		ret.y = DMath::Max<DReal>(ret.y, -h * 0.5f);

		return ret;
	}

	void DGGUIMathTool::getWindowPoint( uint32& outLeft, uint32& outTop, const DVector2& pos, const DGSize& clipSize )
	{
		DVector2 p = pos;
		DReal w = clipSize.getWidth();
		DReal h = clipSize.getHeight();

		p.x = DMath::Min<DReal>(p.x, w * 0.5f);
		p.x = DMath::Max<DReal>(p.x, -w * 0.5f);
		p.y = DMath::Min<DReal>(p.y, h * 0.5f);
		p.y = DMath::Max<DReal>(p.y, -h * 0.5f);

		outLeft = (uint32)DMath::Abs(p.x + w * 0.5f);
		outTop = (uint32)DMath::Abs(-(p.y - h * 0.5f));
	}

	DMatrix3 DGGUIMathTool::getClippedUVTransform( const DGSize& size, const DGMargin& clipMargin )
 	{
		return getClippedUVTransform(size,
			(DReal)clipMargin.getLeft(),
			(DReal)clipMargin.getRight(),
			(DReal)clipMargin.getTop(),
			(DReal)clipMargin.getBottom());
	}

	DMatrix3 DGGUIMathTool::getClippedUVTransform( const DGSize& size, DReal clipL, DReal clipR, DReal clipT, DReal clipB )
	{
		DMatrix3 ret = DMatrix3::IDENTITY;
		DReal geoW = size.getWidth();
		DReal geoH = size.getHeight();
		DReal xScale = (geoW - (clipL + clipR)) / geoW;
		DReal yScale = (geoH - (clipT + clipB)) / geoH;

		DVector2 spacePos;
		spacePos.x = clipL / geoW;
		spacePos.y = clipT / geoH;

		ret[0][0] = xScale;
		ret[1][1] = yScale;
 		ret[2][0] = spacePos.x;
 		ret[2][1] = spacePos.y;
		return ret;
	}


}