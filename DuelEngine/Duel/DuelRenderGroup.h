//  [12/17/2012 OMEGA] created
//  [4/12/2013 OMEGA] 修改group的运作方式, 使用Technique作为索引, 并且使用了新的sort函数.

#ifndef _DUELRENDERGROUP_H_
#define _DUELRENDERGROUP_H_

#include "DuelCommon.h"
#include "DuelRenderPass.h"
#include "DuelRenderQueue.h"

namespace Duel
{


	// renderables' sorting type according to the distance to the camera.
	enum CameraDepthSort
	{
		// sorting by the depth to the camera, ascendingly.
		CDS_Ascending	= 0,
		// sorting by the depth to the camera, descendingly.
		CDS_Descending	= 1,
		// not sort by camera depth. 
		CDS_NoSort
	};

	// a render group is a container that holds the collection of renderables,
	// it can sort the renderable by the distance to the camera ascending or descending.
	// render group has sorting ability, and it must be sorted before you reach final
	// available renderables, client can call sort(), then call getRenderElementsIterator() to get the final renderables.
	class DUEL_API DRenderGroup : public DObject
	{
	DUEL_DECLARE_RTTI(DRenderGroup)
	public:
		// the DRenderGroup should only be created by DRenderQueue.
		DRenderGroup(DRenderQueue* parent, uint32 groupID);
		virtual ~DRenderGroup();
		// get the id of this group
		uint32			getGroupID() const { return mID; }
		// get the parent of this group
		DRenderQueue*	getParent() const { return mParent; }

		// set/get shadow state for current group, it is important to have a proper light
		// to render the group efficiently, so spot light and directional light is recommanded.
		void			setShadowsEnabled(bool enabled) { mbShadowEnabled = enabled; }
		bool			isShadowsEnabled(void) const { return mbShadowEnabled; }



		// set the sorting order.
		void			setCameraSortingOrder(CameraDepthSort order) { mSortOrder = order; }
		// query the sorting order, because the order is designed as a mask, so you 
		// can only query one order at a time.
		CameraDepthSort	getCameraSortingOrder() const { return mSortOrder; }

		typedef std::vector<DRenderable*>	RenderableList;
		typedef	VectorIterator<RenderableList>	RenderableIterator;
		RenderableIterator	getRederableIterator() {return RenderableIterator(mRenderableList); }

		// add a renderable and a tech to form a render component.
		void			addRenderable(DRenderable* rend);
		// sort renderables with camara order.
		void			sort(const DCamera* cam);

		// clear all renderables in the list, prepare for new renderables
		void			clear();


	protected:
		// the creator of this group.
		DRenderQueue*	mParent;
		// the id, also the priority of this group.
		uint32			mID;
		// indicate whether the group enabled in shadow processing. 
		// default : false;
		bool			mbShadowEnabled;
		// the current sorting order.
		CameraDepthSort	mSortOrder;

		RenderableList	mRenderableList;

	private:
		// not allowed.
		DRenderGroup() {}
	};

}


#endif