//  [11/15/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelRenderState.h"

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(DRasterizerStateObject, DObject);
	DUEL_IMPLEMENT_RTTI_1(DDepthStencilStateObject, DObject);
	DUEL_IMPLEMENT_RTTI_1(DBlendStateObject, DObject);
//	DUEL_IMPLEMENT_RTTI(DTextureSamplerStateObject, DObject);

	DRasterizerState::DRasterizerState() :
		polygonMode(PM_Solid),
		shadeMode(SM_Gouraud),
		cullingMode(CM_None),
		polygonOffsetFactor(0.0f),
		polygonOffsetUnit(0.0f),
		depthClipEnable(true),
		scissorEnable(false),
		multisampleEnable(true)
	{

	}


	DRasterizerState& DRasterizerState::operator=( const DRasterizerState& rs )
	{
		// TODO : can we do memcpy?
		polygonMode = rs.polygonMode;
		shadeMode	= rs.shadeMode;
		cullingMode	= rs.cullingMode;
		polygonOffsetFactor = rs.polygonOffsetFactor;
		polygonOffsetUnit	= rs.polygonOffsetUnit;
		depthClipEnable		= rs.depthClipEnable;
		scissorEnable		= rs.scissorEnable;
		multisampleEnable	= rs.multisampleEnable;
//		memcpy(this, &rs, sizeof(DRasterizerState));
		return *this;
	}

	bool DRasterizerState::operator==( const DRasterizerState& rs )
	{
		return (rs.cullingMode == cullingMode &&
				rs.depthClipEnable	== depthClipEnable &&
				rs.multisampleEnable == multisampleEnable &&
				rs.polygonMode == polygonMode &&
				rs.polygonOffsetFactor == polygonOffsetFactor &&
				rs.polygonOffsetUnit == polygonOffsetUnit &&
				rs.scissorEnable == scissorEnable &&
				rs.shadeMode == shadeMode);
	}


	DDepthStencilState::DDepthStencilState() :
		depthEnable(true),
		depthWriteEnable(true),
		depthComparison(CF_Less),
		frontStencilEnable(false),
		frontStencilComparison(CF_AlwaysPass),
		frontStencilReadMask(0xffffffff),
		frontStencilWriteMask(0xffffffff),
		frontStencilFail(SO_Keep),
		frontStencilDepthFail(SO_Keep),
		frontStencilPass(SO_Keep),
		backStencilEnable(false),
		backStencilComparison(CF_AlwaysPass),
		backStencilReadMask(0xffffffff),
		backStencilWriteMask(0xffffffff),
		backStencilFail(SO_Keep),
		backStencilDepthFail(SO_Keep),
		backStencilPass(SO_Keep)

	{
		
	}

	DDepthStencilState& DDepthStencilState::operator=( const DDepthStencilState& dss )
	{
		// TODO : can we do memcpy?
		depthEnable	= dss.depthEnable;
		depthWriteEnable	= dss.depthWriteEnable;
		depthComparison		= dss.depthComparison;
		frontStencilEnable	= dss.frontStencilEnable;
		frontStencilComparison	= dss.frontStencilComparison;
		frontStencilReadMask	= dss.frontStencilReadMask;
		frontStencilWriteMask	= dss.frontStencilWriteMask;
		frontStencilFail		= dss.frontStencilFail;
		frontStencilDepthFail	= dss.frontStencilDepthFail;
		frontStencilPass		= dss.frontStencilPass;

		backStencilEnable		= dss.backStencilEnable;
		backStencilComparison	= dss.backStencilComparison;
		backStencilReadMask		= dss.backStencilReadMask;
		backStencilWriteMask	= dss.backStencilWriteMask;
		backStencilFail			= dss.backStencilFail;
		backStencilDepthFail	= dss.backStencilDepthFail;
		backStencilPass			= dss.backStencilPass;
//		memcpy(this, &dss, sizeof(DDepthStencilState));
		return *this;
	}

	bool DDepthStencilState::operator==( const DDepthStencilState& dss )
	{
		return (dss.backStencilComparison == backStencilComparison &&
				dss.backStencilDepthFail == backStencilDepthFail &&
				dss.backStencilEnable == backStencilEnable &&
				dss.backStencilFail == backStencilFail &&
				dss.backStencilPass == backStencilPass &&
				dss.backStencilReadMask == backStencilReadMask &&
				dss.backStencilWriteMask == backStencilWriteMask &&
				dss.depthComparison == depthComparison &&
				dss.depthEnable == depthEnable &&
				dss.depthWriteEnable == depthWriteEnable &&
				dss.frontStencilComparison == frontStencilComparison &&
				dss.frontStencilDepthFail == frontStencilDepthFail &&
				dss.frontStencilEnable == frontStencilEnable &&
				dss.frontStencilFail == frontStencilFail &&
				dss.frontStencilPass == frontStencilPass &&
				dss.frontStencilReadMask == frontStencilReadMask &&
				dss.frontStencilWriteMask == frontStencilWriteMask);
	}

// 
// 	DSceneBlendState::DSceneBlendState() :
// 		blendop
// 	{
// 		blendOpeartion.assign(BO_ADD);
// 		srcBlend.assign(BF_ONE);
// 		destBlend.assign(BF_ZERO);
// 		alphaBlendOpeartion.assign(BO_ADD);
// 		alphaSrcBlend.assign(BF_ONE);
// 		alphaDestBlend.assign(BF_ZERO);
// 		colorWriteMask.assign(CMASK_ALL);
// 	}

// 	TextureBlendState::TextureBlendState()
// 	{
// 	}
// 
// 
// 	DSceneBlendState::DSceneBlendState() :
// 		alphaToCoverageEnable(false),
// 		colorBlendOpeartion(SBO_ADD),
// 		colorSrcBlend(SBF_ONE),
// 		colorDestBlend(SBF_ZERO),
// 		alphaBlendOpeartion(SBO_ADD),
// 		alphaSrcBlend(SBF_ONE),
// 		alphaDestBlend(SBF_ZERO)
// 	{
// 
// 	}
// 
// 	DSceneBlendState& DSceneBlendState::operator=( const DSceneBlendState& sbs )
// 	{
// 		// TODO : can we do memcpy?
// 		alphaToCoverageEnable	= sbs.alphaToCoverageEnable;
// 		colorBlendOpeartion		= sbs.colorBlendOpeartion;
// 		colorSrcBlend			= sbs.colorSrcBlend;
// 		colorDestBlend			= sbs.colorDestBlend;
// 
// 		alphaBlendOpeartion		= sbs.alphaBlendOpeartion;
// 		alphaSrcBlend			= sbs.alphaSrcBlend;
// 		alphaDestBlend			= sbs.alphaDestBlend;
// //		memcpy(this, &sbs, sizeof(DSceneBlendState));
// 		return *this;
// 	}
// 
// 	bool DSceneBlendState::operator==( const DSceneBlendState& sbs )
// 	{
// 		return (sbs.alphaBlendOpeartion == alphaBlendOpeartion &&
// 				sbs.alphaDestBlend == alphaDestBlend &&
// 				sbs.alphaSrcBlend == alphaSrcBlend &&
// 				sbs.alphaToCoverageEnable == alphaToCoverageEnable &&
// 				sbs.colorBlendOpeartion == colorBlendOpeartion &&
// 				sbs.colorDestBlend == colorDestBlend &&
// 				sbs.colorSrcBlend == colorSrcBlend);
// 	}


// 	TextureState::TextureState()
// 	{
// 
// 	}


	DTextureSamplerState::DTextureSamplerState() :
		addressU(TAM_Clamp),
		addressV(TAM_Clamp),
		addressW(TAM_Clamp),
		minFilter(FO_Linear),
		magFilter(FO_Linear),
		mipFilter(FO_Linear),
		samplerComparison(CF_AlwaysFail),
		borderColor(DColor::BLACK),
		minLod(DMath::NEG_INFINITY),
		maxLod(DMath::POS_INFINITY),
		maxAnisotropy(16),
		mipLodBias(0.0f)
	{

	}

	DTextureSamplerState& DTextureSamplerState::operator=( const DTextureSamplerState& tss )
	{
		// TODO : can we do memcpy?
		addressU	= tss.addressU;
		addressV	= tss.addressV;
		addressW	= tss.addressW;
		minFilter	= tss.minFilter;
		magFilter	= tss.magFilter;
		mipFilter	= tss.mipFilter;
		samplerComparison	= tss.samplerComparison;
		borderColor	= tss.borderColor;
		minLod		= tss.minLod;
		maxLod		= tss.maxLod;
		maxAnisotropy	= tss.maxAnisotropy;
		mipLodBias	= tss.mipLodBias;
//		memcpy(this, &tss, sizeof(DTextureSamplerState));
		return *this;
	}

	bool DTextureSamplerState::operator==( const DTextureSamplerState& tss )
	{
		return (addressU == tss.addressU &&
				addressV == tss.addressV &&
				addressW == tss.addressW &&
				minFilter == tss.minFilter &&
				magFilter == tss.magFilter &&
				mipFilter == tss.mipFilter &&
				samplerComparison == tss.samplerComparison &&
				borderColor == tss.borderColor &&
				minLod == tss.minLod &&
				maxLod == tss.maxLod &&
				maxAnisotropy == tss.maxAnisotropy &&
				mipLodBias == tss.mipLodBias
			);
	}



	DBlendState::DBlendState() :
		alphaToCoverageEnable(false)
	{

	}


	bool DBlendState::operator==( const DBlendState& sbs )
	{
		if (alphaToCoverageEnable != sbs.alphaToCoverageEnable )
		{
			return false;
		}
		for (uint32 i = 0; i < DUEL_MAX_BLEND_LAYERS; ++i)
		{
			if (targetStates[i] != sbs.targetStates[i])
			{
				return false;
			}
		}
		return true;
	}


	DRenderTargetBlendState::DRenderTargetBlendState() :
		blendEnable(false),
		colorBlendOpeartion(BO_Add),
		colorSrcBlend(BF_One),
		colorDestBlend(BF_Zero),
		alphaBlendOpeartion(BO_Add),
		alphaSrcBlend(BF_One),
		alphaDestBlend(BF_Zero),
		colorWriteMask(CWM_All)
	{

	}

	DRenderTargetBlendState& DRenderTargetBlendState::operator=( const DRenderTargetBlendState& sbs )
	{
		blendEnable				= sbs.blendEnable;
		colorBlendOpeartion		= sbs.colorBlendOpeartion;
		colorSrcBlend			= sbs.colorSrcBlend;
		colorDestBlend			= sbs.colorDestBlend;

		alphaBlendOpeartion		= sbs.alphaBlendOpeartion;
		alphaSrcBlend			= sbs.alphaSrcBlend;
		alphaDestBlend			= sbs.alphaDestBlend;

		colorWriteMask			= sbs.colorWriteMask;
		return *this;
	}

	bool DRenderTargetBlendState::operator==( const DRenderTargetBlendState& sbs )
	{
		return (sbs.alphaBlendOpeartion == alphaBlendOpeartion &&
			sbs.alphaDestBlend == alphaDestBlend &&
			sbs.alphaSrcBlend == alphaSrcBlend &&
			sbs.blendEnable == blendEnable &&
			sbs.colorBlendOpeartion == colorBlendOpeartion &&
			sbs.colorDestBlend == colorDestBlend &&
			sbs.colorSrcBlend == colorSrcBlend &&
			sbs.colorWriteMask == colorWriteMask);
	}

}