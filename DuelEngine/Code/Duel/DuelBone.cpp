//  [11/2/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelException.h"
#include "DuelBone.h"


namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DBone, DNode);

	DBone::DBone( uint16 handle, DSubSkeleton* creator ) :
		mHandle(handle),
		mCreator(creator)
	{
		setDefaultProperties(DVector3::ZERO, DVector3::UNIT_SCALE, DQuaternion::IDENTITY);
	}

	DBone::DBone( uint16 handle, const DString& name, DSubSkeleton* creator ) :
		DNode(name),
		mHandle(handle),
		mCreator(creator)
	{
		setDefaultProperties(DVector3::ZERO, DVector3::UNIT_SCALE, DQuaternion::IDENTITY);
	}

	void DBone::addChild( DNode* child )
	{
		if (!(DUEL_IS_EXACT_KIND(DBone, child)))
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
				"Cannot accept child which is not a DBone",
				"Duel::DBone::addChild")
		}
		DNode::addChild(child);
	}

	DBone* DBone::createChild( uint16 handle )
	{
		DBone* b = mCreator->createBone(handle);
		b->setParent(this);
		mChildren.insert(b);
		signalAttached(this, b);
		return b;
	}

	DBone* DBone::createChild( uint16 handle, const DString& name )
	{
		DBone* b = mCreator->createBone(handle, name);
		b->setParent(this);
		mChildren.insert(b);
		signalAttached(this, b);
		return b;
	}

	void DBone::setDefaultProperties( const DVector3& dPos, const DVector3& dScale, const DQuaternion& dOrient )
	{
		mDefaultPos = dPos;
		mDefaultScale = dScale;
		mDefaultOrient = dOrient;
	}

	void DBone::reset()
	{
		mRelativePos = mDefaultPos;
		mRelativeScale = mDefaultScale;
		mRelativeOrient = mDefaultOrient;
	}

	void DBone::setParent( DBone* p )
	{
		DNode::setParent(static_cast<DNode*>(p));
	}

// 	void DBone::copyToNode( DNode* destNode )
// 	{
// 		assert(DUEL_IS_EXACT_KIND(DBone, destNode));
// 		DBone* destBone= (DBone*)destNode;
// 		destBone->mCreator = mCreator;
// 		//destBone->mHandle = mHandle;
// 		destBone->mDefaultPos = mDefaultPos;
// 		destBone->mDefaultScale = mDefaultScale;
// 		destBone->mDefaultOrient = mDefaultOrient;
// 		DNode::copyToNode(destNode);
// 	}

	DNode* DBone::createChildImpl()
	{
		return mCreator->createBone();
	}

	DNode* DBone::createChildImpl( const DString& name )
	{
		return mCreator->createBone(name);
	}

}

