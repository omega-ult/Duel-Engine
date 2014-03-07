//  [8/14/2013 OMEGA] created

#include "DuelGUICommon.h"
#include "Duel.h"
#include "DuelInputReceiver.h"
#include "DuelGUIWidget.h"
#include "DuelGUIHotSpot.h"
#include "DuelGUI2DButton.h"
#include "DuelGUI2DButtonHS.h"

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(DG2DButtonHS, DGHotspot);


	DG2DButtonHS::DG2DButtonHS(DG2DButton* parent) :
		DGHotspot(parent),
		mAbsRect(0,0,0,0)
	{

	}




	bool DG2DButtonHS::response( const DEventBase* e )
	{
		const DMouseEvent* me = e->getAs<DMouseEvent>(false);
		if (me)
		{
			int32 x = me->state.X.abs;
			int32 y = me->state.Y.abs;
			if (x >= mAbsRect.left && x <= mAbsRect.left + mAbsRect.width &&
				y >= mAbsRect.top && y <= mAbsRect.top + mAbsRect.height)
			{
				//std::cout<<x << "," << y << std::endl;
				std::cout<<x << "," <<y << std::endl;
			}
			return true;
		}
		return false;
	}

	void DG2DButtonHS::update()
	{
		if (mParent)
		{
			DVector2 pos = mParent->getPosition();
			mAbsRect = IRect((int32)pos.x, (int32)pos.y, 
				mParent->getGeometry().width, mParent->getGeometry().height);
		}
	}

}