//  [1/1/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelException.h"
#include "DuelDRECodec.h"
#include "DuelGpuProgramManager.h"
#include "DuelRenderEffect.h"
#include "DuelRenderTechnique.h"
#include "DuelRenderPass.h"
#include "DuelMemoryStream.h"
#include "DuelFileStream.h"

namespace Duel
{
	//DUEL_IMPLEMENT_RTTI_1(DRECodec_v1_0, DRenderEffectCodec);
	DUEL_IMPLEMENT_RTTI_1(DRECodec_v2_0, DResourceCodec);

	// from DString value to native value------------------
	PolygonMode	getPolygonMode(const DString& val)
	{
		DString value(val);
		DStringTool::toUpperCase(value);
		if ("POINTS" == value)
		{
			return PM_Points;
		}
		else if ("WIREFRAME" == value)
		{
			return PM_Wireframe;
		}
		else if ("SOLID" == value)
		{
			return PM_Solid;
		}
		return PM_Solid;
	}
	ShadeMode	getShadeMode(const DString& val)
	{
		DString value(val);
		DStringTool::toUpperCase(value);
		if ("FLAT" == value)
		{
			return SM_Flat;
		}
		else if ("GOURAUD" == value)
		{
			return SM_Gouraud;
		}
		else if ("PHONG" == value)
		{
			return SM_Phong;
		}
		return SM_Gouraud;
	}
	CullingMode	getCullingMode(const DString& val)
	{
		DString value(val);
		DStringTool::toUpperCase(value);
		if ("NONE" == value)
		{
			return CM_None;
		}
		else if ("CLOCKWISE" == value)
		{
			return CM_ClockWise;
		}
		else if ("ANTICLOCKWISE" == value)
		{
			return CM_AntiClockWise;
		}
		return CM_ClockWise;
	}
	bool		getBool(const DString& val)
	{
		DString value(val);
		DStringTool::toLowerCase(value);
		if ("false" == value)
		{
			return false;
		}
		else if ("true" == value)
		{
			return true;
		}
		return false;
	}
	CompareFunction		getCompareFunction(const DString& val)
	{
		DString value(val);
		DStringTool::toUpperCase(value);
		if ("ALWAYSFAIL" == value)
		{
			return CF_AlwaysFail;
		}
		else if ("ALWAYSPASS" == value)
		{
			return CF_AlwaysPass;
		}
		else if ("LESS" == value)
		{
			return CF_Less;
		}
		else if ("LESSEQUAL" == value)
		{
			return CF_LessEqual;
		}
		else if ("EQUAL" == value)
		{
			return CF_Equal;
		}
		else if ("NOTEQUAL" == value)
		{
			return CF_NotEqual;
		}
		else if ("GRATEREQUAL" == value)
		{
			return CF_GreaterEqual;
		}
		else if ("GRATER" == value)
		{
			return CF_Greater;
		}
		return CF_AlwaysPass;
	}
	StencilOperation	getStencilOperation(const DString& val)
	{
		DString value(val);
		DStringTool::toUpperCase(value);
		if ("KEEP" == value)
		{
			return SO_Keep;
		}
		else if ("ZERO" == value)
		{
			return SO_Zero;
		}
		else if ("REPLACE" == value)
		{
			return SO_Replace;
		}
		else if ("INCREMENT" == value)
		{
			return SO_Increment;
		}
		else if ("DECREMENT" == value)
		{
			return SO_Decrement;
		}
		else if ("INCREMENTWRAP" == value)
		{
			return SO_DecrementWrap;
		}
		else if ("DECREMENTWRAP" == value)
		{
			return SO_DecrementWrap;
		}
		else if ("INVERT" == value)
		{
			return SO_Invert;
		}
		return SO_Keep;
	}
	BlendOperation	getSceneBlendOperation(const DString& val)
	{
		DString value(val);
		DStringTool::toUpperCase(value);
		if ("ADD" == value)
		{
			return BO_Add;
		}
		else if ("SUB" == value)
		{
			return BO_Sub;
		}
		else if ("REVSUB" == value)
		{
			return BO_RevSub;
		}
		else if ("MIN" == value)
		{
			return BO_Min;
		}
		else if ("MAX" == value)
		{
			return BO_Max;
		}
		return BO_Add;
	}
	BlendFactor	getSceneBlendFactor(const DString& val)
	{
		DString value(val);
		DStringTool::toUpperCase(value);
		if ("ZERO" == value)
		{
			return BF_Zero;
		}
		else if ("ONE" == value)
		{
			return BF_One;
		}
		else if ("SRCALPHA" == value)
		{
			return BF_SrcAlpha;
		}
		else if ("DSTALPHA" == value)
		{
			return BF_DstAlpha;
		}
		else if ("INVSRCALPHA" == value)
		{
			return BF_InvSrcAlpha;
		}
		else if ("INVDSTALPHA" == value)
		{
			return BF_InvDstAlpha;
		}
		else if ("SRCCOLOR" == value)
		{
			return BF_SrcColor;
		}
		else if ("DSTCOLOR" == value)
		{
			return BF_DstColor;
		}
		else if ("INVSRCCOLOR" == value)
		{
			return BF_InvSrcColor;
		}
		else if ("INVDSTCOLOR" == value)
		{
			return BF_InvDstColor;
		}
		else if ("SRCALPHASAT" == value)
		{
			return BF_SrcAlphaSat;
		}
		return BF_Zero;
	}
	TextureAddressMode	getTextureAddressMode(const DString& val)
	{
		DString value(val);
		DStringTool::toUpperCase(value);
		if ("WRAP" == value)
		{
			return TAM_Wrap;
		}
		else if ("MIRROR" == value)
		{
			return TAM_Mirror;
		}
		else if ("CLAMP" == value)
		{
			return TAM_Clamp;
		}
		else if ("BORDER" == value)
		{
			return TAM_Border;
		}
		return TAM_Clamp;
	}
	FilterOption	getFilterOption(const DString& val)
	{
		DString value(val);
		DStringTool::toUpperCase(value);
		if ("NONE" == value)
		{
			return FO_None;
		}
		else if ("POINT" == value)
		{
			return FO_Point;
		}
		else if ("LINEAR" == value)
		{
			return FO_Linear;
		}
		else if ("ANISOTROPIC" == value)
		{
			return FO_Anistropic;
		}
		return FO_Anistropic;
	}

	// internal method used for parsing val
	void	parseState(const DString& type, const DString& name, const DString& val, DRenderPass* targetPass)
	{
		//RasterizerState----------------------------------------------
		if ("RasterizerState" == type)
		{
			if ( "PolygonMode" == name)
			{
				targetPass->rasterizeState.polygonMode = getPolygonMode(val);
			}
			else if ( "ShadeMode" == name )
			{
				targetPass->rasterizeState.shadeMode = getShadeMode(val);
			}
			else if ( "CullingMode" == name )
			{
				targetPass->rasterizeState.cullingMode = getCullingMode(val);
			}
			else if ( "PolygonOffsetFactor" == name )
			{
				DStringTool::parseString(val, targetPass->rasterizeState.polygonOffsetFactor);
			}
			else if ( "PolygonOffsetUnit" == name )
			{
				DStringTool::parseString(val, targetPass->rasterizeState.polygonOffsetUnit);
			}
			else if ( "DepthClipEnable" == name )
			{
				targetPass->rasterizeState.depthClipEnable = getBool(val);
			}
			else if ( "ScissorEnable" == name )
			{
				targetPass->rasterizeState.scissorEnable = getBool(val);
			}
			else if ( "MultisampleEnable" == name )
			{
				targetPass->rasterizeState.multisampleEnable = getBool(val);
			}
		}
		//DepthStencilState--------------------------------------------
		else if ("DepthStencilState" == type)
		{
			if ( "DepthEnable" == name )
			{
				targetPass->depthStencilState.depthEnable = getBool(val);
			}
			else if ( "DepthWriteEnable" == name )
			{
				targetPass->depthStencilState.depthWriteEnable = getBool(val);
			}
			else if ( "DepthComparison" == name )
			{
				targetPass->depthStencilState.depthComparison = getCompareFunction(val);
			}
			else if ( "FrontStencilEnable" == name )
			{
				targetPass->depthStencilState.frontStencilEnable = getBool(val);
			}
			else if ( "FrontStencilComparison" == name )
			{
				targetPass->depthStencilState.frontStencilComparison = getCompareFunction(val);
			}
			else if ( "FrontStencilReadMask" == name )
			{
				DStringTool::parseString(val, targetPass->depthStencilState.frontStencilReadMask);
			}
			else if ( "FrontStencilWriteMask" == name )
			{
				DStringTool::parseString(val, targetPass->depthStencilState.frontStencilWriteMask);
			}
			else if ( "FrontStencilFail" == name )
			{
				targetPass->depthStencilState.frontStencilFail = getStencilOperation(val);
			}
			else if ( "FrontStencilDepthFail" == name )
			{
				targetPass->depthStencilState.frontStencilDepthFail = getStencilOperation(val);
			}
			else if ( "FrontStencilPass" == name )
			{
				targetPass->depthStencilState.frontStencilPass = getStencilOperation(val);
			}
			else if ( "BackStencilEnable" == name )
			{
				targetPass->depthStencilState.backStencilEnable = getBool(val);
			}
			else if ( "BackStencilComparison" == name )
			{
				targetPass->depthStencilState.backStencilComparison = getCompareFunction(val);
			}
			else if ( "BackStencilReadMask" == name )
			{
				DStringTool::parseString(val, targetPass->depthStencilState.backStencilReadMask);
			}
			else if ( "BackStencilWriteMask" == name )
			{
				DStringTool::parseString(val, targetPass->depthStencilState.backStencilWriteMask);
			}
			else if ( "BackStencilFail" == name )
			{
				targetPass->depthStencilState.backStencilFail = getStencilOperation(val);
			}
			else if ( "BackStencilDepthFail" == name )
			{
				targetPass->depthStencilState.backStencilDepthFail = getStencilOperation(val);
			}
			else if ( "BackStencilPass" == name )
			{
				targetPass->depthStencilState.backStencilPass = getStencilOperation(val);
			}
		}
		else if ("SceneBlendState" == type)
		{
			//  [4/2/2013 OMEGA]
// 			if ("AlphaToCoverageEnable" == name)
// 			{
// 				targetPass->sceneBlendState.alphaToCoverageEnable = getBool(val);
// 			}
// 			else if ( "ColorBlendOpeartion" == name )
// 			{
// 				targetPass->sceneBlendState.colorBlendOpeartion = getSceneBlendOperation(val);
// 			}
// 			else if ( "ColorSrcBlend" == name )
// 			{
// 				targetPass->sceneBlendState.colorSrcBlend = getSceneBlendFactor(val);
// 			}
// 			else if ( "ColorDestBlend" == name )
// 			{
// 				targetPass->sceneBlendState.colorDestBlend = getSceneBlendFactor(val);
// 			}
// 			else if ( "AlphaBlendOpeartion" == name )
// 			{
// 				targetPass->sceneBlendState.alphaBlendOpeartion = getSceneBlendOperation(val);
// 			}
// 			else if ( "AlphaSrcBlend" == name )
// 			{
// 				targetPass->sceneBlendState.alphaSrcBlend = getSceneBlendFactor(val);
// 			}
// 			else if ( "AlphaDestBlend" == name )
// 			{
// 				targetPass->sceneBlendState.alphaDestBlend = getSceneBlendFactor(val);
// 			}
		}

	}

	uint8	readColorMask(const DString& value)
	{

		DString val(value);
		DStringTool::toUpperCase(val);
		if (val == "RED")
		{
			return CWM_Red;
		}
		else if(val == "GREEN")
		{
			return CWM_Green;
		}
		else if(val == "BLUE")
		{
			return CWM_Blue;
		}
		else if(val == "ALPHA")
		{
			return CWM_Alpha;
		}
		else if(val == "ALL")
		{
			return CWM_All;
		}
		return 0;
	}
	//////////////////////////////////////////////////////////////////////////
	// v2.0
	void	parseRTBlendState(const DString& name, const DString& val, DRenderTargetBlendState& blendState)
	{
		if ("BlendEnable" == name)
		{
			blendState.blendEnable = getBool(val);
		}
		else if ( "ColorBlendOpeartion" == name )
		{
			blendState.colorBlendOpeartion = getSceneBlendOperation(val);
		}
		else if ( "ColorSrcBlend" == name )
		{
			blendState.colorSrcBlend = getSceneBlendFactor(val);
		}
		else if ( "ColorDestBlend" == name )
		{
			blendState.colorDestBlend = getSceneBlendFactor(val);
		}
		else if ( "AlphaBlendOpeartion" == name )
		{
			blendState.alphaBlendOpeartion = getSceneBlendOperation(val);
		}
		else if ( "AlphaSrcBlend" == name )
		{
			blendState.alphaSrcBlend = getSceneBlendFactor(val);
		}
		else if ( "AlphaDestBlend" == name )
		{
			blendState.alphaDestBlend = getSceneBlendFactor(val);
		}
		else if ( "ColorWriteMask" == name)
		{
			uint8 msk = 0;
			DStringVector maskValue = DStringTool::split(val, "|");
			size_t len = maskValue.size();
			for (uint32 i = 0; i < len; ++i)
			{
				msk |= readColorMask(maskValue[i]);
			}
			blendState.colorWriteMask = msk;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// v2.0
	void	parseSamplerState(const DString& name, const DString& val, DTextureSamplerState& sampState)
	{
		if ("AddressU" == name)
		{
			sampState.addressU = getTextureAddressMode(val);
		}
		else if ( "AddressV" == name )
		{
			sampState.addressV = getTextureAddressMode(val);
		}
		else if ( "AddressW" == name )
		{
			sampState.addressW = getTextureAddressMode(val);
		}
		else if ( "MinFilter" == name )
		{
			sampState.minFilter = getFilterOption(val);
		}
		else if ( "MagFilter" == name )
		{
			sampState.magFilter = getFilterOption(val);
		}
		else if ( "MipFilter" == name )
		{
			sampState.mipFilter = getFilterOption(val);
		}
		else if ( "SamplerComparison" == name )
		{
			sampState.samplerComparison = getCompareFunction(val);
		}
		else if ( "BorderColor" == name )
		{
			DVector4 c;
			if (val.find_first_of(',') != DString::npos)
			{
				sampState.borderColor = DColor(DStringTool::parseVector4(val, c, ","));
			}
			else
			{
				sampState.borderColor = DColor(DStringTool::parseVector4(val, c));
			}
		}

		else if ( "MinLod" == name )
		{
			DReal lod;
			DStringTool::parseString(val, lod);
			sampState.minLod = lod;
		}
		else if ( "MaxLod" == name )
		{
			DReal lod;
			DStringTool::parseString(val, lod);
			sampState.maxLod = lod;
		}
		else if ( "MaxAnisotropy" == name )
		{
			uint32 anis;
			DStringTool::parseString(val, anis);
			sampState.maxAnisotropy = anis;
		}
		else if ( "MipLodBias" == name )
		{
			DReal bias;
			DStringTool::parseString(val, bias);
			sampState.mipLodBias = bias;
		}
	}
	
	// from native val to DString val-----------------------
	DString	parsePolygonMode(PolygonMode val)
	{
		if (PM_Points == val)
		{
			return "Points";
		}
		else if ( PM_Wireframe == val)
		{
			return "Wireframe";
		}
		else if (PM_Solid == val)
		{
			return "Solid";
		}
		return "Solid";
	}
	DString	parseShadeMode(ShadeMode val)
	{
		if (SM_Flat == val)
		{
			return "Flat";
		}
		else if (SM_Gouraud == val)
		{
			return "Gouraoud";
		}
		else if (SM_Phong == val)
		{
			return "Phong";
		}
		return "Gouraoud";
	}
	DString	parseCullingMode(CullingMode val)
	{
		if (CM_None == val)
		{
			return "None";
		}
		else if (CM_ClockWise == val)
		{
			return "ClockWise";
		}
		else if (CM_AntiClockWise == val)
		{
			return "AntiClockWise";
		}
		return "ClockWise";
	}
	DString	parseBool(bool val)
	{
		if (false == val)
		{
			return "false";
		}
		else if (true == val)
		{
			return "true";
		}
		return "false";
	}
	DString	parseCompareFunction(CompareFunction val)
	{
		if (CF_AlwaysFail == val)
		{
			return "AlwaysFail";
		}
		else if (CF_AlwaysPass == val)
		{
			return "AlwaysPass";
		}
		else if (CF_Less == val)
		{
			return "Less";
		}
		else if (CF_LessEqual == val)
		{
			return "LessEqual";
		}
		else if (CF_Equal == val)
		{
			return "Equal";
		}
		else if (CF_NotEqual == val)
		{
			return "NotEqual";
		}
		else if (CF_GreaterEqual == val)
		{
			return "GreaterEqual";
		}
		else if (CF_Greater == val)
		{
			return "Greater";
		}
		return "AlwaysPass";
	}
	DString	parseStencilOperation(StencilOperation val)
	{
		if (SO_Keep == val)
		{
			return "Keep";
		}
		else if (SO_Zero == val)
		{
			return "Zero";
		}
		else if (SO_Replace == val)
		{
			return "Replace";
		}
		else if (SO_Increment == val)
		{
			return "Increment";
		}
		else if (SO_Decrement == val)
		{
			return "Decrement";
		}
		else if (SO_IncrementWrap == val)
		{
			return "IncrementWrap";
		}
		else if (SO_DecrementWrap == val)
		{
			return "DecrementWrap";
		}
		else if (SO_Invert == val)
		{
			return "Invert";
		}
		return "Keep";
	}
	DString	parseSceneBlendOperation(BlendOperation val)
	{
		if (BO_Add == val)
		{
			return "Add";
		}
		else if (BO_Sub == val)
		{
			return "Sub";
		}
		else if (BO_RevSub == val)
		{
			return "RevSub";
		}
		else if (BO_Min == val)
		{
			return "Min";
		}
		else if (BO_Max == val)
		{
			return "Max";
		}
		return "Add";
	}
	DString	parseSceneBlendFactor(BlendFactor val)
	{
		if (BF_Zero == val)
		{
			return "Zero";
		}
		else if (BF_One == val)
		{
			return "One";
		}
		else if (BF_SrcAlpha == val)
		{
			return "SrcAlpha";
		}
		else if (BF_DstAlpha == val)
		{
			return "DstAlpha";
		}
		else if (BF_InvSrcAlpha == val)
		{
			return "InvSrcAlpha";
		}
		else if (BF_InvDstAlpha == val)
		{
			return "InvDstAlpha";
		}
		else if (BF_SrcColor == val)
		{
			return "SrcColor";
		}
		else if (BF_DstColor == val)
		{
			return "DstColor";
		}
		else if (BF_InvSrcColor == val)
		{
			return "InvSrcColor";
		}
		else if (BF_InvDstColor == val)
		{
			return "InvDstColor";
		}
		else if (BF_SrcAlphaSat == val)
		{
			return "SrcAlphaSat";
		}
		return "Zero";
	}
	DString	parseTextureAddressMode(TextureAddressMode val)
	{
		if (TAM_Wrap == val)
		{
			return "Wrap";
		}
		else if (TAM_Mirror == val)
		{
			return "Mirror";
		}
		else if (TAM_Clamp == val)
		{
			return "Clamp";
		}
		else if (TAM_Border == val)
		{
			return "Border";
		}
		return "Clamp";
	}
	DString	parseFilterOption(FilterOption val)
	{
		if (FO_None == val)
		{
			return "None";
		}
		else if (FO_Point == val)
		{
			return "Point";
		}
		else if (FO_Linear == val)
		{
			return "Linear";
		}
		else if (FO_Anistropic == val)
		{
			return "Anistropic";
		}
		return "Anistropic";
	}

	void DRECodec_v2_0::decode( DDataStream* input, DResource* targetRes )
	{
		DRenderEffect* targetEffect = targetRes->getAs<DRenderEffect>(false);
		if (targetEffect == NULL)
		{
			return;
		}
		DXMLDoc	doc;
		// because the xml parsing is destructive, we use 
		// embeded DXMLDoc's memory pool to handle memory.
		char*	xmlContent;
		size_t	size = input->getSize();
		// spare 1 char for the last '\0'
		xmlContent = doc.allocate_string(0, size+1);
		input->read(xmlContent, size);
		xmlContent[size] = '\0';

		doc.parse<0>(xmlContent);
		DXMLNode* effectNode = doc.first_node("RenderEffect");
		if (effectNode == NULL)
		{
			// failed to parse xml, is anything wrong with this file?
#ifdef DUEL_DEBUG
			DUEL_EXCEPT(DException::ET_ItemNotFound,
				"Error in parsing XML",
				"Error in parsing XML, no node named: 'GroupDescription' found",
				"Duel::DRECodec_v2_0::decodeImpl")
#endif
				return;
		}
		DXMLAttribute*	effectName = effectNode->first_attribute("name");
		if (effectName)
		{
			targetEffect->setName(DXMLTool::readValue(effectName));
		}
		DXMLAttribute*	effectAtt = effectNode->first_attribute("version");
		if (effectAtt == NULL)
		{
			// failed to parse xml, is anything wrong with this file?
			DString	attVal = DXMLTool::readValue(effectAtt);
			if (attVal != "2.0")
			{
#ifdef DUEL_DEBUG
				DUEL_EXCEPT(DException::ET_ItemNotFound,
					"Error in parsing XML",
					"Error in parsing XML, version not correct(only version 2.0 available)",
					"Duel::DRECodec_v2_0::decodeImpl")
#endif
			}
			return;
		}

		// set name for the Effect
		//targetEffect->setName(effectNode->first_attribute("name")->value());
		DXMLNode* techniqueNode = effectNode->first_node("Technique");
		DXMLNode* passNode;
		DRenderTechniquePtr newTech;
		DRenderPassPtr newPass;
		// clear before adding
		targetEffect->removeAllTechniques();
		//Technique---------------------------
		while(techniqueNode)
		{
			newTech = targetEffect->addTechnique(DXMLTool::readValue(techniqueNode->first_attribute("name")));
			passNode = techniqueNode->first_node("Pass");
			//Pass-------------------------------------
			while(passNode)
			{
				newPass = newTech->addPass(DXMLTool::readValue(passNode->first_attribute("name")));
				DXMLNode* stateNode;
				DXMLNode* rasterizerNode = passNode->first_node("RasterizerState");
				//RasterizerState------------------------------
				if (rasterizerNode)
				{
					stateNode = rasterizerNode->first_node("state");
					while(stateNode)
					{
						parseState("RasterizerState", DXMLTool::readValue(stateNode->first_attribute("name")), DXMLTool::readValue(stateNode), newPass.get());
						stateNode = stateNode->next_sibling("state");
					}
				}
				//DepthStencil------------------------------------
				DXMLNode* depthStencilNode = passNode->first_node("DepthStencilState");
				if (depthStencilNode)
				{
					stateNode = depthStencilNode->first_node("state");
					while(stateNode)
					{
						parseState("DepthStencilState", DXMLTool::readValue(stateNode->first_attribute("name")), DXMLTool::readValue(stateNode), newPass.get());
						stateNode = stateNode->next_sibling("state");
					}
				}
				

				//////////////////////////////////////////////////////////////////////////
				DXMLNode* blendNode = passNode->first_node("BlendState");
				if (blendNode)
				{

					DXMLNode* alphaCov = blendNode->first_node("state");
					if (alphaCov)
					{
						if (DXMLTool::readValue(alphaCov->first_attribute("name")) == "AlphaToCoverageEnable")
						{
							newPass->blendState.alphaToCoverageEnable = getBool(DXMLTool::readValue(alphaCov));
						}
					}
					DXMLNode* rtBlendState = blendNode->first_node("RenderTargetBlendState");
					while (rtBlendState)
					{
						DXMLAttribute* rtBSAtt = rtBlendState->first_attribute("index");
						uint32 rtIndx = 0;
						if (rtBSAtt)
						{
							if(DStringTool::parseString(DXMLTool::readValue(rtBSAtt), rtIndx))
							{
								// 为了适应图形API的现状, 限定这个序号小于8, 
								if (rtIndx < DUEL_MAX_BLEND_LAYERS)
								{
									stateNode = rtBlendState->first_node("state");
									DRenderTargetBlendState	tmpState;
									while (stateNode)
									{
										parseRTBlendState(DXMLTool::readValue(stateNode->first_attribute("name")), DXMLTool::readValue(stateNode), tmpState);
										stateNode = stateNode->next_sibling("state");
									}
									newPass->blendState.targetStates[rtIndx] = tmpState;
								}
							}
						}
						rtBlendState = rtBlendState->next_sibling("RenderTargetBlendState");
					}
				}
				DXMLNode* vsNode = passNode->first_node("VertexProgram");
				if (vsNode)
				{
					DResourcePtr vs;
					DString vsName = DXMLTool::readValue(vsNode);
					DString vsGroup = DXMLTool::readValue(vsNode->first_attribute("group"));
					vs = DGpuProgramManager::getSingleton().getResource( 
						vsGroup,vsName);
					newPass->setVertexProgram(vs);
				}
				//PixelShader------------------------------------
				DXMLNode* psNode = passNode->first_node("PixelProgram");
				if (psNode)
				{
					DResourcePtr ps;
					DString psName = DXMLTool::readValue(psNode);
					DString psGroup = DXMLTool::readValue(psNode->first_attribute("group"));
					ps = DGpuProgramManager::getSingleton().getResource( 
						psGroup, psName);
					newPass->setPixelProgram(ps);
				}
				passNode = passNode->next_sibling("Pass");
			}

			techniqueNode = techniqueNode->next_sibling("Technique");
		}
	}

	Duel::DDataStreamPtr DRECodec_v2_0::encode( DResource* targetRes )
	{
		DRenderEffect* targetEffect = targetRes->getAs<DRenderEffect>(false);
		if (targetEffect == NULL)
		{
			return DDataStreamPtr();
		}
		DXMLDoc doc;
		DRenderEffect::RenderTechniqueIterator ti =  targetEffect->getRenderTechniqueIterator();

		DXMLNode* root = doc.allocate_node(DUEL_XML::node_pi, doc.allocate_string("xml version='1.0'"));
		doc.append_node(root);
		DXMLNode* effectNode = doc.allocate_node(DUEL_XML::node_element, "RenderEffect");
		effectNode->append_attribute(doc.allocate_attribute("name", targetEffect->getName().c_str()));
		effectNode->append_attribute(doc.allocate_attribute("version", "2.0"));
		root->append_node(effectNode);

 		while(ti.hasMoreElements())
 		{
 			DXMLNode* techniqueNode = doc.allocate_node(DUEL_XML::node_element, "RenderTechnique");
 			techniqueNode->append_attribute(doc.allocate_attribute("name", ti.current()->get()->getName().c_str()));
 			effectNode->append_node(techniqueNode);
 
 			DRenderTechnique::RenderPassIterator pi = ti.current()->get()->getRenderPassIterator();
 			while(pi.hasMoreElements())
 			{
 				DRenderPass* curPass = pi.current()->get();
				DXMLNode* passNode = doc.allocate_node(DUEL_XML::node_element, "RenderPass");
 				passNode->append_attribute(doc.allocate_attribute("name", curPass->getName().c_str()));
 				techniqueNode->append_node(passNode);
 
 				DXMLNode* stateNode; 
 				DXMLNode* rasterizerNode = doc.allocate_node(DUEL_XML::node_element, "RaRasterizerState");
 				passNode->append_node(rasterizerNode);
				//RasterizerState--------------------------------------
 				stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", parsePolygonMode(curPass->rasterizeState.polygonMode).c_str());
 				stateNode->append_attribute(doc.allocate_attribute("name","PolygonMode"));
 				rasterizerNode->append_node(stateNode);
 
 				stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", parseShadeMode(curPass->rasterizeState.shadeMode).c_str());
 				stateNode->append_attribute(doc.allocate_attribute("name","ShadeMode"));
 				rasterizerNode->append_node(stateNode);
 
 				stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", parseCullingMode(curPass->rasterizeState.cullingMode).c_str());
 				stateNode->append_attribute(doc.allocate_attribute("name","CullingMode"));
 				rasterizerNode->append_node(stateNode);
 
 				stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", DStringTool::toString(curPass->rasterizeState.polygonOffsetFactor).c_str());
 				stateNode->append_attribute(doc.allocate_attribute("name","PolygonOffsetFactor"));
 				rasterizerNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", DStringTool::toString(curPass->rasterizeState.polygonOffsetUnit).c_str());
				stateNode->append_attribute(doc.allocate_attribute("name","PolygonOffsetUnit"));
				rasterizerNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", parseBool(curPass->rasterizeState.depthClipEnable).c_str());
				stateNode->append_attribute(doc.allocate_attribute("name","DepthClipEnable"));
				rasterizerNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", parseBool(curPass->rasterizeState.scissorEnable).c_str());
				stateNode->append_attribute(doc.allocate_attribute("name","ScissorEnable"));
				rasterizerNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", parseBool(curPass->rasterizeState.multisampleEnable).c_str());
				stateNode->append_attribute(doc.allocate_attribute("name","MultisampleEnable"));
				rasterizerNode->append_node(stateNode);
 
 
 				DXMLNode* depthStencilNode = doc.allocate_node(DUEL_XML::node_element, "DepthStencilState");
				passNode->append_node(depthStencilNode);
				//DepthStencilState-----------------------------------
				stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", parseBool(curPass->depthStencilState.depthEnable).c_str());
				stateNode->append_attribute(doc.allocate_attribute("name","DepthEnable"));
				depthStencilNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", parseBool(curPass->depthStencilState.depthWriteEnable).c_str());
				stateNode->append_attribute(doc.allocate_attribute("name","DepthWriteEnable"));
				depthStencilNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", parseCompareFunction(curPass->depthStencilState.depthComparison).c_str());
				stateNode->append_attribute(doc.allocate_attribute("name","DepthComparison"));
				depthStencilNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", parseBool(curPass->depthStencilState.frontStencilEnable).c_str());
				stateNode->append_attribute(doc.allocate_attribute("name","FrontStencilEnable"));
				depthStencilNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", parseCompareFunction(curPass->depthStencilState.frontStencilComparison).c_str());
				stateNode->append_attribute(doc.allocate_attribute("name","FrontStencilComparison"));
				depthStencilNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", DStringTool::toString(curPass->depthStencilState.frontStencilReadMask).c_str());
				stateNode->append_attribute(doc.allocate_attribute("name","FrontStencilReadMask"));
				depthStencilNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", DStringTool::toString(curPass->depthStencilState.frontStencilWriteMask).c_str());
				stateNode->append_attribute(doc.allocate_attribute("name","FrontStencilWriteMask"));
				depthStencilNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", parseStencilOperation(curPass->depthStencilState.frontStencilFail).c_str());
				stateNode->append_attribute(doc.allocate_attribute("name","FrontStencilFail"));
				depthStencilNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", parseStencilOperation(curPass->depthStencilState.frontStencilDepthFail).c_str());
				stateNode->append_attribute(doc.allocate_attribute("name","FrontStencilDepthFail"));
				depthStencilNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", parseStencilOperation(curPass->depthStencilState.frontStencilPass).c_str());
				stateNode->append_attribute(doc.allocate_attribute("name","FrontStencilPass"));
				depthStencilNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", parseBool(curPass->depthStencilState.backStencilEnable).c_str());
				stateNode->append_attribute(doc.allocate_attribute("name","BackStencilEnable"));
				depthStencilNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", parseCompareFunction(curPass->depthStencilState.backStencilComparison).c_str());
				stateNode->append_attribute(doc.allocate_attribute("name","BackStencilComparison"));
				depthStencilNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", DStringTool::toString(curPass->depthStencilState.backStencilReadMask).c_str());
				stateNode->append_attribute(doc.allocate_attribute("name","BackStencilReadMask"));
				depthStencilNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", DStringTool::toString(curPass->depthStencilState.backStencilWriteMask).c_str());
				stateNode->append_attribute(doc.allocate_attribute("name","BackStencilWriteMask"));
				depthStencilNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", parseStencilOperation(curPass->depthStencilState.backStencilFail).c_str());
				stateNode->append_attribute(doc.allocate_attribute("name","BackStencilFail"));
				depthStencilNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", parseStencilOperation(curPass->depthStencilState.backStencilDepthFail).c_str());
				stateNode->append_attribute(doc.allocate_attribute("name","BackStencilDepthFail"));
				depthStencilNode->append_node(stateNode);

				stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", parseStencilOperation(curPass->depthStencilState.backStencilPass).c_str());
				stateNode->append_attribute(doc.allocate_attribute("name","BackStencilPass"));
				depthStencilNode->append_node(stateNode);

				//////////////////////////////////////////////////////////////////////////
				// deprecated in v2.0
// 				DXMLNode* sceneBlendNode = doc.allocate_node(DUEL_XML::node_element, "SceneBlendState");
// 				passNode->append_node(sceneBlendNode);
// 				//sceneBlendNode-----------------------------------------------
// 				stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", parseBool(curPass->sceneBlendState.alphaToCoverageEnable).c_str());
// 				stateNode->append_attribute(doc.allocate_attribute("name","AlphaToCoverageEnable"));
// 				sceneBlendNode->append_node(stateNode);
// 
// 				stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", parseSceneBlendOperation(curPass->sceneBlendState.colorBlendOpeartion).c_str());
// 				stateNode->append_attribute(doc.allocate_attribute("name","ColorBlendOpeartion"));
// 				sceneBlendNode->append_node(stateNode);
// 
// 				stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", parseSceneBlendFactor(curPass->sceneBlendState.colorSrcBlend).c_str());
// 				stateNode->append_attribute(doc.allocate_attribute("name","ColorSrcBlend"));
// 				sceneBlendNode->append_node(stateNode);
// 
// 				stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", parseSceneBlendFactor(curPass->sceneBlendState.colorDestBlend).c_str());
// 				stateNode->append_attribute(doc.allocate_attribute("name","ColorDestBlend"));
// 				sceneBlendNode->append_node(stateNode);
// 
// 				stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", parseSceneBlendOperation(curPass->sceneBlendState.alphaBlendOpeartion).c_str());
// 				stateNode->append_attribute(doc.allocate_attribute("name","AlphaBlendOpeartion"));
// 				sceneBlendNode->append_node(stateNode);
// 
// 				stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", parseSceneBlendFactor(curPass->sceneBlendState.alphaSrcBlend).c_str());
// 				stateNode->append_attribute(doc.allocate_attribute("name","AlphaSrcBlend"));
// 				sceneBlendNode->append_node(stateNode);
// 
// 				stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", parseSceneBlendFactor(curPass->sceneBlendState.alphaDestBlend).c_str());
// 				stateNode->append_attribute(doc.allocate_attribute("name","AlphaDestBlend"));
// 				sceneBlendNode->append_node(stateNode);
				//////////////////////////////////////////////////////////////////////////

				//////////////////////////////////////////////////////////////////////////
				// v2.0
				DXMLNode* blendNode = doc.allocate_node(DUEL_XML::node_element, "BlendState");
				stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", parseBool(curPass->blendState.alphaToCoverageEnable).c_str());
				stateNode->append_attribute(doc.allocate_attribute("name", "AlphaToCoverageEnable"));
				blendNode->append_node(stateNode);

				// we only encode those states that are not the default value.
				DBlendState defaultState;
				if ( curPass->blendState != defaultState)
				{
					for (uint32 i = 0; i < DUEL_MAX_BLEND_LAYERS; ++i)
					{
						if (curPass->blendState.targetStates[i] != defaultState.targetStates[i])
						{
							DXMLNode* rtbsNode = doc.allocate_node(DUEL_XML::node_comment, "RenderTargetBlendState");
							rtbsNode->append_attribute(doc.allocate_attribute("index", DStringTool::toString(i).c_str()));

							stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", parseBool(curPass->blendState.targetStates[i].blendEnable).c_str());
							stateNode->append_attribute(doc.allocate_attribute("name", "BlendEnable"));
							rtbsNode->append_node(stateNode);

							stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", parseSceneBlendOperation(curPass->blendState.targetStates[i].colorBlendOpeartion).c_str());	
							stateNode->append_attribute(doc.allocate_attribute("name","ColorBlendOpeartion"));
							rtbsNode->append_node(stateNode);


							stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", parseSceneBlendFactor(curPass->blendState.targetStates[i].colorSrcBlend).c_str());
							stateNode->append_attribute(doc.allocate_attribute("name","ColorSrcBlend"));
							rtbsNode->append_node(stateNode);

							stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", parseSceneBlendFactor(curPass->blendState.targetStates[i].colorDestBlend).c_str());
							stateNode->append_attribute(doc.allocate_attribute("name","ColorDestBlend"));
							rtbsNode->append_node(stateNode);

							stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", parseSceneBlendOperation(curPass->blendState.targetStates[i].alphaBlendOpeartion).c_str());
							stateNode->append_attribute(doc.allocate_attribute("name","AlphaBlendOpeartion"));
							rtbsNode->append_node(stateNode);

							stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", parseSceneBlendFactor(curPass->blendState.targetStates[i].alphaSrcBlend).c_str());
							stateNode->append_attribute(doc.allocate_attribute("name","AlphaSrcBlend"));
							rtbsNode->append_node(stateNode);

							stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", parseSceneBlendFactor(curPass->blendState.targetStates[i].alphaDestBlend).c_str());
							stateNode->append_attribute(doc.allocate_attribute("name","AlphaDestBlend"));
							rtbsNode->append_node(stateNode);

							stateNode = doc.allocate_node(DUEL_XML::node_comment, "state", DStringTool::toString(curPass->blendState.targetStates[i].colorWriteMask).c_str());
							stateNode->append_attribute(doc.allocate_attribute("name","ColorWriteMask"));
							rtbsNode->append_node(stateNode);

							blendNode->append_node(rtbsNode);

						}
					}
				}
				passNode->append_node(blendNode);


				// vertex shader------------------------------------------------
				if (curPass->getVertexProgram() != NULL)
				{
					DXMLNode* vsNode = doc.allocate_node(DUEL_XML::node_element, "VertexProgram", curPass->getVertexProgram()->getName().c_str());
					vsNode->append_attribute(doc.allocate_attribute("group",curPass->getVertexProgram()->getGroupName().c_str()));
					passNode->append_node(vsNode);
				}
				else
				{
					DXMLNode* vsNode = doc.allocate_node(DUEL_XML::node_element, "VertexProgram", "");
					vsNode->append_attribute(doc.allocate_attribute("group", ""));
					passNode->append_node(vsNode);
				}
				// pixel shader--------------------------------------------------
				if (curPass->getPixelProgram() != NULL)
				{
					DXMLNode* psNode = doc.allocate_node(DUEL_XML::node_element, "PixelProgram", curPass->getPixelProgram()->getName().c_str());
					psNode->append_attribute(doc.allocate_attribute("group",curPass->getPixelProgram()->getGroupName().c_str()));
					passNode->append_node(psNode);
				}
				else
				{
					DXMLNode* psNode = doc.allocate_node(DUEL_XML::node_element, "PixelProgram", "");
					psNode->append_attribute(doc.allocate_attribute("group", ""));
					passNode->append_node(psNode);
				}

				pi.moveNext();
			}
			ti.moveNext();
 		}
// 
// 		// output 
		DString result;
		DUEL_XML::print(std::back_inserter(result), doc);

		char* buf = new char[result.size()];
		DMemoryStream* ret = new DMemoryStream((void*)buf, result.size(), DA_Write, true); 
		ret->write(result.c_str(), result.size());

		return DMemoryStreamPtr(ret);
	}

	void DRECodec_v2_0::encodeToFile( const DString& fileName, DResource* targetRes )
	{
		DRenderEffect* targetEffect = targetRes->getAs<DRenderEffect>(false);
		if (targetEffect == NULL)
		{
			return;
		}
		DMemoryStream* memData = static_cast<DMemoryStream*>(encode(targetEffect).get());
		// output 
		std::ofstream output(fileName);
		output.write(memData->getChunkPtr(), memData->getSize());
		output.flush();
	}

}