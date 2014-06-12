//  [6/7/2014 OMEGA] created

#include "DuelD3D9Common.h"
#include "DuelFrameBuffer.h"
#include "DuelD3D9FrameBuffer.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(D3D9FrameBuffer, DFrameBuffer);

	D3D9FrameBuffer::D3D9FrameBuffer( DRenderResourceFactory* creator, uint32 w, uint32 h, uint32 colorBits ) :
		DFrameBuffer(creator, w, h, colorBits)
	{

	}

	void D3D9FrameBuffer::attachRenderColorView( ElementAttachment elem, DRenderColorView* v )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void D3D9FrameBuffer::detachRenderColorView( ElementAttachment elem )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void D3D9FrameBuffer::attachRenderDepthStencilView( DRenderDepthStencilView* v )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void D3D9FrameBuffer::detachRenderDepthStencilView()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	DRenderColorView* D3D9FrameBuffer::getRenderColorView( ElementAttachment elem )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	DRenderDepthStencilView* D3D9FrameBuffer::getRenderDepthStencilView()
	{
		throw std::exception("The method or operation is not implemented.");
	}

}