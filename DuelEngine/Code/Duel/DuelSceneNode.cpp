//  [12/16/2012 OMEGA]

#include "DuelCommon.h"
#include "DuelNode.h"
#include "DuelSceneManager.h"
#include "DuelSceneNode.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DSceneNode, DNode)

	DSceneNode::DSceneNode( DSceneManager* creator, const DString& name ) :
		mCreator(creator),
		mName(name),
		mbEnable(true)
	{

	}

	DSceneNode::~DSceneNode()
	{

	}

	void DSceneNode::destroyChild( uint32 index )
	{
		DSceneNode *n = static_cast<DSceneNode*>(removeChild(index));
		if (n != NULL)
		{
			n->destroyAllChildren();
			DSceneManager* mgr = n->getCreator();
			mgr->destroySceneNode(n);
		}
	}

	void DSceneNode::destroyChild( const DString& name )
	{
		DSceneNode *n = static_cast<DSceneNode*>(removeChild(name));
		if (n != NULL)
		{
			n->destroyAllChildren();
			DSceneManager* mgr = n->getCreator();
			mgr->destroySceneNode(n);
		}
	}

	void DSceneNode::destroyChild( DSceneNode* n )
	{
		DSceneNode *node = static_cast<DSceneNode*>(removeChild(n));
		if (node != NULL)
		{
			node->destroyAllChildren();
			DSceneManager* mgr = node->getCreator();
			mgr->destroySceneNode(node);
		}
	}

	void DSceneNode::destroyAllChildren()
	{
		ChildSet::iterator i, iend = mChildren.end();
		for (i = mChildren.begin(); i != iend; ++i)
		{
			DSceneNode *n = static_cast<DSceneNode*>(*i);
			n->destroyAllChildren();
			DSceneManager* mgr = n->getCreator();
			mgr->destroySceneNode(n);
		}
		mChildren.clear();
		needUpdate();
	}

	void DSceneNode::attachMovable( DMovable* obj )
	{
		if(obj != NULL)
		{
			// check duplication.
			MovableIterator mbi = getMovableMapIterator();
			while (mbi.hasMoreElements())
			{
				DMovable* m = mbi.getNext();
				if (m == obj)
				{
					return;
				}
			}
			obj->attachToNode(this);
			mMovableList.push_back(obj);

		}
		// Make sure bounds get updated (must go right to the top)
		needUpdate();
	}

	size_t DSceneNode::getAttachedMovableCount() const
	{
		return mMovableList.size();
	}



	bool DSceneNode::hasMovable( DMovable* obj )
	{
		MovableList::iterator i, iend = mMovableList.end();
		for (i = mMovableList.begin(); i != iend; ++i)
		{
			if ((*i) == obj)
			{
				return true;
			}
		}
		return false;
	}


	void DSceneNode::detachMovable( DMovable* obj )
	{
		MovableList::iterator i, iend = mMovableList.end();
		for (i = mMovableList.begin(); i != iend; ++i)
		{
			if ((*i) == obj)
			{
				(*i)->attachToNode(NULL);
				mMovableList.erase(i);
				// Make sure bounds get updated (must go right to the top)
				needUpdate();
			}
		}
	}

	void DSceneNode::detachAllMovables()
	{
		MovableList::iterator i, iend = mMovableList.end();
		for (i = mMovableList.begin(); i != iend; ++i)
		{
			(*i)->attachToNode(NULL);
		}
		mMovableList.clear();
		// Make sure bounds get updated (must go right to the top)
		needUpdate();
	}

	void DSceneNode::updateBound()
	{
		mBoundingBox.setNull();

		MovableList::iterator i, iend = mMovableList.end();
		for (i = mMovableList.begin(); i != iend; ++i)
		{
			mBoundingBox.merge((*i)->getBoundingBox());
		}
		// merge with children
		ChildSet::iterator ci, ciend = mChildren.end();
		for(ci = mChildren.begin(); ci != ciend; ++ci)
		{
			DSceneNode* sceneChild = static_cast<DSceneNode*>(*ci);
			mBoundingBox.merge(sceneChild->getBoundingBox());
		}
		DVector3 trans = getInheritedPosition();
		DVector3 wMin = mBoundingBox.getMinimum() + trans;
		DVector3 wMax = mBoundingBox.getMaximum() + trans;
		mWorldAABB.setExtent(wMin, wMax);
	}

	DAxisAlignedBox DSceneNode::getBoundingBox()
	{
		return mBoundingBox;
	}

	void DSceneNode::applyToRenderQueue( DRenderQueue* destQueue)
	{
		MovableIterator i = getMovableMapIterator();
		while(i.hasMoreElements())
		{
			i.getNext()->applyToRenderQueue(destQueue);
		}
	}

	Duel::DAxisAlignedBox DSceneNode::getInheritedBox()
	{
		return mWorldAABB;
	}

}