//  [4/7/2013 OMEGA] created

#ifndef _DUELGLGPUPROGRAMMANAGER_H_
#define _DUELGLGPUPROGRAMMANAGER_H_

#include "Duel.h"
#include "DuelGLCommon.h"
// 
// #include <Cg/include/Cg/cg.h>
// #include <Cg/include/Cg/cgGL.h>

namespace Duel
{

	class DUELGL_API GLGpuProgramManger : public DGpuProgramManager
	{
	DUEL_DECLARE_RTTI(GLGpuProgramManger)
	public:
		GLGpuProgramManger();
	protected:		
		virtual DResource* createImpl( DResourceDescription* createParam );

	};

}

#endif