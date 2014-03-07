//  [7/30/2013 OMEGA] created

#ifndef _DUELGLGPUPARAMETERS_H_
#define _DUELGLGPUPARAMETERS_H_

#include "Duel.h"
#include "DuelGLCommon.h"

namespace Duel
{
	// here we have two class can be used as gpu texture constant, GLRenderView and GLTexture
	class DUELGL_API GLGpuTextureConstant : public DGpuTextureConstant
	{
		DUEL_DECLARE_RTTI(GLGpuTextureConstant)
	public:
		GLGpuTextureConstant(GLRenderView* parent);
		GLGpuTextureConstant(GLTexture* parent);
		uint32			getWidth();
		uint32			getHeight();
		DPixelFormat	getColorFormat();

		GLenum			getGLTextureTarget();
		GLuint			getTextureID();

		virtual void	setSamplerState( const DTextureSamplerState& state );



	protected:
		GLRenderView*	mViewParent;
		GLTexture*		mTextureParent;
	};


}


#endif