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
		if (mType == TT_1D || mType == TT_2D)
		{
			dev->CreateTexture(mWidth, mHeight, mMipMapCount + 1, 0,
				texFormat, D3DPOOL_MANAGED, &mTexture.p2DTexture, NULL);
			mTexture.pBaseTexture = mTexture.p2DTexture;
		}
		else if (mType == TT_3D)
		{
			dev->CreateVolumeTexture(mWidth, mHeight, mDepth, mMipMapCount + 1, 0, 
				texFormat, D3DPOOL_MANAGED, &mTexture.p3DTexture, NULL);
			mTexture.pBaseTexture = mTexture.p3DTexture;
		}
		else
		{
			dev-> CreateCubeTexture(mWidth, mMipMapCount + 1, 0,
				texFormat, D3DPOOL_MANAGED, &mTexture.pCubeTexture, NULL);
			mTexture.pBaseTexture = mTexture.pCubeTexture;
		}

		createSurfaceList();
		mTextureConstant = DGpuTextureConstantPtr(new D3D9GpuTextureConstant(mTexture.pBaseTexture));
	}

	void D3D9Texture::releaseHardwareResource()
	{
		if (mTextureConstant != NULL)
		{
			mTextureConstant->getAs<D3D9GpuTextureConstant>()->discard();
			mTextureConstant = DGpuTextureConstantPtr();
		}
		if (mSurfaceList.size() != 0)
		{
			mSurfaceList.clear();
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

		size_t surfaceCount = (mMipMapCount+1) * faceCount;
		D3D9PixelBuffer* buf = NULL;
		for (size_t i = 0; i < faceCount; ++i)
		{
			// '<=' means main surface will also be taken into consideration
			for (uint32 j = 0; j <= mMipMapCount; ++j)
			{
				buf = new D3D9PixelBuffer(mType,mFormat,mUsage);
				mSurfaceList.push_back(DPixelBufferPtr(buf));
			}
		}


		IDirect3DSurface9 *surface = NULL;
		IDirect3DVolume9 *volume = NULL;
		if (mType == TT_1D || mType == TT_2D)
		{
			assert(mTexture.p2DTexture != NULL);
			// '<=' means main surface will also be taken into consideration
			for(uint32 i = 0; i <=mMipMapCount; ++i)
			{
				// this method will increase a ref count to the surface, then we will
				// remove it later
				if (FAILED(mTexture.p2DTexture->GetSurfaceLevel(i,&surface)))
				{
					DUEL_EXCEPT_BRIEF(DException::ET_InternalError,
						"Texture creation failed, check parameters",
						"Duel::D3D9Texture::createSufaceList")
				}
				D3D9PixelBuffer* curBuf = mSurfaceList[i]->getAs<D3D9PixelBuffer>();
				curBuf->bind(surface);
				// decrease reference 
				surface->Release();
			}
		}
		else if (mType == TT_3D)
		{
			assert(mTexture.p3DTexture != NULL);
			// '<=' means main surface will also be taken into consideration
			for(uint32 i = 0; i <=mMipMapCount; ++i)
			{
				// this method will increase a ref count to the surface, then we will
				// remove it later
				if (FAILED(mTexture.p3DTexture->GetVolumeLevel(i,&volume)))
				{
					DUEL_EXCEPT_BRIEF(DException::ET_InternalError,
						"Texture creation failed, check parameters",
						"Duel::D3D9Texture::createSufaceList")
				}
				D3D9PixelBuffer* curBuf = mSurfaceList[i]->getAs<D3D9PixelBuffer>();
				curBuf->bind(volume);
				// decrease reference 
				volume->Release();
			}
		}
		else //TT_CUBE
		{
			assert(mTexture.pCubeTexture != NULL);
			// '<=' means main surface will also be taken into consideration
			for(uint32 face = 0; face < 6; ++face)
			{
				D3DCUBEMAP_FACES d3dFace;
				switch(face)
				{
				case 0:
					d3dFace = D3DCUBEMAP_FACE_POSITIVE_X;
					break;
				case 1:
					d3dFace = D3DCUBEMAP_FACE_NEGATIVE_X;
					break;
				case 2:
					d3dFace = D3DCUBEMAP_FACE_POSITIVE_Y;
					break;
				case 3:
					d3dFace = D3DCUBEMAP_FACE_NEGATIVE_Y;
					break;
				case 4:
					d3dFace = D3DCUBEMAP_FACE_POSITIVE_Z;
					break;
				case 5:
					d3dFace = D3DCUBEMAP_FACE_NEGATIVE_Z;
					break;
				}
				for(uint32 i = 0; i <=mMipMapCount; ++i)
				{
					// this method will increase a ref count to the surface, then we will
					// remove it later
					if (FAILED(mTexture.pCubeTexture->GetCubeMapSurface(d3dFace,i,&surface)))
					{
						DUEL_EXCEPT_BRIEF(DException::ET_InternalError,
							"Texture creation failed, check parameters",
							"Duel::D3D9Texture::createSufaceList")
					}
					D3D9PixelBuffer* curBuf = mSurfaceList[face* 6 + i]->getAs<D3D9PixelBuffer>();
					curBuf->bind(surface);
					// decrease reference 
					surface->Release();
				}
			}
		}
	}

}