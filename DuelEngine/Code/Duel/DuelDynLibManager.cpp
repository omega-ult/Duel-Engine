//  [12/4/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelDynLibManager.h"
#include "DuelDynLib.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DDynLibManager, DObject);

	DDynLibManager::DDynLibManager()
	{

	}

	DDynLibManager::~DDynLibManager()
	{
		for (DynLibMap::iterator i = mDynLibMap.begin(); i != mDynLibMap.end(); ++i)
		{
			i->second->unload();
			delete i->second;
		}
		mDynLibMap.clear();
	}

	DDynLib* DDynLibManager::load( const DString& fileName )
	{
		DynLibMap::iterator	i = mDynLibMap.find(fileName);
		if (i != mDynLibMap.end())
		{
			return i->second;
		}
		else
		{
			DDynLib*	newLib = new DDynLib(fileName);
			newLib->load();
			mDynLibMap[fileName] = newLib;
			return newLib;
		}
	}

	void DDynLibManager::unload( DDynLib* lib )
	{
		DynLibMap::iterator	i = mDynLibMap.find(lib->getName());
		if (i != mDynLibMap.end())
		{
			mDynLibMap.erase(i);
			lib->unload();
			delete lib;
		}
	}

}