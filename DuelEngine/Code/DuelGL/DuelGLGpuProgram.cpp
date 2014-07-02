//  [4/12/2013 OMEGA] created

#include "DuelGLCommon.h"
#include "DuelGLGpuProgram.h"
#include "DuelGLGpuProgramManager.h"
#include "DuelGLGpuParameters.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(GLGpuProgram, DGpuProgram);

	DString GLGpuProgram::msLanguage = "GLSL";

	GLGpuProgram::GLGpuProgram(DResourceManager* fathermanager, const DString& name, const DString& groupName) :
		DGpuProgram(fathermanager, name, groupName)
	{
	}

	void GLGpuProgram::loadFromSource()
	{
		mbFailed = false;
	}

	size_t GLGpuProgram::calculateSize()
	{
		return 0;
	}

	const DString& GLGpuProgram::getLanguage( void ) const
	{
		return msLanguage;
	}

	void GLGpuProgram::setProfile( const DString& profile )
	{
		if (profile == "GL_VERTEX_SHADER")
		{
			mProgramType = GPT_Vertex;
		}
		else if (profile == "GL_FRAGMENT_SHADER")
		{
			mProgramType = GPT_Pixel;
		}// ÔÝÊ±Ã»ÓÐgs
		else
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
				"Unknown shader type: " + profile,
				"Duel::GLGpuProgram::setProfile")
		}
		DGpuProgram::setProfile(profile);
	}


	
	const DString& GLGpuProgram::getCompileError() const
	{
		return mCompileError;
	}


}