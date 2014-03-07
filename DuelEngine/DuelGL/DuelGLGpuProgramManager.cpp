//  [4/12/2013 OMEGA] created

#include "DuelGLCommon.h"
#include "DuelGLGpuProgram.h"
#include "DuelGLGpuProgramManager.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(GLGpuProgramManger, DGpuProgramManager);



	GLGpuProgramManger::GLGpuProgramManger() 
	{

	}

	DResource* GLGpuProgramManger::createImpl( DResourceDescription* createParam )
	{
		DGpuProgramDescription* resParam = createParam->getAs<DGpuProgramDescription>(false);
		if (resParam == NULL)
		{
			return NULL;
		}
		GLGpuProgram* ret = new GLGpuProgram(this, resParam->getName(), resParam->getGroupName());

		//ret->setSourceCompiled(resParam->compiled);
		ret->setProfile(resParam->profile);
		ret->setEntry(resParam->entry);
		return ret;
	}

}
