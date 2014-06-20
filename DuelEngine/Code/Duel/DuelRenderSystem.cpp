//  [7/15/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelRenderSystem.h"
#include "DuelRenderState.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DRenderSystem, DObject);

	DRenderSystem::DRenderSystem()
	{

	}

	DRenderSystem::~DRenderSystem()
	{

	}

// 	Viewport* DRenderSystem::getViewport()
// 	{
// 		return mCurViewport;
// 	}

	uint32 DRenderSystem::queryGpuConstantCaps( GpuProgramType gpt, GpuConstantSlot gcs )
	{
		switch(gpt)
		{
		case GPT_Vertex:
			switch(gcs)
			{
			case GCS_Float:
				return	mCaps.VertexProgramConstantFloatCount;
			case GCS_Int:
				return	mCaps.VertexProgramConstantIntCount;
			case GCS_Bool:
				return	mCaps.VertexProgramConstantBoolCount;
			}
			break;
		case GPT_Geometry:
			switch(gcs)
			{
			case GCS_Float:
				return	mCaps.GeometryProgramConstantFloatCount;
			case GCS_Int:
				return	mCaps.GeometryProgramConstantIntCount;
			case GCS_Bool:
				return	mCaps.GeometryProgramConstantBoolCount;
			}
			break;
		case GPT_Pixel:
			switch(gcs)
			{
			case GCS_Float:
				return	mCaps.PixelProgramConstantFloatCount;
			case GCS_Int:
				return	mCaps.PixelProgramConstantIntCount;
			case GCS_Bool:
				return	mCaps.PixelProgramConstantBoolCount;
			}
			break;
		}
		// should never occur
		return 0;
	}



}
