//  [5/24/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelAudioManager.h"

namespace Duel
{

	DAudioManager::DAudioManager() :
		DResourceManager("Audio")
	{

	}

	DAudioManager::~DAudioManager()
	{

	}
	DResource* DAudioManager::createImpl( DResourceDescription* createParam )
	{
		throw std::exception("The method or operation is not implemented.");
	}


}