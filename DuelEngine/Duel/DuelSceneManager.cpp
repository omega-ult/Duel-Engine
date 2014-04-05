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
	
	void DSceneManager::populateLights( DRenderQueue* queue, DCamera* cam )
	{
		if (mOwner != NULL)
		{
			DSceneInstance::LightIterator li = mOwner->getLightIterator();
			while (li.hasMoreElements())
			{
				DLightSource* l = li.getNext();
				if (isAffectedByLight(l, cam))
				{
					queue->addLight(l);
				}
			}
		}
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

	bool DSceneManager::isAffectedByLight( DLightSource* light, DCamera* cam )
	{
		LightType t = light->getLightType();
		if (t == LT_Directional || t == LT_Ambient) 
		{
			return true;
		}
		else if (t == LT_Point)
		{
			DReal outRange;
			light->getAttenuation(&outRange, NULL, NULL, NULL);
			DSphere lightSphere(light->getPosition(), outRange);

			if (cam->isInside(lightSphere) != DCamera::FTS_Out)
			{
				return true;
			}
		}
		else if (t == LT_Spotlight)
		{
			// TODO: calculate visiblity.
			// using common perpendicular line to calcu late intersection between camera
			// view cone and spot light cone.
			DReal cpLength;
			DRay camRay(cam->getEyePosition(), cam->getDirection());
			DRay spotRay(light->getPosition(), light->getDirection());
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
					if (r < light->getSpotlightOuterAngle()/2)
					{
						return true;
					}
				}
				else
				{
					// calculate intersection between two cone
					DReal camDist = (comPerp.getOrigin()-cam->getEyePosition()).length() * DMath::Tan(cam->getFOV()/2);
					DReal spotDist = ((comPerp.getOrigin()+comPerp.getDirection()*cpLength)
						- spotRay.getOrigin()).length() * DMath::Tan(light->getSpotlightOuterAngle()/2);
					if (camDist + spotDist > cpLength)
					{
						return true;
					}
				}

			}
		}
		return false;
	}


}