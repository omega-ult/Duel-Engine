//  [4/12/2013 OMEGA] created

#include "DuelGLCommon.h"
#include "DuelGLTexture.h"
#include "DuelGLTranslator.h"
#include "DuelGLPixelBuffer.h"
#include "DuelImage2D.h"
#include "DuelImage3D.h"
#include "DuelImageCube.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(GLTexture, DTexture);
	GLTexture::GLTexture( DResourceManager* fathermanager, const DString& name, const DString& groupName ) :
		DTexture(fathermanager, name, groupName),
		mGpuConstant(this)
	{

	}

	GLTexture::~GLTexture()
	{
		// have to call this here reather than in Resource destructor
		// since calling virtual methods in base destructors causes crash
		if (isLoaded())
		{
			unload(); 
		}
		else
		{
			releaseHardwareResource();
		}
	}

	bool GLTexture::isHardwareGammaSupported()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	bool GLTexture::isHardwareMipMapSupported()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	GLenum GLTexture::getGLTextureTarget(void) const
    {
        switch(mType)
        {
            case TT_1D:
                return GL_TEXTURE_1D;
            case TT_2D:
                return GL_TEXTURE_2D;
            case TT_3D:
                return GL_TEXTURE_3D;
            case TT_Cube:
                return GL_TEXTURE_CUBE_MAP;
			default:
				return GL_TEXTURE_2D;
        };
    }
	void GLTexture::copyToTexture( DTexture* dstTex )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	Duel::DPixelBufferPtr GLTexture::getBuffer( CubeFace face /*= CF_POSITIVEX*/, size_t mipmap /*= 0 */ )
	{
		assert(mipmap < mMipMapCount);
		if (mType != TT_Cube)
		{
			assert(face == CF_PositiveX);
		}
		return mSurfaceList[((uint32)face * 6) + mipmap];
	}

	void GLTexture::createHardwareResource()
	{
		// Generate texture name
		glGenTextures( 1, &mTextureID );

		// Set texture type
		GLenum	texTarget = getGLTextureTarget();
		glBindTexture( texTarget, mTextureID );
		// This needs to be set otherwise the texture doesn't get rendered
		glTexParameteri(texTarget, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(texTarget, GL_TEXTURE_MAX_LEVEL, mMipMapCount - 1 );

		// Set some misc default parameters so NVidia won't complain, these can of course be changed later
		glTexParameteri(texTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(texTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(texTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(texTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


		GLFormatGroup fg = GLTranslator::getGLFormat(mFormat);
		
		size_t width = mWidth;
		size_t height = mHeight;
		size_t depth = mDepth;

		if (DPixelFormatTool::isCompressed(mFormat))
		{
			// Compressed formats
			size_t size = DPixelFormatTool::getSurfaceSize(mWidth, mHeight, mDepth, mFormat);
			// Provide temporary buffer filled with zeroes as glCompressedTexImageXD does not
			// accept a 0 pointer like normal glTexImageXD
			// Run through this process for every mipmap to pregenerate mipmap piramid
			uint8 *tmpdata = new uint8[size];
			memset(tmpdata, 0, size);


			for(size_t mip=0; mip<mMipMapCount; mip++)
			{
				size = DPixelFormatTool::getSurfaceSize(width, height, depth, mFormat);
				switch(mType)
				{
				case TT_1D:
					glCompressedTexImage1DARB(GL_TEXTURE_1D, mip, fg.internalFormat, 
						width, 0, 
						size, tmpdata);
					break;
				case TT_2D:
					glCompressedTexImage2DARB(GL_TEXTURE_2D, mip, fg.internalFormat,
						width, height, 0, 
						size, tmpdata);
					break;
				case TT_3D:
					glCompressedTexImage3DARB(GL_TEXTURE_3D, mip, fg.internalFormat,
						width, height, depth, 0, 
						size, tmpdata);
					break;
				case TT_Cube:
					for(int face=0; face<6; face++) {
						glCompressedTexImage2DARB(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, mip, fg.internalFormat,
							width, height, 0, 
							size, tmpdata);
					}
					break;
				};
				if(width>1)		width = width/2;
				if(height>1)	height = height/2;
				if(depth>1)		depth = depth/2;
			}
			delete [] tmpdata;
		}
		else
		{
			// Run through this process to pregenerate mipmap piramid
			for(size_t mip=0; mip<mMipMapCount; mip++)
			{
				// Normal formats
				switch(mType)
				{
				case TT_1D:
					glTexImage1D(GL_TEXTURE_1D, mip, fg.internalFormat,
						width, 0, 
						fg.glFormat, fg.glFormatType, 0);

					break;
				case TT_2D:
					glTexImage2D(GL_TEXTURE_2D, mip, fg.internalFormat,
						width, height, 0, 
						fg.glFormat, fg.glFormatType, 0);
					break;
				case TT_3D:
					glTexImage3D(GL_TEXTURE_3D, mip, fg.internalFormat,
						width, height, depth, 0, 
						fg.glFormat, fg.glFormatType, 0);
					break;
				case TT_Cube:
					for(int face=0; face<6; face++) {
						glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, mip, fg.internalFormat,
							width, height, 0, 
							fg.glFormat, fg.glFormatType, 0);
					}
					break;
				};
				if(width>1)		width = width/2;
				if(height>1)	height = height/2;
				if(depth>1)		depth = depth/2;
			}
		}
		createSufaceList();
		// Get final internal format
		mFormat = getBuffer((CubeFace)0,0)->getFormat();
	}

	void GLTexture::releaseHardwareResource()
	{
		mSurfaceList.clear();
		glDeleteTextures( 1, &mTextureID );
	}


	void GLTexture::createSufaceList()
	{
		mSurfaceList.clear();

		size_t faceCount = mType == TT_Cube ? 6 : 1;
		for(uint32 face = 0; face<faceCount; face++)
		{
			for(size_t mip=0; mip < mMipMapCount; mip++)
			{
				GLPixelBuffer* newBuf = new GLPixelBuffer(getGLTextureTarget(), mTextureID, face, mip, mType, mFormat, HBU_Static);
				mSurfaceList.push_back(DPixelBufferPtr(newBuf));

				/// Check for error
				if(newBuf->getWidth()==0 || newBuf->getHeight()==0 || newBuf->getDepth()==0)
				{
					DUEL_EXCEPT_BRIEF(
						DException::ET_InternalError, 
						"Zero sized texture surface on texture "+getName()+
						" face "+DStringTool::toString(face)+
						" mipmap "+DStringTool::toString(mip)+
						". Probably, the GL driver refused to create the texture.", 
						"GLTexture::createSurfaceList");
				}
			}
		}
	}
	/*
	void GLTexture::setSamplerState( const DTextureSamplerState& val )
	{
		GLenum	addrModeU = GLTranslator::getGLTextureAddressMode(val.addressU);
		GLenum	addrModeV = GLTranslator::getGLTextureAddressMode(val.addressV);
		GLenum	addrModeW = GLTranslator::getGLTextureAddressMode(val.addressW);
		GLenum	GLMinFilter;
		GLenum	GLMagFilter;

		if (val.mipFilter == FO_Linear)
		{
			if (val.minFilter == FO_Linear)
			{
				GLMinFilter = GL_LINEAR_MIPMAP_LINEAR;
			}
			else // min point
			{
				GLMinFilter = GL_NEAREST_MIPMAP_LINEAR;
			}
		}
		else // mip point
		{
			if (val.minFilter == FO_Linear)
			{
				GLMinFilter = GL_LINEAR_MIPMAP_NEAREST;
			}
			else // min point
			{
				GLMinFilter = GL_NEAREST_MIPMAP_NEAREST;
			}
		}
		if (val.magFilter == FO_Point)
		{
			GLMagFilter = GL_NEAREST;
		}
		else
		{
			GLMagFilter = GL_LINEAR;
		}
		// 各向异性过滤..啊.... 假设有一个开了 咱就都算了吧.
		bool bAnistropic = false;
		if (val.minFilter == FO_Anistropic || val.magFilter == FO_Anistropic || val.mipFilter == FO_Anistropic)
		{
			GLMagFilter = GL_LINEAR;
			GLMinFilter = GL_LINEAR_MIPMAP_LINEAR;
			bAnistropic = true;
		}

		GLuint	texID = getTextureID();
		GLenum	texType = getGLTextureTarget();


		glBindTexture(texType, texID);
		glTexParameteri(texType, GL_TEXTURE_WRAP_S, addrModeU);
		glTexParameteri(texType, GL_TEXTURE_WRAP_T, addrModeV);
		glTexParameteri(texType, GL_TEXTURE_WRAP_R, addrModeW);

		glTexParameterfv(texType, GL_TEXTURE_BORDER_COLOR, val.borderColor.ptr());

		glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GLMagFilter);
		glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GLMinFilter);

		if (bAnistropic)
		{
			glTexParameteri(texType, GL_TEXTURE_MAX_ANISOTROPY_EXT, val.maxAnisotropy);
		}
		else
		{
			glTexParameteri(texType, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1);
		}
// 		if (glloader_GL_EXT_texture_filter_anisotropic())
// 		{
// 	
		glTexParameterf(texType, GL_TEXTURE_MIN_LOD, val.minLod);
		glTexParameterf(texType, GL_TEXTURE_MAX_LOD, val.maxLod);

		if (val.samplerComparison != CF_AlwaysFail)
		{
			glTexParameteri(texType, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
		}
		else
		{
			glTexParameteri(texType, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		}
		glTexParameteri(texType, GL_TEXTURE_COMPARE_FUNC, GLTranslator::getGLCompareFunction(val.samplerComparison));
		// TODO: MIPMAP_BIAS.

		// unbind.
		glBindTexture(texType, 0);
	}
	*/
}