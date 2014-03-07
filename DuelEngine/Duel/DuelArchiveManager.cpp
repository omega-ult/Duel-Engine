//  [9/17/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelArchiveManager.h"
#include "DuelFolderArchive.h"
#include "DuelFileStream.h"

namespace Duel
{
	//------------------------------------
// 	template<> ArchiveManager* Singleton<ArchiveManager>::msInstance = 0;
// 	ArchiveManager& ArchiveManager::getSingleton(){
// 		return *msInstance;
// 	}
// 	ArchiveManager* ArchiveManager::getSingletonPtr(){
// 		return msInstance;
// 	}
	//-------------------------------------
	
	DArchiveManager::DArchiveManager()
	{

	}

	DArchiveManager::DArchiveManager( const DString& configFileName )
	{
		registerFromXML(configFileName);
	}

	DArchiveManager::~DArchiveManager()
	{

	}



	DArchivePtr DArchiveManager::getArchive( const DString& name )
	{
		ArchiveMap::iterator i = mArchives.find(name);

		if (i == mArchives.end())
		{
			DUEL_EXCEPT(DException::ET_ItemNotFound,
				"Specified archive cannot be found: " + name,
				"Specified archive cannot be found, check input name, or maybe it does not register yet",
				"Duel::ArchiveManager::load")
		}

		DArchive* retA;
		switch(i->second)
		{
		case AT_Folder:
			retA = new DFolderArchive(name, AT_Folder);
			break;
		case AT_Zip:
			// lalalalalal~~
			retA = NULL;
			break;
		case AT_WP8AppFolder:
			retA = new DFolderArchive(name, AT_WP8AppFolder);
			break;
		default:
			DUEL_EXCEPT(DException::ET_ItemNotFound,
				"DArchive type not match",
				"Check whether a wrong archive type has been registered",
				"Duel::ArchiveManager::load")
		}

		retA->load();
		return DArchivePtr(retA);
	}


	void DArchiveManager::registerArchive( const DString& name, ArchiveType type )
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
			if ( type == "folder" )
			{
				registerArchive(name, AT_Folder);
			}
			else if ( type == "zip" )
			{
				// not complete yet.
			}
			else if ( type == "appfolder_wp8")
			{
				registerArchive(name, AT_WP8AppFolder);
			}
			else
			{
				// meet an unknown type, 
#ifdef DUEL_DEBUG
				DUEL_EXCEPT(DException::ET_ItemNotFound,
					"Error in parsing XML",
					"Error in parsing XML, unknown type of " + type,
					"Duel::ArchiveManager::registerFromXML")
#endif				
			}

			recordNode = recordNode->next_sibling("Archive");
		}
	}

}