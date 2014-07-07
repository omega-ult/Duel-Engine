//  [5/23/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelMaterial.h"
#include "DuelMaterialManager.h"

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(DMaterialManager, DObject);

	DMaterialManager::DMaterialManager()
	{

	}

	DMaterialManager::~DMaterialManager()
	{

	}

	void DMaterialManager::registerMaterial( DMaterialPtr mat )
	{
		if (mat != NULL)
		{
			MaterialMap::iterator i = mMatMap.find(mat->getMaterialName());
			if (i == mMatMap.end())
			{
				mMatMap[mat->getMaterialName()] = mat;
			}
		}
	}

	void DMaterialManager::unregisterMaterial( const DString& name )
	{
		MaterialMap::iterator i = mMatMap.find(name);
		if (i != mMatMap.end())
		{
			mMatMap.erase(i);
		}
	}

	Duel::DMaterialInstancePtr DMaterialManager::createMaterialInstance( const DString& name )
	{
		DMaterialInstancePtr ret;
		MaterialMap::iterator i = mMatMap.find(name);
		if (i != mMatMap.end())
		{
			ret = i->second->createInstance();
		}
		return ret;
	}

	Duel::DMaterialInstancePtr DMaterialManager::copyMaterialInstance( DMaterialInstancePtr inst )
	{
		DMaterialInstancePtr ret;
		if (inst != NULL)
		{
			ret = createMaterialInstance(inst->getParent()->getMaterialName());
			inst->copyTo(ret.get());
		}
		return ret;		
	}


	
}