//  [6/6/2014 OMEGA] created

#include "DuelD3D9Common.h"
#include "DuelD3D9RenderSystem.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(D3D9RenderSystem, DRenderSystem);

	D3D9RenderSystem::D3D9RenderSystem()
	{

	}

	const DString& D3D9RenderSystem::getName() const
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void D3D9RenderSystem::shutdown()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void D3D9RenderSystem::bindFrameBuffer( DFrameBuffer* buf )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	DFrameBuffer* D3D9RenderSystem::getCurrentFrameBuffer()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void D3D9RenderSystem::setRasterizerState( DRasterizerStateObject* rs )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void D3D9RenderSystem::setDepthStencilState( DDepthStencilStateObject* dss, uint32 frontStencilRef /*= 0*/, uint32 backStencilRef /*= 0 */ )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void D3D9RenderSystem::setBlendState( DBlendStateObject* sbs, const DColor& blendFactor )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	Duel::DRasterizerStateObjectPtr D3D9RenderSystem::getCurrentRaseterizerState()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	Duel::DDepthStencilStateObjectPtr D3D9RenderSystem::getCurrentDepthStencilStateObject()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	Duel::DBlendStateObjectPtr D3D9RenderSystem::getCurrentBlendState()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void D3D9RenderSystem::bindShaderObject( DShaderObject* so )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void D3D9RenderSystem::render( DRenderLayout* layout )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void D3D9RenderSystem::fillDeviceCaps()
	{
		throw std::exception("The method or operation is not implemented.");
	}

}