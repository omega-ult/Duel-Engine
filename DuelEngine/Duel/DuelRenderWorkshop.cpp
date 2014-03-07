//  [1/5/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelRenderWorkshop.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DRenderWorkshop, DObject);


	DRenderWorkshop::DRenderWorkshop() :
		mPresentTarget(NULL)
	{
	}

	DRenderWorkshop::~DRenderWorkshop()
	{
		
	}

	DRenderQueue* DRenderWorkshop::createRenderQueue()
	{
		DRenderQueue* ret = new DRenderQueue();
		mQueueSet.insert(ret);
		return ret;
	}

	void DRenderWorkshop::destroyRenderQueue( DRenderQueue* queue )
	{
		RenderQueueSet::iterator i = mQueueSet.find(queue);
		if (i != mQueueSet.end())
		{
			delete *i;
			mQueueSet.erase(i);
		}
	}

	void DRenderWorkshop::destroyAllRenderQueues()
	{
		RenderQueueSet::iterator i, iend = mQueueSet.end();
		for(i = mQueueSet.begin(); i != iend; ++i)
		{
			delete *i;
		}
		mQueueSet.clear();
	}



}