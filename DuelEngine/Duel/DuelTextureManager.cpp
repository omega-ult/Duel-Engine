//  [10/10/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelTextureManager.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DTextureDescription, DResourceDescription);
	DUEL_IMPLEMENT_RTTI_1(DTextureManager, DResourceManager);
// 	//------------------------------------
// 	template<> TextureManager* Singleton<TextureManager>::msInstance = 0;
// 	TextureManager& TextureManager::getSingleton(){
// 		return *msInstance;
// 	}
// 	TextureManager* TextureManager::getSingletonPtr(){
// 		return msInstance;
// 	}
//	//------------------------------------

	DTextureDescription::DTextureDescription( const DString& name, const DString& groupName ) :
		DResourceDescription(name, groupName, "Texture")
	{

	}


	DTextureManager::DTextureManager() : 
		DResourceManager("Texture")
	{

	}

	DTextureManager::~DTextureManager()
	{

	}

	Duel::ResourceDescriptionList DTextureManager::parseParameter( DDataStream* data )
	{
		ResourceDescriptionList ret;

		// if data is empty.
		if (data == NULL)
		{
			return ret;
		}
		DXMLDoc	doc;

		// because the xml parsing is destructive, we use 
		// embeded DXMLDoc's memory pool to handle memory.
		char* xmlContent;
		size_t xmlSize = data->getSize();
		// spare 1 char for the last '\0'
		xmlContent = doc.allocate_string(0, xmlSize+1);
		data->read(xmlContent, xmlSize);
		xmlContent[xmlSize] = '\0';

		doc.parse<0>(xmlContent);
		DXMLNode* rootNode = doc.first_node("ResourceParameters");
		if (rootNode == NULL)
		{
			return ret;
		}
		DXMLAttribute* rootAtt = rootNode->first_attribute("type");
		assert(strcmp("Texture", rootAtt->value()) == 0);
		DXMLNode* listNode = rootNode->first_node("TextureResourceParameter");
		if (listNode == NULL)
		{
			return ret;
		}


		DString name;
		DString groupName;
		DXMLNode* contentNode;
		CodecFormat codeFormat;
		HardwareBufferUsage usage; 
		while(listNode != NULL)
		{
			contentNode = listNode->first_node("Name");
			name = contentNode->value();
			contentNode = contentNode->next_sibling("Group");
			groupName = contentNode->value();
			DTextureDescription* texParam = new DTextureDescription(name, groupName);
 
			contentNode = contentNode->next_sibling("CodecFormat");
			assert(contentNode != NULL);
			if (strcmp(contentNode->value(), "dds") == 0)
			{
				codeFormat = CF_DDS;
			}
			else
			{
				DUEL_EXCEPT_BRIEF(DException::ET_NotImplemented,
					"Unknown codec format for texture : " + DString(contentNode->value()),
					"Duel::WPDXTextureManager::parseParameter");
			}
			contentNode = contentNode->next_sibling("MemoryStrategy");
			assert(contentNode != NULL);
			DString memStg = contentNode->value();
			DStringTool::toUpperCase(memStg);
			if (memStg == "STATIC")
			{
				usage = HBU_Static;
			}
			else if (memStg  == "DYNAMIC")
			{
				// default : dynamic
				usage = HBU_Dynamic;
			}
			else
			{
				usage = HBU_Dynamic;
			}
			texParam->codecFormat = codeFormat;
			texParam->memoryStrategy = usage;
			ret.push_back(DResourceDescriptionPtr(texParam));
			listNode = listNode->next_sibling("TextureResourceParameter");
		}
		return ret;
	}

}
