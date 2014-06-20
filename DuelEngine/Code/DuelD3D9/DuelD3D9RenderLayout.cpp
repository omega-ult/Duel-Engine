//  [6/6/2014 OMEGA] created

#include "DuelD3D9Common.h"
#include "DuelD3D9RenderLayout.h"
#include "DuelD3D9RenderResourceFactory.h"
#include "DuelD3D9Translator.h"


namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(D3D9RenderLayout, DRenderLayout);

	D3DDECLTYPE	getD3DElementType(VertexElementType elem)
	{
		switch (elem)
		{
		case VET_ARGB:
		case VET_ABGR:
			return D3DDECLTYPE_D3DCOLOR;
		case VET_Float1:
			return D3DDECLTYPE_FLOAT1;
		case VET_Float2:
			return D3DDECLTYPE_FLOAT2;
		case VET_Float3:
			return D3DDECLTYPE_FLOAT3;
		case VET_Float4:
			return D3DDECLTYPE_FLOAT4;
		case VET_UByte4:
			return D3DDECLTYPE_UBYTE4N;
		case VET_Short2:
			return D3DDECLTYPE_SHORT2N;
		case VET_Short4:
			return D3DDECLTYPE_SHORT4N;
		case VET_UShort2:
			return D3DDECLTYPE_USHORT2N;
		case VET_UShort4:
			return D3DDECLTYPE_USHORT4N;
		default:
			DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
				"Unsupported vertex format encountered",
				"Duel::D3D9RenderLayout::seal")
			break;
		}
	}

	D3DDECLUSAGE getD3DElementUsage(VertexElementSemantic elem)
	{
		switch (elem)
		{
		case VES_Position:
			return D3DDECLUSAGE_POSITION;
		case VES_BlendWeight:
			return D3DDECLUSAGE_BLENDWEIGHT;
		case VES_BlendIndices:
			return D3DDECLUSAGE_BLENDINDICES;
		case VES_Normal:
			return D3DDECLUSAGE_NORMAL;
		case VES_TexCoord:
			return D3DDECLUSAGE_TEXCOORD;
		case VES_Diffuse:
		case VES_Specular:
			return D3DDECLUSAGE_COLOR;
		case VES_Binormal:
			return D3DDECLUSAGE_BINORMAL;
		case VES_Tangent:
			return D3DDECLUSAGE_TANGENT;
		default:
			DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
				"Unsupported vertex semantic encountered",
				"Duel::D3D9RenderLayout::seal")
			break;
		}
	}

	D3D9RenderLayout::D3D9RenderLayout( DRenderResourceFactory* creator ) :
		DRenderLayout(creator),
		mVDecl(NULL),
		mD3DPrimitiveType(D3DPT_POINTLIST)
	{

	}


	void D3D9RenderLayout::seal()
	{
		IDirect3DDevice9* dev = mCreator->getAs<D3D9RenderResourceFactory>()->getMainDevice();
		ReleaseCOM(mVDecl);

		DVertexDeclaration vDecl = getVertexData().getDeclaration();
		DVertexDeclaration::VertexElementIterator ei = vDecl.getVertexElementIterator();
		std::vector<D3DVERTEXELEMENT9> d3dDecl;
		while (ei.hasMoreElements())
		{
			const DVertexElement elem = ei.getNext();
			D3DVERTEXELEMENT9 d3dElem;
			d3dElem.Stream = elem.getSource();
			d3dElem.Offset = elem.getOffset();
			d3dElem.Type = getD3DElementType(elem.getElementType());
			d3dElem.Method = D3DDECLMETHOD_DEFAULT;
			d3dElem.Usage = getD3DElementUsage(elem.getSemantic());
			d3dElem.UsageIndex = (BYTE)elem.getSemanticIndex();
			if (elem.getSemantic() == VES_Specular)
			{
				d3dElem.UsageIndex = 1;
			}
			d3dDecl.push_back(d3dElem);
		}
		D3DVERTEXELEMENT9 end;
		end.Stream = 0xff;
		end.Offset = 0;
		end.Type = D3DDECLTYPE_UNUSED;
		end.Method = 0;
		end.Usage = 0;
		end.UsageIndex = 0;
		d3dDecl.push_back(end);

		dev->CreateVertexDeclaration(&d3dDecl[0], &mVDecl);

		mD3DPrimitiveType = D3D9Translator::getD3DPrimitiveType(mPrimitiveTopology);
	}

	IDirect3DVertexDeclaration9* D3D9RenderLayout::getD3DVertexDeclaration()
	{
		return mVDecl;
	}

}