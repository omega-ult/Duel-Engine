//  [7/6/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelRTTI.h"
#include "DuelString.h"

namespace Duel
{

	bool recursiveRTTICheck(const Duel::RTTI* dst, const Duel::RTTI* src)
	{
		size_t pCount = src->getParentCount();
		for (uint32 i = 0; i < pCount; ++i)
		{
			const Duel::RTTI* tmp = src->getParentRTTI(i);
			if(tmp == dst)				
			{
				return true;
			}
			return RTTI::isKind(dst, tmp);
		}
		return false;

	}
	bool RTTI::isKind(const Duel::RTTI* dst, const Duel::RTTI* src)
	{
		if (!(dst == src))
		{
			return recursiveRTTICheck(dst, src);
		}
		return true;
	}
	bool RTTI::isExactKind(const Duel::RTTI* dst, const Duel::RTTI* src)
	{
		return dst == src;
	}
}