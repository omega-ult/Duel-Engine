//  [10/29/2012 OMEGA] created

#ifndef _DUELMESH_H_
#define _DUELMESH_H_

#include "DuelCommon.h"
#include "DuelAnimation.h"
#include "DuelResource.h"

namespace Duel
{

	// DMesh is a 'static' object, meaning that it will not move or animate in the scene.
	// just a resource.
	class DUEL_API DMesh : public DResource
	{
	DUEL_DECLARE_RTTI(DMesh)
	public:
		// this class will do encode/decode work for DMesh object, so full access is
		// necessary
		DMesh(DResourceManager* fatherManager, const DString& name, const DString& groupName);
		virtual ~DMesh();

		// TODO:
		// void			encodeToFile(const DString& fileName);
		// mesh name is not the resource name, it should be(not required) identical to the file 
		// name without extension.
		const DString&		getMeshName() { return mMeshName; }
		void				setMeshName(const DString& name) { mMeshName = name; }
		// create a submesh. if the mesh existed, the current existing one will be
		// returned
		DSubMeshPtr			createSubMesh(const DString& name);
		// get the count of submeshs.
		size_t				getSubMeshCount() const;
		// get specified submesh with its index, if the submesh does not exist,
		// a NULL pointer will be returned.
		DSubMeshPtr			getSubMesh(uint32 index);
		// get specified submesh with its name, if the submesh does not exist,
		// a NULL pointer will be returned.
		DSubMeshPtr			getSubMesh(const DString& name);
		// remove a sub-mesh use index as key.
		void				removeSubMesh(uint32 index);
		// remove an existing SubMesh, 
		void				removeSubMesh(const DString& name);
		// remove all SubMeshes, just release SharedPtr.
		void				removeAllSubMeshes();
		// used to traverse sub-meshs
		typedef	std::vector<DSubMeshPtr>	SubMeshes;
		typedef	VectorIterator<SubMeshes>	SubMeshIterator;
		SubMeshIterator		getSubMeshIterator() { return SubMeshIterator(mSubMeshes.begin(), mSubMeshes.end()); }

		// query whether the mesh contains a skeleton.
		bool				hasSkeleton();
		// get the skeleton.
		const DString&		getSkeleton();
		// set the skeleton name, use "groupName/skelName" format to identify the resource.
		void				setSkeleton(const DString& name);

		// set true to use shadow buffer when loading this mesh, see VertexBuffer for more information about shadow buffer.
		// this flag will not affect loaded mesh.
		void				setShadowBufferEnable(bool flag);
		bool				isShadowBufferEnable() const;
		//////////////////////////////////////////////////////////////////////////
		// ¹¹½¨ÖÐ.
		//  [10/24/2013 OMEGA]
		// create a new animation, notice that the name of the animation 
		// MUST be unique, if there existed an DAnimation with the same name,
		// the existing one will be returned.
		DAnimation*			createAnimation(const DString& name);
		// query whether the specified DAnimation has existed.
		bool				hasAnimation(const DString& name);
		// remove an DAnimation using specified name.
		void				destroyAnimation(const DString& name);
		// clear DAnimation list.
		void				destroyAllAnimations();
		// make this data struct public.
		typedef std::map<DString, DAnimation*>	AnimationMap;
		typedef	MapIterator<AnimationMap>		AnimationIterator;
		// used to traverse animations
		AnimationIterator	getAnimationIterator() { return AnimationIterator(mAnimationMap.begin(), mAnimationMap.end()); }
		size_t				getAnimationCount() { return mAnimationMap.size(); }

		// a struct that contains the information of animation affectiong target.
		// this information is build by DMCodec when loading the mesh file, so you
		// can only query the result.
		struct AnimationTarget
		{
			uint32	trackHandle;
			DString	subMeshName;
		};
		typedef	std::vector<AnimationTarget>		AnimationTargetList;
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
		// override:DResource----------------------------------------------
		void				loadImpl() ;
		// override:DResource----------------------------------------------
		void				unloadImpl();
		// override:DResource----------------------------------------------
		size_t				calculateSize();
		
		// if use hardware skinning, this value must be set and should not bigger than 2.
		//size_t		mPerVertexBlendCount;

		DString				mMeshName;

		// pointer to the skeleton, if the mesh does have one.
		DString				mSkeletonName;

		bool				mbShadowBuf;

		// a storage for SubMeshes
		SubMeshes			mSubMeshes;
		// store animations.
		AnimationMap		mAnimationMap;

		AnimationTargetsMap	mAnimTargetMap;

	};
}

#endif