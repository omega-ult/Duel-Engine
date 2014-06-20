//  [3/1/2014 OMEGA] created

#include "DuelCommon.h"
#include "DuelPostEffect.h"
#include "DuelPostEffectManager.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DPostEffectManager, DObject);



	DPostEffectManager::DPostEffectManager()
	{

	}

	void DPostEffectManager::registerPostEffect( DPostEffectPtr pe )
	{
		if (pe != NULL)
		{
			PostEffectMap::iterator i = mPostEffectMap.find(pe->getPostEffectName());
			if (i == mPostEffectMap.end())
			{
				mPostEffectMap[pe->getPostEffectName()] = pe;
			}
		}
	}

	void DPostEffectManager::unregisterPostEffect( const DString& name )
	{
		PostEffectMap::iterator i = mPostEffectMap.find(name);
		if (i != mPostEffectMap.end())
		{
			mPostEffectMap.erase(i);
		}
	}

	Duel::DPostEffectInstancePtr DPostEffectManager::createPostEffectInstance( const DString& name )
	{
		DPostEffectInstancePtr ret;
		PostEffectMap::iterator i = mPostEffectMap.find(name);
		if (i != mPostEffectMap.end())
		{
			ret = i->second->createInstance();
		}
		return ret;
	}

}