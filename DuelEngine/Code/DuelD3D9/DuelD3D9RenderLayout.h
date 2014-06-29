//  [6/6/2014 OMEGA] created

#ifndef _DUELD3D9RENDERLAYOUT_H_
#define _DUELD3D9RENDERLAYOUT_H_

#include "DuelD3D9Common.h"
#include "DuelRenderLayout.h"
#include "DuelD3D9Include.h"

namespace Duel
{
	class D3D9RenderLayout : public DRenderLayout
	{
		DUEL_DECLARE_RTTI(D3D9RenderLayout)
	public:
		D3D9RenderLayout(DRenderResourceFactory* creator);
		~D3D9RenderLayout();
		void		seal();

		uint32		getVertexCount() { return mVertexCount; }
		IDirect3DVertexDeclaration9*	getD3DVertexDeclaration();
		D3DPRIMITIVETYPE	getD3DPrimitiveType() { return mD3DPrimitiveType; }
	protected:
		IDirect3DVertexDeclaration9*	mVDecl;
		D3DPRIMITIVETYPE	mD3DPrimitiveType;
		uint32		mVertexCount;
	};

}


#endif