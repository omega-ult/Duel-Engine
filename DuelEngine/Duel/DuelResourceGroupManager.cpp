//  [9/19/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelException.h"
#include "DuelArchive.h"
#include "DuelFileStream.h"
#include "DuelArchiveManager.h"
#include "DuelResourceGroupManager.h"
#include "DuelLogManager.h"

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(DResourceGroup, DObject);
	DUEL_IMPLEMENT_RTTI_1(DResourceGroupManager, DObject);

	//-------------------------------------

	DResourceGroup::~DResourceGroup()
	{
// 		ActiveResourceMap::iterator i;
// 		for(i = mActiveResourceMap.begin(); i != mActiveResourceMap.end(); ++i)
// 		{
// 			i->second->getManager()->remove(i->second);
// 		}
	}


	void DResourceGroup::addArchive( const DString& archiveName )
	{
		// ArchiveManager will handle exception, so we just do it.
		DArchivePtr a = DArchiveManager::getSingleton().getArchive(archiveName);
		mArchiveMap[archiveName] = a;
	}

	void DResourceGroup::removeArchive( const DString& archiveName )
	{
		ArchiveMap::iterator i;
		i = mArchiveMap.find(archiveName);
		if (i != mArchiveMap.end())
		{
			i->second.reset();
			mArchiveMap.erase(i);
		}
		else
		{
			DUEL_EXCEPT(DException::ET_ItemNotFound,
				"Specified Archive named:" + archiveName + "not found",
				"Specified Archive named:" + archiveName + "not found, check its existence before remove it",
				"Duel::ResourceGroup::removeArchive")
		}
	}


	DArchivePtr DResourceGroup::getArchive( const DString& resourceName )
	{
		ArchiveMap::iterator i;
		for (i = mArchiveMap.begin(); i != mArchiveMap.end(); ++i)
		{
			if (i->second->isExisted(resourceName))
			{
				return i->second;
			}
		}
		return DArchivePtr();
	}

	bool DResourceGroup::isArchiveExisted( const DString& archiveName ) const
	{
		ArchiveMap::const_iterator i;
		i = mArchiveMap.find(archiveName);
		if (i != mArchiveMap.end())
		{
			return true;
		}
		return false;
	}

	void DResourceGroup::declareResource( DResourceDescriptionPtr param )
	{
		mDeclarationMap[param->getName()] = param;
	}

	void DResourceGroup::undeclareResource( const DString& name )
	{
		ResourceDeclarationMap::iterator di = mDeclarationMap.find(name);
		if (di != mDeclarationMap.end())
		{
			mDeclarationMap.erase(di);
		}
	}

	bool DResourceGroup::isResourceDeclared( const DString& name ) const
	{
		ResourceDeclarationMap::const_iterator i = mDeclarationMap.find(name);
		if (i != mDeclarationMap.end())
		{
			return true;
		}
		return false;
	}

	DResourceDescriptionPtr DResourceGroup::getResourceDescription( const DString& name )
	{
		ResourceDeclarationMap::iterator i = mDeclarationMap.find(name);
		if (i != mDeclarationMap.end())
		{
			return i->second;
		}
		return DResourceDescriptionPtr();
	}

	void DResourceGroup::addActiveResource( DResourcePtr res )
	{
		mActiveResourceMap[res->getName()] = res;
	}

	void DResourceGroup::removeActiveResource( const DString& name )
	{
		ActiveResourceMap::iterator i = mActiveResourceMap.find(name);
		if (i != mActiveResourceMap.end())
		{
			mActiveResourceMap.erase(i);
		}
	}

	bool DResourceGroup::isResourceActive( const DString& name ) const
	{
		ActiveResourceMap::const_iterator i = mActiveResourceMap.find(name);
		return i != mActiveResourceMap.end();
	}


	//----------------------------------------



	DResourceGroupManager::DResourceGroupManager()
	{
	}

	DResourceGroupManager::~DResourceGroupManager()
	{
		ResourceGroupMap::iterator i, iend = mGroupMap.end();
		for(i = mGroupMap.begin(); i != iend; ++i)
		{
			delete i->second;
		}
		mGroupMap.clear();
	}

	const DResourceGroup* DResourceGroupManager::createGroup( const DString& name )
	{
		ResourceGroupMap::iterator i;
		i = mGroupMap.find(name);
		if (i != mGroupMap.end())
		{
			return i->second;
		}
		else
		{
			DResourceGroup* newGrp = new DResourceGroup(name);
			mGroupMap[name] = newGrp;
			return newGrp;
		}
	}

	void DResourceGroupManager::destroyGroup( const DString& name )
	{
		ResourceGroupMap::iterator i;
		i = mGroupMap.find(name);
		if (i != mGroupMap.end())
		{
			signalGroupDestroyed(name);
			delete i->second;
			mGroupMap.erase(i);
		}
	
	}

	bool DResourceGroupManager::isGroupExisted( const DString& name )
	{
		return getResourceGroup(name) != NULL;
	}



	const DResourceGroup* DResourceGroupManager::getResourceGroup( const DString& name)
	{
		ResourceGroupMap::iterator i;
		i = mGroupMap.find(name);
		if (i != mGroupMap.end())
		{
			return i->second;
		}
		return NULL;
	}

	void DResourceGroupManager::addLocation( const DString& groupName, const DString& location )
	{
		ResourceGroupMap::iterator g = mGroupMap.find(groupName);
		if (g != mGroupMap.end())
		{
			g->second->addArchive(location);
		}
	}

	void DResourceGroupManager::removeLocation( const DString& groupName, const DString& location )
	{
		ResourceGroupMap::iterator g = mGroupMap.find(groupName);
		if (g != mGroupMap.end())
		{
			g->second->removeArchive(location);
		}
	}

	bool DResourceGroupManager::isLocationExisted( const DString& groupName, const DString& location )
	{
		ResourceGroupMap::iterator g = mGroupMap.find(groupName);
		if (g != mGroupMap.end())
		{
			return g->second->isArchiveExisted(location);
		}
		else
		{
			return NULL;
		}
	}

	void DResourceGroupManager::registerManager( DResourceManager* manager )
	{
		if (manager == NULL)
		{
			return;
		}
		mManagerMap[manager->getName()] = manager;
		manager->signalResourceAdding.connect(
			DBind(&DResourceGroupManager::onManagerResourceAdding, this, _1, _2));
		manager->signalResourceAdded.connect(
			DBind(&DResourceGroupManager::onManagerResourceAdded, this, _1, _2));
		manager->signalResourceRemoving.connect(
			DBind(&DResourceGroupManager::onManagerResourceRemoving, this, _1, _2));

	}

	void DResourceGroupManager::unregisterManager( DResourceManager* manager )
	{
		if (manager == NULL)
		{
			return;
		}
		ResourceManagerMap::iterator i;
		i = mManagerMap.find(manager->getName());
		if (i != mManagerMap.end())
		{
			//i->second->unloadAll();		
			manager->signalResourceAdding.disconnect(
				DBind(&DResourceGroupManager::onManagerResourceAdding, this, _1, _2));
			manager->signalResourceAdded.disconnect(
				DBind(&DResourceGroupManager::onManagerResourceAdded, this, _1, _2));
			manager->signalResourceRemoving.disconnect(
				DBind(&DResourceGroupManager::onManagerResourceRemoving, this, _1, _2));

			mManagerMap.erase(i);
		}
	}

	void DResourceGroupManager::declareResource( DResourceDescriptionPtr param )
	{
		if (param == NULL)
		{
			return;
		}
		ResourceGroupMap::iterator g = mGroupMap.find(param->getGroupName());
		if (g != mGroupMap.end())
		{
			g->second->declareResource(param);
		}
	}

	void DResourceGroupManager::undeclareResource( const DString& groupName, const DString& name )
	{
		ResourceGroupMap::iterator g = mGroupMap.find(groupName);
		if (g != mGroupMap.end())
		{
			g->second->undeclareResource(name);
		}
	}

	bool DResourceGroupManager::isResourceDeclared( const DString& groupName, const DString& name )
	{
		ResourceGroupMap::iterator g = mGroupMap.find(groupName);
		if (g != mGroupMap.end())
		{
			return g->second->isResourceDeclared(name);
		}
		return false;
	}

	DResourceManager* DResourceGroupManager::getResouceManager( const DString& name )
	{
		ResourceManagerMap::iterator i = mManagerMap.find(name);
		if (i != mManagerMap.end())
		{
			return i->second;
		}
		return NULL;
	}

	void DResourceGroupManager::createDeclaredResource(const DString& groupName)
	{
		ResourceGroupMap::iterator g = mGroupMap.find(groupName);
		if (g == mGroupMap.end())
		{
			return;
		}
		DResourceGroup* gp = g->second;
		DResourceGroup::ResourceDeclarationIterator di = gp->getResourceDeclarationIterator();
		DResourceManager* manager = NULL;
		while (di.hasMoreElements())
		{
			DResourceDescriptionPtr desc = di.getNext();
			// if it is created, pass over.
			if (!(gp->isResourceActive(desc->getName())))
			{
				manager = getResouceManager(desc->getTypeName());
				if (manager != NULL)
				{
					manager->create(desc.get());
				}
			}
		}
	}


	bool DResourceGroupManager::isResourceActive( const DString& groupName, const DString& name )
	{
		ResourceGroupMap::iterator g = mGroupMap.find(groupName);
		if (g != mGroupMap.end())
		{
			return g->second->isResourceActive(name);
		}
		return false;
	}

	bool DResourceGroupManager::handleCollision( DResourceManager* manager, DResourcePtr res )
	{
		bool result = false;
		std::vector<bool> sigResult = signalResourceCollided(manager, res);
		std::vector<bool>::iterator i, iend = sigResult.end();
		for(i = sigResult.begin(); i != iend; ++i)
		{
			if (*i == true)
			{
				result = true;
			}
		}
		return result;
	}


	void DResourceGroupManager::addActiveResource( DResourcePtr res )
	{
		ResourceGroupMap::iterator g = mGroupMap.find(res->getGroupName());
		if (g != mGroupMap.end())
		{
			g->second->addActiveResource(res);
		}
		else
		{
			createGroup(res->getGroupName());
			g = mGroupMap.find(res->getGroupName());
			g->second->addActiveResource(res);
		}
	}


	void DResourceGroupManager::removeActiveResource( DResourcePtr res )
	{
		ResourceGroupMap::iterator g = mGroupMap.find(res->getGroupName());
		if (g != mGroupMap.end())
		{
			g->second->removeActiveResource(res->getName());
		}
	}

	Duel::DDataStreamPtr DResourceGroupManager::openResource( DResource* res )
	{
		ResourceGroupMap::iterator g = mGroupMap.find(res->getGroupName());
		if (g == mGroupMap.end())
		{
			return DDataStreamPtr();
		}
		DArchivePtr arch = g->second->getArchive(res->getName());
		if (arch != NULL)
		{
			return arch->open(res->getName());
		}
		return DDataStreamPtr();
	}

	void DResourceGroupManager::loadFromXML( const DString& configFileName )
	{
		DDataStreamPtr filePtr = DDataStreamPtr(new DFileStream(configFileName));
		if (!filePtr->isValid())
		{
#ifdef DUEL_DEBUG
			DUEL_EXCEPT(DException::ET_FileNotFound,
				"Open file named: " + configFileName + " failed",
				"Open file named: " + configFileName + " failed",
				"Duel::ResourceGroupManager::loadFromXML")
#endif
				return;
		}

		loadFromXML(filePtr.get());
	}

	void DResourceGroupManager::loadFromXML( DDataStream* xmlData )
	{
		// if data is empty.
		if (xmlData == NULL)
		{
			return;
		}
		if (!xmlData->isValid())
		{
			return;
		}
		DXMLDoc	doc;
		// because the xml parsing is destructive, we use 
		// embeded DXMLDoc's memory pool to handle memory.
		char* xmlContent;
		size_t size = xmlData->getSize();
		// spare 1 char for the last '\0'
		xmlContent = doc.allocate_string(0, size+1);
		xmlData->read(xmlContent, size);
		xmlContent[size] = '\0';


		doc.parse<0>(xmlContent);
		DXMLNode* listNode = doc.first_node("GroupDescription");
		if (listNode == NULL)
		{
			// failed to parse xml, is anything wrong with this file?
#ifdef DUEL_DEBUG
			DUEL_EXCEPT(DException::ET_ItemNotFound,
				"Error in parsing XML",
				"Error in parsing XML, no node named: 'GroupDescription' found",
				"Duel::ResourceGroupManager::loadFromXML")
#endif
				return;
		}
		loadFromXML(listNode);
	}

	void DResourceGroupManager::loadFromXML( DXMLNode* startNode )
	{
		// validate input.
		if (startNode == NULL)
		{
			return;
		}
		else if (strcmp(startNode->name(), "GroupDescription") != 0)
		{
			return;
		}

		DXMLNode* recordNode = startNode->first_node("Group");
		DXMLNode* contentNode;
		DXMLAttribute*	nameAttr;
		DString	name;
		DString	location;
		while(recordNode)
		{
			nameAttr = recordNode->first_attribute("name");
			name = DXMLTool::readValue(nameAttr);
			createGroup(name);
			contentNode = recordNode->first_node("LocationList");
			if (contentNode != NULL)
			{
				contentNode = contentNode->first_node("Location");
				while(contentNode != NULL)
				{
					location = DXMLTool::readValue(contentNode);
					// just add it.
					addLocation(name, location);
					contentNode = contentNode->next_sibling("Location");
				}
			}
			recordNode = recordNode->next_sibling("Group");
		}
	}

	bool DResourceGroupManager::onManagerResourceAdding( DResourceManager* manager, DResourcePtr& res )
	{
		// if collide, notify all listeners.
		bool collision = isResourceActive(res->getGroupName(), res->getName());
		bool isHandled = true;	
		if (collision)
		{
			isHandled = handleCollision(manager, res);
		}
		return isHandled;
	}

	void DResourceGroupManager::onManagerResourceAdded( DResourceManager* manager, DResourcePtr& res )
	{
		addActiveResource(res);
	}

	void DResourceGroupManager::onManagerResourceRemoving( DResourceManager* manager, DResourcePtr& res )
	{
		removeActiveResource(res);
	}

}