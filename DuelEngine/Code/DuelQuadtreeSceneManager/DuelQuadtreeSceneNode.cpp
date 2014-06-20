//  [3/9/2013 OMEGA] created

#include "DuelQuadtreeCommon.h"
#include "DuelSceneNode.h"
#include "DuelQuadtreeSceneNode.h"
#include "DuelQuadtreeSceneManager.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(QuadtreeSceneNode, DSceneNode);

	QuadtreeSceneNode::QuadtreeSceneNode( QuadtreeSceneManager* creator, const DString& name ) :
		DSceneNode(creator, name),
		mQuadrant(NULL)
	{

	}

	QuadtreeSceneNode::~QuadtreeSceneNode()
	{
	}

	void QuadtreeSceneNode::unbindQuadrant()
	{
		assert(DUEL_IS_EXACT_KIND(QuadtreeSceneManager,mCreator));
		static_cast<QuadtreeSceneManager*>(mCreator)->removeQuadtreeSceneNode(this);
		ChildSet::iterator i, iend = mChildren.end();
		for (i = mChildren.begin(); i != iend; ++i)
		{
			static_cast<QuadtreeSceneNode*>(*i)->unbindQuadrant();
		}
	}

	bool QuadtreeSceneNode::isIn( const DAxisAlignedBox& box )
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
		DVector3 quadSize = bmax - bmin;
		DVector3 nodeSize = mBoundingBox.getSize();
		return nodeSize < quadSize;
	}

	DNode* QuadtreeSceneNode::removeChild( uint32 index )
	{
		DNode* sn = DSceneNode::removeChild(index);
		assert(DUEL_IS_EXACT_KIND(QuadtreeSceneNode, sn));
		QuadtreeSceneNode* qsn = static_cast<QuadtreeSceneNode*>(sn);
		qsn->unbindQuadrant();
		return qsn;
	}

	DNode* QuadtreeSceneNode::removeChild( const DString& name )
	{
		DNode* sn = DSceneNode::removeChild(name);
		assert(DUEL_IS_EXACT_KIND(QuadtreeSceneNode, sn));
		QuadtreeSceneNode* qsn = static_cast<QuadtreeSceneNode*>(sn);
		qsn->unbindQuadrant();
		return qsn;
	}

	DNode* QuadtreeSceneNode::removeChild( DNode* n )
	{
		DNode* sn = DSceneNode::removeChild(n);
		assert(DUEL_IS_EXACT_KIND(QuadtreeSceneNode, sn));
		QuadtreeSceneNode* qsn = static_cast<QuadtreeSceneNode*>(sn);
		qsn->unbindQuadrant();
		return qsn;
	}

	void QuadtreeSceneNode::removeAllChildren()
	{
		ChildSet::iterator i, iend = mChildren.end();
		for(i = mChildren.begin(); i != iend; ++i)
		{
			assert(DUEL_IS_EXACT_KIND(QuadtreeSceneNode, *i));
			QuadtreeSceneNode* qsn = static_cast<QuadtreeSceneNode*>(*i);
			qsn->unbindQuadrant();
			qsn->setParent(NULL);
		}
		mChildren.clear();
		mChildrenToUpdate.clear();
	}

	void QuadtreeSceneNode::updateBound()
	{

		DSceneNode::updateBound();
		if (mbEnable)
		{
			mCreator->updateSceneNode(this);
		}
	}
}