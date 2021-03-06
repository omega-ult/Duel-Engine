//  [12/21/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelCamera.h"
#include "DuelAutoGpuParameter.h"
#include "DuelRenderQueue.h"

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(DRenderQueue, DAutoGpuParameterDelegate);

	DRenderQueue::DRenderQueue() : 
		mRenderCamera(NULL)
	{

	}

	DRenderQueue::~DRenderQueue()
	{
		destroyAllRenderGroups();
	}

	void DRenderQueue::setRenderCamera( DCamera* cam )
	{
		mRenderCamera = cam;
	}

	DCamera* DRenderQueue::getRenderCamera()
	{
		return mRenderCamera;
	}

	void DRenderQueue::addRenderable( uint32 groupID, DRenderable* rend )
	{
		RenderGroupMap::iterator i = mGroupMap.find(groupID);
		if (i == mGroupMap.end())
		{
			DRenderGroup* group = new DRenderGroup(this, groupID);
			group->addRenderable(rend);
			mGroupMap[groupID] = group;
		}
		else
		{
			i->second->addRenderable(rend);
		}
	}

	DRenderGroup* DRenderQueue::createRenderGroup( uint32 groupID )
	{
		RenderGroupMap::iterator i = mGroupMap.find(groupID);
		if (i == mGroupMap.end())
		{
			DRenderGroup* group = new DRenderGroup(this, groupID);
			mGroupMap[groupID] = group;
			return group;
		}
		else
		{
			return i->second;
		}
	}

	DRenderGroup* DRenderQueue::getRenderGroup( uint32 groupID )
	{
		RenderGroupMap::iterator i = mGroupMap.find(groupID);
		if (i == mGroupMap.end())
		{
			return NULL;
		}
		else
		{
			return i->second;
		}
	}

	void DRenderQueue::destroyRenderGroup( uint32 groupID )
	{
		RenderGroupMap::iterator i = mGroupMap.find(groupID);
		if (i != mGroupMap.end())
		{
			delete i->second;
			mGroupMap.erase(i);
		}
	}

	void DRenderQueue::destroyAllRenderGroups()
	{
		RenderGroupMap::iterator i, iend = mGroupMap.end();
		for (i = mGroupMap.begin(); i != iend; ++i)
		{
			delete i->second;
		}
		mGroupMap.clear();
	}

	void DRenderQueue::clear()
	{
		RenderGroupMap::iterator i, iend = mGroupMap.end();
		for (i = mGroupMap.begin(); i != iend; ++i)
		{
			i->second->clear();
		}
		clearAllLights();
		removeAllPostEffects();
	}

	void DRenderQueue::addLight( DLightSource* light )
	{
		mLightSet.insert(light);
	}


	void DRenderQueue::clearAllLights()
	{
		mLightSet.clear();
	}

	void DRenderQueue::addPostEffect( DPostEffectInstancePtr pe )
	{
		mPostEffectList.push_back(pe);
	}

	void DRenderQueue::removePostEffect( DPostEffectInstancePtr pe )
	{
		PostEffectList::iterator i, iend = mPostEffectList.end();
		for (i = mPostEffectList.begin(); i != iend; ++i)
		{
			if (*i == pe)
			{
				mPostEffectList.erase(i);
				return;
			}
		}
	}

	void DRenderQueue::removeAllPostEffects()
	{
		mPostEffectList.clear();
	}

	DMatrix4 DRenderQueue::getViewMatrix()
	{
		if (mRenderCamera == NULL)
		{
			return DMatrix4::IDENTITY;
		}
		return mRenderCamera->getViewMatrix();
	}

	DMatrix4 DRenderQueue::getProjectionMatrix()
	{
		if (mRenderCamera == NULL)
		{
			return DMatrix4::IDENTITY;
		}
		return mRenderCamera->getProjectionMatrix();
	}

	Duel::DMatrix4 DRenderQueue::getInverseViewMatrix()
	{
		if (mRenderCamera == NULL)
		{
			return DMatrix4::IDENTITY;
		}
		return mRenderCamera->getInverseViewMatrix();
	}

	Duel::DMatrix4 DRenderQueue::getInverseProjectionMatrix()
	{
		if (mRenderCamera == NULL)
		{
			return DMatrix4::IDENTITY;
		}
		return mRenderCamera->getInverseProjectionMatrix();
	}

}