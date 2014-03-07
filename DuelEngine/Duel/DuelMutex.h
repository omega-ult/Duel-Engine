//  [7/9/2013 OMEGA] created

#ifndef _DUELMUTEX_H_
#define _DUELMUTEX_H_

#include "DuelCommon.h"

#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread.hpp>

// warning of exporting non-dll class
#pragma warning( disable : 4275 )

namespace Duel
{
	// just  a wrap of boost::recursive_mutex
	class DUEL_API DMutex : public DObject, public boost::recursive_mutex
	{
	DUEL_DECLARE_RTTI(DMutex)
	};
	class DUEL_API DTimedMutex : public DObject, public boost::timed_mutex
	{
	DUEL_DECLARE_RTTI(DTimedMutex)
	};
	class DUEL_API DSharedMutex : public DObject, public boost::shared_mutex
	{
	DUEL_DECLARE_RTTI(DSharedMutex)
	};
	typedef	boost::shared_lock<DSharedMutex>	DReadLock;
	typedef	boost::unique_lock<DSharedMutex>	DWriteLock;
}

#pragma warning( default : 4275 )

#endif