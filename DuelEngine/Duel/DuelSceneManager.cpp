//  [7/15/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelAnimation.h"
#include "DuelCamera.h"
#include "DuelSceneManager.h"
#include "DuelLightSource.h"

namespace Duel 
{
	DUEL_IMPLEMENT_RTTI_1(DSceneManager, DObject);

	
	DSceneManager::DSceneManager( DSceneManagerFactory* creator, const DString& type, const DString& name ) :
		mCreator(creator),
		mType(type),
		mName(name)
// 		,
// 		mSceneCamera(NULL)
	{
		
	}

	DSceneManager::~DSceneManager()
	{
		clearScene();
// 		destroyAllCameras();
// 		destroyAllAnimations();
	}
// 
// 	DCamera* DSceneManager::createCamera( const DString& name )
// 	{
// 		CameraMap::iterator i = mCameraMap.find(name);
// 		DCamera* cam;
// 		if (i != mCameraMap.end())
// 		{
// 			return i->second;
// 		}
// 		else
// 		{
// 			cam = new DCamera(this, name);
// 			mSceneCamera = cam;
// 			mCameraMap[name] = cam;
// 		}
// 		return cam;
// 	}
// 
// 	DCamera* DSceneManager::getCamera( const DString& name ) const
// 	{
// 		CameraMap::const_iterator i = mCameraMap.find(name);
// 		if (i != mCameraMap.end())
// 		{
// 			return i->second;
// 		}
// 		return NULL;
// 	}
// 
// 	bool DSceneManager::hasCamera( const DString& name ) const
// 	{
// 		CameraMap::const_iterator i = mCameraMap.find(name);
// 		if (i != mCameraMap.end())
// 		{
// 			return true;
// 		}
// 		return false;
// 	}
// 
// 	void DSceneManager::destroyCamera( DCamera* cam )
// 	{
// 		if (cam->getCreator() == this)
// 		{
// 			CameraMap::iterator i = mCameraMap.find(cam->getName());
// 			if (i != mCameraMap.end())
// 			{
// 				delete i->second;
// 				mCameraMap.erase(i);
// 			}
// 		}
// 	}
// 
// 	void DSceneManager::destroyCamera( const DString& name )
// 	{
// 		CameraMap::iterator i = mCameraMap.find(name);
// 		if (i != mCameraMap.end())
// 		{
// 			delete i->second;
// 			mCameraMap.erase(i);
// 		}
// 	}
// 
// 	void DSceneManager::destroyAllCameras()
// 	{
// 		CameraMap::iterator i, iend = mCameraMap.end();
// 		for (i = mCameraMap.begin(); i != iend; ++i)
// 		{
// 			delete i->second;
// 		}
// 		mCameraMap.clear();
// 	}

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
// 
// 	DSceneNode* DSceneManager::getRootSceneNode()
// 	{
// 		return NULL;
// 	}

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
// 
// 	DAnimation* DSceneManager::createAnimation( const DString& name )
// 	{
// 		AnimationMap::iterator i = mAnimationMap.find(name);
// 		if (i != mAnimationMap.end())
// 		{
// 			return i->second;
// 		}
// 		DAnimation* newAnim = new DAnimation(name);
// 		mAnimationMap[name] = newAnim;
// 		return newAnim;
// 	}
// 
// 	DAnimation* DSceneManager::getAnimation( const DString& name )
// 	{
// 		AnimationMap::iterator i = mAnimationMap.find(name);
// 		if (i != mAnimationMap.end())
// 		{
// 			return i->second;
// 		}
// 		return NULL;
// 	}
// 
// 	bool DSceneManager::hasAnimation( const DString& name )
// 	{
// 		AnimationMap::iterator i = mAnimationMap.find(name);
// 		if (i != mAnimationMap.end())
// 		{
// 			return true;
// 		}
// 		return false;
// 	}
// 
// 	void DSceneManager::destroyAnimation( const DString& name )
// 	{
// 		AnimationMap::iterator i = mAnimationMap.find(name);
// 		if (i != mAnimationMap.end())
// 		{
// 			delete i->second;
// 		}
// 	}
// 
// 	void DSceneManager::destroyAllAnimations()
// 	{
// 		AnimationMap::iterator i, iend = mAnimationMap.end();
// 		for(i = mAnimationMap.begin(); i != iend; ++i)
// 		{
// 			delete i->second;
// 		}
// 		mAnimationMap.clear();
// 	}

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
// 
// 	void DSceneManager::populateLights( LightMap& outMap )
// 	{
// 
// 	}

// 	Viewport* DSceneManager::getCurrentViewport()
// 	{
// 		return NULL;
// 	}

// 	void DSceneManager::applyToRenderQueue( RenderQueue* queue )
// 	{
// 
// 	}

// 	DSceneNode* DSceneManager::createSceneNodeImpl()
// 	{
// 		return NULL;
// 	}
// 
// 	DSceneNode* DSceneManager::createSceneNodeImpl( const DString& name )
// 	{
// 		return NULL;
// 	}
// 
// 	void DSceneManager::destroySceneNodeImpl( DSceneNode* node )
// 	{
// 		// leave to sub-class.
// 	}

	void DSceneManager::updateAutoGpuParameterMaps()
	{
		// TODO:
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

// 	void DSceneManager::setViewMatrix( const Matrix4& m )
// 	{
// 
// 	}
// 
// 	void DSceneManager::bindGpuProgram( GpuProgram* prog )
// 	{
// 
// 	}

}