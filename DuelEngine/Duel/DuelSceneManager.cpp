//  [7/15/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelAnimation.h"
#include "DuelCamera.h"
#include "DuelSceneManager.h"
#include "DuelSceneInstance.h"
#include "DuelLightSource.h"

namespace Duel 
{
	DUEL_IMPLEMENT_RTTI_1(DSceneManager, DAutoGpuParameterDelegate);

	
	DSceneManager::DSceneManager( DSceneManagerFactory* creator, const DString& type, const DString& name ) :
		mCreator(creator),
		mOwner(NULL),
		mType(type),
		mName(name),
		mSceneBox(BBE_Null),
		mGranularity(1000.0f)
	{
		
	}

	DSceneManager::~DSceneManager()
	{
		clearScene();
	}
	void DSceneManager::initialize( DSceneInstance* owner, DAxisAlignedBox region, DReal granularity )
	{
		assert(owner != NULL);
		mOwner = owner;
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
			destroySceneNodeImpl(i->second);
			i->second = NULL;
		}
		mSceneNodeMap.clear();
	}

	void DSceneManager::updateSceneNode( DSceneNode* node )
	{
		// leave to sub-class
	}

	DLightSource* DSceneManager::createLight( const DString& name )
	{
		LightMap::iterator i = mLightMap.find(name);
		if (i != mLightMap.end())
		{
			return i->second;
		}
		DLightSource* ret = new DLightSource(name);
		mLightMap[name] = ret;
		return ret;
	}

	DLightSource* DSceneManager::getLight( const DString& name )
	{
		LightMap::iterator i = mLightMap.find(name);
		if (i != mLightMap.end())
		{
			return i->second;
		}
		return NULL;
	}

	bool DSceneManager::hasLight( const DString& name )
	{
		LightMap::iterator i = mLightMap.find(name);
		if (i != mLightMap.end())
		{
			return true;
		}
		return false;
	}

	void DSceneManager::destroyLight( const DString& name )
	{
		LightMap::iterator i = mLightMap.find(name);
		if (i != mLightMap.end())
		{
			delete i->second;
			mLightMap.erase(i);
		}
	}

	void DSceneManager::destroyLight( DLightSource* light )
	{
		LightMap::iterator i = mLightMap.find(light->getName());
		if (i != mLightMap.end())
		{
			delete i->second;
			mLightMap.erase(i);
		}
	}

	void DSceneManager::destroyAllLights()
	{
		LightMap::iterator i, iend = mLightMap.end();
		for(i = mLightMap.begin(); i != iend; ++i)
		{
			delete i->second;
		}
		mLightMap.clear();
	}

	void DSceneManager::populateLights( DRenderQueue* queue, DCamera* cam )
	{
		LightMap tempMap;
		populateLights(tempMap, cam);
		LightMap::iterator i, iend = tempMap.end();
		for (i = tempMap.begin(); i != iend; ++i)
		{
			queue->addLight(i->second);
		}
	}


	void DSceneManager::clearScene()
	{
		destroyAllSceneNodes();
		destroyAllLights();
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

	DMatrix4 DSceneManager::getViewMatrix()
	{
		if (mOwner != NULL && mOwner->getSceneCamera() != NULL)
		{
			return mOwner->getSceneCamera()->getViewMatrix();
		}
		return DMatrix4::IDENTITY;
	}

	DMatrix4 DSceneManager::getProjectionMatrix()
	{
		if (mOwner != NULL && mOwner->getSceneCamera() != NULL)
		{
			return mOwner->getSceneCamera()->getProjectionMatrix();
		}
		return DMatrix4::IDENTITY;
	}


}