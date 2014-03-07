//  [5/24/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelAudio.h"

namespace Duel
{

	DAudio::DAudio( DResourceManager* parent, const DString& name, const DString& groupName ) :
		DResource(parent, name, groupName, "Audio")
	{

	}

	DAudio::~DAudio()
	{

	}

}