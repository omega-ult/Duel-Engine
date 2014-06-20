//  [10/9/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelException.h"
#include "DuelImageCube.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DImageCube, DImage);
	
	DImageCube::DImageCube() :
		DImage()
	{
		mWorkRect = ImageRect(0,0,0,0);
		mWorkFace = ICF_PositiveX;
	}


	DImageCube::DImageCube( const DString& name ) :
		DImage(name)
	{
		mWorkRect = ImageRect(0,0,0,0);
		mWorkFace = ICF_PositiveX;
	}

	DImageCube::DImageCube( const DImage& img ) :
		DImage(img)
	{
		mWorkRect = ImageRect(0,0,0,0);
		mWorkFace = ICF_PositiveX;
	}

	DImageCube::~DImageCube()
	{

	}



	void DImageCube::scale( DImage& destImg, ImageFilter filter /*= IF_BILINEAR*/ )
	{
		if (!DUEL_IS_EXACT_KIND(DImageCube,&destImg))
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
				"Cannot scale an Image2D to DImage with different image type",
				"Duel::DImageCube::scale");
		}
		// av
		// avoid zero image
		if (destImg.getWidth() == 0 || destImg.getHeight() == 0 ||
			mDesc.width == 0 || mDesc.height == 0)
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
				nearestSample(*(DImageCube*)&destImg);
				break;
			case IF_Bilinear:
				bilinearSample(*(DImageCube*)&destImg);
				break;
			}
			bicubicSample(*(DImageCube*)&destImg,filter);
		}
	}

	void DImageCube::resize( uint32 width, ImageFilter filter /*= IF_BILINEAR*/ )
	{
		// avoid zero image
		if (width == 0 )
		{
			return;
		}
		// 新建一个temp Image, new 一段数据, scale 到temp上, 再将自身赋值.
		assert(DPixelFormatTool::isDirectColor(mDesc.pixelFormat));
		char* tempData = new char[6*DPixelFormatTool::getSurfaceSize(width,width,1,mDesc.pixelFormat)];
		DImageDesc tempDesc = mDesc;
		tempDesc.width = width;
		tempDesc.height = width;
		DImageCube tempImg;
		tempImg.loadImage(tempData,tempDesc);
		tempImg.setAccess(IL_Write);
		tempImg.maxmizeWorkRect();
		maxmizeWorkRect();
		{
			tempImg.setWorkFace(ICF_PositiveX);
			setWorkFace(ICF_PositiveX);
			scale(tempImg, filter);
		}
		{
			tempImg.setWorkFace(ICF_NegativeX);
			setWorkFace(ICF_NegativeX);
			scale(tempImg, filter);
		}
		{
			tempImg.setWorkFace(ICF_PositiveY);
			setWorkFace(ICF_PositiveY);
			scale(tempImg, filter);
		}
		{
			tempImg.setWorkFace(ICF_NegativeY);
			setWorkFace(ICF_NegativeY);
			scale(tempImg, filter);
		}
		{
			tempImg.setWorkFace(ICF_PositiveZ);
			setWorkFace(ICF_PositiveZ);
			scale(tempImg, filter);
		}
		{
			tempImg.setWorkFace(ICF_PositiveZ);
			setWorkFace(ICF_PositiveZ);
			scale(tempImg, filter);
		}
		{
			tempImg.setWorkFace(ICF_NegativeZ);
			setWorkFace(ICF_NegativeZ);
			scale(tempImg, filter);
		}
		loadFromData(tempImg.getChunkPtr(), tempDesc);
		delete[] tempData;
	}

	void DImageCube::loadImage( const char* pData, const DImageDesc& iDesc )
	{
		// we only accept Cube image here.
		if (iDesc.imageType == IT_ImageCube)
		{
			// it is best to handle only one cube.
			assert(iDesc.arraySize == 1 && iDesc.mipMapCount == 1);
			assert(iDesc.width == iDesc.height);
			DImage::loadFromData(pData, iDesc);
			validateWorkRect();
		}
		else
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
				"Can not load an DImage without ImageType of IT_IMAGECUBE to an DImageCube",
				"Duel::DImageCube::loadImage")
		}
	}


	void DImageCube::loadImage(DDataStream& ds, const  DImageDesc& iDesc)
	{
		// we only accept Cube image here.
		if (iDesc.imageType == IT_ImageCube)
		{
			// it is best to handle only one cube.
			assert(iDesc.arraySize == 1 && iDesc.mipMapCount == 1);
			assert(iDesc.width == iDesc.height);
			DImage::loadFromData(ds, iDesc);
			validateWorkRect();
		}
		else
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
				"Can not load an DImage without ImageType of IT_IMAGECUBE to an DImageCube",
				"Duel::DImageCube::loadImage")
		}
	}

	DColor DImageCube::getColorAt( size_t x, size_t y, ImageCubeFace face )
	{
		DColor ret;
		if (x >= mDesc.width ||
			y >= mDesc.height)
		{
			ret == DColor::WHITE;
		}
		else
		{
			size_t blockSize = DPixelFormatTool::getFormatBytes(mDesc.pixelFormat);
			DPixelFormatTool::unpackColor(&ret, mDesc.pixelFormat, 
				mData + mSlicePitch*mWorkFace + mRowPitch*y + blockSize*x);
		}
		return ret;
	}

	void DImageCube::setColorAt( const DColor& color, size_t x, size_t y, ImageCubeFace face )
	{
		// if this image is writabe, write it, otherwise return.
		if (mLock == IL_Write)
		{
			if (x < mDesc.width &&
				y < mDesc.height)
			{
				size_t blockSize = DPixelFormatTool::getFormatBytes(mDesc.pixelFormat);
				DPixelFormatTool::packColor(color, mDesc.pixelFormat, 
					mData + mSlicePitch*mWorkFace + mRowPitch*y + blockSize*x);	
			}
		}
	}



	void DImageCube::setWorkRect( const ImageRect& rect )
	{
		mWorkRect = rect;
		validateWorkRect();
	}

	void DImageCube::setWorkFace( ImageCubeFace face )
	{
		mWorkFace = face;
	}

	void DImageCube::maxmizeWorkRect()
	{
		mWorkRect.xBegin		= 0;
		mWorkRect.yBegin		= 0;
		mWorkRect.xExtend		= mDesc.width;
		mWorkRect.yExtend	= mDesc.height;
	}

	const ImageRect& DImageCube::getWorkRect()
	{
		return mWorkRect;
	}

	ImageCubeFace DImageCube::getWorkFace()
	{
		return mWorkFace;
	}

	char* DImageCube::getWorkChunkPtr()
	{
		return mData + mSlicePitch*mWorkFace
			+ mWorkRect.yBegin*mRowPitch 
			+ mWorkRect.xBegin*DPixelFormatTool::getFormatBytes(mDesc.pixelFormat);
	}

	const char* DImageCube::getWorkChunkPtr() const
	{

		return mData + mSlicePitch*mWorkFace
			+ mWorkRect.yBegin*mRowPitch 
			+ mWorkRect.xBegin*DPixelFormatTool::getFormatBytes(mDesc.pixelFormat);
	}
	
	size_t DImageCube::getWorkChunkSize()
	{
		validateWorkRect();
		return mWorkRect.xExtend*mWorkRect.yExtend*DPixelFormatTool::getFormatBytes(mDesc.pixelFormat);
	}

	void DImageCube::validateWorkRect()
	{
		if (mWorkRect.xBegin > mDesc.width)
		{
			mWorkRect.xBegin = mDesc.width;
			mWorkRect.xExtend = 0;
		}
		else if ((mWorkRect.xBegin + mWorkRect.xExtend) > mDesc.width)
		{
			mWorkRect.xExtend = mDesc.width - mWorkRect.xBegin;
		}
		if (mWorkRect.yBegin > mDesc.height)
		{
			mWorkRect.yBegin = mDesc.height;
			mWorkRect.yExtend = 0;
		}
		else if ((mWorkRect.yBegin + mWorkRect.yExtend) > mDesc.height)
		{
			mWorkRect.yExtend = mDesc.height - mWorkRect.yBegin;
		}
	}

	void DImageCube::nearestSample( DImageCube& destImg )
	{
		// we translate the float value to a big int, this method
		// will increase performance a lot.
		ImageRect destRect = destImg.getWorkRect();
		size_t width = destRect.xExtend;
		size_t height = destRect.yExtend;
		uint64 stepX = ((uint64)mDesc.width << 48) / width;
		uint64 stepY = ((uint64)mDesc.height << 48) / height;

		uint64 curY = 0;
		uint64 curX = 0;
		char* pDest = destImg.getWorkChunkPtr();
		char* curDest = pDest;
		char* pSrc = getWorkChunkPtr();
		char* curSrc = pSrc;
		size_t destRowPitch = destImg.getRowPitch();
		size_t destPixelSize = DPixelFormatTool::getFormatBytes(destImg.getFormat());

		// note: ((step*>>1) - 1) is an extra half-step increment to adjust
		// for the center of the destination pixel, not the top-left corner
		if (destImg.getFormat() == mDesc.pixelFormat)
		{
			// very good, we can just complete task with memcpy.
			curY = (stepY >> 1) - 1;
			for (size_t y = 0; y < height; ++y)
			{
				curSrc = pSrc + mRowPitch*(curY>>48);
				curX = (stepX >> 1) -1;
				for (size_t x = 0; x < width; ++x)
				{
					memcpy(&curDest[x*destPixelSize], &curSrc[curX>>48], destPixelSize);
					curX += stepX;
				}
				curY += stepY;
				curDest += destRowPitch;
			}
		}
		else
		{
			// we must convert color here. a bit slowly
			DColor color;
			DPixelFormat srcFormat = mDesc.pixelFormat;
			DPixelFormat destFormat = destImg.getFormat();

			curY = (stepY >> 1) - 1;
			for (size_t y = 0; y < height; ++y)
			{
				curSrc = pSrc + mRowPitch*(curY>>48);
				curX = (stepX >> 1) -1;
				for (size_t x = 0; x < width; ++x)
				{
					DPixelFormatTool::unpackColor(&color,srcFormat,&curSrc[curX>>48]);
					DPixelFormatTool::packColor(color,destFormat,&curDest[x*destPixelSize]);
					curX += stepX;
				}
				curY += stepY;
				curDest += destRowPitch;
			}
		}
	}

	void DImageCube::bilinearSample( DImageCube& destImg )
	{
		// we translate the float value to a big int, this method
		// will increase performance a lot.
		ImageRect destRect = destImg.getWorkRect();
		size_t srcWidth = mDesc.width;
		size_t srcHeight = mDesc.height;
		size_t destWidth = destRect.xExtend;
		size_t destHeight = destRect.yExtend;
		uint64 stepX = ((uint64)mDesc.width << 48) / destWidth;
		uint64 stepY = ((uint64)mDesc.height << 48) / destHeight;

		uint64 curY = 0;
		uint64 curX = 0;
		char* pDest = destImg.getWorkChunkPtr();
		char* curDest = pDest;
		char* pSrc = getWorkChunkPtr();
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
		curY = (stepY >> 1) - 1;
		for (size_t y = 0; y < destHeight; ++y)
		{
			temp = static_cast<uint32>(curY >> 32);
			temp = (temp > 0x8000) ? temp - 0x8000 : 0;
			size_t srcY1 = temp >> 16;	
			srcY1 = DMath::Min<size_t>(srcY1, srcHeight-1);
			size_t srcY2 = DMath::Min<size_t>((srcY1+1), (srcHeight-1));
			float  sYfct = (temp & 0xffff) / 65536.f; // weight of Y#2

			// reset cursor X;
			curX = (stepX >> 1) -1;
			for (size_t x = 0; x < destWidth; ++x)
			{
				temp = static_cast<uint32>(curX >> 32);
				temp = (temp > 0x8000) ? temp - 0x8000 : 0;
				size_t srcX1 = temp >> 16;	
				srcX1 = DMath::Min<size_t>(srcX1, srcWidth-1);
				size_t srcX2 = DMath::Min<size_t>((srcX1+1), (srcWidth-1));
				float  sXfct = (temp & 0xffff) / 65536.f; // weight of X#2

				DColor x1y1, x2y1, x1y2, x2y2;
				DPixelFormatTool::unpackColor(&x1y1, srcFormat, pSrc+(srcY1*mRowPitch)+srcX1*srcPixelSize);
				DPixelFormatTool::unpackColor(&x2y1, srcFormat, pSrc+(srcY1*mRowPitch)+srcX2*srcPixelSize);
				DPixelFormatTool::unpackColor(&x1y2, srcFormat, pSrc+(srcY2*mRowPitch)+srcX1*srcPixelSize);
				DPixelFormatTool::unpackColor(&x2y2, srcFormat, pSrc+(srcY2*mRowPitch)+srcX2*srcPixelSize);

				DColor dstColor =x1y1*(1.0f-sXfct)*(1.0f-sYfct) +
					x2y1*(	   sXfct)*(1.0f-sYfct) +
					x1y2*(1.0f-sXfct)*(	    sYfct) +
					x2y2*(	   sXfct)*(	    sYfct);
				DPixelFormatTool::packColor(dstColor, destFormat, &curDest[x*destPixelSize]);
				curX += stepX;
			}
			curY += stepY;
			curDest += destRowPitch;
		}
	}

	void DImageCube::bicubicSample( DImageCube& destImg, ImageFilter filter )
	{

	}

}