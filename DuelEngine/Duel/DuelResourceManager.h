//  [8/6/2012 OMEGA] created


#ifndef _DUELRESOURCEMANAGER_H_
#define _DUELRESOURCEMANAGER_H_

#include "DuelCommon.h"
#include "DuelException.h"
#include "DuelResource.h"


namespace Duel
{

	// a resource parameter is designed for provide necessary information
	// for a resource's creation, it should be subclassed individually with 
	// any type of resource manager, and used in resource's creation.
	class DUEL_API DResourceDescription : public DObject
	{
	DUEL_DECLARE_RTTI(DResourceDescription)
	public:
		// subclassed class should construct with this
		DResourceDescription(const DString& name, const DString& groupName, const DString& typeName)
			: mName(name), mGroupName(groupName), mResType(typeName) {}
		virtual ~DResourceDescription() {}

		// get/set the name of the resource
		const DString&	getName() { return mName; }
		void			setName(const DString& name) { mName = name; }
		// get/set the group name of the resource.
		const DString&	getGroupName() { return mGroupName; }
		void			setGroupName(const DString& groupName) { mGroupName = groupName; }

		// get/set the group name of the resource.
		const DString&	getTypeName() { return mResType; }

	protected:
		// resource's name, 
		DString	mName;
		// resource's group name, 
		DString	mGroupName;
		// the name of the type, identical to the specified DResourceManager.
		DString	mResType;
	private:
		// not provided
		DResourceDescription() {};
	};

	typedef std::vector<DResourceDescriptionPtr>	ResourceDescriptionList;

	// resource manager base class, it is designed for subclassing, used to
	// manage resource such as mesh, audio and so on, user can and only can
	// get a resource form a resource manager, and the manager may unload
	// unused resource when memory is leaking.
	class DUEL_API DResourceManager : public DObject
	{
	DUEL_DECLARE_RTTI(DResourceManager)
	public:
		DUEL_AUTO_MUTEX // public to allow external locking.
		DResourceManager(const DString& name, size_t memBudget = 0x1111/*65536*/);
		virtual ~DResourceManager();

		//////////////////////////////////////////////////////////////////////////
		// signals block
		// notify listener when a new resource is created.
		DSignal<void(DResourceManager* manager, DResourcePtr& res)>	
			signalResourceCreated;
		// notify listener when a new resource is ready to add, return true will let manager
		// add the newly created resource to its resource list.
		DSignal<bool(DResourceManager* manager, DResourcePtr& res), ResultCombiner<bool>>
			signalResourceAdding;
		// a resource is successfully added
		DSignal<void(DResourceManager* manager, DResourcePtr& res)>			
			signalResourceAdded;
		// notify listener when a resource try to remove.
		DSignal<void(DResourceManager* manager, DResourcePtr& res)>			
			signalResourceRemoving;		
		// called when the manager is out of budget, return true if it is handled.
		DSignal<bool(DResourceManager* manager), ResultCombiner<bool>>
			signalExceedBudget;

		//////////////////////////////////////////////////////////////////////////

		// get/set the name of the resource
		const DString&		getName() { return mName; }

		// create a resource using specified parameters, if the resource with the
		// same name is already exist, the existing one will be returned.
		virtual	DResourcePtr	create(DResourceDescription* createParam);

		// query whether the resource is existing in this manager.
		// if no group name specified, it will try to go through all group.
		virtual bool		isExisted(const DString& groupName, const DString& name);

		// get a resource by its name, if it is not exist, the ResoucePtr will be a shared_ptr(0).
		// if no group name specified, it will try to go through all group.
		virtual DResourcePtr	getResource(const DString& groupName, const DString& name);

		// set the limit memory amount of this manager, when a new resource is created,
		// and this manager exceed the bugdet, the manager will unload a existing resource.
		virtual	void		setMemoryBudget(size_t budget) { mBudget = budget; checkMemoryBudget(); }
		// get the memory budget.
		virtual size_t		getMemoryBudget() { return mBudget; }
		// the the current memory usage, in bytes
		virtual	size_t		getMemoryUsage() { return mBudget; }

		// unload all resources
		virtual void		unloadAll();
		// reload all resources(unload it and then reload it)
		virtual void		reloadAll();
		// unload all resources which are not refered by any other object(SharePtr that has count 2(the other one is ResourceGroup)).
		virtual	void		unloadUnused();
		// reload all resources which are not refered by any other object(SharePtr that has count 2(the other one is ResourceGroup)).
		virtual	void		reloadUnused();
		
		// remove a resource from manager, it can no longer belongs to this manager, unload will be executed.
		// but other objects may still using this resource, so this resource will not destroy immediately,
		virtual void		remove(DResourcePtr res);
		// remove all resources
		virtual	void		removeAll();
		// remove unused resource, i.e resource which is not refered by other objects
		virtual	void		removeUnused();

		// query how many resources this manager have.
		uint32				getResourceCount() { return mResourceCount; }

		typedef	HashMap<DString, DResourcePtr>	ResourceMap;
		typedef	MapIterator<ResourceMap>		ResourceIterator;
		// use this method to traverse all resrouces created in this resource manager.
		ResourceIterator	getResourceIterator(const DString& groupName);

		// if some resourceManager need to parse script or config to generate a 
		// resource Parameter indeed, rewrite this method.
		virtual ResourceDescriptionList parseParameter(DDataStream* data)
		{ 
			return ResourceDescriptionList();
		}

	protected:

		// slots called by dresources.
		INTERNAL void		onResourceLoadingCompleted(DResource* res);
		INTERNAL void		onResourceGroupChanged(DResource* newRes, const DString& oldGroup);
		INTERNAL void		onResourceUnloadCompleted(DResource* res);


		// check memory usage and notify listeners, 
		// return true if it passed check.
		bool				checkMemoryBudget();
		// change a resource group.
		void				changeGroup(DResource* newRes, const DString& oldGroup);
		// sub-class must override this method 
		virtual DResource*	createImpl(DResourceDescription* createParam) = 0;
		// sub-class could rewrite this method if any thing need to do when removing.
		virtual void		removeImpl(DResource* res) {}
		// add a newly created resource to current manager.
		virtual void		addResource(DResourcePtr& res);


		// unique name of DResourceManager.
		DString				mName;
		// use name as keyword to restore resources.
		typedef HashMap<DString, ResourceMap>	GroupMap;
		GroupMap			mGroupMap;

		// record resource count.
		uint32				mResourceCount;

		// memory budget, in bytes
		size_t				mBudget;
		// currently used memory. in bytes
		size_t				mMemUsage;

	private:
		// not allowed.
		DResourceManager(){}
	};


}



#endif