//  [6/7/2014 OMEGA] created

#include "DuelD3D9Common.h"
#include "DuelD3D9GpuProgramManager.h"
#include "DuelD3D9GpuProgram.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(D3D9GpuProgramManger, DGpuProgramManager);

	

	DResource* D3D9GpuProgramManger::createImpl( DResourceDescription* createParam )
	{
		DGpuProgramDescription* desc = createParam->getAs<DGpuProgramDescription>();
		D3D9GpuProgram* ret = new D3D9GpuProgram(this, desc->getName(), desc->getGroupName());
		ret->setProfile(desc->profile);
		ret->setEntry(desc->entry);
		return ret;
	}

}