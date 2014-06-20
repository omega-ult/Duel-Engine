//  [10/24/2013 OMEGA] created

#ifndef _DUELSUBSKELETON_H_
#define _DUELSUBSKELETON_H_


#include "DuelCommon.h"


namespace Duel
{

	// notice a SubSkeleton is a complete skeleton with one root.
	class DUEL_API DSubSkeleton : public DObject
	{
		DUEL_DECLARE_RTTI(DSubSkeleton)
	public:
		// commonly, the name will be identical to the root bone, even it is not required.
		DSubSkeleton(DSkeleton* parent, const DString& name);

		// notice that the parent skeleton could be invalid if 
		// it was released.
		DSkeleton*		getParent() { return mParent; }
		const DString&	getName() { return mName; }

		// create a new bone, but do not attached it to current skeleton,
		virtual	DBone*	createBone();
		// create a bone with specified name, name should be unique.
		virtual	DBone*	createBone(const DString& name);
		// create bone with specified handle, notice that the handle must be unique. otherwise
		// the current bone will be returned.
		virtual	DBone*	createBone(uint16 handle);
		// if there was a bone with the same handle as parameter, it will be returned with new name.
		virtual DBone*	createBone(uint16 handle, const DString& name);

		// query the bone's existence 
		virtual	bool	hasBone(uint16 handle);
		// use name as key, slower than handle key.
		virtual	bool	hasBone(const DString& name);

		// destroy a bone with handle.
		virtual void	destroyBone(uint16 handle);
		// destroy a bone with name.
		virtual void	destroyBone(const DString&  name);
		// destroy all bones
		virtual	void	destroyAllBones();

		// used to traverse bone map.
		typedef	std::map<uint16, DBone*>	BoneMap;
		typedef	MapIterator<BoneMap>	BoneIterator;
		BoneIterator	getBoneIterator() { return BoneIterator(mBoneMap.begin(), mBoneMap.end()); }

		// query whether the bones in this subskeleton have the same root bone.
		bool			hasUniqueRootBone();

		// get bone with specified handle, if the bone does not exist. then it will return
		// a NULL
		virtual	DBone*	getBone(uint16 handle);
		// get a bone with specified name, if there were bones share
		// one name(try to avoid that), the first bone will be returned.
		virtual	DBone*	getBone(const DString& name);

		// get the count of bones.
		uint16			getBoneCount();

		// make the current pose as a default pose.
		virtual	void	snapshotPose();

		// reset to the default pose.
		virtual	void	resetPose();

		// make this data struct public.
		typedef HashMap<DString, DAnimation*>	AnimationMap;
		typedef	MapIterator<AnimationMap>	AnimationIterator;
		// used to traverse animations
		AnimationIterator	getAnimationIterator() { return AnimationIterator(mAnimationMap.begin(), mAnimationMap.end()); }

		// get the count of animations
		size_t			getAnimationCount() const;
		// create a new animation, notice that the name of the animation 
		// MUST be unique, if there existed an DAnimation with the same name,
		// the existing one will be returned.
		DAnimation*		createAnimation(const DString& name);
		// query whether the specified DAnimation has existed.
		bool			hasAnimation(const DString& name);
		// remove an DAnimation using specified name.
		void			destroyAnimation(const DString& name);
		// clear DAnimation list.
		void			destroyAllAnimations();
		
		// a struct that contains the information of animation affectiong target.
		// this information is build by DMCodec when loading the mesh file, so you
		// can only query the result.
		struct AnimationTarget
		{
			uint32	trackHandle;
			DString boneName;
		};
		typedef	std::vector<AnimationTarget>			AnimationTargetList;
		typedef	std::map<DString, AnimationTargetList>	AnimationTargetsMap;
		typedef	MapIterator<AnimationTargetsMap>		AnimationTargetsIterator;
		AnimationTargetsIterator	getAnimationTargetsIterator() { return AnimationTargetsIterator(mAnimTargetMap.begin(), mAnimTargetMap.end()); }
		
		// use specified animation name to query the animation target.
		AnimationTargetList		getAnimationTargets(const DString& animName);
		void				addAnimationTarget(const DString& animName, const AnimationTarget& target);
		bool				hasAnimationTarget(const DString& animName, const AnimationTarget& target);
		void				removeAnimationTarget(const DString& animName, const AnimationTarget& target);
		void				removeAllAnimationTargets(const DString& animName, const AnimationTarget& target);


	protected:
		// generate a handle
		uint16			generateHandle();


		DSkeleton*		mParent;
		DString			mName;
		// auto generate handle;
		uint16			mHandleGen;

		// bones storage.
		BoneMap			mBoneMap;

		// name generator
		DNameGenerator	mNameGen;

		// animation for this skeleton
		AnimationMap	mAnimationMap;
		AnimationTargetsMap	mAnimTargetMap;

	};


}
#endif