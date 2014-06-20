//  [9/17/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelArchiveManager.h"
#include "DuelFolderArchive.h"
#include "DuelFileStream.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DArchiveFactory, DObject);
	DUEL_IMPLEMENT_RTTI_1(DFolderArchiveFactory, DArchiveFactory);
		
	const DString& DArchiveFactory::getArchiveType()
	{
		return mArchType;
	}

	DArchiveFactory::DArchiveFactory( const DString& archType ) : mArchType(archType)
	{

	}

	DFolderArchiveFactory::DFolderArchiveFactory() :
		DArchiveFactory("folder")
	{

	}

	Duel::DArchivePtr DFolderArchiveFactory::getArchive( const DString& name )
	{
		return DArchivePtr(new DFolderArchive(name));
	}


	DArchiveManager::DArchiveManager()
	{
		mFolderFactory = new DFolderArchiveFactory();
		registerArchiveFactory(mFolderFactory);
	}

	DArchiveManager::DArchiveManager( const DString& configFileName )
	{
		registerFromXML(configFileName);
	}


	DArchiveManager::~DArchiveManager()
	{
		unregisterArchiveFactory(mFolderFactory);
		delete mFolderFactory;
	}

	DArchivePtr DArchiveManager::getArchive( const DString& name )
	{
		ArchiveMap::iterator i = mArchives.find(name);
		DArchivePtr ret;
		if (i == mArchives.end())
		{
			return ret;
		}
		ArchiveFactoryMap::iterator fi = mArchFactoryMap.find(i->second);
		if (fi != mArchFactoryMap.end())
		{
			ret = fi->second->getArchive(name);
		}
		if (ret != NULL)
		{
			ret->load();
		}
		return ret;
	}


	void DArchiveManager::registerArchive( const DString& name, const DString& type)
	{
		mArchives.insert(std::make_pair(name, type));
	}


	void DArchiveManager::registerFromXML( const DString& configFileName )
	{
		DDataStreamPtr filePtr = DDataStreamPtr(new DFileStream(configFileName));
		if (!filePtr->isValid())
		{
#ifdef DUEL_DEBUG
			DUEL_EXCEPT(DException::ET_FileNotFound,
				"Open file named: " + configFileName + " failed",
				"Open file named: " + configFileName + " failed",
				"Duel::ArchiveManager::registerFromXML")
#endif
				return;
		}
		registerFromXML(filePtr);
	}


	void DArchiveManager::registerFromXML(DDataStreamPtr& xmlData)
	{
		// if data is empty.
		if (xmlData == 0)
		{
			return;
		}
		DXMLDoc	doc;
		// because the xml parsing is destructive, we use 
		// embeded DXMLDoc's memory pool to handle memory.
		char* xmlContent;
		size_t size = xmlData->getSize();
		// spare 1 for the last '\0'
		xmlContent = doc.allocate_string(0, size+1);
		xmlData->read(xmlContent, size);
		xmlContent[size] = '\0';

		
		doc.parse<0>(xmlContent);
		DXMLNode* listNode = doc.first_node("ArchiveList");
		if (listNode == NULL)
		{
			// failed to parse xml, is anything wrong with this file?
#ifdef DUEL_DEBUG
			DUEL_EXCEPT(DException::ET_ItemNotFound,
				"Error in parsing XML",
				"Error in parsing XML, no node named: 'ArchiveList' found",
				"Duel::ArchiveManager::registerFromXML")
#endif
			return;
		}
		registerFromXML(listNode);
	}

	void DArchiveManager::registerFromXML( DXMLNode* startNode )
	{
		// validate input.
		if (startNode == NULL)
		{
			return;
		}
		else if (strcmp(startNode->name(), "ArchiveList") != 0)
		{
			return;
		}

		DXMLNode* recordNode = startNode->first_node("Archive");
		DXMLNode* contentNode;
		DString	name;
		DString	type;
		while(recordNode)
		{
			contentNode = recordNode->first_node("Name");
			name = DXMLTool::readValue(contentNode);
			contentNode = contentNode->next_sibling("Type");
			type = DXMLTool::readValue(contentNode);
			DStringTool::toLowerCase(type);
			registerArchive(name, type);

			recordNode = recordNode->next_sibling("Archive");
		}
	}

	void DArchiveManager::registerArchiveFactory( DArchiveFactory* af )
	{
		if ( af == NULL)
		{
			return;
		}
		if (mArchFactoryMap.find(af->getArchiveType()) == mArchFactoryMap.end())
		{
			mArchFactoryMap[af->getArchiveType()] = af;
		}
	}

	void DArchiveManager::unregisterArchiveFactory( DArchiveFactory* af )
	{
		if ( af == NULL)
		{
			return;
		}
		ArchiveFactoryMap::iterator i = mArchFactoryMap.find(af->getArchiveType());
		if (i != mArchFactoryMap.end())
		{
			mArchFactoryMap.erase(i);
		}
	}


}