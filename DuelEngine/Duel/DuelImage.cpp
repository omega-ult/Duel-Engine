//  [8/28/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelMemoryStream.h"
#include "DuelImageDesc.h"
#include "DuelImage.h"

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(DImage, DObject);

	DImage::DImage()
	{
		mDesc.width = 0;
		mDesc.height = 0;
		mDesc.depth = 0;
		mDesc.mipMapCount = 0;
		mDesc.arraySize = 0;
		mDesc.imageProperty = 0;
		mDesc.imageType = IT_Image2D;
		mDesc.pixelFormat = PF_Unknown;

		mSize = 0;
		mRowPitch = 0;
		mSlicePitch = 0;

		mLock = IL_Read;
		mData = NULL;
	}

	DImage::DImage( const DString& name ) :
		mName(name)
	{
		mDesc.width = 0;
		mDesc.height = 0;
		mDesc.depth = 0;
		mDesc.mipMapCount = 0;
		mDesc.arraySize = 0;
		mDesc.imageProperty = 0;
		mDesc.imageType = IT_Image2D;
		mDesc.pixelFormat = PF_Unknown;

		mSize = 0;
		mRowPitch = 0;
		mSlicePitch = 0;

		mLock = IL_Read;
		mData = NULL;
	}


	DImage::DImage( const DImage& img )
	{
		mLock = img.mLock;
		mDesc = img.mDesc;
		size_t imgSize = img.getImageSize();
		mData = new char[imgSize];
		mSize = imgSize;
		mRowPitch = img.getRowPitch();
		mSlicePitch = img.getSlicePitch();
		memcpy(mData, img.mData, imgSize);
	}

	DImage::~DImage()
	{
		if (mData)
		{
			delete[] mData;
			mData = NULL;
		}
	}


	bool DImage::hasProperty( ImageProperty ip ) const 
	{
		return ((mDesc.imageProperty & ip) != 0);
	}

	size_t DImage::getImageSize() const
	{
		return	mSize;
	}



	void DImage::enableProperty( ImageProperty ip )
	{
		mDesc.imageProperty |= ip;
	}

	void DImage::disableProperty( ImageProperty ip )
	{
		mDesc.imageProperty &= ~ip;
	}

	ImageLock DImage::getAccess() const 
	{
		return	mLock;
	}

	void DImage::setAccess( ImageLock il )
	{
		mLock	= il;
	}



	void DImage::saveToFile( const DString& fileName, DMediaCodec* codec )
	{
		DMemoryStream ms(mData, mSize, DA_Read, false);
		codec->encodeToFile(&ms, fileName, &mDesc);
	}

	DDataStreamPtr DImage::encode( DMediaCodec* codec )
	{
		DMemoryStream ms(mData, mSize, DA_Read, false);
		return codec->encode(&ms,&mDesc);
	}


	void DImage::loadFromData( DDataStream& ds, const DImageDesc& iDesc )
	{
		size_t imgSize = iDesc.getImageSize();
		mSize = imgSize;
		mRowPitch = mDesc.width * DPixelFormatTool::getFormatBytes(mDesc.pixelFormat);
		mSlicePitch = mDesc.width * mDesc.height * DPixelFormatTool:: getFormatBytes(mDesc.pixelFormat);
		// size is the same, no need to realloc
		if (imgSize != mDesc.getImageSize())
		{
			if (mData != NULL)
			{
				delete mData;
			}
			mData = new char[imgSize];
		}
		mDesc = iDesc;
		ds.read(mData, imgSize);
	}


	void DImage::loadFromData( const char* pData, const DImageDesc& iDesc )
	{
		size_t imgSize = iDesc.getImageSize();
		mSize = imgSize;
		mRowPitch = iDesc.width * DPixelFormatTool::getFormatBytes(iDesc.pixelFormat);
		mSlicePitch = iDesc.width * iDesc.height * DPixelFormatTool:: getFormatBytes(iDesc.pixelFormat);
		// size is the same, no need to realloc
		if (imgSize != mDesc.getImageSize())
		{
			if (mData != NULL)
			{
				delete mData;
			}
			mData = new char[imgSize];
		}
		mDesc = iDesc;
		memcpy(mData, pData, imgSize);
	}

}