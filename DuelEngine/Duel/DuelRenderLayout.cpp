//  [8/15/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelRenderResourceFactory.h"
#include "DuelRenderLayout.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DRenderLayout, DObject);

	DRenderLayout::DRenderLayout(DRenderResourceFactory* creator) :
		mCreator(creator),
		mPrimitiveTopology(PT_TriangleList)
	{
		
	}


	DRenderLayout::~DRenderLayout()
	{

	}

}