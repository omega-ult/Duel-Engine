//  [10/29/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelDMCodec.h"
#include "DuelString.h"
#include "DuelXML.h"
#include "DuelMeshManager.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DMeshDescription, DResourceDescription);
	DUEL_IMPLEMENT_RTTI_1(DMeshManager, DResourceManager);

	DMeshDescription::DMeshDescription( const DString& name, const DString& groupName ) :
		DResourceDescription(name, groupName, "Mesh"),
		shadowBuffer(false)
	{

	}

	DMeshManager::DMeshManager() : 
		DResourceManager("Mesh")
	{
		// current version.
		mCommonCodec = new DEFAULT_MESH_CODEC();
	}

	ResourceDescriptionList DMeshManager::parseParameter( DDataStream* data )
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
		size_t size = data->getSize();
		// spare 1 for the last '\0'
		xmlContent = doc.allocate_string(0, size+1);
		data->read(xmlContent, size);
		xmlContent[size] = '\0';


		doc.parse<0>(xmlContent);
		DXMLNode* rootNode = doc.first_node("ResourceParameters");
		if (rootNode == NULL)
		{
			return ret;
		}
		DXMLAttribute* rootAtt = rootNode->first_attribute("type");
		if(DXMLTool::readValue(rootAtt) != "Mesh")
		{
			return ret;
		}
		DXMLNode* listNode = rootNode->first_node("MeshResourceParameter");
		if (listNode == NULL)
		{
			return ret;
		}
		// data content should be an XML, format like:
		// <MeshResourceParameter>
		//		<Name>xxxx</Name>
		//		<Group>xxxx</Group>
		//		<FileFormat>xxxx</FileFormat> 
		//		<ShadowBuffer>xxxx</ShadowBuffer> 
		// </MeshResourceParameter>
		// <MeshResourceParameter>
		//		............
		//		............
		// </MeshResourceParameter>
		DString name;
		DString groupName;
		DXMLNode* contentNode;
		while(listNode)
		{
			contentNode = listNode->first_node("Name");
			name = DXMLTool::readValue(contentNode);
			contentNode = contentNode->next_sibling("Group");
			groupName = DXMLTool::readValue(contentNode);
			
			// create new DResourceParameter.
			DMeshDescription* meshParam = new DMeshDescription(name,groupName);
			contentNode = contentNode->next_sibling("FileFormat");
			meshParam->fileFormat = DXMLTool::readValue(contentNode);
			contentNode = contentNode->next_sibling("ShadowBuffer");
			DString val = DXMLTool::readValue(contentNode);
			DStringTool::toLowerCase(val);
			meshParam->shadowBuffer = (val == "true");
			// add ResourceParamter to the result
			ret.push_back(DResourceDescriptionPtr(meshParam));
			listNode = listNode->next_sibling("MeshResourceParameter");
		}


		return ret;
	}

	DResource* DMeshManager::createImpl( DResourceDescription* createParam )
	{
		DMeshDescription* resDesc = createParam->getAs<DMeshDescription>(false);
		DMesh* ret = NULL;
		if (resDesc)
		{
			ret = new DMesh(this,createParam->getName(),createParam->getGroupName());
			if (resDesc->fileFormat == "dm")
			{
				ret->setCodec(mCommonCodec);
			}
			else
			{
				ret->setCodec(NULL);
			}
			ret->setShadowBufferEnable(resDesc->shadowBuffer);
		}
		return ret;
	}


}