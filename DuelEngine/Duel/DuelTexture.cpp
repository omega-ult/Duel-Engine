//  [10/10/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelLogManager.h"
#include "DuelImage2D.h"
#include "DuelImage3D.h"
#include "DuelImageCube.h"
#include "DuelException.h"
#include "DuelPixelBuffer.h"
#include "DuelTexture.h"
#include "DuelResourceGroupManager.h"
#include "DuelDDSCodec.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DTexture, DResource);

	DTexture::DTexture( DResourceManager* fathermanager, const DString& name, const DString& groupName ) :
		DResource(fathermanager, name, groupName, "Texture"),
		mWidth(512),
		mHeight(512),
		mDepth(1),
		mType(TT_2D),
		mMipMapCount(0),
		mFormat(PF_Unknown),
		mbHWMipMapSupport(true), // initial 'true' will enable us to use setHardwareMipMap function.
		mbHWGammaSupport(false),
		mGamma(1.0f)
	{
	}

	void DTexture::saveToImage( DImage& dstImg, bool withMipMap )
	{
		DImageDesc desc;
		ImageType destType = dstImg.getImageType();
		if( !((mType == TT_1D && destType == IT_Image1D) ||
			  (mType == TT_2D && destType == IT_Image2D) ||
			  (mType == TT_3D && destType == IT_Image3D) ||
			  (mType == TT_Cube && destType == IT_ImageCube)))
		{
			return;
		}
		desc.imageType = destType;

		size_t mipmapCount = 1;
		if (withMipMap)
		{
			mipmapCount = mMipMapCount;
		}

		desc.arraySize = 1;
		desc.width = mWidth;
		desc.height = mHeight;
		desc.depth = mDepth;
		desc.mipMapCount = mipmapCount;
		desc.pixelFormat = mFormat;

		char* tempData = new char[desc.getImageSize()];
		char* curData = tempData;
		
		DPixelBufferPtr tempBuf;
		if ( mType == TT_Cube)
		{		
			LockedRect lrect;
			URect rect;
			size_t	mipSize;
			rect.xBegin = 0;
			rect.yBegin = 0;
			for (size_t mip = 0; mip < mMipMapCount; ++mip)
			{
				tempBuf = getBuffer(CF_PositiveX, mip);
				rect.xExtend = tempBuf->getWidth();
				rect.yExtend = tempBuf->getHeight();
				mipSize = tempBuf->getSize();

				tempBuf = getBuffer(CF_PositiveX, mip);
				lrect = tempBuf->lockRect(rect, HBL_ReadOnly);
				memcpy(curData, lrect.dataPtr, mipSize);
				curData += mipSize;
				tempBuf->unlock();

				tempBuf = getBuffer(CF_NegativeX, mip);
				lrect = tempBuf->lockRect(rect, HBL_ReadOnly);
				memcpy(curData, lrect.dataPtr, mipSize);
				curData += mipSize;
				tempBuf->unlock();

				tempBuf = getBuffer(CF_PositiveY, mip);
				lrect = tempBuf->lockRect(rect, HBL_ReadOnly);
				memcpy(curData, lrect.dataPtr, mipSize);
				curData += mipSize;
				tempBuf->unlock();

				tempBuf = getBuffer(CF_NegativeY, mip);
				lrect = tempBuf->lockRect(rect, HBL_ReadOnly);
				memcpy(curData, lrect.dataPtr, mipSize);
				curData += mipSize;
				tempBuf->unlock();

				tempBuf = getBuffer(CF_PositiveZ, mip);
				lrect = tempBuf->lockRect(rect, HBL_ReadOnly);
				memcpy(curData, lrect.dataPtr, mipSize);
				curData += mipSize;
				tempBuf->unlock();

				tempBuf = getBuffer(CF_NegativeZ, mip);
				lrect = tempBuf->lockRect(rect, HBL_ReadOnly);
				memcpy(curData, lrect.dataPtr, mipSize);
				curData += mipSize;
				tempBuf->unlock();

			}
		}
		else if (mType == TT_3D)
		{
			LockedBox lbox;
			UBox box;
			size_t	mipSize;
			box.left = 0;
			box.top = 0;
			box.back = 0;
			for (size_t mip = 0; mip < mMipMapCount; ++mip)
			{
				tempBuf = getBuffer(CF_PositiveX, mip);
				box.right = tempBuf->getWidth();
				box.bottom = tempBuf->getHeight();
				box.front = tempBuf->getDepth();
				mipSize = tempBuf->getSize();

				tempBuf = getBuffer(CF_PositiveX, mip);
				lbox = tempBuf->lockBox(box, HBL_ReadOnly);
				memcpy(curData, lbox.dataPtr, mipSize);
				curData += mipSize;
				tempBuf->unlock();
			}
		}
		else // TT_1D/TT_2D
		{
			LockedRect lrect;
			URect rect;
			size_t	mipSize;
			rect.xBegin = 0;
			rect.yBegin = 0;
			for (size_t mip = 0; mip < mMipMapCount; ++mip)
			{
				tempBuf = getBuffer(CF_PositiveX, mip);
				rect.xExtend = tempBuf->getWidth();
				rect.yExtend = tempBuf->getHeight();
				mipSize = tempBuf->getSize();

				
				lrect = tempBuf->lockRect(rect, HBL_ReadOnly);
				memcpy(curData, lrect.dataPtr, mipSize);
				curData += mipSize;
				tempBuf->unlock();
			}
		}

		dstImg.loadFromData(tempData, desc);

		delete[] tempData;
	}


	void DTexture::setHardwareMipMap( bool flag )
	{
		if (mbHWMipMapSupport)
		{
			mbUseHWMipmap = flag;
		}
		else
		{
			mbUseHWMipmap = false;
		}
	}

	void DTexture::loadFromImage( DImage* img )
	{


		LoadState old = mLoadState.get();
		if (old != LS_Unloaded && old != LS_Prepared) return;

		// atomically do slower check to make absolutely sure,
		// and set the load state to LOADING
		if (old==LS_Loading || !mLoadState.cas(old, LS_Loading))
		{
			while( mLoadState.get() == LS_Loading )
			{
				DUEL_LOCK_AUTO_MUTEX
			}

			LoadState state = mLoadState.get();
			if( state == LS_Prepared || state == LS_Preparing )
			{
				// another thread is preparing, quit load anyway.
				return;
			}
			else if( state != LS_Loaded )
			{
				// 					DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
				// 						"Another thread failed in resource operation",
				// 						"Duel::Resource::load");
			}
			return;
		}

		DUEL_LOCK_AUTO_MUTEX
		try
		{
			loadFromImageImpl(img);
			mLoadState.set(LS_Loaded);
			signalLoadingCompleted(this);
			// Calculate resource size
			mSize = calculateSize();
		}
		catch (DException* e)
		{
			DLogManager::getSingleton().logMessage("DLog", "Failed to load resource : " + mGroupName + "/" + mName
				+ ", in source : " + e->getFile() + ", line : " + DStringTool::toString(e->getLine())
				+ ", with : " + e->getFullDescription(), DLog::LL_Warn);
#ifdef DUEL_DEBUG
			std::cout<< "Failed to load resource : " + mGroupName + "/" + mName + "\n";
			std::cout<< e->getSource() << "\n" << e->getDescription();
#endif
			mLoadState.set(old);
		}
// 		if(loadFromImageImpl(img))
// 		{
// 			mLoadState.set(LS_Loaded);
// 			// notify listeners
// 			signalLoadingCompleted(this);
// 		}
// 		else
// 		{
// 			mLoadState.set(old);
// 		}
	}

	void DTexture::loadFromImageImpl( DImage* img ) 
	{
		if (img == NULL)
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
				"Failed to load texture with a NULL image pointer",
				"Duel::DTexture::prepareImpl");
		}
		DImageDesc desc;
		DImage* tempImg;
		desc.imageType = img->getImageType();
		switch(desc.imageType)
		{
		case IT_Image1D:
			mType = TT_1D;
			tempImg = new DImage2D();
			break;
		case IT_Image2D:
			mType = TT_2D;
			tempImg = new DImage2D();
			break;
		case IT_Image3D:
			mType = TT_3D;
			tempImg = new DImage3D();
			break;
		case IT_ImageCube:
			mType = TT_Cube;
			tempImg = new DImageCube();
			break;
		default:
			// should never occur
			tempImg = new DImage2D();
		}
		desc.width = mWidth = img->getWidth();
		desc.height = mHeight = img->getHeight();
		desc.depth = mDepth = img->getDepth();

		desc.pixelFormat = mFormat = img->getFormat();
		desc.arraySize = img->getArraySize();
		desc.mipMapCount = mMipMapCount = img->getMipMapCount();

		// now create hardware-related resource.
		createHardwareResource();

		// write to buffer, if the image is not identical to 
		// this texture, then we must scale it.
		DImage* scaled;
// 		if (mWidth != img->getWidth() ||
// 			mHeight != img->getHeight() ||
// 			mDepth != img->getDepth())
// 		{
// 			char* tmp = new char[desc.getImageSize()];
// 			tempImg->loadFromData(tmp,desc);
// 			img->scale(*tempImg);
// 			scaled = tempImg;
// 			delete tmp;
// 		}
// 		else 
		{
			scaled = img;
		}

		size_t surfaceSize = DPixelFormatTool::getSurfaceSize(mWidth,mHeight,mDepth,mFormat);
		char* curData = scaled->getChunkPtr();
		
		DPixelBufferPtr tempBuf;
		if ( mType == TT_Cube)
		{		
			LockedRect lrect;
			URect rect;
			size_t	mipSize;
			rect.xBegin = 0;
			rect.yBegin = 0;
			for (size_t mip = 0; mip < mMipMapCount; ++mip)
			{
				tempBuf = getBuffer(CF_PositiveX, mip);
				rect.xExtend = tempBuf->getWidth();
				rect.yExtend = tempBuf->getHeight();
				mipSize = tempBuf->getSize();

				tempBuf = getBuffer(CF_PositiveX, mip);
				lrect = tempBuf->lockRect(rect, HBL_Discard);
				memcpy(lrect.dataPtr, curData, mipSize);
				curData += mipSize;
				tempBuf->unlock();

				tempBuf = getBuffer(CF_NegativeX, mip);
				lrect = tempBuf->lockRect(rect, HBL_Discard);
				memcpy(lrect.dataPtr, curData, mipSize);
				curData += mipSize;
				tempBuf->unlock();

				tempBuf = getBuffer(CF_PositiveY, mip);
				lrect = tempBuf->lockRect(rect, HBL_Discard);
				memcpy(lrect.dataPtr, curData, mipSize);
				curData += mipSize;
				tempBuf->unlock();

				tempBuf = getBuffer(CF_NegativeY, mip);
				lrect = tempBuf->lockRect(rect, HBL_Discard);
				memcpy(lrect.dataPtr, curData, mipSize);
				curData += mipSize;
				tempBuf->unlock();

				tempBuf = getBuffer(CF_PositiveZ, mip);
				lrect = tempBuf->lockRect(rect, HBL_Discard);
				memcpy(lrect.dataPtr, curData, mipSize);
				curData += mipSize;
				tempBuf->unlock();

				tempBuf = getBuffer(CF_NegativeZ, mip);
				lrect = tempBuf->lockRect(rect, HBL_Discard);
				memcpy(lrect.dataPtr, curData, mipSize);
				curData += mipSize;
				tempBuf->unlock();

			}
		}
		else if (mType == TT_3D)
		{
			LockedBox lbox;
			UBox box;
			size_t	mipSize;
			box.left = 0;
			box.top = 0;
			box.back = 0;
			for (size_t mip = 0; mip < mMipMapCount; ++mip)
			{
				tempBuf = getBuffer(CF_PositiveX, mip);
				box.right = tempBuf->getWidth();
				box.bottom = tempBuf->getHeight();
				box.front = tempBuf->getDepth();
				mipSize = tempBuf->getSize();

				lbox = tempBuf->lockBox(box, HBL_Discard);
				memcpy(lbox.dataPtr, curData, mipSize);
				curData += mipSize;
				tempBuf->unlock();
			}
		}
		else // TT_1D/TT_2D
		{
			LockedRect lrect;
			URect rect;
			size_t	mipSize;
			rect.xBegin = 0;
			rect.yBegin = 0;
			for (size_t mip = 0; mip < mMipMapCount; ++mip)
			{
				tempBuf = getBuffer(CF_PositiveX, mip);
				rect.xExtend = tempBuf->getWidth();
				rect.yExtend = tempBuf->getHeight();
				mipSize = tempBuf->getSize();


				lrect = tempBuf->lockRect(rect, HBL_Discard);
				memcpy(lrect.dataPtr, curData, mipSize);
				curData += mipSize;
				tempBuf->unlock();
			}
		}

		delete tempImg;
	}

	void DTexture::unloadImpl()
	{
		releaseHardwareResource();
		mPreparedData.reset();
		mExternSrc.reset();
	}

	size_t DTexture::calculateSize()
	{
		return getFaceCount() * DPixelFormatTool::getSurfaceSize(mWidth,mHeight,mDepth,mFormat);
	}


	void DTexture::prepareImpl() 
	{
		DDataStreamPtr data;
		if (mExternSrc != NULL)
		{
			data = mExternSrc;
		}
		else
		{
			data = DResourceGroupManager::getSingleton().openResource(this);
		}
		if (data != NULL && data->isValid())
		{
			DMemoryStream* memData = new DMemoryStream(data.get());
			data->read(memData->getChunkPtr(), data->getSize());
			mPreparedData.reset(memData);
		}
		else if (!validateParameters())
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
				"Failed to prepare resource data, and failed to validate texture parameters",
				"Duel::DTexture::prepareImpl");
		}
	}

	void DTexture::loadImpl() 
	{
		// check whether we are available for an empty resource.
		if (mPreparedData == NULL)
		{
			if (validateParameters())
			{
				createHardwareResource();
			}
		}
		// TODO : 增加解码支持.
		else if(mCodecFormat == CF_DDS)
		{
			DDSCodec ddsco;
			DImageDesc imgDesc;
			DDataStreamPtr imgData = ddsco.decode(mPreparedData.get(), &imgDesc);
			DImage* img = NULL;
			if (imgDesc.imageType == IT_Image1D || imgDesc.imageType == IT_Image2D)
			{
				img = new DImage2D(mName);
			}
			else if (imgDesc.imageType == IT_Image3D)
			{
				img = new DImage3D(mName);
			}
			else if (imgDesc.imageType == IT_ImageCube)
			{
				img = new DImageCube(mName);
			}
			if (img != NULL)
			{
				img->loadFromData(*(imgData), imgDesc);
				loadFromImageImpl(img);
				delete img;
			}
		}	
		else
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
				"Failed to load resource data with unknown format",
				"Duel::DTexture::loadImpl");

		}
	}

	void DTexture::setDepth( size_t depth )
	{
		if ( mType != TT_3D) 
		{
			return;
		}
		mDepth = depth;
	}

	bool DTexture::validateParameters()
	{
		if (mWidth == 0 || mHeight == 0 || mDepth == 0 || mMipMapCount == 0 || mFormat == PF_Unknown)
		{
			return false;
		}
		return true;
	}

}