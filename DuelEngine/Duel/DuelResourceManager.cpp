//  [8/6/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelException.h"
#include "DuelLogManager.h"
#include "DuelResourceManager.h"
#include "DuelResource.h"


namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DResourceDescription, DObject);
	DUEL_IMPLEMENT_RTTI_1(DResourceManager, DObject);


	DResourceManager::DResourceManager(const DString& name, size_t memBudget /*65536*/)
		: mMemUsage(0), mBudget(memBudget), mResourceCount(0), mName(name)
	{
	}

	DResourceManager::~DResourceManager()
	{
		// TODO: remove resources.
	}

	DResourcePtr DResourceManager::create( DResourceDescription* createParam )
	{
		assert(createParam != NULL);
		if (isExisted(createParam->getName(),createParam->getGroupName()))
		{
			return getResource(createParam->getGroupName(), createParam->getName());
		}
		DResourcePtr ret = DResourcePtr(createImpl(createParam));
		if (ret != NULL)
		{
			ret->signalLoadingCompleted.connect(
				DBind(&DResourceManager::onResourceLoadingCompleted, this, _1));
			ret->signalGroupChanged.connect(
				DBind(&DResourceManager::onResourceGroupChanged, this, _1, _2));
			ret->signalUnloadCompleted.connect(
				DBind(&DResourceManager::onResourceUnloadCompleted, this, _1));
			// signal emit
			signalResourceCreated(this, ret);
			addResource(ret);
		}
		return	ret;
	}


	bool DResourceManager::isExisted( const DString& groupName, const DString& name )
	{
		DResourcePtr r = getResource(groupName, name);
		if (r == NULL)
		{
			return false;
		}
		return true;
	}

	Duel::DResourcePtr DResourceManager::getResource( const DString& groupName, const DString& name )
	{
		DResourcePtr ret;
		GroupMap::iterator gi;
		DUEL_LOCK_AUTO_MUTEX
		if (groupName == "")
		{
			for (gi = mGroupMap.begin(); gi != mGroupMap.end(); ++gi)
			{
				ResourceMap::iterator ri = gi->second.find(name);
				if (ri != gi->second.end())
				{
					return ri->second;
				}
				else
				{
					continue;
				}
			}
			return ret;
		}
		else
		{
			gi = mGroupMap.find(groupName);
			if (gi != mGroupMap.end())
			{
				ResourceMap::iterator ri = gi->second.find(name);
				if (ri != gi->second.end())
				{
					return ri->second;
				}
				else
				{
					return ret;
				}
			}
			else
			{
				return ret;
			}
		}
	}


	void DResourceManager::unloadAll()
	{
		// just go through .~
		GroupMap::iterator gi;
		for(gi = mGroupMap.begin(); gi != mGroupMap.end(); ++gi)
		{
			ResourceMap::iterator ri;
			for (ri = gi->second.begin(); ri != gi->second.end(); ++ri)
			{
				ri->second->unload();
			}
		}
	}

	void DResourceManager::reloadAll()
	{
		// just go through .~
		GroupMap::iterator gi;
		ResourceMap::iterator ri;
		for(gi = mGroupMap.begin(); gi != mGroupMap.end(); ++gi)
		{
			for (ri = gi->second.begin(); ri != gi->second.end(); ++ri)
			{
				ri->second->reload();
			}
		}
	}

	void DResourceManager::unloadUnused()
	{
		// just go through .~
		GroupMap::iterator gi;
		for(gi = mGroupMap.begin(); gi != mGroupMap.end(); ++gi)
		{
			ResourceMap::iterator ri;
			for (ri = gi->second.begin(); ri != gi->second.end(); ++ri)
			{
				// we know that only ResourceGroupManager and DResourceManager
				// hold the unused resource.
				if (ri->second.use_count() == 2)
				{
					ri->second->unload();
				}
			}
		}
	}

	void DResourceManager::reloadUnused()
	{
		// just go through .~
		GroupMap::iterator gi;
		for(gi = mGroupMap.begin(); gi != mGroupMap.end(); ++gi)
		{
			ResourceMap::iterator ri;
			for (ri = gi->second.begin(); ri != gi->second.end(); ++ri)
			{
				// we know that only ResourceGroupManager and DResourceManager
				// hold the unused resource.
				if (ri->second.use_count() == 2)
				{
					ri->second->reload();
				}
			}
		}
	}

	void DResourceManager::remove( DResourcePtr res )
	{
		DUEL_LOCK_AUTO_MUTEX

		// find it and remove it from ResourceMap.
		GroupMap::iterator gi = mGroupMap.find(res->getGroupName());
		ResourceMap::iterator ri = gi->second.find(res->getName());
		if(ri != gi->second.end())
		{
			if (ri->second == res)
			{
				// remove listener from that resource.
				res->signalLoadingCompleted.disconnect(
					DBind(&DResourceManager::onResourceLoadingCompleted, this, _1));
				res->signalGroupChanged.disconnect(
					DBind(&DResourceManager::onResourceGroupChanged, this, _1, _2));
				res->signalUnloadCompleted.disconnect(
					DBind(&DResourceManager::onResourceUnloadCompleted, this, _1));
				signalResourceRemoving(this, ri->second);
				// truly remove happend here.
				removeImpl(res.get());
				gi->second.erase(ri);
				// ah we lose a resource.
				mResourceCount--;
			}
		}
	}

	void DResourceManager::removeAll()
	{
		DUEL_LOCK_AUTO_MUTEX
		// just go through .~
		GroupMap::iterator gi;
		ResourceMap::iterator ri;
		for(gi = mGroupMap.begin(); gi != mGroupMap.end(); ++gi)
		{
			for (ri = gi->second.begin(); ri != gi->second.end(); )
			{
				// remove listener from that resource.
				ri->second->signalLoadingCompleted.disconnect(
					DBind(&DResourceManager::onResourceLoadingCompleted, this, _1));
				ri->second->signalGroupChanged.disconnect(
					DBind(&DResourceManager::onResourceGroupChanged, this, _1, _2));
				ri->second->signalUnloadCompleted.disconnect(
					DBind(&DResourceManager::onResourceUnloadCompleted, this, _1));
				signalResourceRemoving(this, ri->second);
				// truly remove happend here.
				removeImpl(ri->second.get());
				// delete record in map, use this way.
				gi->second.erase(ri++);
				// ah we lose a resource.
				mResourceCount--;
			}
		}
	}

	void DResourceManager::removeUnused()
	{
		DUEL_LOCK_AUTO_MUTEX
		// just go through .~
		GroupMap::iterator gi;
		ResourceMap::iterator ri;
		for(gi = mGroupMap.begin(); gi != mGroupMap.end(); ++gi)
		{
			for (ri = gi->second.begin(); ri != gi->second.end();)
			{
				// we know that only ResourceGroupManager and DResourceManager
				// hold the unused resource.
				if (ri->second.use_count() == 2)
				{
					// remove listener from that resource.
					ri->second->signalLoadingCompleted.disconnect(
						DBind(&DResourceManager::onResourceLoadingCompleted, this, _1));
					ri->second->signalGroupChanged.disconnect(
						DBind(&DResourceManager::onResourceGroupChanged, this, _1, _2));
					ri->second->signalUnloadCompleted.disconnect(
						DBind(&DResourceManager::onResourceUnloadCompleted, this, _1));

					signalResourceRemoving(this, ri->second);
					// truly remove happend here.
					removeImpl(ri->second.get());
					// delete record in map, use this way.
					gi->second.erase(ri++);
					// ah we lose a resource.
					mResourceCount--;
				}
				else
				{
					ri++;
				}
			}
		}
	}

	bool DResourceManager::checkMemoryBudget()
	{
		if (mMemUsage <= mBudget)
		{
			return true;
		}
		else
		{		
			// we out of memory budget, if there is no listener try to handle it
			// it will return false.
			bool isHandled = false;
			std::vector<bool> result = signalExceedBudget(this);
			std::vector<bool>::iterator i, iend = result.end();
			for (i = result.begin(); i != iend; ++i)
			{
				if(*i == true)
				{
					isHandled = true;
					break;
				}
			}
			return isHandled;
		}
	}



	void DResourceManager::changeGroup( DResource* newRes, const DString& oldGroup )
	{
		// find it and remove it from ResourceMap.
		// check whether the res is truly under manager's control.
		GroupMap::iterator gi = mGroupMap.find(oldGroup);
		ResourceMap::iterator ri;
		DResourcePtr tempRes;
		if (gi != mGroupMap.end())
		{
			ri = gi->second.find(newRes->getName());
			if (ri != gi->second.end())
			{
				// test wheter the resource is truly under our control.
				if (tempRes.get() == newRes)
				{
					tempRes = ri->second;
					signalResourceRemoving(this, ri->second);

					gi->second.erase(ri);
					mResourceCount -= 1;
					// add it to the manager again.
					addResource(tempRes);
				}
			}
		}
	}


	void DResourceManager::addResource(DResourcePtr& res )
	{
		DUEL_LOCK_AUTO_MUTEX
		bool shouldAdd = true;
		std::vector<bool> result = signalResourceAdding(this, res);
		std::vector<bool>::iterator i, iend = result.end();
		for (i = result.begin(); i != iend; ++i)
		{
			if (*i == false)
			{
				shouldAdd = false;
				break;
			}
		}

		if (shouldAdd)
		{
			mGroupMap[res->getGroupName()][res->getName()] = res;
			mResourceCount += 1;
			signalResourceAdded(this, res);
		}
		else
		{
			// no longer handle it
			res.reset();
		}
	}


	INTERNAL void DResourceManager::onResourceLoadingCompleted( DResource* res )
	{
		mMemUsage += res->getSize();
		// check memory usage and handle memory leak.
		if(!(checkMemoryBudget()))
		{
			DLogManager::getSingleton().logMessage("DResourceManager.log",
				"DResourceManager: " + getName() + " out of budget, after " + res->getName() + " in " 
				+ res->getGroupName() + " loaded with " + DStringTool::toString(res->getSize())
				+ ",\n current budget: " + DStringTool::toString(getMemoryBudget())
				+ " current usage: " + DStringTool::toString(getMemoryUsage()));
		}

	}

	INTERNAL void DResourceManager::onResourceGroupChanged( DResource* newRes, const DString& oldGroup )
	{
		changeGroup(newRes, oldGroup);
	}

	INTERNAL void DResourceManager::onResourceUnloadCompleted( DResource* res )
	{
		mMemUsage -= res->getSize();

	}

	DResourceManager::ResourceIterator DResourceManager::getResourceIterator( const DString& groupName )
	{
		static ResourceMap staticResMap;
		GroupMap::iterator i = mGroupMap.find(groupName);
		if (i != mGroupMap.end())
		{
			return ResourceIterator(i->second);
		}
		return ResourceIterator(staticResMap);
	}

}