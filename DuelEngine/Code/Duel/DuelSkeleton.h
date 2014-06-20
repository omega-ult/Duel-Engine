//  [11/2/2012 OMEGA] created

#ifndef _DUELSKELETON_H_
#define _DUELSKELETON_H_

#include "DuelCommon.h"
#include "DuelAnimation.h"
#include "DuelBone.h"
#include "DuelResource.h"

namespace Duel
{
	// the skeleton used in the game play is object-related, meaning that each
	// object owns a skeleton copy to perform animations, this feature is done
	// by using skeleton instance, which can be created by the skeleton resource.
	// each skeleton instance maps to a sub-skeleton, which has a bone tree with only
	// one root bone, and it can be animated using mapped sub-skeleton's animation.
	class DUEL_API DSkeletonInstance : public DObject
	{
		DUEL_DECLARE_RTTI(DSkeletonInstance)
	public:


	protected:
		DBone*		mRootBone;
	};

	class DUEL_API DSkeleton : public DResource
	{
	DUEL_DECLARE_RTTI(DSkeleton)
	public:
		// this class will do encode/decode work for Mesh class, so full access is
		// necessary
		friend	class	DSCodec_v1_0;	
		DSkeleton(DResourceManager* fatherManager, const DString& name, const DString& groupName);
		virtual ~DSkeleton();

		//////////////////////////////////////////////////////////////////////////
		// [10/24/2013 OMEGA]
		// 施工中.
		typedef	HashMap<DString, DSubSkeletonPtr>	SubSkeletonMap;
		typedef	MapIterator<SubSkeletonMap>			SubSkeletonIterator;
		SubSkeletonIterator	getSubSkeletonIterator() { return SubSkeletonIterator(mSubSkeletonMap.begin(), mSubSkeletonMap.end()); }
		// if specified skeleton existed, it will be returned.
		DSubSkeletonPtr	createSubSkeleton(const DString& name);
		bool			hasSubSkeleton(const DString& name);
		// if specified SubSkeleton does not exist, NULL will be returned.
		DSubSkeletonPtr	getSubSkeleton(const DString& name);
		void			removeSubSkeleton(const DString& name);
		void			removeAllSubskeletons();
		//////////////////////////////////////////////////////////////////////////

		// encode this resource into a DataStream
		DDataStreamPtr		encode();
		// save this resource into a file;
		void				saveToFile(const DString& fileName);

		// only loaded skeleton can create a valid instance,
		DSkeletonInstancePtr	createSkeltonInstance(const DString& subSkelName);


	protected:
		// override:DResource----------------------------------------------
		void			loadImpl() ;
		// override:DResource----------------------------------------------
		void			unloadImpl();
		// override:DResource----------------------------------------------
		size_t			calculateSize();

// 		// generate a handle
// 		uint16			generateHandle();

// 		// auto generate handle;
// 		uint16			mHandleGen;
// 
// 		// bones storage.
// 		BoneMap			mBoneMap;
// 
// 		// name generator
// 		DNameGenerator	mNameGen;
// 
// 		// the root bone of the skeleton.
// 		DBone*			mRootBone;
// 
// 		// animation for this skeleton
// 		AnimationMap	mAnimationMap;

		//////////////////////////////////////////////////////////////////////////
		//  [10/24/2013 OMEGA] 施工中
		SubSkeletonMap	mSubSkeletonMap;

	};
}

#endif