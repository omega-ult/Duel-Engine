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

		IDirect3DBaseTexture9*	getTexture() { return mTexture; }
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
		D3D9ShaderObject();

		virtual DGpuParameters* getVertexProgramParameters();

		virtual DGpuParameters* getPixelProgramParameters();

	};
}

#endif