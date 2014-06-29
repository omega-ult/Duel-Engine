//  [6/6/2014 OMEGA] created

#ifndef _DUELD3D9PIXELBUFFER_H_
#define _DUELD3D9PIXELBUFFER_H_

#include "DuelD3D9Common.h"
#include "DuelPixelBuffer.h"
#include "DuelD3D9Include.h"

namespace Duel
{

	class D3D9PixelBuffer : public DPixelBuffer
	{
		DUEL_DECLARE_RTTI(D3D9PixelBuffer)
	public:
		D3D9PixelBuffer(TextureType type, DPixelFormat fmt, HardwareBufferUsage usage);
		~D3D9PixelBuffer();

		virtual LockedRect lockRect( URect& rect, HardwareBufferLock lockType );

		virtual LockedBox lockBox( UBox& box, HardwareBufferLock lockType );


		// bind actual hardware-related resources.
		// surface can be a mip map part.
		void	bind(uint32 mip, IDirect3DTexture9* texTarget);
		void	bind(uint32 face, uint32 mip, IDirect3DCubeTexture9* texTarget);
		void	bind(uint32 mip, IDirect3DVolumeTexture9* texTarget);
	protected:
		// unbind all resources.
		void	unbind();
		// override:HardwareBuffer--------------------------------
		void	unlockImpl();

		// the pointer to the hardware-realated resources.
		IDirect3DSurface9*			mLockedSurface;
		IDirect3DVolume9*			mLockedVolume;

		IDirect3DTexture9*			m2DTarget;
		IDirect3DCubeTexture9*		mCubeTarget;
		IDirect3DVolumeTexture9*	m3DTarget;

		D3DCUBEMAP_FACES	mFaceIndex;
		uint32	mMipIndex;

	};

}


#endif