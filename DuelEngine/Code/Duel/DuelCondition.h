//  [7/9/2013 OMEGA] created

#ifndef _DUELCONDITION_H_
#define _DUELCONDITION_H_

#include "DuelCommon.h"
#include <boost/thread/condition.hpp>

// warning of exporting non-dll class
#pragma warning( disable : 4275 )

namespace Duel
{
	class DCondition : public DObject, public boost::condition
	{
	DUEL_DECLARE_RTTI(DCondition)
	};

}

#pragma warning( default : 4275 )

#endif