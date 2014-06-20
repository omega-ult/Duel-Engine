//  [8/20/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelDataStream.h"
#include "DuelResourceManager.h"
#include "DuelFontManager.h"
#include "DuelFont.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DFontDescription, DResourceDescription);
	DUEL_IMPLEMENT_RTTI_1(DFontManager, DResourceManager);





	DFontDescription::DFontDescription( const DString& name, const DString& groupName ) :
		DResourceDescription(name, groupName, "Font")
	{

	}


	DFontManager::DFontManager() :
		DResourceManager("Font")
	{

	}

	Duel::ResourceDescriptionList DFontManager::parseParameter( DDataStream* data )
	{
		ResourceDescriptionList ret;

		// if data is empty.
		if (data == NULL )
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
		assert(strcmp("Font", rootAtt->value()) == 0);
		DXMLNode* listNode = rootNode->first_node("FontResourceParameter");
		if (listNode == NULL)
		{
			return ret;
		}


		DString name;
		DString groupName;
		DString glyphSize;
		DString texSize;
		DXMLNode* contentNode;
		while(listNode != NULL)
		{
			contentNode = listNode->first_node("Name");
			name = contentNode->value();
			contentNode = contentNode->next_sibling("Group");
			groupName = contentNode->value();
			DFontDescription* fontParam = new DFontDescription(name, groupName);

			contentNode = contentNode->next_sibling("GlyphSize");
			glyphSize = contentNode->value();
			uint32 gSize = 32;
			bool ok = DStringTool::parseString(glyphSize, gSize);
			fontParam->glyphSize = ok ? gSize : 32;

			contentNode = contentNode->next_sibling("TextureSize");
			texSize = contentNode->value();
			uint32 tSize = 1024;
			ok = DStringTool::parseString(texSize, tSize);
			fontParam->textureSize = ok ? tSize : 1024;
			
			ret.push_back(DResourceDescriptionPtr(fontParam));
			listNode = listNode->next_sibling("FontResourceParameter");
		}
		return ret;
	}

	DResource* DFontManager::createImpl( DResourceDescription* createParam )
	{
		DFontDescription* fParam = createParam->getAs<DFontDescription>(false);
		if (fParam)
		{
			DFont* newFont = new DFont(this, fParam->getName(), fParam->getGroupName());
			newFont->setGlyphSize(fParam->glyphSize);
			newFont->setTextureSize(fParam->textureSize);
			return newFont;
		}
		return NULL;
	}


}