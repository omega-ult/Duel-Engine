//  [10/29/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelAnimation.h"
#include "DuelResource.h"
#include "DuelSkeletonManager.h"
#include "DuelMesh.h"
#include "DuelSubMesh.h"
#include "DuelDataStream.h"
#include "DuelResourceGroupManager.h"
#include "DuelAnimationTrack.h"
#include "DuelKeyFrame.h"
#include "DuelIndexBuffer.h"
#include "DuelVertexData.h"
#include "DuelCodec.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DMesh, DResource);

	DMesh::DMesh( DResourceManager* fatherManager, const DString& name, const DString& groupName ) :
		DResource(fatherManager, name, groupName, "Mesh"),
		mbShadowBuf(false)
	{

	}

	DMesh::~DMesh()
	{

	}

	DSubMeshPtr DMesh::createSubMesh( const DString& name )
	{
		SubMeshes::iterator i, iend = mSubMeshes.end();
		for (i = mSubMeshes.begin(); i != iend; ++i)
		{
			if ((*i)->getName() == name)
			{
				return (*i);
			}
		}
		DSubMesh* newSubMesh = new DSubMesh(this, name);
		DSubMeshPtr ret(newSubMesh);
		mSubMeshes.push_back(ret);
		return ret;
	}

	size_t DMesh::getSubMeshCount() const
	{
		return mSubMeshes.size();
	}

	DSubMeshPtr DMesh::getSubMesh( const DString& name )
	{
		SubMeshes::iterator i, iend = mSubMeshes.end();
		for (i = mSubMeshes.begin(); i != iend; ++i)
		{
			if ((*i)->getName() == name)
			{
				return (*i);
			}
		}
		DSubMeshPtr ret;
		return ret;
	}

	DSubMeshPtr DMesh::getSubMesh( uint32 index )
	{
		assert(index < mSubMeshes.size());
		if (index < mSubMeshes.size())
		{
			return mSubMeshes.at(index);
		}
		DSubMeshPtr ret;
		return ret;
	}

	void DMesh::removeSubMesh( uint32 index )
	{
		assert(index < mSubMeshes.size());
		SubMeshes::iterator i;
		if (index < mSubMeshes.size())
		{
			mSubMeshes.erase(i+index);
		}
	}

	void DMesh::removeSubMesh( const DString& name )
	{
		SubMeshes::iterator i, iend = mSubMeshes.end();
		for (i = mSubMeshes.begin(); i != iend; ++i)
		{
			if ((*i)->getName() == name)
			{
				mSubMeshes.erase(i);
				return;
			}
		}
	}

	void DMesh::removeAllSubMeshes()
	{
		mSubMeshes.clear();
	}

	bool DMesh::hasSkeleton()
	{
		return mSkeletonName != "";
	}

	const DString& DMesh::getSkeleton()
	{
		return mSkeletonName;
	}

	void DMesh::setSkeleton( const DString& name )
	{
		mSkeletonName = name;
	}

	DAnimation* DMesh::createAnimation( const DString& name )
	{
		AnimationMap::iterator i = mAnimationMap.find(name);
		if ( i != mAnimationMap.end())
		{
			return i->second;
		}
		DAnimation* newAnim = new DAnimation(name);
		mAnimationMap[name] = newAnim;
		return newAnim;
	}

	bool DMesh::hasAnimation( const DString& name )
	{
		AnimationMap::iterator i = mAnimationMap.find(name);
		if ( i != mAnimationMap.end())
		{
			return true;
		}
		return false;
	}

	void DMesh::destroyAnimation( const DString& name )
	{
		AnimationMap::iterator i = mAnimationMap.find(name);
		if ( i != mAnimationMap.end())
		{
			delete i->second;
			mAnimationMap.erase(i);
		}
	}

	void DMesh::destroyAllAnimations()
	{
		AnimationMap::iterator i, iend = mAnimationMap.end();
		for (i = mAnimationMap.begin(); i != iend; ++i)
		{
			delete i->second;
		}
		mAnimationMap.clear();
	}

	void DMesh::loadImpl() 
	{
		if (mCodec != NULL)
		{
			mCodec->decode(mPreparedData.get(), (this));
		}
		else
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
				"No codec found",
				"Duel::DMesh::loadImpl");

		}
	}

	void DMesh::unloadImpl()
	{
		mExternSrc.reset();
		mPreparedData.reset();
		removeAllSubMeshes();
		destroyAllAnimations();
	}

	size_t DMesh::calculateSize()
	{
		mSize = 0;
		SubMeshes::iterator i, iend = mSubMeshes.end();
		for (i = mSubMeshes.begin(); i != iend; ++i)
		{
			DVertexStreams vStream = (*i)->getVertexStreams();
			DVertexStreams::VertexStreamIterator bi = vStream.getVertexStreamIterator();
			while (bi.hasMoreElements())
			{
				mSize += bi.current()->second->getSize();
				bi.moveNext();
			}
			mSize += (*i)->getIndices()->getSize();
		}

		AnimationIterator ai = getAnimationIterator();
		while(ai.hasMoreElements())
		{
			DAnimation::MorphTrackIterator mi = ai.current()->second->getMorphTrackIterator();
			while(mi.hasMoreElements())
			{
				DAnimationTrack::KeyFrameIterator ki = mi.current()->second->getKeyFrameIterator();
				while(ki.hasMoreElements())
				{
					// assert the key frame is morph keyframe.
					if (DUEL_IS_EXACT_KIND(DMorphKeyFrame, *(ki.current())))
					{
						mSize += ((DMorphKeyFrame*)*(ki.current()))->getPositionBuffer()->getSize();
					}
					ki.moveNext();
				}
				mi.moveNext();
			}
			ai.moveNext();
		}

		return mSize;
	}

	DMesh::AnimationTargetList DMesh::getAnimationTargets( const DString& animName )
	{
		AnimationTargetList empty;
		AnimationTargetsMap::iterator i = mAnimTargetMap.find(animName);
		if (i != mAnimTargetMap.end())
		{
			return i->second;
		}
		return empty;
	}

	void DMesh::addAnimationTarget( const DString& animName, const AnimationTarget& target )
	{
		AnimationTargetsMap::iterator i = mAnimTargetMap.find(animName);
		if (i != mAnimTargetMap.end())
		{
			AnimationTargetList& targetList = i->second;
			AnimationTargetList::iterator ti, tiend = targetList.end();
			for ( ti = targetList.begin(); ti != tiend; ++ti )
			{
				if ((ti)->subMeshName == target.subMeshName &&
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

	bool DMesh::hasAnimationTarget( const DString& animName, const AnimationTarget& target )
	{
		AnimationTargetsMap::iterator i = mAnimTargetMap.find(animName);
		if (i != mAnimTargetMap.end())
		{
			AnimationTargetList& targetList = i->second;
			AnimationTargetList::iterator ti, tiend = targetList.end();
			for ( ti = targetList.begin(); ti != tiend; ++ti )
			{
				if ((ti)->subMeshName == target.subMeshName &&
					(ti)->trackHandle == target.trackHandle)
				{
					return true; // find instance.
				}
			}
		}
		return false;
	}

	void DMesh::removeAnimationTarget( const DString& animName, const AnimationTarget& target )
	{
		AnimationTargetsMap::iterator i = mAnimTargetMap.find(animName);
		if (i != mAnimTargetMap.end())
		{
			AnimationTargetList& targetList = i->second;
			AnimationTargetList::iterator ti, tiend = targetList.end();
			for ( ti = targetList.begin(); ti != tiend; ++ti )
			{
				if ((ti)->subMeshName == target.subMeshName &&
					(ti)->trackHandle == target.trackHandle)
				{
					targetList.erase(ti);
					return;
				}
			}
		}
	}

	void DMesh::removeAllAnimationTargets( const DString& animName, const AnimationTarget& target )
	{
		AnimationTargetsMap::iterator i = mAnimTargetMap.find(animName);
		if (i != mAnimTargetMap.end())
		{
			mAnimTargetMap.erase(i);
		}
	}

	void DMesh::setShadowBufferEnable( bool flag )
	{
		mbShadowBuf = flag;
	}

	bool DMesh::isShadowBufferEnable() const
	{
		return mbShadowBuf;
	}

}