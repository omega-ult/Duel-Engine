//  [11/10/2012 OMEGA] created

#include "DuelGUICommon.h"
#include "DuelCommon.h"
#include "DuelNode.h"
#include "DuelGUIHotSpot.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DGHotspot, DObject);
	//DUEL_IMPLEMENT_RTTI(GUIRect2DHotspot, DGHotspot);


// 	bool GUIRect2DHotspot::response( uint16 x, uint16 y, HotSpotResponsor& resp ) const
// 	{
// 		Real left = getLeft();
// 		Real right = getRight();
// 		Real top = getTop();
// 		Real bottom = getBottom();
// 		if (x > left && x < right &&
// 			y > top && y < bottom)
// 		{
// 			resp.hotspotType = HST_RECT2D;
// 			resp.responsor = (DGHotspot*)(this);
// 			if (x < left+5)
// 			{
// 				resp.hotspotArea = HSA_RECT2D_L;
// 				if (y < top+5)
// 				{
// 					resp.hotspotArea = HSA_RECT2D_TL;
// 				}
// 				else if (y > bottom-5)
// 				{
// 					resp.hotspotArea = HSA_RECT2D_BL;
// 				}
// 			}
// 			else if (x > right-5)
// 			{
// 				resp.hotspotArea = HSA_RECT2D_R;
// 				if (y < top+5)
// 				{
// 					resp.hotspotArea = HSA_RECT2D_TR;
// 				}
// 				else if (y > bottom-5)
// 				{
// 					resp.hotspotArea = HSA_RECT2D_BR;
// 				}
// 			}
// 			return true;
// 		}
// 		return false;
// 	}
// 
// 	bool GUIRect2DHotspot::response( const Vector2& pos, HotSpotResponsor& resp ) const
// 	{
// 		Real x = pos.x;
// 		Real y = pos.y;
// 		return response(static_cast<uint16>(x), static_cast<uint16>(y), resp);
// 	}

}