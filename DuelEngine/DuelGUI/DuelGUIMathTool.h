//  [9/2/2013 OMEGA] created

#ifndef _DUELGUI2DTOOL_H_
#define _DUELGUI2DTOOL_H_

#include "DuelGUICommon.h"
#include "Duel.h"
#include "DuelGUISizeAdjustment.h"
#include "DuelGUIGeometry.h"
#include "DuelGUISize.h"

namespace Duel
{
	// auxiliary class for calculation in 2D gui suite.
	class DUELGUI_API DGGUIMathTool : public DObject
	{
	public:
// 		// this transform matrix can make a screen quad {(-1, 1), (1, 1), (1, -1), (-1, -1)}
// 		// into a scaled and translated quad as a rect in the window
// 		static DMatrix4	getScreenSpaceTransform(const IRect& rectInWindow, const DGSize& windSize);
// 
// 		// this transform matrix can make a screen quad {(-1, 1), (1, 1), (1, -1), (-1, -1)}
// 		// in to the gui world space.(a world space with 1windSize 1234*768 has x range [-512, 512] and y range [-384, 384])
// 		static DMatrix4 getWorldSpaceTransform(const IRect& rectInWindow, const DGSize& windSize);
// 		
		// this transform matrix can make a screen quad {(-1, 1), (1, 1), (1, -1), (-1, -1)}
		// into a scaled and translated quad as a rect in the window, while using UI screen space coordinate.
		static DMatrix4	getScreenSpaceTransform(const DVector2 posInScreen, const DGSize& quadSize, const DGSize& winSize);
		static DMatrix4	getScreenSpaceTransform(const DGGeometry& geo, const DGSize& winSize);

		// this transform matrix can make a quad {(-1, 1), (1, 1), (1, -1), (-1, -1)}
		// scale to the proper size, which looks as it is "clipped" by the clipMargin.
		static DMatrix4	getClippedTransform(const DGSize& size, const DGMargin& clipMargin);
		static DMatrix4 getClippedTransform(const DGSize& size, DReal clipL, DReal clipR, DReal clipT, DReal clipB);

		//this transform matrix work on uv coordinate, make a quad {(-1, 1), (1, 1), (1, -1), (-1, -1)}
		// scale to the proper size and position, which looks as it is "clipped" by the clipMargin.
		static DMatrix3	getClippedUVTransform(const DGSize& size, const DGMargin& clipMargin);
		static DMatrix3	getClippedUVTransform(const DGSize& size, DReal clipL, DReal clipR, DReal clipT, DReal clipB);

		// get a screen point (use middle point of the screen as origin).
		static DVector2	getScreenPoint(uint32 left, uint32 top, const DGSize& clipWindSize);
		// get a window point (use top-left corner as origin).
		static void		getWindowPoint(uint32& outLeft, uint32& outTop, const DVector2& inPos, const DGSize& clipWindSize);
	};
}

#endif