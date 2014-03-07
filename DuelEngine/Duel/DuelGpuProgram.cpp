//  [9/29/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelResourceGroupManager.h"
#include "DuelGpuProgram.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DGpuProgram, DResource);
	DGpuProgram::DGpuProgram(DResourceManager* fathermanager, const DString& name, const DString& groupName) : 
		DResource(fathermanager, name, groupName, "GpuProgram"), mProgramType(GPT_Vertex)
	{

	}

	DGpuProgram::~DGpuProgram()
	{

	}


	void DGpuProgram::setProgramType( GpuProgramType type )
	{
		mProgramType = type;
	}

	GpuProgramType DGpuProgram::getProgramType()
	{
		return mProgramType;
	}

	bool DGpuProgram::hasCompileError( void ) const
	{
		return mbFailed;
	}

	void DGpuProgram::resetCompileError( void )
	{
		mbFailed = false;
	}



	void DGpuProgram::loadImpl() 
	{
		if (mPreparedData == NULL)
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
				"Prepared data is empty",
				"Duel::DGpuProgram::loadImpl()");
		}
		size_t length = mPreparedData->getSize();
		if (length == 0)
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
				"Prepared data is empty",
				"Duel::DGpuProgram::loadImpl()");
		}
		char* buffer = new char[length];
		mPreparedData->read(buffer,length);
		mSourceCode.assign(buffer, length);
		delete[] buffer;
		loadFromSource();
	}

	void DGpuProgram::unloadImpl()
	{
		mSourceCode.clear();
	}


}