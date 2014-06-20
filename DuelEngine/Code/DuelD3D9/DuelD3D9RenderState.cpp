//  [6/12/2014 OMEGA] created

#include "DuelD3D9Common.h"
#include "DuelD3D9RenderState.h"
#include "DuelD3D9Translator.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(D3D9RasterizerStateObject, DRasterizerStateObject);
	DUEL_IMPLEMENT_RTTI_1(D3D9DepthStencilStateObject, DDepthStencilStateObject);
	DUEL_IMPLEMENT_RTTI_1(D3D9BlendStateObject, DBlendStateObject);
	DUEL_IMPLEMENT_RTTI_1(D3D9TextureSamplerObject, DTextureSamplerObject);

	D3D9RasterizerStateObject::D3D9RasterizerStateObject( const DRasterizerState& state ) :
		mPreserveState(state)
	{
		D3DPolygonMode = D3D9Translator::getD3DFillMode(state.polygonMode);
		D3DShadeMode = D3D9Translator::getD3DShadeMode(state.shadeMode);
		D3DCullMode = D3D9Translator::getD3DCullingMode(state.cullingMode);

	}

	void D3D9RasterizerStateObject::getRasterizerState( DRasterizerState& outState )
	{
		outState = mPreserveState;
	}

	D3D9DepthStencilStateObject::D3D9DepthStencilStateObject( const DDepthStencilState& state ) :
		mPreserveState(state)
	{
		D3DDepthComparison = D3D9Translator::getD3DCompareFunction(state.depthComparison);
		
		D3DFrontStencilComparison = D3D9Translator::getD3DCompareFunction(state.frontStencilComparison);
		D3DFrontStencilFail = D3D9Translator::getD3DStencilOperation(state.frontStencilFail);
		D3DFrontStencilDepthFail = D3D9Translator::getD3DStencilOperation(state.frontStencilDepthFail);
		D3DFrontStencilPass = D3D9Translator::getD3DStencilOperation(state.frontStencilPass);

		D3DBackStencilComparison = D3D9Translator::getD3DCompareFunction(state.backStencilComparison);
		D3DBackStencilFail = D3D9Translator::getD3DStencilOperation(state.backStencilFail);
		D3DBackStencilDepthFail = D3D9Translator::getD3DStencilOperation(state.backStencilDepthFail);
		D3DBackStencilPass = D3D9Translator::getD3DStencilOperation(state.backStencilPass);
	}

	void D3D9DepthStencilStateObject::getDepthStencilState( DDepthStencilState& outState )
	{
		outState = mPreserveState;
	}

	D3D9BlendStateObject::D3D9BlendStateObject( const DBlendState& state ) :
		mPreserveState(state)
	{
		for (uint32 i = 0; i < DUEL_MAX_BLEND_LAYERS; ++i)
		{
			D3DColorBlendOperation[i] = D3D9Translator::getD3DBlendOperation(state.targetStates[i].colorBlendOpeartion);
			D3DColorSrcFactor[i] = D3D9Translator::getD3DBlendFactor(state.targetStates[i].colorSrcFactor);
			D3DColorDstFactor[i] = D3D9Translator::getD3DBlendFactor(state.targetStates[i].colorDstFactor);

			D3DAlphaBlendOperation[i] = D3D9Translator::getD3DBlendOperation(state.targetStates[i].alphaBlendOpeartion);
			D3DAlphaSrcFactor[i] = D3D9Translator::getD3DBlendFactor(state.targetStates[i].alphaSrcFactor);
			D3DAlphaDstFactor[i] = D3D9Translator::getD3DBlendFactor(state.targetStates[i].alphaDstFactor);
		}
	}

	void D3D9BlendStateObject::getBlendState( DBlendState& outState )
	{
		outState = mPreserveState;
	}

	D3D9TextureSamplerObject::D3D9TextureSamplerObject( const DTextureSampler& samp ) :
		mPreserveSamp(samp)
	{
		D3DTextureAddressU = D3D9Translator::getD3DTextureAddressMode(samp.addressU);
		D3DTextureAddressV = D3D9Translator::getD3DTextureAddressMode(samp.addressV);
		D3DTextureAddressW = D3D9Translator::getD3DTextureAddressMode(samp.addressW);

		D3DMinFilter = D3D9Translator::getD3DTextureFilterMode(samp.minFilter);
		D3DMagFilter = D3D9Translator::getD3DTextureFilterMode(samp.magFilter);
		D3DMipFilter = D3D9Translator::getD3DTextureFilterMode(samp.mipFilter);

		D3DSamplerComparison = D3D9Translator::getD3DCompareFunction(samp.samplerComparison);
		D3DBorderColor = D3D9Translator::getD3DColor(samp.borderColor);
	}

	void D3D9TextureSamplerObject::getTextureSampler( DTextureSampler& outSamp )
	{
		outSamp = mPreserveSamp;
	}



}
