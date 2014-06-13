//  [7/27/2013 OMEGA] created

#include "DuelGLCommon.h"
#include "DuelGLRenderState.h"
#include "DuelGLTranslator.h"

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(GLRasterizerStateObject, DRasterizerStateObject);
	DUEL_IMPLEMENT_RTTI_1(GLDepthStencilStateObject, DDepthStencilStateObject);
	DUEL_IMPLEMENT_RTTI_1(GLBlendStateObject, DBlendStateObject);
	DUEL_IMPLEMENT_RTTI_1(GLTextureSamplerObject, DTextureSamplerObject);


	void GLRasterizerStateObject::getRasterizerState( DRasterizerState& outState )
	{
		outState = mState;
	}

	GLRasterizerStateObject::GLRasterizerStateObject( const DRasterizerState& state )
	{
		mGLPolygonMode	= GLTranslator::getGLPolygonMode(state.polygonMode);
		mGLShadeMode	= GLTranslator::getGLShadeMode(state.shadeMode);
		if (state.cullingMode == CM_None)
		{
			mbCulling = false;
			// not used
			mGLFrontFace	= GL_CCW;
		}
		else
		{		
			mbCulling = true;
			mGLFrontFace	= state.cullingMode == CM_AntiClockWise ? GL_CCW : GL_CW;
		}
		mState = state;
	}


	void GLDepthStencilStateObject::getDepthStencilState( DDepthStencilState& outState )
	{
		outState = mState;
	}

	GLDepthStencilStateObject::GLDepthStencilStateObject( const DDepthStencilState& state )
	{
		mGLDepthWriteMask = state.depthWriteEnable ? GL_TRUE : GL_FALSE;
		mGLDepthFunc = GLTranslator::getGLCompareFunction(state.depthComparison);
		mGLFrontStencilFunc = GLTranslator::getGLCompareFunction(state.frontStencilComparison);
		mGLFrontStencilFail = GLTranslator::getGLStencilOperation(state.frontStencilFail);
		mGLFrontStencilDepthFail = GLTranslator::getGLStencilOperation(state.frontStencilDepthFail);
		mGLFrontStencilPass = GLTranslator::getGLStencilOperation(state.frontStencilPass);
		
		mGLBackStencilFunc = GLTranslator::getGLCompareFunction(state.backStencilComparison);
		mGLBackStencilFail = GLTranslator::getGLStencilOperation(state.backStencilFail);
		mGLBackStencilDepthFail = GLTranslator::getGLStencilOperation(state.backStencilDepthFail);
		mGLBackStencilPass = GLTranslator::getGLStencilOperation(state.backStencilPass);

		mState = state;
	}


	void GLBlendStateObject::getBlendState( DBlendState& outState )
	{
		outState = mState;
	}

	GLBlendStateObject::GLBlendStateObject( const DBlendState& state )
	{
		for (uint32 i = 0; i < DUEL_MAX_BLEND_LAYERS; ++i)
		{		
			mGLColorBlendOp[i]		= GLTranslator::getGLBlendOperation(state.targetStates[i].colorBlendOpeartion);
			mGLAlphaBlendOp[i]		= GLTranslator::getGLBlendOperation(state.targetStates[i].alphaBlendOpeartion);
			mGLColorSrcBlend[i]		= GLTranslator::getGLBlendFactor(state.targetStates[i].colorSrcFactor);
			mGLColorDstBlend[i]		= GLTranslator::getGLBlendFactor(state.targetStates[i].colorDstFactor);
			mGLAlphaSrcBlend[i]		= GLTranslator::getGLBlendFactor(state.targetStates[i].alphaSrcFactor);
			mGLAlphaDstBlend[i]		= GLTranslator::getGLBlendFactor(state.targetStates[i].alphaDstBlend);
		}
		mState = state;
	}

// 
// 	void GLTextureSamplerStateObject::getTextureSamplerState( DTextureSamplerState& outState )
// 	{
// 		outState = mState;
// 	}
// 
// 	GLTextureSamplerStateObject::GLTextureSamplerStateObject( const DTextureSamplerState& state )
// 	{
// 		mGLAddressModeU = GLTranslator::getGLTextureAddressMode(state.addressU);
// 		mGLAddressModeV = GLTranslator::getGLTextureAddressMode(state.addressV);
// 		mGLAddressModeW = GLTranslator::getGLTextureAddressMode(state.addressW);
// 		if (state.mipFilter == FO_LINEAR)
// 		{
// 			if (state.minFilter == FO_LINEAR)
// 			{
// 				mGLMinFilter = GL_LINEAR_MIPMAP_LINEAR;
// 			}
// 			else // min point
// 			{
// 				mGLMinFilter = GL_NEAREST_MIPMAP_LINEAR;
// 			}
// 		}
// 		else // mip point
// 		{
// 			if (state.minFilter == FO_LINEAR)
// 			{
// 				mGLMinFilter = GL_LINEAR_MIPMAP_NEAREST;
// 			}
// 			else // min point
// 			{
// 				mGLMinFilter = GL_NEAREST_MIPMAP_NEAREST;
// 			}
// 		}
// 		if (state.magFilter == FO_POINT)
// 		{
// 			mGLMagFilter = GL_NEAREST;
// 		}
// 		else
// 		{
// 			mGLMagFilter = GL_LINEAR;
// 		}
// 		// 各向异性过滤..啊.... 假设有一个开了 咱就都算了吧.
// 		if (state.minFilter == FO_ANISOTROPIC || state.magFilter == FO_ANISOTROPIC || state.mipFilter == FO_ANISOTROPIC)
// 		{
// 			mGLMagFilter = GL_LINEAR;
// 			mGLMinFilter = GL_LINEAR_MIPMAP_LINEAR;
// 		}
// 		mState = state;
// 	}


	GLTextureSamplerObject::GLTextureSamplerObject( const DTextureSampler& state )
	{
		glGenSamplers(1, &mGLSampObj);
		mState = state;
		GLenum	addrModeU = GLTranslator::getGLTextureAddressMode(state.addressU);
		GLenum	addrModeV = GLTranslator::getGLTextureAddressMode(state.addressV);
		GLenum	addrModeW = GLTranslator::getGLTextureAddressMode(state.addressW);

		glSamplerParameteri(mGLSampObj, GL_TEXTURE_WRAP_S, addrModeU);
		glSamplerParameteri(mGLSampObj, GL_TEXTURE_WRAP_T, addrModeV);
		glSamplerParameteri(mGLSampObj, GL_TEXTURE_WRAP_R, addrModeW);
		
		GLenum	minFilter;
		GLenum	magFilter;

		float maxAnisotropy = 1.0f;
		if (state.minFilter == FO_Anistropic || state.magFilter == FO_Anistropic || state.mipFilter == FO_Anistropic)
		{
			magFilter = GL_LINEAR;
			minFilter = GL_LINEAR_MIPMAP_LINEAR;
		}
		else
		{
			if (state.mipFilter != FO_None)
			{
				if (state.mipFilter == FO_Linear)
				{
					minFilter = (state.minFilter == FO_Linear) ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR;
				}
				else
				{
					minFilter = (state.minFilter == FO_Linear) ? GL_LINEAR_MIPMAP_NEAREST : GL_NEAREST_MIPMAP_NEAREST;
				}
				magFilter = (state.magFilter == FO_Linear) ? GL_LINEAR : GL_NEAREST;
			}
			else
			{
				minFilter = (state.minFilter == FO_Linear) ? GL_LINEAR : GL_NEAREST;
				magFilter = (state.magFilter == FO_Linear) ? GL_LINEAR : GL_NEAREST;
			}
		}
		glSamplerParameteri(mGLSampObj, GL_TEXTURE_MIN_FILTER, minFilter);
		glSamplerParameteri(mGLSampObj, GL_TEXTURE_MAG_FILTER, magFilter);
		glSamplerParameterf(mGLSampObj, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);

		glSamplerParameterf(mGLSampObj, GL_TEXTURE_MIN_LOD, state.minLod);
		glSamplerParameterf(mGLSampObj, GL_TEXTURE_MAX_LOD, state.maxLod);

		if (state.samplerComparison != CF_AlwaysFail)
		{
			glSamplerParameteri(mGLSampObj, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		}
		else
		{
			glSamplerParameteri(mGLSampObj, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		}
		glSamplerParameteri(mGLSampObj, GL_TEXTURE_COMPARE_FUNC, GLTranslator::getGLCompareFunction(state.samplerComparison));
		glSamplerParameterf(mGLSampObj, GL_TEXTURE_LOD_BIAS, state.mipLodBias);
	}

	void GLTextureSamplerObject::getTextureSampler( DTextureSampler& outState )
	{
		outState = mState;
	}

	GLTextureSamplerObject::~GLTextureSamplerObject()
	{
		if (mGLSampObj != 0)
		{
			glDeleteSamplers(1, &mGLSampObj);
		}
	}

}