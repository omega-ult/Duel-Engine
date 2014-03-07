//  [1/7/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelSceneManagerEnumerator.h"
#include "DuelSceneManager.h"

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(DSceneManagerFactory, DObject);
	DUEL_IMPLEMENT_RTTI_1(DSceneManagerEnumerator, DObject);

	void DSceneManagerEnumerator::registerSceneManagerFactory( DSceneManagerFactory* factory )
	{
		SceneManagerFactoryMap::iterator i = mFactoryMap.find(factory->getTypeName());
		if (i == mFactoryMap.end())
		{
			mFactoryMap[factory->getTypeName()] = factory;
		}
	}

	DSceneManager* DSceneManagerEnumerator::createSceneManager( const DString& typeName, const DString& name )
	{
		SceneManagerFactoryMap::iterator i = mFactoryMap.find(typeName);
		if (i != mFactoryMap.end())
		{
			return i->second->createSceneManager(name);
		}
		return NULL;
	}

	void DSceneManagerEnumerator::destroySceneManager( DSceneManager* sm )
	{
		SceneManagerFactoryMap::iterator i = mFactoryMap.find(sm->getType());
		if (i != mFactoryMap.end())
		{
			i->second->destroySceneManager(sm);
		}
	}

	void DSceneManagerEnumerator::unregisterSceneManagerFactory( DSceneManagerFactory* factory )
	{
		SceneManagerFactoryMap::iterator i = mFactoryMap.find(factory->getTypeName());
		if (i == mFactoryMap.end())
		{
			mFactoryMap.erase(i);
		}
	}

}