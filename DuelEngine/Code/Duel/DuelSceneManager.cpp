//  [7/15/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelAnimation.h"
#include "DuelCamera.h"
#include "DuelSceneManager.h"
#include "DuelSceneInstance.h"
#include "DuelLightSource.h"

namespace Duel 
{
	DUEL_IMPLEMENT_RTTI_1(DSceneManager, DObject);

	
	DSceneManager::DSceneManager( DSceneManagerFactory* creator, const DString& type, const DString& name ) :
		mCreator(creator),
		mType(type),
		mName(name),
		mSceneBox(BBE_Null),
		mGranularity(1000.0f)
	{
		
	}

	DSceneManager::~DSceneManager()
	{
	}
	void DSceneManager::initialize( DAxisAlignedBox region, DReal granularity )
	{
		mSceneBox = region;
		mGranularity = granularity;
	}


	DSceneNode* DSceneManager::createSceneNode()
	{
		DSceneNode* ret;
		DSceneNode* n = createSceneNodeImpl();
		SceneNodeMap::iterator i = mSceneNodeMap.find(n->getName());
		if (i != mSceneNodeMap.end())
		{
			ret = n;
			mSceneNodeMap[n->getName()] = n;
		}
		else
		{
			destroySceneNodeImpl(n);
			ret = i->second;
		}
		return ret;
	}

	DSceneNode* DSceneManager::createSceneNode( const DString& name )
	{
		SceneNodeMap::iterator i = mSceneNodeMap.find(name);
		if (i != mSceneNodeMap.end())
		{
			return i->second;
		}
		DSceneNode* n = createSceneNodeImpl(name);
		mSceneNodeMap[name] = n;
		return n;
	}

	DSceneNode* DSceneManager::getSceneNode( const DString& name )
	{
		SceneNodeMap::iterator i = mSceneNodeMap.find(name);
		if (i != mSceneNodeMap.end())
		{
			return i->second;
		}
		return NULL;
	}

	bool DSceneManager::hasSceneNode(const DString& name)
	{
		SceneNodeMap::iterator i = mSceneNodeMap.find(name);
		if (i != mSceneNodeMap.end())
		{
			return true;
		}
		return false;
	}


	void DSceneManager::destroySceneNode( DSceneNode* node )
	{
		SceneNodeMap::iterator i = mSceneNodeMap.find(node->getName());
		if (i != mSceneNodeMap.end())
		{
			node->detachAllMovables();
			node->removeAllChildren();
			destroySceneNodeImpl(node);
			mSceneNodeMap.erase(i);
		}
	}

	void DSceneManager::destroyAllSceneNodes()
	{
		SceneNodeMap::iterator i, iend = mSceneNodeMap.end();
		for (i = mSceneNodeMap.begin(); i != iend ; ++i)
		{
			i->second->detachAllMovables();
			i->second->removeAllChildren();
			destroySceneNodeImpl(i->second);
			i->second = NULL;
		}
		mSceneNodeMap.clear();
	}

	void DSceneManager::updateSceneNode( DSceneNode* node )
	{
		// leave to sub-class
	}
	
	void DSceneManager::clearScene()
	{
		destroyAllSceneNodes();
	}

	void DSceneManager::updateScene()
	{
		DSceneManager::SceneNodeIterator si = getSceneNodeIterator();
		while (si.hasMoreElements())
		{
			DSceneNode* n = si.getNext();
			n->updateBound();
			updateSceneNode(n);
		}
	}

}