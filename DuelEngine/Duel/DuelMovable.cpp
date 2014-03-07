//  [12/4/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelSceneNode.h"
#include "DuelMovable.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DMovable, DObject);


	DMovable::DMovable(const DString& name ) :
		mName(name)
	{

	}

	DMovable::~DMovable()
	{
		if (mAttachedNode && DUEL_IS_KIND(DSceneNode, mAttachedNode))
		{
			static_cast<DSceneNode*>(mAttachedNode)->detachMovable(this);
			signalDetached(this);
		}
		signalDestroyed(this);
	}

	void DMovable::attachToNode( DNode* parent )
	{
		if (parent == NULL)
		{
			signalDetached(this);
			mAttachedNode = parent;
		}
		else if (parent != mAttachedNode)
		{
			signalDetached(this);
			mAttachedNode = parent;
			signalAttached(this);
		}
	}



}