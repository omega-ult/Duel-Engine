//  [6/12/2014 OMEGA] created

#ifndef _DUELD3D9RENDERSTATE_H_
#define _DUELD3D9RENDERSTATE_H_

#include "DuelD3D9Common.h"
#include "DuelRenderState.h"
#include "DuelD3D9Include.h"

namespace Duel
{
	class D3D9RasterizerStateObject : public DRasterizerStateObject
	{
		DUEL_DECLARE_RTTI(D3D9RasterizerStateObject)
	public:
		D3D9RasterizerStateObject(const DRasterizerState& state);
		virtual void getRasterizerState( DRasterizerState& outState );

		// public for efficiency
		D3DFILLMODE		D3DPolygonMode;
		D3DSHADEMODE	D3DShadeMode;
		D3DCULL			D3DCullMode;

		DRasterizerState	mPreserveState;

	};

	class D3D9DepthStencilStateObject : public DDepthStencilStateObject
	{
		DUEL_DECLARE_RTTI(D3D9DepthStencilStateObject)
	public:
		D3D9DepthStencilStateObject(const DDepthStencilState& state);
		virtual void getDepthStencilState( DDepthStencilState& outState );
	
		D3DCMPFUNC		D3DDepthComparison;

		D3DCMPFUNC		D3DFrontStencilComparison;
		D3DSTENCILOP	D3DFrontStencilFail;
		D3DSTENCILOP	D3DFrontStencilDepthFail;
		D3DSTENCILOP	D3DFrontStencilPass;

		D3DCMPFUNC		D3DBackStencilComparison;
		D3DSTENCILOP	D3DBackStencilFail;
		D3DSTENCILOP	D3DBackStencilDepthFail;
		D3DSTENCILOP	D3DBackStencilPass;



		DDepthStencilState	mPreserveState;
	};

	class D3D9BlendStateObject : public DBlendStateObject
	{
		DUEL_DECLARE_RTTI(D3D9BlendStateObject)
	public:
		D3D9BlendStateObject(const DBlendState& state);
		virtual void getBlendState( DBlendState& outState );

		D3DBLENDOP	D3DColorBlendOperation[DUEL_MAX_BLEND_LAYERS];
		D3DBLEND	D3DColorSrcFactor[DUEL_MAX_BLEND_LAYERS];
		D3DBLEND	D3DColorDstFactor[DUEL_MAX_BLEND_LAYERS];

		D3DBLENDOP	D3DAlphaBlendOperation[DUEL_MAX_BLEND_LAYERS];
		D3DBLEND	D3DAlphaSrcFactor[DUEL_MAX_BLEND_LAYERS];
		D3DBLEND	D3DAlphaDstFactor[DUEL_MAX_BLEND_LAYERS];

		DBlendState		mPreserveState;

	};

	class D3D9TextureSamplerObject : public DTextureSamplerObject
	{
		DUEL_DECLARE_RTTI(D3D9TextureSamplerObject)
	public:
		D3D9TextureSamplerObject(const DTextureSampler& samp);
		virtual void getTextureSampler( DTextureSampler& outSamp );

		D3DTEXTUREADDRESS	D3DTextureAddressU;
		D3DTEXTUREADDRESS	D3DTextureAddressV;
		D3DTEXTUREADDRESS	D3DTextureAddressW;
		D3DTEXTUREFILTERTYPE	D3DMinFilter;
		D3DTEXTUREFILTERTYPE	D3DMagFilter;
		D3DTEXTUREFILTERTYPE	D3DMipFilter;
		D3DCMPFUNC			D3DSamplerComparison;
		D3DCOLOR			D3DBorderColor;

		DTextureSampler mPreserveSamp;

	};
}

#endif