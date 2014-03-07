//  [12/3/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelResourceManager.h"
#include "DuelRenderEffect.h"
#include "DuelRenderEffectManager.h"
#include "DuelDRECodec.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DRenderEffectDescription, DResourceDescription);
	DUEL_IMPLEMENT_RTTI_1(DRenderEffectManager, DResourceManager);




	DRenderEffectDescription::DRenderEffectDescription( const DString& name, const DString& groupName ) :
		DResourceDescription(name, groupName, "RenderEffect")
	{
		
	}


	DRenderEffectManager::DRenderEffectManager() :
		DResourceManager("RenderEffect")
	{
		mCommonCodec = new DEFAULT_RENDEREFFECT_CODEC();
	}

	DRenderEffectManager::~DRenderEffectManager()
	{
		delete mCommonCodec;
	}

	ResourceDescriptionList DRenderEffectManager::parseParameter( DDataStream* data )
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
		assert(strcmp("RenderEffect", rootAtt->value()) == 0);
		DXMLNode* listNode = rootNode->first_node("RenderEffectResourceParameter");
		if (listNode == NULL)
		{
			return ret;
		}
		// data content should be an XML, format like:
		// <RenderEffectResourceParameter>
		//		<Name>xxxx</Name>
		//		<Group>xxxx</Group>
		//		<FileFormat>xxxx</FileFormat> 
		// </RenderEffectResourceParameter>
		// <RenderEffectResourceParameter>
		//		............
		//		............
		// </RenderEffectResourceParameter>
		DString name;
		DString groupName;
		DXMLNode* contentNode;
		while(listNode)
		{
			contentNode = listNode->first_node("Name");
			name = contentNode->value();
			contentNode = contentNode->next_sibling("Group");
			groupName = contentNode->value();
			// create new DResourceParameter.
			DRenderEffectDescription* REParam = new DRenderEffectDescription(name,groupName);
			contentNode = contentNode->next_sibling("FileFormat");
			REParam->fileFormat = contentNode->value();
			// add ResourceParamter to the result
			ret.push_back(DResourceDescriptionPtr(REParam));
			listNode = listNode->next_sibling("RenderEffectResourceParameter");
		}


		return ret;

	}

	DResource* DRenderEffectManager::createImpl( DResourceDescription* createParam )
	{
		DRenderEffectDescription* resDesc = createParam->getAs<DRenderEffectDescription>(false);
		DRenderEffect* ret = NULL; 
		if (resDesc)
		{
			ret = new DRenderEffect(this, createParam->getName(), createParam->getGroupName());
			if(resDesc->fileFormat == "dre")
			{
				ret->setCodec(mCommonCodec);
			}
		}
		return ret;
	}

}