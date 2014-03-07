//  [4/7/2013 OMEGA] created

#ifndef _DUELGLTEXTURE_H_
#define _DUELGLTEXTURE_H_

#include "Duel.h"
#include "DuelGLCommon.h"
#include "DuelGLGpuParameters.h"

namespace Duel
{

	class DUELGL_API GLTexture : public DTexture
	{
	DUEL_DECLARE_RTTI(GLTexture)
	public:
		GLTexture(DResourceManager* fathermanager, const DString& name, const DString& groupName);
		~GLTexture();
		// override : DTexture-----------------------------
		virtual bool isHardwareGammaSupported();
		// override : DTexture-----------------------------
		virtual bool isHardwareMipMapSupported();
		// override : DTexture-----------------------------
		virtual void copyToTexture( DTexture* dstTex );
		// override : DTexture-----------------------------
		virtual DPixelBufferPtr getBuffer( CubeFace face = CF_PositiveX, size_t mipmap = 0 );
		

		// override : DTexture-----------------------------
		DGpuTextureConstant*	getGpuTexutureConstant() { return &mGpuConstant; }

//		void			setSamplerState(const DTextureSamplerState& state);
		INTERNAL GLenum getGLTextureTarget() const;
		GLuint			getTextureID() const { return mTextureID; }


	protected:
		// override : DTexture-----------------------------
		virtual void createHardwareResource();
		// override : DTexture-----------------------------
		virtual void releaseHardwareResource();
		// inner method used to create surface list
		virtual void	createSufaceList();
		GLuint			mTextureID;

		GLGpuTextureConstant	mGpuConstant;

		// store surfaces 
		typedef	std::vector<DPixelBufferPtr>	SurfaceList;
		SurfaceList		mSurfaceList;

	};

}

#endif