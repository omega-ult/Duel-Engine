//  [7/9/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelMutex.h"

#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/shared_mutex.hpp>

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DMutex, DObject);
	DUEL_IMPLEMENT_RTTI_1(DTimedMutex, DObject);
	DUEL_IMPLEMENT_RTTI_1(DSharedMutex, DObject);



}