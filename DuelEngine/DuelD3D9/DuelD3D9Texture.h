//  [6/6/2014 OMEGA] created

#ifndef _DUELD3D9TEXTURE_H_
#define _DUELD3D9TEXTURE_H_

#include "DuelD3D9Common.h"
#include "DuelTexture.h"

namespace Duel
{

	class D3D9Texture : public DTexture
	{
		DUEL_DECLARE_RTTI(D3D9Texture)
	public:
		D3D9Texture(DResourceManager* fathermanager, const DString& name, const DString& groupName);
		virtual bool isHardwareGammaSupported();

		virtual bool isHardwareMipMapSupported();

		virtual void copyToTexture( DTexture* dstTex );

		virtual DPixelBufferPtr getBuffer( CubeFace face = CF_PositiveX, size_t mipmap = 0 );

		virtual DGpuTextureConstantPtr getGpuTexutureConstant();

		virtual void createHardwareResource();

		virtual void releaseHardwareResource();


	};
}

#endif