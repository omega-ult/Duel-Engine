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
		GLGpuTextureConstant(GLenum gltexTarget, GLuint gltexID);
		GLenum			getGLTextureTarget();
		GLuint			getTextureID();
		void			discard();
		virtual	bool	isValid() { return mbValid; }
		virtual void setSamplerObject(DTextureSamplerObjectPtr samp);

	protected:
		bool			mbValid;
		GLenum			mTexTarget;
		GLuint			mTexID;
	};


}


#endif