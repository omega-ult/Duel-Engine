//  [7/30/2013 OMEGA] created

#include "DuelGLCommon.h"
#include "DuelGLGpuParameters.h"
#include "DuelException.h"
#include "DuelGLRenderView.h"
#include "DuelGLTexture.h"
#include "DuelGLTranslator.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(GLGpuTextureConstant, DGpuTextureConstant);

	GLGpuTextureConstant::GLGpuTextureConstant( GLenum gltexTarget, GLuint gltexID ) :
		mTexTarget(gltexTarget),
		mTexID(gltexID),
		mbValid(false)
	{
		if (mTexID != 0 && (gltexTarget == GL_TEXTURE_1D
			|| gltexTarget == GL_TEXTURE_2D
			|| gltexTarget == GL_TEXTURE_3D
			|| gltexTarget == GL_TEXTURE_CUBE_MAP))
		{
			mbValid = true;
		}
	}


	void GLGpuTextureConstant::setSamplerState( const DTextureSamplerState& val )
	{
		if (!isValid())
		{
			return;
		}
		GLenum	addrModeU = GLTranslator::getGLTextureAddressMode(val.addressU);
		GLenum	addrModeV = GLTranslator::getGLTextureAddressMode(val.addressV);
		GLenum	addrModeW = GLTranslator::getGLTextureAddressMode(val.addressW);
		GLenum	GLMinFilter;
		GLenum	GLMagFilter;

		if (val.mipFilter == FO_Linear)
		{
			GLMinFilter = (val.minFilter == FO_Linear) ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR;
		}
		else // mip point
		{
			GLMinFilter = (val.minFilter == FO_Linear) ? GL_LINEAR_MIPMAP_NEAREST : GL_NEAREST_MIPMAP_NEAREST;
		}
		GLMagFilter = (val.magFilter == FO_Point) ? GL_NEAREST : GL_LINEAR;

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
// 
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

		glTexParameterf(texType, GL_TEXTURE_MIN_LOD, val.minLod);
		glTexParameterf(texType, GL_TEXTURE_MAX_LOD, val.maxLod);

		if (val.samplerComparison != CF_AlwaysFail)
		{
			glTexParameteri(texType, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		}
		else
		{
			glTexParameteri(texType, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		}
		glTexParameteri(texType, GL_TEXTURE_COMPARE_FUNC, GLTranslator::getGLCompareFunction(val.samplerComparison));
		// TODO: MIPMAP_BIAS.
		glTexParameterf(texType, GL_TEXTURE_LOD_BIAS, val.mipLodBias);
		// unbind.
		glBindTexture(texType, 0);
	}

	GLuint GLGpuTextureConstant::getTextureID()
	{
		return mTexID;
	}

	GLenum GLGpuTextureConstant::getGLTextureTarget()
	{
		return mTexTarget;
	}

	void GLGpuTextureConstant::discard()
	{
		mTexID = 0;
		mTexTarget = 0;
		mbValid = false;
	}

}