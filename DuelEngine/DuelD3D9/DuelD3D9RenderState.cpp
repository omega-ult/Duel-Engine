//  [6/12/2014 OMEGA] created

#include "DuelD3D9Common.h"
#include "DuelD3D9RenderState.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(D3D9RasterizerStateObject, DRasterizerStateObject);
	DUEL_IMPLEMENT_RTTI_1(D3D9DepthStencilStateObject, DDepthStencilStateObject);
	DUEL_IMPLEMENT_RTTI_1(D3D9BlendStateObject, DBlendStateObject);
	DUEL_IMPLEMENT_RTTI_1(D3D9TextureSamplerObject, DTextureSamplerObject);

	D3D9RasterizerStateObject::D3D9RasterizerStateObject( const DRasterizerState& state )
	{

	}

	void D3D9RasterizerStateObject::getRasterizerState( DRasterizerState& outState )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	D3D9DepthStencilStateObject::D3D9DepthStencilStateObject( const DDepthStencilState& state )
	{

	}

	void D3D9DepthStencilStateObject::getDepthStencilState( DDepthStencilState& outState )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	D3D9BlendStateObject::D3D9BlendStateObject( const DBlendState& state )
	{

	}

	void D3D9BlendStateObject::getBlendState( DBlendState& outState )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	D3D9TextureSamplerObject::D3D9TextureSamplerObject( const DTextureSampler& samp )
	{

	}

	void D3D9TextureSamplerObject::getTextureSampler( DTextureSampler& outSamp )
	{
		throw std::exception("The method or operation is not implemented.");
	}



}
