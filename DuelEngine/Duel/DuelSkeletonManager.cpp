//  [12/30/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelSkeleton.h"
#include "DuelSkeletonManager.h"
#include "DuelDSCodec.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DSkeletonDescription, DResourceDescription);
	DUEL_IMPLEMENT_RTTI_1(DSkeletonManager, DResourceManager);


	DSkeletonDescription::DSkeletonDescription( const DString& name, const DString& groupName ) :
		DResourceDescription(name, groupName, "Skeleton")
	{
	}

	DSkeletonManager::DSkeletonManager() : 
		DResourceManager("Skeleton")
	{
		mCommonCodec = new DEFAULT_SKELETON_CODEC();
	}

	ResourceDescriptionList DSkeletonManager::parseParameter( DDataStream* data )
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

		DXMLAttribute* rootAtt = rootNode->first_attribute("type");;
		if(DXMLTool::readValue(rootAtt) != "Skeleton")
		{
			return ret;
		}
		DXMLNode* listNode = rootNode->first_node("SkeletonResourceParameter");
		if (listNode == NULL)
		{
			return ret;
		}
		// <SkeletonResourceParameter>
		//		<Name>xxxx</Name>
		//		<Group>xxxx</Group>
		//		<FileFormat>xxxx</FileFormat> 
		// </SkeletonResourceParameter>
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
			DSkeletonDescription* skelParam = new DSkeletonDescription(name,groupName);
			contentNode = contentNode->next_sibling("FileFormat");
			skelParam->fileFormat = DXMLTool::readValue(contentNode);
			// add ResourceParamter to the result
			ret.push_back(DResourceDescriptionPtr(skelParam));
			listNode = listNode->next_sibling("SkeletonResourceParameter");
		}
		return ret;
	}

	DResource* DSkeletonManager::createImpl( DResourceDescription* createParam )
	{
		DSkeletonDescription* resDesc = createParam->getAs<DSkeletonDescription>(false);
		DSkeleton* ret = NULL; 
		if (resDesc != NULL)
		{
			ret = new DSkeleton(this, createParam->getName(), createParam->getGroupName());
			if (resDesc->fileFormat == "ds")
			{
				ret->setCodec(mCommonCodec);
			}
		}
		return ret;
	}

}