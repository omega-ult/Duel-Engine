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
		mLockedSurface(NULL),
		mLockedVolume(NULL),
		m2DTarget(NULL),
		mCubeTarget(NULL),
		m3DTarget(NULL),
		mFaceIndex(D3DCUBEMAP_FACE_POSITIVE_X),
		mMipIndex(0)
	{

	}


	D3D9PixelBuffer::~D3D9PixelBuffer()
	{
		if (mbIsLocked)
		{
			unlock();
		}
		
	}


	Duel::LockedRect D3D9PixelBuffer::lockRect( URect& rect, HardwareBufferLock lockType )
	{

		if (!(mType == TT_1D || mType == TT_2D || mType == TT_Cube))
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
				"Failed to lock a rect on a PixelBuffer with type: TT_3D",
				"Duel::D3D9PixelBuffer::lockRect")
		}
		if (m2DTarget == NULL && mCubeTarget == NULL)
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InvalidStatus,
				"Pixel buffer is not binded to any texture",
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

		if (mCubeTarget != NULL)
		{
			mCubeTarget->GetCubeMapSurface(mFaceIndex, mMipIndex, &mLockedSurface);
		}
		else if (m2DTarget != NULL)
		{
			m2DTarget->GetSurfaceLevel(mMipIndex, &mLockedSurface);
		}

		D3DLOCKED_RECT lrect;
		if (rect.xBegin == 0 && rect.yBegin == 0 && 
			rect.xExtend == mWidth && rect.yExtend == mHeight)
		{
			hr = mLockedSurface->LockRect(&lrect, NULL, 0);
		}
		else
		{
			RECT d3dRect;
			d3dRect.left = rect.xBegin;
			d3dRect.right = rect.xExtend + rect.xBegin;
			d3dRect.top = rect.yBegin;
			d3dRect.bottom = rect.yExtend + rect.yBegin;
			hr = mLockedSurface->LockRect(&lrect, &d3dRect, 0);
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
		if (m3DTarget == NULL)
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InvalidStatus,
				"Pixel buffer is not binded to any texture",
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


		m3DTarget->GetVolumeLevel(mMipIndex, &mLockedVolume);

		D3DLOCKED_BOX lbox;
		if (box.left == 0 && box.top == 0 && box.back == 0 && 
			box.right == mWidth && box.bottom == mHeight && box.front == mDepth)
		{
			hr = mLockedVolume->LockBox(&lbox, NULL, 0);
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
			hr = mLockedVolume->LockBox(&lbox, &d3dBox, 0);
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

	void D3D9PixelBuffer::bind( uint32 mip, IDirect3DTexture9* texTarget )
	{
		unbind();
		mMipIndex = mip;
		m2DTarget = texTarget;
		IDirect3DSurface9* suf = NULL;
		if (FAILED(texTarget->GetSurfaceLevel(mip,&suf)))
		{
			unbind();
			return;
		}
		D3DSURFACE_DESC desc;
		suf->GetDesc(&desc);
		mWidth	= desc.Width;
		mHeight	= desc.Height;
		mDepth	= 1;
		mFormat	= D3D9Translator::getPixelFormat(desc.Format);
		mRowPitch = mWidth * DPixelFormatTool::getFormatBytes(mFormat);
		mSlicePitch = mHeight * mRowPitch;
		mByteSize	= mSlicePitch;
		ReleaseCOM(suf);
	}

	void D3D9PixelBuffer::bind( uint32 face, uint32 mip, IDirect3DCubeTexture9* texTarget )
	{
		unbind();
		mMipIndex = mip;
		mCubeTarget = texTarget;
		switch(face)
		{
		case 0:
			mFaceIndex = D3DCUBEMAP_FACE_POSITIVE_X;
			break;
		case 1:
			mFaceIndex = D3DCUBEMAP_FACE_NEGATIVE_X;
			break;
		case 2:
			mFaceIndex = D3DCUBEMAP_FACE_POSITIVE_Y;
			break;
		case 3:
			mFaceIndex = D3DCUBEMAP_FACE_NEGATIVE_Y;
			break;
		case 4:
			mFaceIndex = D3DCUBEMAP_FACE_POSITIVE_Z;
			break;
		case 5:
			mFaceIndex = D3DCUBEMAP_FACE_NEGATIVE_Z;
			break;
		}
		IDirect3DSurface9* suf = NULL;
		if (FAILED(texTarget->GetCubeMapSurface(mFaceIndex,mip,&suf)))
		{
			unbind();
			return;
		}
		D3DSURFACE_DESC desc;
		suf->GetDesc(&desc);
		mWidth	= desc.Width;
		mHeight	= desc.Height;
		mDepth	= 1;
		mFormat	= D3D9Translator::getPixelFormat(desc.Format);
		mRowPitch = mWidth * DPixelFormatTool::getFormatBytes(mFormat);
		mSlicePitch = mHeight * mRowPitch;
		mByteSize	= mSlicePitch;
		ReleaseCOM(suf);
	}

	void D3D9PixelBuffer::bind( uint32 mip, IDirect3DVolumeTexture9* texTarget )
	{
		unbind();
		mMipIndex = mip;
		m3DTarget = texTarget;
		IDirect3DVolume9* vol = NULL;
		if (FAILED(texTarget->GetVolumeLevel(mip, &vol)))
		{
			unbind();
			return;
		}
		D3DVOLUME_DESC desc;
		vol->GetDesc(&desc);
		mWidth	= desc.Width;
		mHeight	= desc.Height;
		mDepth	= desc.Depth;
		mFormat	= D3D9Translator::getPixelFormat(desc.Format);
		mRowPitch = mWidth * DPixelFormatTool::getFormatBytes(mFormat);
		mSlicePitch = mHeight * mRowPitch;
		mByteSize	= mSlicePitch * mDepth;
		ReleaseCOM(vol);
	}




	void D3D9PixelBuffer::unbind()
	{
		m2DTarget = NULL;
		m3DTarget = NULL;
		mCubeTarget = NULL;
		mWidth = 0;
		mHeight = 0;
		mDepth = 0;
		mMipIndex = 0;
		mFaceIndex = D3DCUBEMAP_FACE_POSITIVE_X;
	}

	void D3D9PixelBuffer::unlockImpl()
	{
		if (mLockedSurface != NULL)
		{
			mLockedSurface->UnlockRect();
			ReleaseCOM(mLockedSurface);
		}
		if (mLockedVolume != NULL)
		{
			mLockedVolume->UnlockBox();
			ReleaseCOM(mLockedVolume);
		}	
	}

}
