//  [6/6/2014 OMEGA] created

#include "DuelD3D9Common.h"
#include "DuelD3D9PixelBuffer.h"
#include "DuelD3D9Translator.h"
#include "DuelException.h"

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(D3D9PixelBuffer, DPixelBuffer);


	D3D9PixelBuffer::D3D9PixelBuffer( TextureType type, DPixelFormat fmt, HardwareBufferUsage usage ) :
		DPixelBuffer(0,0,0, type, fmt, usage),
		mSurface(NULL),
		mVolume(NULL),
		mBaseTex(NULL)
	{

	}


	Duel::LockedRect D3D9PixelBuffer::lockRect( URect& rect, HardwareBufferLock lockType )
	{

		if (!(mType == TT_1D || mType == TT_2D || mType == TT_Cube))
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
				"Failed to lock a rect on a PixelBuffer with type: TT_3D",
				"Duel::D3D9PixelBuffer::lockRect")
		}

		if (mbIsLocked)
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InvalidStatus,
				"Buffer lock is aready locked.",
				"Duel::D3D9PixelBuffer::lockRect")
		}
		mbIsLocked = true;

		HRESULT hr;
		LockedRect ret;
		ret.dataPtr = NULL;
		ret.rowPitch = 0;

		mLockFlag = 0;
		switch(lockType)
		{
		case HBL_Discard:
			// D3D only likes D3DLOCK_DISCARD if you created the texture with D3DUSAGE_DYNAMIC
			// debug runtime flags this up, could cause problems on some drivers
			if(mUsage == HBU_Dynamic)
				mLockFlag |= D3DLOCK_DISCARD;
			break;
		case HBL_ReadOnly:
			mLockFlag |= D3DLOCK_READONLY;
			break;
		}

		D3DLOCKED_RECT lrect;
		if (rect.xBegin == 0 && rect.yBegin == 0 && 
			rect.xExtend == mWidth && rect.yExtend == mHeight)
		{
			hr = mSurface->LockRect(&lrect, NULL, mLockFlag);
		}
		else
		{
			RECT d3dRect;
			d3dRect.left = rect.xBegin;
			d3dRect.right = rect.xExtend + rect.xBegin;
			d3dRect.top = rect.yBegin;
			d3dRect.bottom = rect.yExtend + rect.yBegin;
			hr = mSurface->LockRect(&lrect, &d3dRect, mLockFlag);
		}
		if (FAILED(hr))
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InternalError,
				"Lock failed in current surface",
				"Duel::D3D9PixelBuffer::lockRect")
		}
		ret.dataPtr = lrect.pBits;
		ret.rowPitch = lrect.Pitch;
		return ret;
	}

	Duel::LockedBox D3D9PixelBuffer::lockBox( UBox& box, HardwareBufferLock lockType )
	{
		if (mType != TT_3D)
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InternalError,
				"Failed to lock a box on a PixelBuffer without type: TT_3D",
				"Duel::D3D9PixelBuffer::lockBox")
		}

		if (mbIsLocked)
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InvalidStatus,
				"Buffer lock is aready locked.",
				"Duel::D3D9PixelBuffer::lockBox")
		}
		mbIsLocked = true;

		HRESULT hr;
		LockedBox ret;
		ret.dataPtr = NULL;
		ret.rowPitch = 0;
		ret.slicePitch = 0;

		mLockFlag = 0;
		switch(lockType)
		{
		case HBL_Discard:
			// D3D only likes D3DLOCK_DISCARD if you created the texture with D3DUSAGE_DYNAMIC
			// debug runtime flags this up, could cause problems on some drivers
			if(mUsage == HBU_Dynamic)
				mLockFlag |= D3DLOCK_DISCARD;
			break;
		case HBL_ReadOnly:
			mLockFlag |= D3DLOCK_READONLY;
			break;
		}

		D3DLOCKED_BOX lbox;
		if (box.left == 0 && box.top == 0 && box.back == 0 && 
			box.right == mWidth && box.bottom == mHeight && box.front == mDepth)
		{
			hr = mVolume->LockBox(&lbox, NULL, mLockFlag);
		}
		else
		{
			D3DBOX d3dBox;
			d3dBox.Left = box.left;
			d3dBox.Top = box.top;
			d3dBox.Back	= box.back;
			d3dBox.Right = box.right;
			d3dBox.Bottom = box.bottom;
			d3dBox.Front = box.front;
			hr = mVolume->LockBox(&lbox, &d3dBox, mLockFlag);
		}
		if (FAILED(hr))
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InternalError,
				"Lock failed in current volume",
				"Duel::D3D9PixelBuffer::lockBox")
		}
		ret.dataPtr = lbox.pBits;
		ret.rowPitch = lbox.RowPitch;
		ret.slicePitch = lbox.SlicePitch;
		return ret;
	}

	void D3D9PixelBuffer::bind( IDirect3DSurface9* surface )
	{
		unbind();

		void *pTex = NULL;
		HRESULT hr;
		hr = surface->GetContainer(IID_IDirect3DBaseTexture9, &pTex);
		if (SUCCEEDED(hr) && pTex)
		{
			mBaseTex = (IDirect3DTexture9*)pTex;
		}
		mSurface = surface;
		// hold a reference.
		mSurface->AddRef();

		D3DSURFACE_DESC desc;
		hr = surface->GetDesc(&desc);
		if (FAILED(hr))
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InternalError,
				"Can't retrieve surface information from given parameter",
				"Duel::D3D9PixelBuffer::bind")
		}
		mWidth	= desc.Width;
		mHeight	= desc.Height;
		mDepth	= 1;
		mFormat	= D3D9Translator::getPixelFormat(desc.Format);
		mRowPitch = mWidth * DPixelFormatTool::getFormatBytes(mFormat);
		mSlicePitch = mHeight * mRowPitch;
		mByteSize	= mSlicePitch;
	}

	void D3D9PixelBuffer::bind( IDirect3DVolume9* volume )
	{
		unbind();

		void *pTex = NULL;
		HRESULT hr;
		hr = volume->GetContainer(IID_IDirect3DBaseTexture9, &pTex);
		if (SUCCEEDED(hr) && pTex)
		{
			mBaseTex = (IDirect3DTexture9*)pTex;
		}
		mVolume = volume;
		// hold a reference.
		mVolume->AddRef();

		D3DVOLUME_DESC desc;
		hr = volume->GetDesc(&desc);
		if (FAILED(hr))
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InternalError,
				"Can't retrieve surface information from given parameter",
				"Duel::D3D9PixelBuffer::bind")
		}
		mWidth	= desc.Width;
		mHeight	= desc.Height;
		mDepth	= desc.Depth;
		mFormat	= D3D9Translator::getPixelFormat(desc.Format);
		mRowPitch = mWidth * DPixelFormatTool::getFormatBytes(mFormat);
		mSlicePitch = mHeight * mRowPitch;
		mByteSize	= mSlicePitch * mDepth;
	}

	void D3D9PixelBuffer::unbind()
	{
		if (mSurface != NULL)
		{
			ReleaseCOM(mSurface);
		}
		if (mVolume != NULL)
		{
			ReleaseCOM(mSurface);
		}
		mBaseTex = NULL;
	}

	void D3D9PixelBuffer::unlockImpl()
	{
		if (mSurface != NULL)
		{
			mSurface->UnlockRect();
		}
		if (mVolume != NULL)
		{
			mVolume->UnlockBox();
		}	
	}

}
