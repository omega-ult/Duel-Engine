//  [6/6/2014 OMEGA] created

#ifndef _DUELD3D9TEXTURE_H_
#define _DUELD3D9TEXTURE_H_

#include "DuelD3D9Common.h"
#include "DuelTexture.h"
#include "DuelHardwareBuffer.h"
#include "DuelD3D9Include.h"

namespace Duel
{

	class D3D9Texture : public DTexture
	{
		DUEL_DECLARE_RTTI(D3D9Texture)
	public:
		D3D9Texture(DResourceManager* fathermanager, const DString& name, const DString& groupName);
		~D3D9Texture();
		virtual bool isHardwareGammaSupported();

		virtual bool isHardwareMipMapSupported();

		virtual void copyToTexture( DTexture* dstTex );

		virtual DPixelBufferPtr getBuffer( CubeFace face = CF_PositiveX, size_t mipmap = 0 );

		virtual DGpuTextureConstantPtr getGpuTexutureConstant();

		virtual void createHardwareResource();

		virtual void releaseHardwareResource();

		void	setHardwareBufferUsage(HardwareBufferUsage usage);

	protected:
		void	createSurfaceList();
		struct D3D9TexturePointer
		{
			D3D9TexturePointer() : pBaseTexture(NULL), p2DTexture(NULL), p3DTexture(NULL), pCubeTexture(NULL) {}
			IDirect3DBaseTexture9*	pBaseTexture;
			IDirect3DTexture9*		p2DTexture;
			IDirect3DVolumeTexture9*	p3DTexture;
			IDirect3DCubeTexture9*	pCubeTexture;
		};
		D3D9TexturePointer	mTexture;
		typedef	std::vector<DPixelBufferPtr>	SurfaceList;
		SurfaceList	mSurfaceList;
		// indicate the HardwareBuffers' usage
		HardwareBufferUsage mUsage;
		DGpuTextureConstantPtr	mTextureConstant;
	};
}

#endif