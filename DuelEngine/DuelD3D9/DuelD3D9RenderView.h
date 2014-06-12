//  [6/10/2014 OMEGA] created

#ifndef _DUELD3D9RENDERVIEW_H_
#define _DUELD3D9RENDERVIEW_H_

#include "DuelD3D9Common.h"
#include "DuelRenderView.h"

namespace Duel
{
	class DUELD3D9_API D3D9RenderColorView : public DRenderColorView
	{
		DUEL_DECLARE_RTTI(D3D9RenderColorView)
	public:
		D3D9RenderColorView(DRenderResourceFactory* creator, DPixelFormat fmt);
		virtual DGpuTextureConstantPtr getGpuTexutureConstant();

	};

	class DUELD3D9_API D3D9RenderDepthStencilView : public DRenderDepthStencilView
	{
		DUEL_DECLARE_RTTI(D3D9RenderDepthStencilView)
	public:
		D3D9RenderDepthStencilView(DRenderResourceFactory* creator);
	};

}

#endif