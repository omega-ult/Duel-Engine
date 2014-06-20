//  [11/25/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelInputComponent.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DEventBase, DObject);
	DUEL_IMPLEMENT_RTTI_1(DInputComponent, DObject);

	DUEL_IMPLEMENT_RTTI_1(DInputButton, DInputComponent);
	DUEL_IMPLEMENT_RTTI_1(DInputAxis, DInputComponent);
	DUEL_IMPLEMENT_RTTI_1(DInputVector3, DInputComponent);
	DUEL_IMPLEMENT_RTTI_1(DInputPOV, DInputComponent);
	DUEL_IMPLEMENT_RTTI_1(DInputSlider, DInputComponent);


}