//  [6/6/2014 OMEGA] created

#ifndef _DUELD3D9TRANSLATOR_H_
#define _DUELD3D9TRANSLATOR_H_

#include "DuelD3D9Common.h"
#include "Duel.h"
#include "DuelD3D9Include.h"


namespace Duel
{
	// for translation between abstract type and dx type.
	class D3D9Translator
	{
	public:
		static	D3DFORMAT			getD3DFormat(DPixelFormat pf);
		static	DPixelFormat		getPixelFormat(D3DFORMAT df);
		static	DXGI_FORMAT			getDXGIFormat(DPixelFormat pf);
		static	DPixelFormat		getPixelFormat(DXGI_FORMAT df);
		static	D3DSTENCILOP		getD3DStencilOperation(StencilOperation so);
		static	D3DSHADEMODE		getD3DShadeMode(ShadeMode sm);
		static	D3DBLENDOP			getD3DBlendOperation(BlendOperation bo);
		static	D3DBLEND			getD3DBlendFactor(BlendFactor bf);
		static	D3DCULL				getD3DCullingMode(CullingMode cm);
		static	D3DTEXTUREADDRESS	getD3DTextureAddressMode(TextureAddressMode tam);
		static	D3DTEXTUREFILTERTYPE	getD3DTextureFilterMode(FilterOption fo);
		static	D3DCOLOR			getD3DColor(DColor c);
		static	D3DFILLMODE			getD3DFillMode(PolygonMode pm);
		static	D3DCMPFUNC				getD3DCompareFunction(CompareFunction cf);
		static	DWORD				getD3DUsage(HardwareBufferUsage usage);
		static	DWORD				getD3DLock(HardwareBufferLock lock, HardwareBufferUsage usage);
		static	D3DDECLTYPE			getD3DDeclaraType(VertexElementType elemType);
		static	D3DDECLUSAGE		getD3DDeclaraUsage(VertexElementSemantic elemSem);
		static	D3DPRIMITIVETYPE	getD3DPrimitiveType(PrimitiveTopology tp);
	protected:
		// do not modify these maps!!
		typedef	std::map<DPixelFormat, DXGI_FORMAT>	PixelFormatToDXGIMap;
		typedef	std::map<DPixelFormat, D3DFORMAT>	PixelFormatToD3DFORMATMap;
		static const	PixelFormatToDXGIMap		DXGIMap;
		static const	PixelFormatToD3DFORMATMap	D3DFORMATMap;
		static	PixelFormatToDXGIMap		initDXGIMap();
		static	PixelFormatToD3DFORMATMap	initD3DFORMATMap();
	};

}

#endif