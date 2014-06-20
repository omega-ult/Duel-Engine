//  [1/12/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelSceneQuery.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(SceneQuery, DObject);
		DUEL_IMPLEMENT_RTTI_1(RayQuery, SceneQuery);
		DUEL_IMPLEMENT_RTTI_1(RegionQuery, SceneQuery);
			DUEL_IMPLEMENT_RTTI_1(SphereQuery, RegionQuery);
			DUEL_IMPLEMENT_RTTI_1(AxisAlignedBoxQuery, RegionQuery);
		DUEL_IMPLEMENT_RTTI_1(IntersectionQuery, SceneQuery);
		
}