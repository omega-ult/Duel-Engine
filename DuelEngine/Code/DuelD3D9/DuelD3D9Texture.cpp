//  [6/6/2014 OMEGA] created

#include "DuelD3D9Common.h"
#include "DuelD3D9Texture.h"
#include "DuelD3D9TextureManager.h"
#include "DuelD3D9RenderResourceFactory.h"
#include "DuelD3D9Include.h"
#include "DuelD3D9Translator.h"
#include "DuelD3D9PixelBuffer.h"
#include "DuelD3D9ShaderObject.h"
#include "DuelException.h"

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(D3D9Texture, DTexture);

	D3D9Texture::D3D9Texture( DResourceManager* fathermanager, const DString& name, const DString& groupName ) :
		DTexture(fathermanager, name, groupName),
		mUsage(HBU_Dynamic)
	{

	}

	D3D9Texture::~D3D9Texture()
	{
		if (isLoaded())
		{
			unload();
		}
		else
		{
			releaseHardwareResource();
		}
	}

	bool D3D9Texture::isHardwareGammaSupported()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	bool D3D9Texture::isHardwareMipMapSupported()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void D3D9Texture::copyToTexture( DTexture* dstTex )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	Duel::DPixelBufferPtr D3D9Texture::getBuffer( CubeFace face /*= CF_PositiveX*/, size_t mipmap /*= 0 */ )
	{
		uint32 index = (uint32)face;
		size_t validatedMip = (mipmap > mMipMapCount) ? mMipMapCount : mipmap;
		return mSurfaceList[index * 6 + validatedMip];
	}

	Duel::DGpuTextureConstantPtr D3D9Texture::getGpuTexutureConstant()
	{
		return mTextureConstant;
	}

	void D3D9Texture::createHardwareResource()
	{
		IDirect3DDevice9* dev = mFatherManager->getAs<D3D9TextureManager>()
			->getD3DResourceFactory()->getMainDevice();

		// release created resources.
		releaseHardwareResource();
		D3DFORMAT texFormat = (mFormat == PF_Unknown) ? D3DFMT_A8R8G8B8 : D3D9Translator::getD3DFormat(mFormat);
		uint32 mipmap = mMipMapCount == MIPMAP_MAXCOUNT ? 0 : mMipMapCount + 1;
		if (mType == TT_1D || mType == TT_2D)
		{
			dev->CreateTexture(mWidth, mHeight, mipmap, 0,
				texFormat, D3DPOOL_MANAGED, &mTexture.p2DTexture, NULL);
			mTexture.pBaseTexture = mTexture.p2DTexture;
		}
		else if (mType == TT_3D)
		{
			dev->CreateVolumeTexture(mWidth, mHeight, mDepth, mipmap, 0, 
				texFormat, D3DPOOL_MANAGED, &mTexture.p3DTexture, NULL);
			mTexture.pBaseTexture = mTexture.p3DTexture;
		}
		else
		{
			dev-> CreateCubeTexture(mWidth, mipmap, 0,
				texFormat, D3DPOOL_MANAGED, &mTexture.pCubeTexture, NULL);
			mTexture.pBaseTexture = mTexture.pCubeTexture;
		}

		createSurfaceList();
		mTextureConstant = DGpuTextureConstantPtr(new D3D9GpuTextureConstant(mTexture.pBaseTexture));
	}

	void D3D9Texture::releaseHardwareResource()
	{
		if (mSurfaceList.size() != 0)
		{
			mSurfaceList.clear();
		}
		if (mTextureConstant != NULL)
		{
			mTextureConstant->getAs<D3D9GpuTextureConstant>()->discard();
			mTextureConstant = DGpuTextureConstantPtr();
		}
		// release HardwareBuffer resources.
		// ..............
		// ..............
		ReleaseCOM(mTexture.p2DTexture);
		ReleaseCOM(mTexture.p3DTexture);
		ReleaseCOM(mTexture.pCubeTexture);
		mTexture.pBaseTexture = NULL;
	}

	void D3D9Texture::setHardwareBufferUsage( HardwareBufferUsage usage )
	{
		mUsage = usage;
	}

	void D3D9Texture::createSurfaceList()
	{
		mSurfaceList.clear();
		assert(mTexture.pBaseTexture != NULL);

		// push new pixel buffer.
		uint32 faceCount = (mType == TT_Cube) ? 6 : 1;

		uint32 level = mTexture.pBaseTexture->GetLevelCount();
		mMipMapCount = level - 1;
		size_t surfaceCount = level * faceCount;
		D3D9PixelBuffer* buf = NULL;
		for (size_t face = 0; face < faceCount; ++face)
		{
			// '<=' means main surface will also be taken into consideration
			for (uint32 mip = 0; mip <= mMipMapCount; ++mip)
			{
				buf = new D3D9PixelBuffer(mType,mFormat,mUsage);
				if (mType == TT_1D || mType == TT_2D)
				{
					buf->bind(mip, mTexture.p2DTexture);
				}
				if (mType == TT_3D)
				{
					buf->bind(mip, mTexture.p3DTexture);
				}
				if (mType == TT_Cube)
				{
					buf->bind(face, mip, mTexture.pCubeTexture);
				}				
				mSurfaceList.push_back(DPixelBufferPtr(buf));
			}
		}

	}

}