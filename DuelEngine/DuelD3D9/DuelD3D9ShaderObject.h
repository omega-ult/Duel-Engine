//  [6/6/2014 OMEGA] created

#ifndef _DUELD3D9SHADEROBJECT_H_
#define _DUELD3D9SHADEROBJECT_H_

#include "DuelD3D9Common.h"
#include "DuelShaderObject.h"
#include "DuelD3D9Include.h"

namespace Duel
{
	class D3D9GpuTextureConstant : public DGpuTextureConstant
	{
		DUEL_DECLARE_RTTI(D3D9GpuTextureConstant)
	public:
		D3D9GpuTextureConstant(IDirect3DBaseTexture9* tex);

		virtual bool isValid();

		virtual void setSamplerObject( DTextureSamplerObjectPtr samp );

		void	discard() { mbValid = false; }

		IDirect3DBaseTexture9*	getD3DTexture() { return mTexture; }
		DTextureSamplerObjectPtr	getSampler() { return mTexSamp; }

	protected:
		bool	mbValid;
		IDirect3DBaseTexture9*	mTexture;
		DTextureSamplerObjectPtr	mTexSamp;
	};

	class D3D9ShaderObject : public DShaderObject
	{
		DUEL_DECLARE_RTTI(D3D9ShaderObject)
	public:
		D3D9ShaderObject(D3D9RenderResourceFactory* fact);


		// we assert the vertex shader and pixel shader are all loaded.
		void	build(DRenderPass* pass);

		virtual DGpuParameters* getVertexProgramParameters();

		virtual DGpuParameters* getPixelProgramParameters();

		IDirect3DVertexShader9*	getD3DVertexShader() { return mVShader; }
		IDirect3DPixelShader9*	getD3DPixelShader() { return mPShader; }

	protected:		
		void	processParam( DGpuParameters& param, ID3DXConstantTable* constTable,
			D3DXHANDLE parent, DString prefix, uint32 index );


		D3D9RenderResourceFactory*	mCreator;
		IDirect3DVertexShader9*	mVShader;
		IDirect3DPixelShader9*	mPShader;

		DGpuParameters		mVSParameter;
		DGpuParameters		mPSParameter;
	};
}

#endif