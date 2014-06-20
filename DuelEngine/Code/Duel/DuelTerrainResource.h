//  [12/27/2013 OMEGA] created

#ifndef _DUELTERRAINRESOURCE_H_
#define _DUELTERRAINRESOURCE_H_

#include "DuelCommon.h"
#include "DuelResource.h"

namespace Duel
{

	class DUEL_API DTerrainResource : public DResource
	{
		DUEL_DECLARE_RTTI(DTerrainResource)
	public:
		DTerrainResource(DResourceManager* parent, const DString& name, const DString& groupName);



	};


}

#endif