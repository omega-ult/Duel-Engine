//  [6/12/2014 OMEGA] created

#ifndef _DUELD3D9RENDERSTATE_H_
#define _DUELD3D9RENDERSTATE_H_

#include "DuelD3D9Common.h"
#include "DuelRenderState.h"

namespace Duel
{
	class D3D9RasterizerStateObject : public DRasterizerStateObject
	{
		DUEL_DECLARE_RTTI(D3D9RasterizerStateObject)
	public:
		D3D9RasterizerStateObject(const DRasterizerState& state);
		virtual void getRasterizerState( DRasterizerState& outState );

	protected:
		// just a copy.
		DRasterizerState	mState;

	};

	class D3D9DepthStencilStateObject : public DDepthStencilStateObject
	{
		DUEL_DECLARE_RTTI(D3D9DepthStencilStateObject)
	public:
		D3D9DepthStencilStateObject(const DDepthStencilState& state);
		virtual void getDepthStencilState( DDepthStencilState& outState );
	
	protected:
		DDepthStencilState	mState;
	};

	class D3D9BlendStateObject : public DBlendStateObject
	{
		DUEL_DECLARE_RTTI(D3D9BlendStateObject)
	public:
		D3D9BlendStateObject(const DBlendState& state);
		virtual void getBlendState( DBlendState& outState );
	protected:
		DBlendState		mState;

	};

	class D3D9TextureSamplerObject : public DTextureSamplerObject
	{
		DUEL_DECLARE_RTTI(D3D9TextureSamplerObject)
	public:
		D3D9TextureSamplerObject(const DTextureSampler& samp);
		virtual void getTextureSampler( DTextureSampler& outSamp );
	protected:
		DTextureSampler mSamp;

	};
}

#endif