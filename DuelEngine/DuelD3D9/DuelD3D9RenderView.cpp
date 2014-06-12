//  [6/10/2014 OMEGA] created

#include "DuelD3D9Common.h"
#include "DuelD3D9RenderView.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(D3D9RenderColorView, DRenderColorView);
	DUEL_IMPLEMENT_RTTI_1(D3D9RenderDepthStencilView, DRenderDepthStencilView);


	D3D9RenderColorView::D3D9RenderColorView( DRenderResourceFactory* creator, DPixelFormat fmt ) :
		DRenderColorView(creator, fmt)
	{

	}

	Duel::DGpuTextureConstantPtr D3D9RenderColorView::getGpuTexutureConstant()
	{
		throw std::exception("The method or operation is not implemented.");
	}


	D3D9RenderDepthStencilView::D3D9RenderDepthStencilView( DRenderResourceFactory* creator ) : 
		DRenderDepthStencilView(creator)
	{

	}

}