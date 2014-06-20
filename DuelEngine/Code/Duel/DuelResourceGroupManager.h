//  [9/19/2012 OMEGA] created

#ifndef	_DUELRESOURCEGROUPMANAGER_H_
#define _DUELRESOURCEGROUPMANAGER_H_

#include "DuelCommon.h"
#include "DuelResource.h"
#include "DuelResourceManager.h"
#include "DuelArchive.h"

namespace Duel
{

	// group name can not contain "/" 
	// pre declaration.
	class ResourceGroupListener;
	// a ResourceGroup is a collection of resources which share the same group name,
	// ResourceGroup may have many different Archives, it also has a list of 
	// name that used to record Resources.
	class DUEL_API DResourceGroup : public DObject
	{
	DUEL_DECLARE_RTTI(DResourceGroup)
	public:
		DResourceGroup(const DString& name) : mName(name) {}
		virtual ~DResourceGroup();
		const DString&	getName() { return mName; }

		// add an archive using a archive name, this method will
		// get Archive from ArchiveManager.
		void	addArchive(const DString& archiveName);
		// remove an archive with specified name(if it does exist)
		void	removeArchive(const DString& archiveName);
		// get a resource's Archive, if resourceName exist in different
		// Archive, only the first reached one can be return.
		DArchivePtr	getArchive(const DString& resourceName);
		// query if an archive exist.
		bool	isArchiveExisted(const DString& archiveName) const;

		// declare a resource using its creation parameter, but not
		// create it yet.
		void	declareResource(DResourceDescriptionPtr param);
		// undelcare a resource declaration using specified resource name.
		void	undeclareResource(const DString& name);
		// query whether a resource is declared.
		bool	isResourceDeclared(const DString& name) const;

		// add a created resource
		void	addActiveResource(DResourcePtr res);
		// remove a active resource
		void	removeActiveResource(const DString& name);
		// query a resource's existence
		bool	isResourceActive(const DString& name) const;

		// get resource's parameter using specified name.
		DResourceDescriptionPtr	getResourceDescription(const DString& name);


		// archive map, used for quick look up.
		typedef	HashMap<DString, DArchivePtr>	ArchiveMap;
		typedef ConstMapIterator<ArchiveMap>	ArchiveIterator;
		ArchiveIterator	getArchiveIterator() const { return ArchiveIterator(mArchiveMap); }
		// resource map, used to record declared resources.
		typedef	HashMap<DString, DResourceDescriptionPtr>	ResourceDeclarationMap;
		typedef ConstMapIterator<ResourceDeclarationMap>	ResourceDeclarationIterator;
		ResourceDeclarationIterator	getResourceDeclarationIterator() const { return ResourceDeclarationIterator(mDeclarationMap); }
		// resources which are created from this group.
		typedef	HashMap<DString, DResourcePtr>	ActiveResourceMap;
		typedef ConstMapIterator<ActiveResourceMap>	ActiveResourceIterator;
		ActiveResourceIterator	getActiveResourceIterator() const { return ActiveResourceIterator(mActiveResourceMap); }

	protected:
		// group name.
		DString	mName;
		ArchiveMap	mArchiveMap;
		ResourceDeclarationMap	mDeclarationMap;
		ActiveResourceMap	mActiveResourceMap;

	};
	// ResourceGroupManager is a manager class that handles ResourceGroups, 
	// ResourceGroup is a colletcion of resources, no matter what types they are,
	// they share one group name, for example, stage1, stage2 etc. a ResourceGroup
	// can own many Archives, from which the specified resource can be created.
	// DResource have three state: unknown, declared, loaded. All resources in the
	// specified Archive could be unknown cause no one know what to do with them,
	// this will happened to those resources located in the new added Archive, but
	// after XML parsing, some of them could change state into declared, it was 
	// registered to specified DResourceManager and can be obtained from the manager,
	// but at that time, resource is still not loaded, you can use DResource::load to
	// load it, then you can use it as you wish. resources should be removed through
	// its own manager, not through ResourceGroupManager, so does creation.
	class DUEL_API DResourceGroupManager : public DObject, public DSingleton<DResourceGroupManager>
	{
	DUEL_DECLARE_RTTI(DResourceGroupManager)
	public:
		DResourceGroupManager();
		virtual ~DResourceGroupManager();
// 		static	ResourceGroupManager&	getSingleton(void);
// 		static	ResourceGroupManager*	getSingletonPtr(void);
		//////////////////////////////////////////////////////////////////////////
		// signals block
		// when a newly created resource trying to add into resource group,
		// it may cause a insertion conflict, e.g two resource have the same
		// name and group name. return true will make the resource try to 
		// insert again.
		DSignal<bool(DResourceManager* manager, DResourcePtr res), ResultCombiner<bool>>	
			signalResourceCollided;
		// called when a group is destrying.
		DSignal<void(const DString& groupName)>	
			signalGroupDestroyed;

		//////////////////////////////////////////////////////////////////////////

		// used to traverse groups
		typedef	HashMap<DString, DResourceGroup*>	ResourceGroupMap;
		typedef	ConstMapIterator<ResourceGroupMap>	ResourceGroupIterator;
		ResourceGroupIterator	getResourceGroupIterator() { return ResourceGroupIterator(mGroupMap.begin(), mGroupMap.end()); }

		// create a resource group, if it is existed, do nothing.
		const DResourceGroup*	createGroup(const DString& name);
		// get a resource by name, if the specified group does 
		// not exist, return NULL or a new ResourceGroup(if forceCreate is true)
		// this method should will not provide access to ResourceGroup for other class.
		const DResourceGroup*	getResourceGroup(const DString& name);
		// remove a existing resource group using specified name.
		void	destroyGroup(const DString& name);
		// query whether a group exist.
		bool	isGroupExisted(const DString& name);
		

		// add a resource location to specified resource.
		void	addLocation(const DString& groupName, const DString& location);
		// remove a resource location with specified group name, and location, 
		// if they both exist.
		void	removeLocation(const DString& groupName, const DString& location);
		// query whether the specified group and location exist.
		bool	isLocationExisted(const DString& groupName, const DString& location);

		// register a DResourceManager, for declaring resources.
		void	registerManager(DResourceManager* manager);
		// unregister a DResourceManager.
		void	unregisterManager(DResourceManager* manager);
		

		// get a registered resource manager, using specified name, identical to resource's typeName.
		DResourceManager*	getResouceManager(const DString& name);

		// declare a resource, register it to specified DResourceManager.
		void	declareResource(DResourceDescriptionPtr param);
		// undelcare a resource declaration from a group using specified resource name.
		// this method will not affect created resource, nor will erase the resource from
		// specified DResourceManager.
		void	undeclareResource(const DString& groupName, const DString& name);
		// query whether a resource is declared.
		bool	isResourceDeclared(const DString& groupName, const DString& name);
		// create declared resource, if the resource has been created, omit it.
		void	createDeclaredResource(const DString& groupName);
		// query whether a resource is active in specified group.
		bool	isResourceActive(const DString& groupName, const DString& name);

		// resource can get data from this method only, use it's own
		// pointer as key.
		DDataStreamPtr	openResource(DResource* res);

		// pre load defined groups & locations from xml file,
		// the format should look like, notice that location is an Archive.
		// <GroupDescription>
		//		<Group name="lalalala">
		//			<Location>lalalala</Location>
		//			<Location>lllll</Location>
		//		</Group>
		//		<Group name="dadadada">
		//			<Location>dadadada</Location>
		//		</Group>
		//		........
		// </GroupDescription>

		// load groups & locations from specified XML file.
		void	loadFromXML(const DString& configFileName);
		// the xmlData should be valid xml document.
		void	loadFromXML(DDataStream* xmlData);
		// use a given Node to load  groups & locations, notice,
		// the startNode should have name "GroupDescription"
		void	loadFromXML(DXMLNode* startNode);


	private:
		// used with event listener
		// notify all listeners that a resource collision happened,
		// return true if it is successfully handled.
		bool	handleCollision(DResourceManager* manager, DResourcePtr res);

		// internal function used to listen to the manager.
		INTERNAL bool	onManagerResourceAdding(DResourceManager* manager, DResourcePtr& res);
		INTERNAL void	onManagerResourceAdded(DResourceManager* manager, DResourcePtr& res);
		INTERNAL void	onManagerResourceRemoving(DResourceManager* manager, DResourcePtr& res);

		// add newly created resource, this method will not provide, if you need create resource, use
		// corresponding DResourceManager.
		void	addActiveResource(DResourcePtr res);

		// remove resource so we will no longer handle it.
		void	removeActiveResource(DResourcePtr res);

		// DResource group map.
		ResourceGroupMap	mGroupMap;
		// a map between resource type and DResourceManager, all declared resource should
		// be registered to appropriate DResourceManager.
		typedef HashMap<DString, DResourceManager*>	ResourceManagerMap;
		ResourceManagerMap	mManagerMap;


	};


}

#endif
