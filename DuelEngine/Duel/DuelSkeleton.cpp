//  [11/2/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelSkeleton.h"
#include "DuelException.h"
#include "DuelResourceGroupManager.h"
#include "DuelSubSkeleton.h"
#include "DuelBone.h"
#include "DuelAnimation.h"
#include "DuelAnimationTrack.h"
#include "DuelKeyFrame.h"
#include "DuelCodec.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DSkeleton, DResource);
	DUEL_IMPLEMENT_RTTI_1(DSkeletonInstance, DObject);
	

	DSkeleton::DSkeleton( DResourceManager* fatherManager, const DString& name, const DString& groupName ) :
		DResource(fatherManager, name, groupName, "Skeleton")
	{
		
	}

	DSkeleton::~DSkeleton()
	{

	}

	Duel::DSubSkeletonPtr DSkeleton::createSubSkeleton( const DString& name )
	{
		SubSkeletonMap::iterator i = mSubSkeletonMap.find(name);
		if (i != mSubSkeletonMap.end())
		{
			return i->second;
		}
		DSubSkeletonPtr sbSke = DSubSkeletonPtr(new DSubSkeleton(this, name));
		mSubSkeletonMap[name] = sbSke;
		return sbSke;
	}

	bool DSkeleton::hasSubSkeleton( const DString& name )
	{
		SubSkeletonMap::iterator i = mSubSkeletonMap.find(name);
		if (i != mSubSkeletonMap.end())
		{
			return true;
		}
		return false;
	}

	Duel::DSubSkeletonPtr DSkeleton::getSubSkeleton( const DString& name )
	{
		SubSkeletonMap::iterator i = mSubSkeletonMap.find(name);
		if (i != mSubSkeletonMap.end())
		{
			return i->second;
		}
		DSubSkeletonPtr ret;
		return ret;
	}

	void DSkeleton::removeSubSkeleton( const DString& name )
	{
		SubSkeletonMap::iterator i = mSubSkeletonMap.find(name);
		if (i != mSubSkeletonMap.end())
		{
			mSubSkeletonMap.erase(i);
		}
	}

	void DSkeleton::removeAllSubskeletons()
	{
		mSubSkeletonMap.clear();
	}
	DDataStreamPtr DSkeleton::encode()
	{
		if (mCodec)
		{
			return (mCodec)->encode(static_cast<DResource*>(this));
		}
		DDataStreamPtr ret;
		return ret;
	}

	void DSkeleton::saveToFile( const DString& fileName )
	{
		if (mCodec)
		{
			(mCodec)->encodeToFile(fileName, static_cast<DResource*>(this));
		}
	}


	void DSkeleton::loadImpl() 
	{
		if (mCodec != NULL)
		{
			mCodec->decode(mPreparedData.get(), (this));
		}
		else
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
				"No codec found",
				"Duel::DSkeleton::loadImpl");

		}
	}

	void DSkeleton::unloadImpl()
	{
		mExternSrc.reset();
		mPreparedData.reset();
		mSubSkeletonMap.clear();
	}

	size_t DSkeleton::calculateSize()
	{
		mSize = 0;
// 		AnimationIterator ai = getAnimationIterator();
// 		while(ai.hasMoreElements())
// 		{
// 			DAnimation::NodeTrackIterator ni = ai.current()->second->getNodeTrackIterator();
// 			while(ni.hasMoreElements())
// 			{
// 				mSize += sizeof(NodeAnimationTrack)*ni.current()->second->getKeyFrameCount();
// 				ni.moveNext();
// 			}
// 			ai.moveNext();
// 		}
// 		mSize += getBoneCount() * sizeof(DBone);
		return mSize;
	}

	DSkeletonInstancePtr DSkeleton::createSkeltonInstance( const DString& subSkelName )
	{
		return DSkeletonInstancePtr();
	}


}