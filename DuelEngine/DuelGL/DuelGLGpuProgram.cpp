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
		mProfile = profile;
		if (mProfile == "GL_VERTEX_SHADER")
		{
			mProgramType = GPT_Vertex;
		}
		else if (mProfile == "GL_FRAGMENT_SHADER")
		{
			mProgramType = GPT_Pixel;
		}// ÔÝÊ±Ã»ÓÐgs
		else
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
				"Unknown shader type: " + mProfile,
				"Duel::GLGpuProgram::setProfile")
		}
	}

	const DString& GLGpuProgram::getProfile() const
	{
		return mProfile;
	}

	void GLGpuProgram::setEntry( const DString& entry )
	{
		mEntry = entry;
	}

	const DString& GLGpuProgram::getEntry() const
	{
		return mEntry;
	}

	const DString& GLGpuProgram::getCompilationError()
	{
		return mCompileError;
	}


}