//  [8/22/2013 OMEGA] created

#ifndef _DUELGLSHADEROBJECT_H_
#define _DUELGLSHADEROBJECT_H_

#include "DuelGLCommon.h"
#include "DuelGLGpuProgram.h"

namespace Duel
{
	class DUELGL_API GLShaderObject : public DShaderObject
	{
		DUEL_DECLARE_RTTI(GLShaderObject)
	public:
		GLShaderObject();
		virtual ~GLShaderObject();
		// we assert the vertex shader and pixel shader are all loaded.
		void	build(DRenderPass* pass);

		GLuint	getProgramID() { return mProgramID; }
		// override : DShaderObject-------------------
		virtual DGpuParameters* getVertexProgramParameters();
		virtual DGpuParameters* getPixelProgramParameters();

	protected:
		// internal method used for building parameters for the program.
		void	buildParameters();
		GLuint	mProgramID;
		GLuint	mGLVSShaderID;
		GLuint	mGLPSShaderID;
		// since glsl's parameters can be generated after linking the shader
		// and vs/ps share the same parameters, so we just need one param.
		DGpuParameters*	mLinkedParam;
	};
}

#endif