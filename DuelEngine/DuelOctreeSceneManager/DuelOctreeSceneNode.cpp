//  [1/18/2013 OMEGA] created

#include "DuelOctreeCommon.h"
#include "DuelSceneNode.h"
#include "DuelOctreeSceneNode.h"
#include "DuelOctreeSceneManager.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(OctreeSceneNode, DSceneNode);

	OctreeSceneNode::OctreeSceneNode( DSceneManager* creator, const DString& name ) :
		DSceneNode(creator, name)
	{

	}

	OctreeSceneNode::~OctreeSceneNode()
	{
	}

	void OctreeSceneNode::unbindOctant()
	{
		assert(DUEL_IS_EXACT_KIND(OctreeSceneManager,mCreator));
		static_cast<OctreeSceneManager*>(mCreator)->removeOctreeSceneNode(this);
		ChildSet::iterator i, iend = mChildren.end();
		for (i = mChildren.begin(); i != iend; ++i)
		{
			static_cast<OctreeSceneNode*>(*i)->unbindOctant();
		}
	}

	bool OctreeSceneNode::isIn( const DAxisAlignedBox& box )
	{
		// Always fail if not in the scene graph or box is null
		if (!mbEnable || box.isNull())
		{
			return false;
		}
		if (box.isInfinite())
		{
			return true;
		}

		DVector3 cent = mBoundingBox.getCenter();
		DVector3 bmin = box.getMinimum();
		DVector3 bmax = box.getMaximum();

		// center is out of box.
		if (!(bmax > cent && bmin < cent))
		{
			return false;
		}
		// Even if covering the centre line, need to make sure this BB is not large
		// enough to require being moved up into parent. When added, bboxes would
		// end up in parent due to cascade but when updating need to deal with
		// bbox growing too large for this child
		DVector3 octSize = bmax - bmin;
		DVector3 nodeSize = mBoundingBox.getSize();
		return nodeSize < octSize;
	}

	DNode* OctreeSceneNode::removeChild( uint32 index )
	{
		DNode* sn = DSceneNode::removeChild(index);
		assert(DUEL_IS_EXACT_KIND(OctreeSceneNode, sn));
		OctreeSceneNode* osn = static_cast<OctreeSceneNode*>(sn);
		osn->unbindOctant();
		return osn;
	}

	DNode* OctreeSceneNode::removeChild( const DString& name )
	{
		DNode* sn = DSceneNode::removeChild(name);
		assert(DUEL_IS_EXACT_KIND(OctreeSceneNode, sn));
		OctreeSceneNode* osn = static_cast<OctreeSceneNode*>(sn);
		osn->unbindOctant();
		return osn;
	}

	DNode* OctreeSceneNode::removeChild( DNode* n )
	{
		DNode* sn = DSceneNode::removeChild(n);
		assert(DUEL_IS_EXACT_KIND(OctreeSceneNode, sn));
		OctreeSceneNode* osn = static_cast<OctreeSceneNode*>(sn);
		osn->unbindOctant();
		return osn;
	}

	void OctreeSceneNode::removeAllChildren()
	{
		ChildSet::iterator i, iend = mChildren.end();
		for(i = mChildren.begin(); i != iend; ++i)
		{
			assert(DUEL_IS_EXACT_KIND(OctreeSceneNode, *i));
			OctreeSceneNode* osn = static_cast<OctreeSceneNode*>(*i);
			osn->unbindOctant();
			osn->setParent(NULL);
		}
		mChildren.clear();
		mChildrenToUpdate.clear();
	}

	void OctreeSceneNode::updateBound()
	{
		DSceneNode::updateBound();
		if (mbEnable)
		{
			mCreator->updateSceneNode(this);
		}
	}

}