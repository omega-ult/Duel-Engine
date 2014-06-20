//  [7/16/2013 OMEGA] created

#include "DuelGUICommon.h"
#include "DuelCommon.h"
#include "DuelGUISystem.h"
#include "DuelGUIManager.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DGUISystem, DObject);
	DGUISystem::DGUISystem()
	{

	}

	DGUISystem::~DGUISystem()
	{
	}

	DGUIManager* DGUISystem::createGUIManager( DRenderWindow* wind)
	{
		GUIManagerMap::iterator i = mManagerMap.find(wind);
		if (i != mManagerMap.end())
		{
			return i->second;
		}
		DGUIManager* ret = new DGUIManager(wind);
		mManagerMap[wind] = ret;
		return ret;
	}

	DGUIManager* DGUISystem::getGUIManager( DRenderWindow* wind )
	{
		GUIManagerMap::iterator i = mManagerMap.find(wind);
		if (i != mManagerMap.end())
		{
			return i->second;
		}
		return NULL;
	}

	void DGUISystem::destroyGUIManager( DRenderWindow* wind )
	{

		GUIManagerMap::iterator i = mManagerMap.find(wind);
		if (i != mManagerMap.end())
		{
			delete i->second;
			mManagerMap.erase(i);
		}
	}

}