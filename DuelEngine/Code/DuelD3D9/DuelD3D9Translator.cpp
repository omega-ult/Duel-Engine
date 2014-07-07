//  [6/6/2014 OMEGA] created

#include "DuelD3D9Common.h"
#include "DuelD3D9Translator.h"

namespace Duel
{

	const	D3D9Translator::PixelFormatToD3DFORMATMap D3D9Translator::D3DFORMATMap = D3D9Translator::initD3DFORMATMap();

	const	D3D9Translator::PixelFormatToDXGIMap D3D9Translator::DXGIMap = D3D9Translator::initDXGIMap();

	D3DFORMAT D3D9Translator::getD3DFormat( DPixelFormat pf )
	{
		PixelFormatToD3DFORMATMap::const_iterator i;
		i	= D3DFORMATMap.find(pf);

		if (i != D3DFORMATMap.end())
		{
			return	i->second;
		}

		return	D3DFMT_UNKNOWN;
	}

	Duel::DPixelFormat D3D9Translator::getPixelFormat( D3DFORMAT df )
	{

		PixelFormatToD3DFORMATMap::const_iterator	i;

		for (i	= D3DFORMATMap.begin(); i != D3DFORMATMap.end(); ++i)
		{
			// once hit
			if (i->second == df)
			{
				return	i->first;
			}
		}
		return	PF_Unknown;
	}

	Duel::DPixelFormat D3D9Translator::getPixelFormat( DXGI_FORMAT df )
	{
		PixelFormatToD3DFORMATMap::const_iterator i;

		for (i	= D3DFORMATMap.begin(); i != D3DFORMATMap.end(); ++i)
		{
			// once hit
			if (i->second == df)
			{
				return	i->first;
			}
		}
		return	PF_Unknown;
	}

	DXGI_FORMAT D3D9Translator::getDXGIFormat( DPixelFormat pf )
	{
		PixelFormatToDXGIMap::const_iterator i;
		i	= DXGIMap.find(pf);
		if (i != DXGIMap.end())
		{
			return	i->second;
		}
		return	DXGI_FORMAT_UNKNOWN;
	}

	D3DSTENCILOP D3D9Translator::getD3DStencilOperation( StencilOperation so )
	{
		switch(so)
		{
		case SO_Keep:
			return D3DSTENCILOP_KEEP;
		case SO_Zero:
			return D3DSTENCILOP_ZERO;
		case SO_Replace:
			return D3DSTENCILOP_REPLACE;
		case SO_Increment:
			return D3DSTENCILOP_INCR;
		case SO_Decrement:
			return D3DSTENCILOP_DECR;
		case SO_IncrementWrap:
			return D3DSTENCILOP_INCRSAT;
		case SO_DecrementWrap:
			return D3DSTENCILOP_DECRSAT;
		case SO_Invert:
			return D3DSTENCILOP_INVERT;
		}
		return D3DSTENCILOP_KEEP;
	}

	D3DSHADEMODE D3D9Translator::getD3DShadeMode( ShadeMode sm )
	{
		switch(sm)
		{
		case SM_Flat:
			return D3DSHADE_FLAT;
		case SM_Gouraud:
			return D3DSHADE_GOURAUD;
		case SM_Phong:
			return D3DSHADE_PHONG;
		}
		return D3DSHADE_FORCE_DWORD;
	}

	D3DBLENDOP D3D9Translator::getD3DBlendOperation( BlendOperation bo )
	{
		switch(bo)
		{
		case BO_Add:
			return D3DBLENDOP_ADD;
		case BO_Sub:
			return D3DBLENDOP_SUBTRACT;
		case BO_RevSub:
			return D3DBLENDOP_REVSUBTRACT;
		case BO_Min:
			return D3DBLENDOP_MIN;
		case BO_Max:
			return D3DBLENDOP_MAX;
		}
		return D3DBLENDOP_ADD;
	}

	D3DBLEND D3D9Translator::getD3DBlendFactor( BlendFactor bf )
	{
		switch(bf)
		{
		case BF_Zero:
			return D3DBLEND_ZERO;
		case BF_One:
			return D3DBLEND_ONE;
		case BF_SrcAlpha:
			return D3DBLEND_SRCALPHA;
		case BF_DstAlpha:
			return D3DBLEND_DESTALPHA;
		case BF_InvSrcAlpha:
			return D3DBLEND_INVSRCALPHA;
		case BF_InvDstAlpha:
			return D3DBLEND_INVDESTALPHA;
		case BF_SrcColor:
			return D3DBLEND_SRCCOLOR;
		case BF_DstColor:
			return D3DBLEND_DESTCOLOR;
		case BF_InvSrcColor:
			return D3DBLEND_INVSRCCOLOR;
		case BF_InvDstColor:
			return D3DBLEND_INVDESTCOLOR;
		case BF_SrcAlphaSat:
			return D3DBLEND_SRCALPHASAT;
		}
		return D3DBLEND_ZERO;
	}

	D3DCULL D3D9Translator::getD3DCullingMode( CullMode cm )
	{
		switch(cm)
		{
		case CM_None:
			return D3DCULL_NONE;
		case CM_Front:
			return D3DCULL_CCW;
		case CM_Back:
			return D3DCULL_CW;
		}
		return D3DCULL_NONE;
	}

	D3DTEXTUREADDRESS D3D9Translator::getD3DTextureAddressMode( TextureAddressMode tam )
	{
		switch(tam)
		{
		case TAM_Wrap:
			return D3DTADDRESS_WRAP;
		case TAM_Mirror:
			return D3DTADDRESS_MIRROR;
		case TAM_Clamp:
			return D3DTADDRESS_CLAMP;
		case TAM_Border:
			return D3DTADDRESS_BORDER;
		}
		return D3DTADDRESS_WRAP;
	}

	D3DTEXTUREFILTERTYPE D3D9Translator::getD3DTextureFilterMode( FilterOption fo )
	{
		switch(fo)
		{
		case FO_None:
			return D3DTEXF_NONE;
		case FO_Point:
			return D3DTEXF_POINT;
		case FO_Linear:
			return D3DTEXF_LINEAR;
		case FO_Anistropic:
			return D3DTEXF_ANISOTROPIC;
		}
		return D3DTEXF_POINT;
	}

	D3DCOLOR D3D9Translator::getD3DColor( DColor c )
	{
		DColor	clamped = c;
		clamped.clamp();

		return D3DCOLOR_ARGB(	static_cast<uint8>(clamped.a * 255),
			static_cast<uint8>(clamped.r * 255),
			static_cast<uint8>(clamped.g * 255),
			static_cast<uint8>(clamped.b * 255));
	}

	D3DFILLMODE D3D9Translator::getD3DFillMode( PolygonMode pm )
	{
		switch(pm)
		{
		case PM_Points:
			return D3DFILL_POINT;
		case PM_Wireframe:
			return D3DFILL_WIREFRAME;
		case PM_Solid:
			return D3DFILL_SOLID;
		}
		return D3DFILL_FORCE_DWORD;
	}

	D3DCMPFUNC D3D9Translator::getD3DCompareFunction( CompareFunction cf )
	{
		switch(cf)
		{
		case CF_AlwaysFail:
			return D3DCMP_NEVER;
		case CF_AlwaysPass:
			return D3DCMP_ALWAYS;
		case CF_Less:
			return D3DCMP_LESS;
		case CF_LessEqual:
			return D3DCMP_LESSEQUAL;
		case CF_Equal:
			return D3DCMP_EQUAL;
		case CF_NotEqual:
			return D3DCMP_NOTEQUAL;
		case CF_Greater:
			return D3DCMP_GREATER;
		case CF_GreaterEqual:
			return D3DCMP_GREATEREQUAL;
		}
		return D3DCMP_FORCE_DWORD;
	}

	DWORD D3D9Translator::getD3DUsage( HardwareBufferUsage usage )
	{
		DWORD ret = 0;
		if (usage & HBU_Dynamic)
		{
			ret |= D3DUSAGE_DYNAMIC;
		}
		if (usage & HBU_WriteOnly)
		{
			ret |= D3DUSAGE_WRITEONLY;
		}
		return ret;
	}

	DWORD D3D9Translator::getD3DLock( HardwareBufferLock lock, HardwareBufferUsage usage )
	{
		DWORD ret = 0;
		if (lock == HBL_Discard)
		{
			// D3D doesn't like discard or no_overwrite on non-dynamic buffers
			if (usage & HBU_Dynamic)
			{
				ret |= D3DLOCK_DISCARD;
			}
		}
		if (lock == HBL_ReadOnly)
		{
			// D3D debug runtime doesn't like you locking managed buffers readonly
			// when they were created with write-only (even though you CAN read
			// from the software backed version)
			if (!(usage & HBU_WriteOnly))
				ret |= D3DLOCK_READONLY;
		}
		if (lock == HBL_NoOverwrite)
		{
			// D3D doesn't like discard or no_overwrite on non-dynamic buffers
			if (usage & HBU_Dynamic)
			{
				ret |= D3DLOCK_NOOVERWRITE;
			}
		}
		return ret;
	}

	D3DDECLTYPE D3D9Translator::getD3DDeclaraType( VertexElementType elemType )
	{
		switch (elemType)
		{
		case VET_ARGB:
		case VET_ABGR:
			return D3DDECLTYPE_D3DCOLOR;
			break;
		case VET_Float1:
			return D3DDECLTYPE_FLOAT1;
			break;
		case VET_Float2:
			return D3DDECLTYPE_FLOAT2;
			break;
		case VET_Float3:
			return D3DDECLTYPE_FLOAT3;
			break;
		case VET_Float4:
			return D3DDECLTYPE_FLOAT4;
			break;
		case VET_Short2:
			return D3DDECLTYPE_SHORT2;
			break;
		case VET_Short4:
			return D3DDECLTYPE_SHORT4;
			break;
		case VET_UByte4:
			return D3DDECLTYPE_UBYTE4;
			break;
		}
		// to keep compiler happy
		return D3DDECLTYPE_FLOAT3;
	}

	D3DDECLUSAGE D3D9Translator::getD3DDeclaraUsage( VertexElementSemantic elemSem )
	{
		switch (elemSem)
		{
		case VES_BlendIndices:
			return D3DDECLUSAGE_BLENDINDICES;
			break;
		case VES_BlendWeight:
			return D3DDECLUSAGE_BLENDWEIGHT;
			break;
		case VES_Diffuse:
			return D3DDECLUSAGE_COLOR; // NB index will differentiate
			break;
		case VES_Specular:
			return D3DDECLUSAGE_COLOR; // NB index will differentiate
			break;
		case VES_Normal:
			return D3DDECLUSAGE_NORMAL;
			break;
		case VES_Position:
			return D3DDECLUSAGE_POSITION;
			break;
		case VES_TexCoord:
			return D3DDECLUSAGE_TEXCOORD;
			break;
		case VES_Binormal:
			return D3DDECLUSAGE_BINORMAL;
			break;
		case VES_Tangent:
			return D3DDECLUSAGE_TANGENT;
			break;
		}
		// to keep compiler happy
		return D3DDECLUSAGE_POSITION;
	}

	D3D9Translator::PixelFormatToDXGIMap D3D9Translator::initDXGIMap()
	{

		PixelFormatToDXGIMap retMap;

		retMap.insert(std::make_pair(PF_Unknown, DXGI_FORMAT_UNKNOWN));

		retMap.insert(std::make_pair(PF_R32G32B32A32, DXGI_FORMAT_R32G32B32A32_UINT));
		retMap.insert(std::make_pair(PF_R32G32B32A32_Float, DXGI_FORMAT_R32G32B32A32_FLOAT));

		retMap.insert(std::make_pair(PF_R32G32B32, DXGI_FORMAT_R32G32B32_UINT));
		retMap.insert(std::make_pair(PF_R32G32B32_Float, DXGI_FORMAT_R32G32B32_FLOAT));

		retMap.insert(std::make_pair(PF_R16G16B16A16, DXGI_FORMAT_R16G16B16A16_UNORM));
		retMap.insert(std::make_pair(PF_Q16W16V16U16, DXGI_FORMAT_R16G16B16A16_SNORM));
		retMap.insert(std::make_pair(PF_R16G16B16A16_Float, DXGI_FORMAT_R16G16B16A16_FLOAT));

		retMap.insert(std::make_pair(PF_G32R32, DXGI_FORMAT_R32G32_UINT));
		retMap.insert(std::make_pair(PF_G32R32_Float, DXGI_FORMAT_R32G32_FLOAT));

		retMap.insert(std::make_pair(PF_A2B10G10R10, DXGI_FORMAT_R10G10B10A2_UNORM));
		retMap.insert(std::make_pair(PF_A2R10G10B10, DXGI_FORMAT_UNKNOWN)); // not available.

		retMap.insert(std::make_pair(PF_G16R16, DXGI_FORMAT_R16G16_UNORM));
		retMap.insert(std::make_pair(PF_G16R16_Float, DXGI_FORMAT_R16G16_FLOAT));

		retMap.insert(std::make_pair(PF_R32, DXGI_FORMAT_R32_UINT));
		retMap.insert(std::make_pair(PF_D32_Float, DXGI_FORMAT_D32_FLOAT));
		retMap.insert(std::make_pair(PF_R32_Float, DXGI_FORMAT_R32_FLOAT));

		retMap.insert(std::make_pair(PF_A8B8G8R8, DXGI_FORMAT_R8G8B8A8_UNORM));
		retMap.insert(std::make_pair(PF_A8R8G8B8, DXGI_FORMAT_B8G8R8A8_UNORM));
		retMap.insert(std::make_pair(PF_B8G8R8A8, DXGI_FORMAT_UNKNOWN)); // not available.
		retMap.insert(std::make_pair(PF_R8G8B8A8, DXGI_FORMAT_UNKNOWN)); // not available.
		retMap.insert(std::make_pair(PF_X8R8G8B8, DXGI_FORMAT_B8G8R8X8_UNORM));
		retMap.insert(std::make_pair(PF_X8B8G8R8, DXGI_FORMAT_UNKNOWN));	// not available

		retMap.insert(std::make_pair(PF_R8G8B8, DXGI_FORMAT_UNKNOWN));	// not available
		retMap.insert(std::make_pair(PF_B8G8R8, DXGI_FORMAT_UNKNOWN));	// not available

		retMap.insert(std::make_pair(PF_A1R5G5B5, DXGI_FORMAT_B5G5R5A1_UNORM));	// only available in DXGI 1.2
		retMap.insert(std::make_pair(PF_X1R5G5B5, DXGI_FORMAT_UNKNOWN));	// not available.
		retMap.insert(std::make_pair(PF_R5G6B5, DXGI_FORMAT_B5G6R5_UNORM));
		retMap.insert(std::make_pair(PF_B5G6R5, DXGI_FORMAT_UNKNOWN));	// not available.

		retMap.insert(std::make_pair(PF_A4R4G4B4, DXGI_FORMAT_UNKNOWN));	// only available in DXGI 1.2
		retMap.insert(std::make_pair(PF_X4R4G4B4, DXGI_FORMAT_UNKNOWN));	// not available

		retMap.insert(std::make_pair(PF_R16, DXGI_FORMAT_R16_UNORM));
		retMap.insert(std::make_pair(PF_R16_Float, DXGI_FORMAT_R16_FLOAT));
		retMap.insert(std::make_pair(PF_D16, DXGI_FORMAT_D16_UNORM));
		retMap.insert(std::make_pair(PF_A8L8, DXGI_FORMAT_UNKNOWN));	// not available
		retMap.insert(std::make_pair(PF_L16, DXGI_FORMAT_R16_UNORM));
		retMap.insert(std::make_pair(PF_R8, DXGI_FORMAT_R8_UNORM));
		retMap.insert(std::make_pair(PF_A8, DXGI_FORMAT_A8_UNORM));
		retMap.insert(std::make_pair(PF_L8, DXGI_FORMAT_R8_UNORM));
		retMap.insert(std::make_pair(PF_A4L4, DXGI_FORMAT_UNKNOWN));	// not available

		retMap.insert(std::make_pair(PF_BC1, DXGI_FORMAT_BC1_UNORM));
		retMap.insert(std::make_pair(PF_BC2, DXGI_FORMAT_BC2_UNORM));
		retMap.insert(std::make_pair(PF_BC3, DXGI_FORMAT_BC3_UNORM));
		retMap.insert(std::make_pair(PF_BC4, DXGI_FORMAT_BC4_UNORM));
		retMap.insert(std::make_pair(PF_BC5, DXGI_FORMAT_BC5_UNORM));

		return	retMap;
	}

	D3D9Translator::PixelFormatToD3DFORMATMap D3D9Translator::initD3DFORMATMap()
	{

		PixelFormatToD3DFORMATMap retMap;

		//////////////////////////////////////////////////////////////////////////

		retMap.insert(std::make_pair(PF_Unknown, D3DFMT_UNKNOWN));

		retMap.insert(std::make_pair(PF_R32G32B32A32, D3DFMT_UNKNOWN)); // not available
		retMap.insert(std::make_pair(PF_R32G32B32A32_Float, D3DFMT_A32B32G32R32F));

		retMap.insert(std::make_pair(PF_R32G32B32, D3DFMT_UNKNOWN)); // not available
		retMap.insert(std::make_pair(PF_R32G32B32_Float, D3DFMT_UNKNOWN)); // not available

		retMap.insert(std::make_pair(PF_R16G16B16A16, D3DFMT_A16B16G16R16));
		retMap.insert(std::make_pair(PF_Q16W16V16U16, D3DFMT_Q16W16V16U16));
		retMap.insert(std::make_pair(PF_R16G16B16A16_Float, D3DFMT_A16B16G16R16F));

		retMap.insert(std::make_pair(PF_G32R32, D3DFMT_UNKNOWN)); // not available
		retMap.insert(std::make_pair(PF_G32R32_Float, D3DFMT_G32R32F));

		retMap.insert(std::make_pair(PF_A2B10G10R10, D3DFMT_A2B10G10R10));
		retMap.insert(std::make_pair(PF_A2R10G10B10, D3DFMT_A2R10G10B10));

		retMap.insert(std::make_pair(PF_G16R16, D3DFMT_UNKNOWN)); // not available
		retMap.insert(std::make_pair(PF_G16R16_Float, D3DFMT_G16R16F));

		retMap.insert(std::make_pair(PF_R32, D3DFMT_INDEX32));
		retMap.insert(std::make_pair(PF_D32_Float, D3DFMT_D32));
		retMap.insert(std::make_pair(PF_R32_Float, D3DFMT_R32F));

		retMap.insert(std::make_pair(PF_A8B8G8R8, D3DFMT_A8B8G8R8));
		retMap.insert(std::make_pair(PF_A8R8G8B8, D3DFMT_A8R8G8B8));
		retMap.insert(std::make_pair(PF_B8G8R8A8, D3DFMT_UNKNOWN)); // not available.
		retMap.insert(std::make_pair(PF_R8G8B8A8, D3DFMT_UNKNOWN)); // not available.
		retMap.insert(std::make_pair(PF_X8R8G8B8, D3DFMT_X8R8G8B8));
		retMap.insert(std::make_pair(PF_X8B8G8R8, D3DFMT_X8B8G8R8));	

		retMap.insert(std::make_pair(PF_R8G8B8, D3DFMT_R8G8B8));
		retMap.insert(std::make_pair(PF_B8G8R8, D3DFMT_UNKNOWN));	// not available

		retMap.insert(std::make_pair(PF_A1R5G5B5, D3DFMT_A1R5G5B5));
		retMap.insert(std::make_pair(PF_X1R5G5B5, D3DFMT_X1R5G5B5));
		retMap.insert(std::make_pair(PF_R5G6B5, D3DFMT_R5G6B5));
		retMap.insert(std::make_pair(PF_B5G6R5, D3DFMT_UNKNOWN));	// not available.

		retMap.insert(std::make_pair(PF_A4R4G4B4, D3DFMT_A4R4G4B4));
		retMap.insert(std::make_pair(PF_X4R4G4B4, D3DFMT_X4R4G4B4));	// not available

		retMap.insert(std::make_pair(PF_R16, D3DFMT_INDEX16));
		retMap.insert(std::make_pair(PF_R16_Float, D3DFMT_R16F));
		retMap.insert(std::make_pair(PF_D16, D3DFMT_D16));
		retMap.insert(std::make_pair(PF_A8L8, D3DFMT_A8L8));	// not available
		retMap.insert(std::make_pair(PF_L16, D3DFMT_L16));
		retMap.insert(std::make_pair(PF_R8, D3DFMT_UNKNOWN));	// not available
		retMap.insert(std::make_pair(PF_A8, D3DFMT_A8));
		retMap.insert(std::make_pair(PF_L8, D3DFMT_L8));
		retMap.insert(std::make_pair(PF_A4L4, D3DFMT_A4L4));	// not available

		retMap.insert(std::make_pair(PF_BC1, D3DFMT_DXT1));
		retMap.insert(std::make_pair(PF_BC2, D3DFMT_DXT2));
		retMap.insert(std::make_pair(PF_BC3, D3DFMT_DXT3));
		retMap.insert(std::make_pair(PF_BC4, D3DFMT_DXT4));
		retMap.insert(std::make_pair(PF_BC5, D3DFMT_DXT5));

		return	retMap;
	}

	D3DPRIMITIVETYPE D3D9Translator::getD3DPrimitiveType( PrimitiveTopology tp )
	{
		switch (tp)
		{
		case PT_PointList:
			return D3DPT_POINTLIST;
		case PT_LineList:
			return D3DPT_LINELIST;
		case PT_LineStrip:
			return D3DPT_LINESTRIP;
		case PT_TriangleList:
			return D3DPT_TRIANGLELIST;
		case PT_TriangleStrip:
			return D3DPT_TRIANGLESTRIP;
		case PT_TriangleFan:
			return D3DPT_TRIANGLEFAN;
		default:
			return D3DPT_POINTLIST;
		}
	}


}