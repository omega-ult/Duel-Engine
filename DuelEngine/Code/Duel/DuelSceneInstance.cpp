//  [12/27/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelCore.h"
#include "DuelLightSource.h"
#include "DuelSceneInstance.h"
#include "DuelRenderWorkshop.h"
#include "DuelTerrainPage.h"
#include "DuelSceneManagerEnumerator.h"
#include "DuelSkyDome.h"

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
		mSkyDome = new DSkyDome();
		mSceneCamera = createCamera(mName + " DefaultCamera");
		mRenderQueue->setRenderCamera(mSceneCamera);
	}

	DSceneInstance::~DSceneInstance()
	{
		destroyAllLights();
		destroyAllCameras();
		delete mRenderQueue;
		DSceneManagerEnumerator::getSingletonPtr()->destroySceneManager(mSceneMgr);
		delete mSkyDome;
		delete mTerrain;
	}

	void DSceneInstance::initialize( DAxisAlignedBox region, DReal granularity )
	{
		if (mSceneMgr)
		{
			mSceneMgr->initialize(region, granularity);
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
			applyToRenderQueue(vp);
		}
	}

	void DSceneInstance::applyToRenderQueue( DViewport vp )
	{
		if (mSceneCamera)
		{
			mSkyDome->applyToRenderQueue(mRenderQueue, mSceneCamera);
		}		
		if (mTerrain && mSceneCamera)
		{
			mTerrain->applyToRenderQueue(mRenderQueue, mSceneCamera, vp);
		}
		if (mSceneMgr && mSceneCamera)
		{
			mSceneMgr->applyToRenderQueue(mRenderQueue, mSceneCamera);
			populateLights(mSceneCamera);
		}
	}

	void DSceneInstance::populateLights( DCamera* cam )
	{
		DSceneInstance::LightIterator li = getLightIterator();
		while (li.hasMoreElements())
		{
			bool isAffected = false;
			DLightSource* light = li.getNext();
			LightType t = light->getLightType();
			if (t == LT_Ambient) 
			{
				isAffected = true;
			}
			else if (t == LT_Directional)
			{
				DDirectionalLight* dl = light->getAs<DDirectionalLight>();
				// do visibility testing
				// basic algorithm: calculate the plane in directional light's abstract cylinder's
				// local space p, calculate the perpendicular point p' toward origin, then testing
				// whether p' is in the cylinder.
				DOrientedBox ob;
				DReal zExt = dl->getDistance();
				DReal r = dl->getRadius();
				ob.setMaximum(r, r, zExt);
				ob.setMinimum(-r, -r, -zExt);
				DVector3 cyDir = dl->getDirection();
				cyDir.normalize();
				DVector3 zToward = DVector3::UNIT_Z;
				ob.setOrientation(zToward.getRotationTo(cyDir));
				ob.setOrigin(light->getPosition());
				if (cam->isInside(ob) != DCamera::FTS_Out)
				{
					isAffected = true;
				}

			}
			else if (t == LT_Point)
			{
				DPointLight* pl = light->getAs<DPointLight>();
				DReal r = pl->getRadius();
				DSphere lightSphere(light->getPosition(), r);

				if (cam->isInside(lightSphere) != DCamera::FTS_Out)
				{
					isAffected = true;
				}
			}
			else if (t == LT_Spotlight)
			{
				DSpotlight* sl = light->getAs<DSpotlight>();
				// TODO: calculate visiblity.
				// using common perpendicular line to calcu late intersection between camera
				// view cone and spot light cone.
				DReal cpLength;
				DRay camRay(cam->getEyePosition(), cam->getDirection());
				DRay spotRay(light->getPosition(), sl->getDirection());
				DRay comPerp = camRay.getCommonPerpendicularTo(spotRay, &cpLength);

				// ensure it is a valid result.
				if (comPerp.getDirection() != DVector3::ZERO)
				{
					// ignore the light if it was in the back side of the camera.
					DVector3 comPerpToCam = (comPerp.getOrigin() - camRay.getOrigin());
					DReal coef = comPerpToCam.dotProduct(cam->getDirection());
					if (coef < 0)
					{
						// check whether the camera's origin is in the spotlight's lighting region
						DVector3 camToSpot = cam->getEyePosition() - spotRay.getOrigin();

						DRadian r = DMath::arcCos(
							camToSpot.dotProduct(spotRay.getDirection()) / 
							(camToSpot.length() * spotRay.getDirection().length()));
						if (r < sl->getOuterAngle()/2)
						{
							isAffected = true;
						}
					}
					else
					{
						// calculate intersection between two cone
						DReal camDist = (comPerp.getOrigin()-cam->getEyePosition()).length() * DMath::Tan(cam->getFOV()/2);
						DReal spotDist = ((comPerp.getOrigin()+comPerp.getDirection()*cpLength)
							- spotRay.getOrigin()).length() * DMath::Tan(sl->getOuterAngle()/2);
						if (camDist + spotDist > cpLength)
						{
							isAffected = true;
						}
					}

				}
			}
			if (isAffected)
			{
				mRenderQueue->addLight(light);
			}
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

	DSkyDome* DSceneInstance::getSkyDome()
	{
		return mSkyDome;
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

	DLightSource* DSceneInstance::createLight( LightType type, const DString& name )
	{
		LightMap::iterator i = mLightMap.find(name);
		if (i != mLightMap.end())
		{
			if (i->second->getLightType() != type)
			{
				delete i->second;
				mLightMap.erase(i);
			}
			else
			{
				return i->second;
			}
		}
		DLightSource* ret = NULL;;
		switch (type)
		{
		case Duel::LT_Ambient:
			ret = new DAmbientLight(name);
			break;
		case Duel::LT_Point:
			ret = new DPointLight(name);
			break;
		case Duel::LT_Directional:
			ret = new DDirectionalLight(name);
			break;
		case Duel::LT_Spotlight:
			ret = new DSpotlight(name);
			break;
		case Duel::LT_SurfaceLight:
		case Duel::LT_VolumeLight:
			break;
		}
		if (ret != NULL)
		{
			mLightMap[name] = ret;
		}
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

	void DSceneInstance::setSceneCamera( DCamera* cam )
	{
		mSceneCamera = cam;
		mRenderQueue->setRenderCamera(mSceneCamera);
	}

}