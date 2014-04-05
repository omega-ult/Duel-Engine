//  [5/23/2013 OMEGA] created

#ifndef _DUELSCENEINSTANCE_H_
#define _DUELSCENEINSTANCE_H_

#include "DuelCommon.h"
#include "DuelTerrain.h"


namespace Duel
{

	class DUEL_API DSceneInstance : public DObject
	{
		DUEL_DECLARE_RTTI(DSceneInstance)
	public:
		DSceneInstance(const DString& sceneName);
		~DSceneInstance();

		// TODO: 作为一个单场景的实现.. 我还在考虑如何实现超大场景.
		// TODO: 等东西都完成的时候, 我们来做个scene的描述文件吧.
		// see DSceneManager::initialize();
		void		initialize(DAxisAlignedBox region, DReal granularity);
		
		const DString&	getName() const;

		// a hahaha - -
		// void		openScene(const DString& fileName) {}

		// call this every frame to update contents, and fill the render queue
		void			update(DViewport vp);
		// client should call update() before get the render queue, 
		DRenderQueue*	getRenderQueue();

		// debug. use QuadTreeSceneManager .
		DSceneManager*	getSceneManager();

		DTerrainPage*	getTerrain();

		// camera operations. you can only get/destroy a camera from a scene manager.
		typedef	std::map<DString, DCamera*>	CameraMap;
		typedef	MapIterator<CameraMap>		CameraIterator;
		CameraIterator		getCameraIterator() { return CameraIterator(mCameraMap.begin(), mCameraMap.end()); }
		size_t		getCameraCount() const { return mCameraMap.size(); }
		DCamera*	createCamera(const DString& name);
		DCamera*	getCamera(const DString& name) const;
		bool		hasCamera(const DString& name) const;
		void		destroyCamera(DCamera* cam);
		void		destroyCamera(const DString& name);
		void		destroyAllCameras();
		 
		// the only interface for creating/destroying lights. 
		typedef	std::map<DString, DLightSource*>	LightMap;
		typedef	MapIterator<LightMap>				LightIterator;
		LightIterator		getLightIterator() { return LightIterator(mLightMap); }
		DLightSource*		createLight(const DString& name);
		DLightSource*		getLight(const DString& name);
		uint32		getLightCount() const { return mLightMap.size(); }
		bool		hasLight(const DString& name);
		void		destroyLight(const DString& name);
		void		destroyLight(DLightSource* light);
		void		destroyAllLights();


		// set a camera for current scene graph, which is used to update render queue and light.
		void		setSceneCamera(DCamera* cam) { mSceneCamera = cam; }
		DCamera*		getSceneCamera() const { return mSceneCamera; }

	protected:
		// use scenemanager's scene camera to do the culling and push to the render queue.
		// we need extra information, such as viewport for our terrain calculation.
		void		applyToRenderQueue(DRenderQueue* queue, DViewport vp);

		DString			mName;
		DTerrainPage*	mTerrain;
		// DTerrainManager* mTerrainMgr;
		DSceneManager*	mSceneMgr;

		CameraMap		mCameraMap;
		LightMap		mLightMap;
		DCamera*		mSceneCamera;

		DRenderQueue*	mRenderQueue;
	};

}

#endif