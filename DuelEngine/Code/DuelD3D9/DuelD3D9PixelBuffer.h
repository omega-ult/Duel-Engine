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

		virtual LockedRect lockRect( URect& rect, HardwareBufferLock lockType );

		virtual LockedBox lockBox( UBox& box, HardwareBufferLock lockType );


		// bind actual hardware-related resources.
		// surface can be a mip map part.
		void	bind(IDirect3DSurface9* surface);
		// bind actual hardware-related resources, in TT_3D.
		void	bind(IDirect3DVolume9* volume);
	protected:
		// unbind all resources.
		void	unbind();
		// override:HardwareBuffer--------------------------------
		void	unlockImpl();

		// the pointer to the hardware-realated resources.
		IDirect3DSurface9*		mSurface;
		IDirect3DVolume9*		mVolume;
		IDirect3DBaseTexture9*	mBaseTex;


	};

}


#endif