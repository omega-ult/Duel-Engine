//  [11/2/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelNode.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DNode, DObject);

	DNode::DNode() :
		mRelativePos(DVector3::ZERO),
		mRelativeScale(DVector3::UNIT_SCALE),
		mRelativeOrient(DQuaternion::IDENTITY),
		mbPosInherited(true),
		mbScaleInherited(false),
		mbOrientInherited(true),
		mInheritedPos(DVector3::ZERO),
		mInheritedScale(DVector3::UNIT_SCALE),
		mInheritedOrient(DQuaternion::IDENTITY),
		mbNeedParentUpdate(false),
		mbNeedChildUpdate(false),
		mbParentNotified(false),
		mbInheritedTransformOutOfDate(true),
		mParent(NULL)
	{
		needUpdate();
	}

	DNode::DNode( const DString& name ) :
		mName(name),
		mRelativePos(DVector3::ZERO),
		mRelativeScale(DVector3::UNIT_SCALE),
		mRelativeOrient(DQuaternion::ZERO),
		mbPosInherited(true),
		mbScaleInherited(false),
		mbOrientInherited(true),
		mInheritedPos(DVector3::ZERO),
		mInheritedScale(DVector3::UNIT_SCALE),
		mInheritedOrient(DQuaternion::IDENTITY),
		mbNeedParentUpdate(false),
		mbNeedChildUpdate(false),
		mbParentNotified(false),
		mbInheritedTransformOutOfDate(true),
		mParent(NULL)
	{
		needUpdate();
	}

	DNode::~DNode()
	{
		signalDestroyed(this);
		ChildSet::iterator	i;
		for (i  = mChildren.begin(); i != mChildren.end(); ++i)
		{
			delete (*i);
		}
	}

	void DNode::setName( const DString& name )
	{
		mName = name;
	}

	const DString& DNode::getName()
	{
		return mName;
	}

	void DNode::setRelativePosition( const DVector3& pos )
	{
		mRelativePos = pos;
		needUpdate();
	}

	void DNode::setRelativeScale( const DVector3& scale )
	{
		mRelativeScale = scale;
		needUpdate();
	}

	void DNode::setRelativeOrientation( const DQuaternion& orient )
	{
		mRelativeOrient = orient;
		needUpdate();
	}


	const DVector3& DNode::getRelativePosition() const
	{
		return mRelativePos;
	}

	const DVector3& DNode::getRelativeScale() const
	{
		return mRelativeScale;
	}

	const DQuaternion& DNode::getRelativeOrientation() const
	{
		return mRelativeOrient;
	}
	const DVector3& DNode::getInheritedPosition()
	{
		if (mbNeedParentUpdate)
		{
			updateFromParent();
		}
		return mInheritedPos;
	}

	const DVector3& DNode::getInheritedScale()
	{
		if (mbNeedParentUpdate)
		{
			updateFromParent();
		}
		return mInheritedScale;
	}

	const DQuaternion& DNode::getInheritedOrient()
	{
		if (mbNeedParentUpdate)
		{
			updateFromParent();
		}
		return mInheritedOrient;
	}

	const DMatrix4& DNode::getInheritedTransform()
	{
		if (mbInheritedTransformOutOfDate)
		{
			update(mbNeedChildUpdate, true);
			// Use derived values
			mInheritedTransform.compose(
				getInheritedPosition(),
				getInheritedScale(),
				getInheritedOrient());
			mbInheritedTransformOutOfDate = false;
		}
		return mInheritedTransform;
	}

	DNode* DNode::createChild()
	{
		DNode* n = createChildImpl();
		n->setParent(this);
		mChildren.insert(n);
		signalAttached(this, n);
		return n;
	}

	DNode* DNode::createChild( const DString& name )
	{
		DNode* n = createChildImpl(name);
		n->setParent(this);
		mChildren.insert(n);
		signalAttached(this, n);
		return n;
	}

	void DNode::addChild( DNode* child )
	{
		child->setParent(this);
		mChildren.insert(child);
		signalAttached(this, child);
	}

	DNode* DNode::getParent()
	{
		return mParent;
	}

	size_t DNode::getChildCount()
	{
		return mChildren.size();
	}


	DNode* DNode::getChild( uint32 index )
	{
		assert(index < getChildCount());
		ChildSet::iterator	i = mChildren.begin();
		for (uint32 j = 0; j < index; ++j)
		{
			i++;
		}
		if (i != mChildren.end())
		{
			return (*i);
		}
		return NULL;
	}

	DNode* DNode::getChild( const DString& name )
	{
		ChildSet::iterator	i;
		for (i = mChildren.begin();i != mChildren.end(); ++i)
		{
			if ((*i)->mName == name)
			{
				break;
			}
		}
		if (i != mChildren.end())
		{
			return (*i);
		}
		return NULL;
	}

	const DNode::ChildSet& DNode::getChildSet()
	{
		return mChildren;
	}

	DNode* DNode::removeChild( uint32 index )
	{
		DNode* ret;
		assert(index < getChildCount());
		ChildSet::iterator	i = mChildren.begin();
		for (uint32 j = 0; j < index; ++j)
		{
			i++;
		}
		if (i != mChildren.end())
		{
			ret = *i;
			cancelUpdate(ret);
			(ret)->mParent = NULL;
			signalDetached(this, ret);
			mChildren.erase(i);
			return ret;
		}
		return NULL;
	}

	DNode* DNode::removeChild( const DString& name )
	{
		DNode* ret;
		ChildSet::iterator	i, iend = mChildren.end();
		for (i = mChildren.begin();i != iend; ++i)
		{
			if ((*i)->mName == name)
			{
				break;
			}
		}
		if (i != mChildren.end())
		{
			ret = *i;
			cancelUpdate(ret);
			(ret)->mParent = NULL;
			signalDetached(this, ret);
			mChildren.erase(i);
			return ret;
		}
		return NULL;
	}

	DNode* DNode::removeChild( DNode* n )
	{
		if (n != NULL)
		{
			ChildSet::iterator	i = mChildren.find(n);
			if (i != mChildren.end())
			{
				cancelUpdate(n);
				mChildren.erase(i);
				n->setParent(NULL);
			}
		}
		return n;
	}

	void DNode::removeAllChildren()
	{
		ChildSet::iterator	i, iend = mChildren.end();
		for (i = mChildren.begin();i != iend; ++i)
		{
			(*i)->setParent(NULL);
		}
		mChildren.clear();
		mChildrenToUpdate.clear();
	}


	void DNode::setParent( DNode* p )
	{
		bool diff = (p != mParent);
		if (mParent != NULL)
		{
			mParent->removeChild(this);
		}
		
		mParent = p;
		mbParentNotified = false;
		needUpdate();
		if (diff)
		{
			if (mParent)
			{
				signalAttached(mParent,this);
			}
			else
			{
				signalDetached(mParent,this);
			}
		}
	}


	void DNode::translate( const DVector3& t, TransformSpace space /*= TS_LOCAL*/ )
	{
		switch(space)
		{
		case TS_Local:
			mRelativePos += mRelativeOrient * t;
			break;
		case TS_Parent:
			mRelativePos += t;
			break;
		}
		needUpdate();
	}

	void DNode::translateX( DReal x, TransformSpace space /*= TS_LOCAL*/ )
	{
		DVector3 v(x,0.0f,0.0f);
		switch(space)
		{
		case TS_Local:
			mRelativePos += mRelativeOrient * v;
			break;
		case TS_Parent:
			mRelativePos += v;
			break;
		}
		needUpdate();
	}

	void DNode::translateY( DReal y, TransformSpace space /*= TS_LOCAL*/ )
	{
		DVector3 v(0.0f,y,0.0f);
		switch(space)
		{
		case TS_Local:
			mRelativePos += mRelativeOrient * v;
			break;
		case TS_Parent:
			mRelativePos += v;
			break;
		}
		needUpdate();
	}

	void DNode::translateZ( DReal z, TransformSpace space /*= TS_LOCAL*/ )
	{
		DVector3 v(0.0f,0.0f,z);
		switch(space)
		{
		case TS_Local:
			mRelativePos += mRelativeOrient * v;
			break;
		case TS_Parent:
			mRelativePos += v;
			break;
		}
		needUpdate();
	}

	void DNode::scale( const DVector3& s )
	{
		mRelativeScale *= s;
		needUpdate();
	}

	void DNode::rotate( const DVector3& axis, const DRadian& r, TransformSpace space /*= TS_LOCAL*/ )
	{
		DQuaternion q;
		q.fromAngleAxis(r,axis);
		rotate(q,space);
	}

	void DNode::rotate( const DRotator& r, TransformSpace space /*= TS_LOCAL*/ )
	{
		DQuaternion q;
		q = r.toQuaternion();
		rotate(q,space);
	}

	void DNode::rotate( const DQuaternion& r, TransformSpace space /*= TS_LOCAL*/ )
	{
		// Normalise quaternion to avoid drift
		DQuaternion qnorm = r;
		qnorm.normalize();
		switch(space)
		{
		case TS_Local:
			// Note the order of the mult, i.e. q comes after
			mRelativeOrient = mRelativeOrient * qnorm;
			break;
		case TS_Parent:
			// Rotations are normally relative to local axes, transform up
			mRelativeOrient = mRelativeOrient * qnorm;
			break;
		}
		needUpdate();
	}

	void DNode::yaw( const DRadian& y, TransformSpace space /*= TS_LOCAL*/ )
	{
		rotate(DVector3::UNIT_Y, y, space);
	}

	void DNode::pitch( const DRadian& p, TransformSpace space /*= TS_LOCAL*/ )
	{
		rotate(DVector3::UNIT_X, p, space);
	}

	void DNode::roll( const DRadian& r, TransformSpace space /*= TS_LOCAL*/ )
	{
		rotate(DVector3::UNIT_Z, r, space);
	}
	DNode* DNode::createChildImpl()
	{
		return new DNode();
	}

	DNode* DNode::createChildImpl( const DString& name )
	{
		return new DNode(name);
	}


	void DNode::needUpdate( bool forceParentUpdate /*= false*/ )
	{
		mbNeedParentUpdate = true;
		mbNeedChildUpdate = true;
		mbInheritedTransformOutOfDate = true;

		// Make sure we're not root and parent hasn't been notified before
		if (mParent && (!mbParentNotified || forceParentUpdate))
		{
			mParent->requestUpdate(this, forceParentUpdate);
			mbParentNotified = true ;
		}

		// all children will be updated
		mChildrenToUpdate.clear();
	}

	void DNode::requestUpdate( DNode* child, bool forceParentUpdate /*= false*/ )
	{
		// If we're already going to update everything this doesn't matter
		if (mbNeedChildUpdate)
		{
			return;
		}

		mChildrenToUpdate.insert(child);
		// Request selective update of me, if we didn't do it before
		if (mParent && (!mbParentNotified || forceParentUpdate))
		{
			mParent->requestUpdate(this, forceParentUpdate);
			mbParentNotified = true ;
		}
	}

	void DNode::cancelUpdate( DNode* child )
	{
		mChildrenToUpdate.erase(child);

		// Propogate this up if we're done
		if (mChildrenToUpdate.empty() && mParent && !mbNeedChildUpdate)
		{
			mParent->cancelUpdate(this);
			mbParentNotified = false ;
		}
	}


	void DNode::update( bool updateChildren, bool parentHasChanged )
	{
		// always clear information about parent notification
		mbParentNotified = false;

		// Short circuit the off case
		if (!updateChildren && !mbNeedParentUpdate && !mbNeedChildUpdate && !parentHasChanged )
		{
			return;
		}


		// See if we should process everyone
		if (mbNeedParentUpdate || parentHasChanged)
		{
			// Update transforms from parent
			updateFromParent();
		}

		if (mbNeedChildUpdate || parentHasChanged)
		{

			ChildSet::iterator it, itend;
			itend = mChildren.end();
			for (it = mChildren.begin(); it != itend; ++it)
			{
				DNode* child = (*it);
				child->update(true, true);
			}
			mChildrenToUpdate.clear();
		}
		else
		{
			// Just update selected children

			ChildUpdateSet::iterator it, itend;
			itend = mChildrenToUpdate.end();
			for(it = mChildrenToUpdate.begin(); it != itend; ++it)
			{
				DNode* child = *it;
				child->update(true, false);
			}

			mChildrenToUpdate.clear();
		}

		mbNeedChildUpdate = false;

	}

	void DNode::updateFromParent( void )
	{
		updateFromParentImpl();
		signalUpdated(this);
	}

	void DNode::updateFromParentImpl( void )
	{
		if (mParent)
		{

			// Update orientation
			const DQuaternion& parentOrientation = mParent->getInheritedOrient();
			if (mbOrientInherited)
			{
				// Combine orientation with that of parent
				mInheritedOrient = parentOrientation * mRelativeOrient;
			}
			else
			{
				// No inheritence
				mInheritedOrient = mRelativeOrient;
			}

			// Update scale
			const DVector3& parentScale = mParent->getInheritedScale();
			if (mbScaleInherited)
			{
				// Scale own position by parent scale, NB just combine
				// as equivalent axes, no shearing
				mInheritedScale = parentScale * mRelativeScale;
			}
			else
			{
				// No inheritence
				mInheritedScale = mRelativeScale;
			}

			// Change position vector based on parent's orientation & scale
			mInheritedPos = parentOrientation * (parentScale * mRelativePos);

			// Add altered position vector to parents
			mInheritedPos += mParent->getInheritedPosition();
		}
		else
		{
			// Root node, no parent
			mInheritedOrient = mRelativeOrient;
			mInheritedScale = mRelativeScale;
			mInheritedPos = mRelativePos;
		}

		mbInheritedTransformOutOfDate = true;
		mbNeedParentUpdate = false;
	}

	DNode* DNode::getRootNode()
	{
		DNode* r = getParent();
		while (r != NULL)
		{
			r = r->getParent();
		}
		return r;
	}

}