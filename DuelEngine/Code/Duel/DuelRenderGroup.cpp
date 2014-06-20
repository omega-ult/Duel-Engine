//  [12/21/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelRenderGroup.h"

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(DRenderGroup, DObject);

	DRenderGroup::DRenderGroup( DRenderQueue* parent, uint32 groupID ) :
		mParent(parent),
		mID(groupID),
		mbShadowEnabled(false),
		mSortOrder(CDS_NoSort)
	{
	}

	DRenderGroup::~DRenderGroup()
	{
	}


	void DRenderGroup::addRenderable( DRenderable* rend )
	{
		mRenderableList.push_back(rend);
	}

	void DRenderGroup::clear()
	{
		mRenderableList.clear();
	}

	struct DCamSortDepthDescending
	{
		const DCamera* cam;
		DCamSortDepthDescending(const DCamera* c) : cam(c) {}
		bool	operator() (DRenderable* a, DRenderable* b)
		{
			// Sort DESCENDING by depth (i.e. far objects first)
			return a->getSquaredViewDepthToCamera(cam) > b->getSquaredViewDepthToCamera(cam);
		}
	};
	struct DCamSortDepthAscending
	{
		const DCamera* cam;
		DCamSortDepthAscending(const DCamera* c) : cam(c) {}
		bool	operator() (DRenderable* a, DRenderable* b)
		{
			// Sort ASCENDING by depth (i.e. near objects first)
			return a->getSquaredViewDepthToCamera(cam) < b->getSquaredViewDepthToCamera(cam);
		}
	};
	void DRenderGroup::sort( const DCamera* cam )
	{
		switch (mSortOrder)
		{
		case Duel::CDS_Ascending:
			std::stable_sort(mRenderableList.begin(), mRenderableList.end(), 
				DCamSortDepthAscending(cam));
			break;
		case Duel::CDS_Descending:
			std::stable_sort(mRenderableList.begin(), mRenderableList.end(), 
				DCamSortDepthDescending(cam));
			break;
		case Duel::CDS_NoSort:
			break;
		default:
			break;
		}
	}


}