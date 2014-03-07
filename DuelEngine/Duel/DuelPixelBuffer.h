//  [8/13/2012 OMEGA] created

#ifndef _DUELPIXELBUFFER_H_
#define _DUELPIXELBUFFER_H_

#include "DuelCommon.h"
#include "DuelTexture.h"
#include "DuelHardwareBuffer.h"
#include "DuelPixelFormat.h"

namespace Duel
{
	struct	LockedRect
	{
		void*	dataPtr;
		size_t	rowPitch;
	};

	struct	LockedBox
	{
		void*	dataPtr;
		size_t	rowPitch;
		size_t	slicePitch;
	};

	/** Specialisation of DHardwareBuffer for a pixel buffer. The
    HardwarePixelbuffer abstracts an 1D, 2D or 3D quantity of pixels
    stored by the rendering API. The buffer can be located on the card
    or in main memory depending on its usage. One mipmap level of a
    texture is an example of a HardwarePixelBuffer.
    */
	class DUEL_API DPixelBuffer : public DHardwareBuffer
	{
	DUEL_DECLARE_RTTI(DPixelBuffer)
	public:
		DPixelBuffer(size_t width, size_t height, size_t depth, TextureType type,
			DPixelFormat format, HardwareBufferUsage usage);
		virtual ~DPixelBuffer(void);
		
		// DO NOT call this function, it's not supported here:
		// void lock(size_t offset, size_t size, HardwareBufferLock locklockType);

		// override:DHardwareBuffer------------------------------------------
		// DO NOT call this function, its' not supported here:
        void	readData(size_t offset, size_t length, void* pDest);
		// override:DHardwareBuffer------------------------------------------
		// DO NOT call this function, its' not supported here:
        void	writeData(size_t offset, size_t length, const void* pSource,
				bool discardWholeBuffer = false);

		// NOTICE:!!	///////////
		// due to specified rendering api's inadequate support on reading/writing to the pixielbuffer,
		// the final locked area may be different from demanded, the final locked area should be
		// retrieved from the parameter which has been passed, i.e rect or box.
		/////////////////////////////////// known issue: OpenGL can only download entire buffer.
		// read/write buffer use these function instead of readData/writeData/lock:
		// lock specified rect, if the texture type is TT_1D/TT_2D/TT_CUBE.
		// here we regard rect.left as x, while rect.top as y.
		virtual	LockedRect	lockRect(URect& rect, HardwareBufferLock lockType) = 0;
		// lock specified box, used in TT_3D
		// because the box'top is bigger than its bottom, while the x, y, z axis in a 3D texture
		// is left-hand style, so here we must map left<->x; bottom<->y; back<->z; different
		// from the rect style.
		virtual	LockedBox	lockBox(UBox& box, HardwareBufferLock lockType) = 0;

		// query functions
		virtual	size_t		getWidth() const { return mWidth; }
		virtual	size_t		getHeight() const { return mHeight; }
		virtual	size_t		getDepth() const { return mDepth; }
		virtual	size_t		getRowPitch() const { return mRowPitch; }
		virtual	size_t		getSlicePitch() const { return mSlicePitch; }
		virtual	TextureType	getTextureType() const { return mType; }
		virtual	DPixelFormat	getFormat() const { return mFormat; }

	protected:
		// override:DHardwareBuffer--------------------------------
		// this method is not supported in DPixelBuffer.
		void*	lockImpl(size_t offset, size_t size, HardwareBufferLock locklockType);

		size_t		mWidth;
		size_t		mHeight;
		size_t		mDepth;
		TextureType	mType;

		// row pitch size of the buffer, in bytes.
		size_t		mRowPitch;
		// slice pitch size of the buffer, in bytes.
		size_t		mSlicePitch;

		DPixelFormat	mFormat;
	};


}

#endif

