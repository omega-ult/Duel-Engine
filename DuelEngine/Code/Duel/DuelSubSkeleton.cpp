//  [10/24/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelSubSkeleton.h"
#include "DuelBone.h"
#include "DuelAnimation.h"
#include "DuelAnimationTrack.h"
#include "DuelKeyFrame.h"


namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DSubSkeleton, DObject);

	DSubSkeleton::DSubSkeleton( DSkeleton* parent, const DString& name ) :
		mParent(parent),
		mHandleGen(0),
		mName(name),
		mNameGen("unamed_bone_")
	{

	}

	DBone* DSubSkeleton::createBone()
	{
		return createBone(mNameGen.generate());
	}

	DBone* DSubSkeleton::createBone( const DString& name )
	{
		// used to prevent infinite loop.
		uint16 tempHandle = mHandleGen;
		uint16 newHandle;
		BoneMap::iterator i;
		do
		{
			newHandle = generateHandle();
			i = mBoneMap.find(newHandle);
		} while( i != mBoneMap.end() && tempHandle != newHandle);
		if (tempHandle == newHandle)
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InvalidStatus,
				"DBone count exceeded.",
				"Duel::DSubSkeleton::createBone")
		}
		return createBone(newHandle, name);
	}

	DBone* DSubSkeleton::createBone( uint16 handle )
	{
		BoneMap::iterator i = mBoneMap.find(handle);
		if (i != mBoneMap.end())
		{
			return i->second;
		}
		else
		{
			return createBone(handle, mNameGen.generate());
		}
	}

	DBone* DSubSkeleton::createBone( uint16 handle, const DString& name )
	{
		BoneMap::iterator i = mBoneMap.find(handle);
		if (i != mBoneMap.end())
		{
			i->second->setName(name);
			return i->second;
		}
		else
		{
			DBone* newB = new DBone(handle, name, this);
			mBoneMap[handle] = newB;
			return newB;
		}
	}

	bool DSubSkeleton::hasBone( uint16 handle )
	{
		BoneMap::iterator i = mBoneMap.find(handle);
		if (i != mBoneMap.end())
		{
			return true;
		}
		return false;
	}

	bool DSubSkeleton::hasBone( const DString& name )
	{
		BoneMap::iterator i, iend = mBoneMap.end();
		for (i = mBoneMap.begin(); i != iend; ++i)
		{
			if (i->second->getName() == name)
			{
				return true;
			}
		}
		return false;
	}

	void DSubSkeleton::destroyBone( uint16 handle )
	{
		BoneMap::iterator i = mBoneMap.find(handle);
		if (i != mBoneMap.end())
		{
			delete i->second;
		}
	}

	void DSubSkeleton::destroyBone( const DString& name )
	{
		BoneMap::iterator i, iend = mBoneMap.end();
		for (i = mBoneMap.begin(); i != iend; ++i)
		{
			if (i->second->getName() == name)
			{
				delete i->second;
				return;
			}
		}
	}

	void DSubSkeleton::destroyAllBones()
	{
		BoneMap::iterator i, iend = mBoneMap.end();
		for (i = mBoneMap.begin(); i != iend; ++i)
		{
			delete i->second;
		}
		mBoneMap.clear();
	}

	DBone* DSubSkeleton::getBone( uint16 handle )
	{
		BoneMap::iterator i = mBoneMap.find(handle);
		if (i != mBoneMap.end())
		{
			return i->second;
		}
		return NULL;
	}

	DBone* DSubSkeleton::getBone( const DString& name )
	{
		BoneMap::iterator i, iend = mBoneMap.end();
		for (i = mBoneMap.begin(); i != iend; ++i)
		{
			if (i->second->getName() == name)
			{
				return i->second;
			}
		}
		return NULL;
	}


	uint16 DSubSkeleton::getBoneCount()
	{
		return (uint16)mBoneMap.size();
	}

	void DSubSkeleton::snapshotPose()
	{
		BoneMap::iterator i, iend = mBoneMap.end();
		for (i = mBoneMap.begin(); i != iend; ++i)
		{
			i->second->setDefaultProperties(
				i->second->getRelativePosition(),
				i->second->getRelativeScale(),
				i->second->getRelativeOrientation());
		}
	}

	void DSubSkeleton::resetPose()
	{
		BoneMap::iterator i, iend = mBoneMap.end();
		for (i = mBoneMap.begin(); i != iend; ++i)
		{
			i->second->reset();
		}
	}

	size_t DSubSkeleton::getAnimationCount() const
	{
		return mAnimationMap.size();
	}


	DAnimation* DSubSkeleton::createAnimation( const DString& name )
	{
		AnimationMap::iterator i = mAnimationMap.find(name);
		if (i != mAnimationMap.end())
		{
			return i->second;
		}
		DAnimation* newAnim = new DAnimation(name);
		mAnimationMap[name] = newAnim;
		return newAnim;
	}

	bool DSubSkeleton::hasAnimation( const DString& name )
	{
		AnimationMap::iterator i = mAnimationMap.find(name);
		if (i != mAnimationMap.end())
		{
			return true;
		}
		return false;
	}

	void DSubSkeleton::destroyAnimation( const DString& name )
	{
		AnimationMap::iterator i = mAnimationMap.find(name);
		if (i != mAnimationMap.end())
		{
			delete i->second;
		}
	}

	void DSubSkeleton::destroyAllAnimations()
	{
		AnimationMap::iterator i, iend = mAnimationMap.end();
		for (i = mAnimationMap.begin(); i != iend; ++i)
		{
			delete i->second;
		}
		mAnimationMap.clear();
	}

	uint16 DSubSkeleton::generateHandle()
	{
		return mHandleGen++;
	}

	DSubSkeleton::AnimationTargetList DSubSkeleton::getAnimationTargets( const DString& animName )
	{
		AnimationTargetList empty;
		AnimationTargetsMap::iterator i = mAnimTargetMap.find(animName);
		if (i != mAnimTargetMap.end())
		{
			return i->second;
		}
		return empty;
	}

	void DSubSkeleton::addAnimationTarget( const DString& animName, const AnimationTarget& target )
	{
		AnimationTargetsMap::iterator i = mAnimTargetMap.find(animName);
		if (i != mAnimTargetMap.end())
		{
			AnimationTargetList& targetList = i->second;
			AnimationTargetList::iterator ti, tiend = targetList.end();
			for ( ti = targetList.begin(); ti != tiend; ++ti )
			{
				if ((ti)->boneName == target.boneName &&
					(ti)->trackHandle == target.trackHandle)
				{
					return; // find instance.
				}
			}
			targetList.push_back(target);
			return;
		}
		AnimationTargetList newlist;
		newlist.push_back(target);
		mAnimTargetMap[animName] = newlist;
	}

	bool DSubSkeleton::hasAnimationTarget( const DString& animName, const AnimationTarget& target )
	{
		AnimationTargetsMap::iterator i = mAnimTargetMap.find(animName);
		if (i != mAnimTargetMap.end())
		{
			AnimationTargetList& targetList = i->second;
			AnimationTargetList::iterator ti, tiend = targetList.end();
			for ( ti = targetList.begin(); ti != tiend; ++ti )
			{
				if ((ti)->boneName == target.boneName &&
					(ti)->trackHandle == target.trackHandle)
				{
					return true; // find instance.
				}
			}
		}
		return false;
	}

	void DSubSkeleton::removeAnimationTarget( const DString& animName, const AnimationTarget& target )
	{
		AnimationTargetsMap::iterator i = mAnimTargetMap.find(animName);
		if (i != mAnimTargetMap.end())
		{
			AnimationTargetList& targetList = i->second;
			AnimationTargetList::iterator ti, tiend = targetList.end();
			for ( ti = targetList.begin(); ti != tiend; ++ti )
			{
				if ((ti)->boneName == target.boneName &&
					(ti)->trackHandle == target.trackHandle)
				{
					targetList.erase(ti);
					return;
				}
			}
		}
	}

	void DSubSkeleton::removeAllAnimationTargets( const DString& animName, const AnimationTarget& target )
	{
		AnimationTargetsMap::iterator i = mAnimTargetMap.find(animName);
		if (i != mAnimTargetMap.end())
		{
			mAnimTargetMap.erase(i);
		}
	}

	bool DSubSkeleton::hasUniqueRootBone()
	{
		BoneIterator bi = getBoneIterator();
		uint32 rootCount = 0;
		while (bi.hasMoreElements())
		{
			DBone* b = bi.getNext();
			if(b->getParent() == NULL)
			{
				rootCount++;
			}
		}
		if (rootCount == 1)
		{
			return true;
		}
		return false;
	}


}