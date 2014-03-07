//  [8/22/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelShaderObject.h"
#include "DuelRenderPass.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DShaderObject, DObject);

	DShaderObject::DShaderObject() :
		mVSProgram(NULL),
		mPSProgram(NULL)
	{

	}

	void DShaderObject::build( DRenderPass* pass )
	{
		DGpuProgram* vs = pass->getVertexProgram()->getAs<DGpuProgram>(false);
		DGpuProgram* ps = pass->getPixelProgram()->getAs<DGpuProgram>(false);
		if (vs != NULL && vs->isLoaded() &&
			ps != NULL && ps->isLoaded())
		{
			mVSProgram = vs;
			mPSProgram = ps;
			vs->signalUnloadCompleted.connect(
				DBind(&DShaderObject::onGpuProgramUnload, this, _1));
			ps->signalUnloadCompleted.connect(
				DBind(&DShaderObject::onGpuProgramUnload, this, _1));
		}
	}

	void DShaderObject::onGpuProgramUnload( DResource* program )
	{
		DGpuProgram* p = program->getAs<DGpuProgram>(false);
		if (p == NULL)
		{
			return;
		}
		if (p == mVSProgram)
		{
			mVSProgram = NULL;
			p->signalUnloadCompleted.disconnect(
				DBind(&DShaderObject::onGpuProgramUnload, this, _1));
		}
		if (p == mPSProgram)
		{
			mPSProgram = NULL;
			p->signalUnloadCompleted.disconnect(
				DBind(&DShaderObject::onGpuProgramUnload, this, _1));
		}
	}


}