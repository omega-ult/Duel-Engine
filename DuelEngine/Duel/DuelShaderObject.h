//  [8/22/2013 OMEGA] created

#ifndef _DUELSHADEROBJECT_H_
#define _DUELSHADEROBJECT_H_

#include "DuelCommon.h"
#include "DuelGpuProgram.h"

namespace Duel
{

	// a shader object is a wrap of combination of specified GpuPrograms, 
	// usually it is constructed using a vertex shader, and a pixel shader,
	// (maybe geometry shader in the future), different rendersystem 
	// could have different construction method, so it must be retrieved
	// using DRenderSystem::createShaderObject;
	class DUEL_API DShaderObject : public DObject
	{
		DUEL_DECLARE_RTTI(DShaderObject)
	public:
		DShaderObject();
		virtual ~DShaderObject() {}

		const DString&	getPassName() const;
		virtual	void	build(DRenderPass* pass);
		
		bool			isValid()
		{
			return mVSProgram != NULL && mVSProgram->isLoaded() &&
				mPSProgram != NULL && mPSProgram->isLoaded() &&
				getVertexProgramParameters() != NULL &&
				getPixelProgramParameters() != NULL;
		}

		virtual	DGpuProgram*	getVertexProgram() { return mVSProgram; }
		virtual	DGpuProgram*	getPixelProgram() { return mPSProgram; }

		virtual DGpuParameters*	getVertexProgramParameters() = 0;
		virtual	DGpuParameters*	getPixelProgramParameters() = 0;

	protected:
		void			onGpuProgramUnload(DResource* program);
		DGpuProgram*	mVSProgram;
		DGpuProgram*	mPSProgram;
		DString			mPassName;
	};

}


#endif