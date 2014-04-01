//  [12/4/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelSceneNode.h"
#include "DuelMovable.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DMovable, DObject);


	DMovable::DMovable(const DString& name ) :
		mName(name),
		mAttachedNode(NULL)
	{

	}

	DMovable::~DMovable()
	{
		if (mAttachedNode)
		{
			mAttachedNode->detachMovable(this);
			signalDetached(this);
		}
		signalDestroyed(this);
	}

	void DMovable::attachToNode( DSceneNode* parent )
	{
		if (parent == NULL)
		{
			signalDetached(this);
			mAttachedNode = NULL;
		}
		else if (parent != mAttachedNode)
		{
			signalDetached(this);
			mAttachedNode = parent;
			signalAttached(this);
		}
	}



}