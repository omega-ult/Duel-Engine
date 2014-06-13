//  [6/12/2014 OMEGA] created

#include "DuelD3D9Common.h"
#include "DuelD3D9RenderState.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(D3D9RasterizerStateObject, DRasterizerStateObject);
	DUEL_IMPLEMENT_RTTI_1(D3D9DepthStencilStateObject, DDepthStencilStateObject);
	DUEL_IMPLEMENT_RTTI_1(D3D9BlendStateObject, DBlendStateObject);
	DUEL_IMPLEMENT_RTTI_1(D3D9TextureSamplerObject, DTextureSamplerObject);

	D3D9RasterizerStateObject::D3D9RasterizerStateObject( const DRasterizerState& state ) :
		mState(state)
	{

	}

	void D3D9RasterizerStateObject::getRasterizerState( DRasterizerState& outState )
	{
		outState = mState;
	}

	D3D9DepthStencilStateObject::D3D9DepthStencilStateObject( const DDepthStencilState& state ) :
		mState(state)
	{

	}

	void D3D9DepthStencilStateObject::getDepthStencilState( DDepthStencilState& outState )
	{
		outState = mState;
	}

	D3D9BlendStateObject::D3D9BlendStateObject( const DBlendState& state ) :
		mState(state)
	{

	}

	void D3D9BlendStateObject::getBlendState( DBlendState& outState )
	{
		outState = mState;
	}

	D3D9TextureSamplerObject::D3D9TextureSamplerObject( const DTextureSampler& samp ) :
		mSamp(samp)
	{

	}

	void D3D9TextureSamplerObject::getTextureSampler( DTextureSampler& outSamp )
	{
		outSamp = mSamp;
	}



}
