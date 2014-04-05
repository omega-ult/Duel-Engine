//  [12/27/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelCore.h"
#include "DuelLightSource.h"
#include "DuelSceneInstance.h"
#include "DuelRenderWorkshop.h"
#include "DuelTerrainPage.h"
#include "DuelSceneManagerEnumerator.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DSceneInstance, DObject); 

	DSceneInstance::DSceneInstance(const DString& name) :
		mName(name),
		mSceneCamera(NULL),
		mRenderQueue(NULL),
		mTerrain(NULL)
	{
		// debug
		mSceneMgr = DSceneManagerEnumerator::getSingletonPtr()->createSceneManager("QuadtreeSceneManager", mName);
		mRenderQueue = new DRenderQueue();
		mTerrain = new DTerrainPage();
		mSceneCamera = createCamera(mName + " DefaultCamera");
	}

	DSceneInstance::~DSceneInstance()
	{
		destroyAllLights();
		destroyAllCameras();
		delete mRenderQueue;
		DSceneManagerEnumerator::getSingletonPtr()->destroySceneManager(mSceneMgr);
		delete mTerrain;
	}

	void DSceneInstance::initialize( DAxisAlignedBox region, DReal granularity )
	{
		if (mSceneMgr)
		{
			mSceneMgr->initialize(this, region, granularity);
		}
	}

	const DString& DSceneInstance::getName() const
	{
		return mName;
	}

	void DSceneInstance::update(DViewport vp)
	{
		if (mRenderQueue)
		{
			mRenderQueue->clear();
		}
		if (mTerrain)
		{
			mTerrain->update();
		}
		mSceneMgr->updateScene();
		if (mRenderQueue)
		{
			mRenderQueue->clearAllLights();
			applyToRenderQueue(mRenderQueue, vp);
		}
	}

	void DSceneInstance::applyToRenderQueue( DRenderQueue* queue, DViewport vp )
	{
		if (mTerrain && mSceneCamera)
		{
			mTerrain->applyToRenderQueue(queue, mSceneCamera, vp);
		}
		if (mSceneMgr && mSceneCamera)
		{
			mSceneMgr->applyToRenderQueue(queue, mSceneCamera);
			mSceneMgr->populateLights(queue, mSceneCamera);
		}
	}

	DSceneManager* DSceneInstance::getSceneManager()
	{
		return mSceneMgr;
	}

	DTerrainPage* DSceneInstance::getTerrain()
	{
		return mTerrain;
	}

	DCamera* DSceneInstance::createCamera( const DString& name )
	{
		CameraMap::iterator i = mCameraMap.find(name);
		DCamera* cam;
		if (i != mCameraMap.end())
		{
			return i->second;
		}
		else
		{
			cam = new DCamera(name);
			mSceneCamera = cam;
			mCameraMap[name] = cam;
		}
		return cam;
	}

	DCamera* DSceneInstance::getCamera( const DString& name ) const
	{
		CameraMap::const_iterator i = mCameraMap.find(name);
		if (i != mCameraMap.end())
		{
		 	return i->second;
		}
		return NULL;
	}

	bool DSceneInstance::hasCamera( const DString& name ) const
	{
		CameraMap::const_iterator i = mCameraMap.find(name);
		if (i != mCameraMap.end())
		{
		 	return true;
		}
		return false;
	}

	void DSceneInstance::destroyCamera( DCamera* cam )
	{
		if (cam == mSceneCamera)
		{
			mSceneCamera = NULL;
		}
		CameraMap::iterator i = mCameraMap.find(cam->getName());
		if (i != mCameraMap.end())
		{
			delete i->second;
			mCameraMap.erase(i);
		}
	}

	void DSceneInstance::destroyCamera( const DString& name )
	{
		if (mSceneCamera)
		{
			if (mSceneCamera->getName() == name)
			{
				mSceneCamera = NULL;
			}
		}

	}

	void DSceneInstance::destroyAllCameras()
	{
		mSceneCamera = NULL;
		CameraMap::iterator i, iend = mCameraMap.end();
		for (i = mCameraMap.begin(); i != iend; ++i)
		{
		 	delete i->second;
		}
		mCameraMap.clear();
	}

	DRenderQueue* DSceneInstance::getRenderQueue()
	{
		return mRenderQueue;
	}

	DLightSource* DSceneInstance::createLight( const DString& name )
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

	DLightSource* DSceneInstance::getLight( const DString& name )
	{
		LightMap::iterator i = mLightMap.find(name);
		if (i != mLightMap.end())
		{
			return i->second;
		}
		return NULL;
	}

	bool DSceneInstance::hasLight( const DString& name )
	{
		LightMap::iterator i = mLightMap.find(name);
		if (i != mLightMap.end())
		{
			return true;
		}
		return false;
	}

	void DSceneInstance::destroyLight( const DString& name )
	{
		LightMap::iterator i = mLightMap.find(name);
		if (i != mLightMap.end())
		{
			delete i->second;
			mLightMap.erase(i);
		}
	}

	void DSceneInstance::destroyLight( DLightSource* light )
	{
		LightMap::iterator i, iend = mLightMap.end();
		for(i = mLightMap.begin(); i != iend; ++i)
		{
			delete i->second;
		}
		mLightMap.clear();
	}

	void DSceneInstance::destroyAllLights()
	{

	}

}