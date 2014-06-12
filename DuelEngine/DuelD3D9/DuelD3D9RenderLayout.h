//  [6/6/2014 OMEGA] created

#ifndef _DUELD3D9RENDERLAYOUT_H_
#define _DUELD3D9RENDERLAYOUT_H_

#include "DuelD3D9Common.h"
#include "DuelRenderLayout.h"

namespace Duel
{
	class D3D9RenderLayout : public DRenderLayout
	{
		DUEL_DECLARE_RTTI(D3D9RenderLayout)
	public:
		D3D9RenderLayout(DRenderResourceFactory* creator);
	};

}


#endif