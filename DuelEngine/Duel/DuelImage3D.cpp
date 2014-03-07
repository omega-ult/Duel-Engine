//  [10/9/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelException.h"
#include "DuelLogManager.h"
#include "DuelImage.h"
#include "DuelImage3D.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DImage3D, DImage);

	DImage3D::DImage3D() :
		DImage()
	{
		mWorkBox = ImageBox(0,0,0,0,0,0);
	}


	DImage3D::DImage3D( const DString& name ) :
		DImage(name)
	{
		mWorkBox = ImageBox(0,0,0,0,0,0);
	}


	DImage3D::DImage3D( const DImage& img ) :
		DImage(img)
	{
		mWorkBox = ImageBox(0,0,0,0,0,0);
	}

	DImage3D::~DImage3D()
	{

	}



	void DImage3D::loadImage( const char* pData, const DImageDesc& iDesc )
	{
		// we only accept 3D image here.
		if (iDesc.imageType == IT_Image3D)
		{
			// it is best to handle only one 3D image.
			assert(iDesc.arraySize == 1 && iDesc.mipMapCount == 1);
			DImage::loadFromData(pData, iDesc);
			validateWorkBox();
		}
		else
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
				"Can not load an DImage without ImageType of IT_IMAGE3D to an DImage3D",
				"Duel::DImage3D::loadImage")
		}
	}


	void DImage3D::loadImage(DDataStream& ds, const  DImageDesc& iDesc)
	{
		// we only accept 3D image here.
		if (iDesc.imageType == IT_Image3D)
		{
			// it is best to handle only one 3D image.
			assert(iDesc.arraySize == 1 && iDesc.mipMapCount == 1);
			DImage::loadFromData(ds, iDesc);
			validateWorkBox();
		}
		else
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
				"Can not load an DImage without ImageType of IT_IMAGE3D to an DImage3D",
				"Duel::DImage3D::loadImage")
		}
	}

	void DImage3D::scale( DImage& destImg, ImageFilter filter /*= IF_BILINEAR*/ )
	{
		if (!DUEL_IS_EXACT_KIND(DImage3D,&destImg))
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
				"Cannot scale an Image2D to DImage with different image type",
				"Duel::ImageCube::scale");
		}
		// avoid zero image
		if (destImg.getWidth() == 0 || destImg.getHeight() == 0 || destImg.getDepth() == 0
			|| mDesc.width == 0 || mDesc.height == 0 || mDesc.depth == 0)
		{
			return;
		}
		if (destImg.getAccess() == IL_Write)
		{
			assert(DPixelFormatTool::isDirectColor(destImg.getFormat()));
			assert(DPixelFormatTool::isDirectColor(mDesc.pixelFormat));
			switch(filter)
			{
			case IF_Nearest:
				nearestSample(*(DImage3D*)&destImg);
				break;
			case IF_Bilinear:
				bilinearSample(*(DImage3D*)&destImg);
				break;
			}
			DLogManager::getSingleton().logMessage("Debug.log","an attempt of scaling an DImage3D " 
				"with bicubic filter happened, not allowed action, using IF_BILINEAR instead.");
			bilinearSample(*(DImage3D*)&destImg);
			//bicubicSample(destImg,filter);
		}
	}

	void DImage3D::resize( uint32 width, uint32 height, uint32 depth, ImageFilter filter /*= IF_BILINEAR*/ )
	{
		// avoid zero image
		if (width == 0 || height == 0 || depth == 0)
		{
			return;
		}
		// 新建一个temp Image, new 一段数据, scale 到temp上, 再将自身赋值.
		assert(DPixelFormatTool::isDirectColor(mDesc.pixelFormat));
		char* tempData = new char[DPixelFormatTool::getSurfaceSize(width,height,depth,mDesc.pixelFormat)];
		DImageDesc tempDesc = mDesc;
		tempDesc.width = width;
		tempDesc.height = height;
		tempDesc.depth = depth;
		DImage3D tempImg;
		tempImg.loadImage(tempData,tempDesc);
		tempImg.setAccess(IL_Write);
		tempImg.maximizeWorkBox();
		maximizeWorkBox();
		scale(tempImg, filter);
		loadFromData(tempImg.getChunkPtr(), tempDesc);
		delete[] tempData;
	}

	DColor DImage3D::getColorAt( size_t u, size_t v, size_t w )
	{
		DColor ret;
		if (u >= mDesc.width ||
			v >= mDesc.height ||
			w >= mDesc.depth)
		{
			ret == DColor::WHITE;
		}
		else
		{
			size_t blockSize = DPixelFormatTool::getFormatBytes(mDesc.pixelFormat);
			DPixelFormatTool::unpackColor(&ret, mDesc.pixelFormat, 
				mData + mSlicePitch*w + mRowPitch*v + blockSize*u);
		}
		return ret;
	}

	void DImage3D::setColorAt( const DColor& color, size_t u, size_t v, size_t w )
	{
		// if this image is writabe, write it, otherwise return.
		if (mLock == IL_Write)
		{
			if (u < mDesc.width &&
				v < mDesc.height &&
				w < mDesc.depth)
			{
				size_t blockSize = DPixelFormatTool::getFormatBytes(mDesc.pixelFormat);
				DPixelFormatTool::packColor(color, mDesc.pixelFormat, 
					mData + mSlicePitch*w + mRowPitch*v + blockSize*u);	
			}
		}
	}


	void DImage3D::setWorkBox( const ImageBox& dstBox )
	{
		mWorkBox = dstBox;
		validateWorkBox();
	}

	const ImageBox& DImage3D::getWorkBox()
	{
		return mWorkBox;
	}

	void DImage3D::maximizeWorkBox()
	{
		mWorkBox.left	= mWorkBox.top = mWorkBox.back = 0;
		mWorkBox.right	= mDesc.width;
		mWorkBox.bottom	= mDesc.height;
		mWorkBox.front	= mDesc.depth;
	}

	char* DImage3D::getWorkChunkPtr()
	{
		return mData
			+ mSlicePitch*mWorkBox.back
			+ mRowPitch*mWorkBox.top
			+ DPixelFormatTool::getFormatBytes(mDesc.pixelFormat)*mWorkBox.left;
	}

	const char* DImage3D::getWorkChunkPtr() const
	{
		return mData
			+ mSlicePitch*mWorkBox.back
			+ mRowPitch*mWorkBox.top
			+ DPixelFormatTool::getFormatBytes(mDesc.pixelFormat)*mWorkBox.left;
	}

	size_t DImage3D::getWorkChunkSize()
	{
		validateWorkBox();
		return (mWorkBox.right - mWorkBox.left)
			* (mWorkBox.bottom - mWorkBox.top)
			* (mWorkBox.front - mWorkBox.back)
			* DPixelFormatTool::getFormatBytes(mDesc.pixelFormat);
	}

	void DImage3D::validateWorkBox()
	{
		mWorkBox.validate();
		if (mWorkBox.left > mDesc.width)
		{
			mWorkBox.left = mDesc.width;
			mWorkBox.right = mWorkBox.left;
		}
		if (mWorkBox.right > mDesc.width)
		{
			mWorkBox.right = mDesc.width;
		}
		if (mWorkBox.bottom > mDesc.height)
		{
			mWorkBox.bottom = mDesc.height;
			mWorkBox.top = mWorkBox.bottom;
		}
		if (mWorkBox.top > mDesc.height)
		{
			mWorkBox.top = mDesc.height;
		}
		if (mWorkBox.back > mDesc.depth)
		{
			mWorkBox.back = mDesc.depth;
			mWorkBox.front = mWorkBox.back;
		}
		if (mWorkBox.front > mDesc.depth)
		{
			mWorkBox.front = mDesc.depth;
		}
	}

	void DImage3D::nearestSample( DImage3D& destImg )
	{
		// we translate the float value to a big int, this method
		// will increase performance a lot.
		ImageBox destBox = destImg.getWorkBox();
		size_t width = destBox.right - destBox.left;
		size_t height = destBox.bottom - destBox.top;
		size_t depth = destBox.front - destBox.back;
		uint64 stepU = ((uint64)mDesc.width << 48) / width;
		uint64 stepV = ((uint64)mDesc.height << 48) / height;
		uint64 stepW = ((uint64)mDesc.depth << 48) / depth;

		uint64 curW = 0;
		uint64 curV = 0;
		uint64 curU = 0;
		char* pDest = destImg.getWorkChunkPtr();
		char* curDest = pDest;
		char* pSrc = getWorkChunkPtr();
		char* curSrc = pSrc;
		size_t destSlicePitch = destImg.getSlicePitch();
		size_t destRowPitch = destImg.getRowPitch();
		size_t destPixelSize = DPixelFormatTool::getFormatBytes(destImg.getFormat());

		// note: ((step*>>1) - 1) is an extra half-step increment to adjust
		// for the center of the destination pixel, not the top-left corner
		if (destImg.getFormat() == mDesc.pixelFormat)
		{
			// very good, we can just complete task with memcpy.
			curW = (stepW >> 1) - 1;
			for (size_t w = 0; w < depth; ++w)
			{
				curSrc = pSrc + mSlicePitch*(curW>>48);
				curV = (stepV >> 1) - 1;
				for (size_t v = 0; v < height; ++v)
				{
					curSrc = pSrc + mRowPitch*(curV>>48);
					curU = (stepU >> 1) -1;
					for (size_t u = 0; u < width; ++u)
					{
						memcpy(&curDest[u*destPixelSize], &curSrc[curU>>48], destPixelSize);
						curU += stepU;
					}
					curV += stepV;
					curDest += destRowPitch;
				}
				curW += stepW;
				curDest += destSlicePitch;
			}
		}
		else
		{
			// we must convert color here. a bit slowly
			DColor color;
			DPixelFormat srcFormat = mDesc.pixelFormat;
			DPixelFormat destFormat = destImg.getFormat();

			curW = (stepW >> 1) - 1;
			for (size_t w = 0; w < depth; ++w)
			{
				curSrc = pSrc + mSlicePitch*(curW>>48);
				curV = (stepV >> 1) - 1;
				for (size_t v = 0; v < height; ++v)
				{
					curSrc = pSrc + mRowPitch*(curV>>48);
					curU = (stepU >> 1) -1;
					for (size_t u = 0; u < width; ++u)
					{
						DPixelFormatTool::unpackColor(&color,srcFormat,&curSrc[curU>>48]);
						DPixelFormatTool::packColor(color,destFormat,&curDest[u*destPixelSize]);
						curU += stepU;
					}
					curV += stepV;
					curDest += destRowPitch;
				}
				curW += stepW;
				curDest += destSlicePitch;
			}
		}
	}

	void DImage3D::bilinearSample( DImage3D& destImg )
	{
		// we translate the float value to a big int, this method
		// will increase performance a lot.
		ImageBox destBox = destImg.getWorkBox();
		size_t destWidth = destBox.right - destBox.left;
		size_t destHeight = destBox.bottom - destBox.top;
		size_t destDepth = destBox.front - destBox.back;
		size_t srcWidth = mWorkBox.right - mWorkBox.left;
		size_t srcHeight = mWorkBox.bottom - destBox.top;
		size_t srcDepth = mWorkBox.front - destBox.back;
		uint64 stepU = ((uint64)mDesc.width << 48) / destWidth;
		uint64 stepV = ((uint64)mDesc.height << 48) / destHeight;
		uint64 stepW = ((uint64)mDesc.depth << 48) / destDepth;

		uint64 curW = 0;
		uint64 curV = 0;
		uint64 curU = 0;
		char* pDest = destImg.getWorkChunkPtr();
		char* curDest = pDest;
		char* pSrc = getWorkChunkPtr();
		size_t destSlicePitch = destImg.getSlicePitch();
		size_t destRowPitch = destImg.getRowPitch();
		size_t destPixelSize = DPixelFormatTool::getFormatBytes(destImg.getFormat());

		DColor color;
		DPixelFormat srcFormat = mDesc.pixelFormat;
		size_t srcPixelSize = DPixelFormatTool::getFormatBytes(mDesc.pixelFormat);
		DPixelFormat destFormat = destImg.getFormat();

		// temp is 16/16 bit fixed precision, used to adjust a source
		// coordinate (x, y, or z) backwards by half a pixel so that the
		// integer bits represent the first sample (eg, sx1) and the
		// fractional bits are the blend weight of the second sample
		uint32 temp;

		// note: ((step*>>1) - 1) is an extra half-step increment to adjust
		// for the center of the destination pixel, not the top-left corner
		curW = (stepW >> 1) - 1;
		for (size_t w = 0; w < destDepth; ++w)
		{
			temp = static_cast<uint32>(curW >> 32);
			temp = (temp > 0x8000) ? temp - 0x8000 : 0;
			size_t srcW1 = temp >> 16;
			srcW1 = DMath::Min<size_t>(srcW1, srcDepth-1);
			size_t srcW2 = DMath::Min<size_t>((srcW1+1), (srcDepth-1));
			float  sWfct = (temp & 0xffff) / 65536.f; // weight of Z#2

			curV = (stepV >> 1) - 1;
			for (size_t v = 0; v < destHeight; ++v)
			{
				temp = static_cast<uint32>(curV >> 32);
				temp = (temp > 0x8000) ? temp - 0x8000 : 0;
				size_t srcV1 = temp >> 16;	
				srcV1 = DMath::Min<size_t>(srcV1, srcHeight-1);
				size_t srcV2 = DMath::Min<size_t>((srcV1+1), (srcHeight-1));
				float  sVfct = (temp & 0xffff) / 65536.f; // weight of V#2

				// reset cursor X;
				curU = (stepU >> 1) -1;
				for (size_t u = 0; u < destWidth; ++u)
				{
					temp = static_cast<uint32>(curU >> 32);
					temp = (temp > 0x8000) ? temp - 0x8000 : 0;
					size_t srcU1 = temp >> 16;	
					srcU1 = DMath::Min<size_t>(srcU1, srcWidth-1);
					size_t srcU2 = DMath::Min<size_t>((srcU1+1), (srcWidth-1));
					float  sUfct = (temp & 0xffff) / 65536.f; // weight of U#2

					DColor u1v1w1, u2v1w1, u1v2w1, u2v2w1;
					DColor u1v1w2, u2v1w2, u1v2w2, u2v2w2;
			DPixelFormatTool::unpackColor(&u1v1w1, srcFormat, pSrc+(srcW1*mSlicePitch)+(srcV1*mRowPitch)+srcU1*srcPixelSize);
			DPixelFormatTool::unpackColor(&u2v1w1, srcFormat, pSrc+(srcW1*mSlicePitch)+(srcV1*mRowPitch)+srcU2*srcPixelSize);
			DPixelFormatTool::unpackColor(&u1v2w1, srcFormat, pSrc+(srcW1*mSlicePitch)+(srcV2*mRowPitch)+srcU1*srcPixelSize);
			DPixelFormatTool::unpackColor(&u2v2w1, srcFormat, pSrc+(srcW1*mSlicePitch)+(srcV2*mRowPitch)+srcU2*srcPixelSize);
			DPixelFormatTool::unpackColor(&u1v1w2, srcFormat, pSrc+(srcW2*mSlicePitch)+(srcV1*mRowPitch)+srcU1*srcPixelSize);
			DPixelFormatTool::unpackColor(&u2v1w2, srcFormat, pSrc+(srcW2*mSlicePitch)+(srcV1*mRowPitch)+srcU2*srcPixelSize);
			DPixelFormatTool::unpackColor(&u1v2w2, srcFormat, pSrc+(srcW2*mSlicePitch)+(srcV2*mRowPitch)+srcU1*srcPixelSize);
			DPixelFormatTool::unpackColor(&u2v2w2, srcFormat, pSrc+(srcW2*mSlicePitch)+(srcV2*mRowPitch)+srcU2*srcPixelSize);

					DColor dstColor =
						u1v1w1*(1.0f-sUfct)*(1.0f-sVfct)*(1.0f-sWfct) +
						u2v1w1*(     sUfct)*(1.0f-sVfct)*(1.0f-sWfct) +
						u1v2w1*(1.0f-sUfct)*(     sVfct)*(1.0f-sWfct) +
						u2v2w1*(     sUfct)*(     sVfct)*(1.0f-sWfct) +
						u1v1w2*(1.0f-sUfct)*(1.0f-sVfct)*(     sWfct) +
						u2v1w2*(     sUfct)*(1.0f-sVfct)*(     sWfct) +
						u1v2w2*(1.0f-sUfct)*(     sVfct)*(     sWfct) +
						u2v2w2*(     sUfct)*(     sVfct)*(     sWfct);

					DPixelFormatTool::packColor(dstColor, destFormat, &curDest[u*destPixelSize]);
					curU += stepU;
				}
				curV += stepV;
				curDest += destRowPitch;
			}
			curW += stepW;
			curDest += destSlicePitch;
		}


	}

}