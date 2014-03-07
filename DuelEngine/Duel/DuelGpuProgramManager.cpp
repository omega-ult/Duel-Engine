//  [9/19/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelDataStream.h"
#include "DuelGpuProgramManager.h"
#include "DuelGpuProgram.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DGpuProgramDescription, DResourceDescription);
	DUEL_IMPLEMENT_RTTI_1(DGpuProgramManager, DResourceManager);

	DGpuProgramDescription::DGpuProgramDescription( const DString& name, const DString& groupName) :
		DResourceDescription(name, groupName, "GpuProgram")
	{
	}

	DGpuProgramManager::DGpuProgramManager() :
		DResourceManager("GpuProgram")
	{

	}

	DGpuProgramManager::~DGpuProgramManager()
	{

	}

	ResourceDescriptionList DGpuProgramManager::parseParameter( DDataStream* data )
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
		assert(strcmp("GpuProgram", rootAtt->value()) == 0);
		DXMLNode* listNode = rootNode->first_node("GpuResourceParameter");
		if (listNode == NULL)
		{
			return ret;
		}
		// <GpuResourceParameter>
		//		<Name>xxxx</Name>
		//		<Group>xxxx</Group>
		//		<Entry>main</Entry>
		//		<Profile>vs_2_0</Profile>	
		//		<Language>hlsl</Language>
		// </GpuResourceParameter>
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
			DGpuProgramDescription* gpuParam = new DGpuProgramDescription(name,groupName);
// 			contentNode = contentNode->next_sibling("FileName");
// 			// if it is empty, use its default fileName.
// 			if (strcmp(contentNode->value(), "") != 0)
// 			{
// 				gpuParam->fileName = contentNode->value();
// 			}
// 			contentNode = contentNode->next_sibling("MatrixPacking");
// 			gpuParam->matrixPacking = contentNode->value();
			contentNode = contentNode->next_sibling("Entry");
			gpuParam->entry = contentNode->value();
			contentNode = contentNode->next_sibling("Profile");
			gpuParam->profile = contentNode->value();
			contentNode = contentNode->next_sibling("Language");
			gpuParam->language = contentNode->value();
			contentNode = contentNode->next_sibling("Compiled");
			
			if(strcmp(contentNode->value(), "true") == 0)
			{
				gpuParam->compiled = true;
			}
			else
			{
				gpuParam->compiled = false;
			}

			// add ResourceParamter to the result
			ret.push_back(DResourceDescriptionPtr(gpuParam));
			listNode = listNode->next_sibling("GpuResourceParameter");
		}

		return ret;
	}


}