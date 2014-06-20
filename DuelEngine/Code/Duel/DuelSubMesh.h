//  [11/1/2012 OMEGA] created


#ifndef _DUELSUBMESH_H_
#define _DUELSUBMESH_H_

#include "DuelCommon.h"
#include "DuelVertexData.h"
#include "DuelMesh.h"

namespace Duel
{

	// mapping an indexed vertex to specified index, with
	// its weight. notice the index is not the handle of the
	// target SubSkeleton.
	struct DUEL_API VertexBoneAttachment
	{
		uint32	vertexIndex;
		uint16	boneIndex;
		float	weight;
	};


	// this class describe a collection of vertices and its indices. 
	// 
	class DUEL_API DSubMesh : public DObject
	{
		DUEL_DECLARE_RTTI(DSubMesh)
	public:
		// each DSubMesh should have a unique name with in the mesh, and
		// each DSubMesh could only be created by the mesh.
		DSubMesh(DMesh* parent, const DString& name);
		virtual ~DSubMesh();
		
		// set/get name for this sub-mesh.
		const DString&			getName() const { return mName; }
		INTERNAL void			setName(const DString& name) { mName = name; }

		// set/get the parent mesh who created this.
		INTERNAL void			setParent(DMesh* parent) { mParent = parent; }
		DMesh*					getParent() { return mParent; }

		// used to traverse all binding textures.
		struct UVSet
		{
			UVSet() : UVaddress(0) {}
			DString	UVSetName;
			// uv address in the vertexdeclaration, only 1-8 are acceptable.
			uint32 UVaddress;
			// see setTexture(const DString& uvSet, const DString& name);
			DStringVector	textureNames;
		};
		// here the key is the UVSet
		typedef	std::map<DString, UVSet>	UVSetMap;
		typedef	MapIterator<UVSetMap>		UVSetIterator;
		UVSetIterator			getUVSetIterator() { return UVSetIterator(mUVSetMap ); }

		// add a uv set, parameter elem is the binding data source to the graphics pipeline.
		// if uvset with name uvSetName exist, this method equals to setUVSet(uvSetName, elem);
		void					addUVSet(const DString& uvSetName, uint32 elem);
		// set current uvSet, if specified uvSet does not exist, this method changes nothing.
		void					setUVSet(const DString& uvSetName, uint32 elem);
		void					removeUVSet(const DString& uvSetName);
		void					removeAllUVSets();
		// attach a associating texture to this sub-mesh, use uvSet name as the key
		// because render system has limitation on texture count, so the index should never
		// exceed this limitation.
		// the name of the texture should be specified as format "groupName -> textureName"
		// if the group name is not specified,
		// it use the same group as its parent to locate the render effect resource.
		void					addTexture(const DString& uvSet, const DString& name);
		// get the name of the texture, StringTool::BLANK may be returned if it is not existed.
		const DStringVector&	getTexture(const DString& uvSet);
		// detach a texture attached to the uvset.
		void					removeTexture(const DString& uvSet, const DString& name);
		// detach textures attached to the uvset.
		void					removeTextures(const DString& uvSet);
		// detach all texture.
		void					removeAllTextures();

		void					setVertexStreams(const DVertexStreams& vs) { mVertexStream = vs; }
		// the vetices bingding used to store vertex buffers
		const DVertexStreams&	getVertexStreams() { return mVertexStream; }

		// get/set indices
		DIndexBufferPtr			getIndices() { return mIndices; }
		void					setIndices(const DIndexBufferPtr& indices) { mIndices = indices; }

		// set/get vertex declaration of the buffer.
		DVertexDeclaration		getVertexDeclaration() { return mVBDeclaration; }
		void					setVertexDeclaration(const DVertexDeclaration vd) { mVBDeclaration = vd; }
		// TODO: 加入LOD列表,  让场景管理选择当前需要使用的 indexbuffer

		// the target SubSkeleton name is used under parent's Skeleton,
		// used in locating bones.
		void					setTargetSubSkeleton(const DString& name) { mTargetSubSkeletonName = name; }
		const DString&			getTargetSubSkeleton() { return mTargetSubSkeletonName; }

		// set how many bones affected per vertex.	
		// ah.... for now, I can only support 4 bones per vertex.
		void					setBoneInfluenceCount(uint8 count) { mBoneInfluenceCount = count;}
		uint8					getBoneInfluenceCount() const { return mBoneInfluenceCount; }

		// index bridge between submesh and SubSkeleton, use this struct
		// to match specified bone in the SubSkeleton;
		/*
			mapping:
				SubMesh	   |	SubSkeleton
			v1-->boneIndex--->boneName/boneHandle
		*/
		struct BoneInfo 
		{
			enum 
			{
				BI_InvalidBoneHandle	= 0xfffful
			};
			BoneInfo() : boneHandle(BI_InvalidBoneHandle) {}
			uint16	boneIndex;
			DString	boneName;	// use the name to query the bone.
			uint16	boneHandle;	// this value should not be referred, it is only used internally.
		};
		
		// use boneIndex to match the adapter.
		// 使用indexInSubMesh来索引SkeletonBoneIndexAdapter
		typedef	std::map<uint16, BoneInfo>		BoneAdapter;
		typedef MapIterator<BoneAdapter>		BoneAdapterIterator;
		BoneAdapterIterator		getBoneAdapterIterator() { return BoneAdapterIterator(mBoneAdapter.begin(), mBoneAdapter.end()); }

		// create a map relation between bone index in the submesh and the bone name in the subskeleton.
		void					adaptToBone(uint16 boneIndex, const DString& boneName);
		const DString&			getBoneName(uint16 boneIndex);
		void					clearBoneAdapter();

		// this method will fill the bone handle information from target skeleton.
		void					adaptToSkeleton(DSubSkeleton* skel);

		// 通向Material的接口....
		// 这部分得结合渲染管线的设计啊.....
		// defer stage/ forwad stage
		// materials for defer stage/forwad stage.
		// these materials will be created when loading.
		// 今天的决定是...使用一个material接口, 多个参数来应对不同的渲染环节. [2/4/2014 OMEGA]
		void					setMaterialInstance(DMaterialInstancePtr mtl) { mMaterial = mtl; }
		DMaterialInstancePtr	getMaterialInstance() { return mMaterial; }

	protected:
		
		// a pionter to its parent.
		DMesh*					mParent;
		// a unique name within the mesh
		DString					mName;

		// name for target SubSkeleton;
		DString					mTargetSubSkeletonName;

		// stores the textures that this sub-mesh needed in RenderEffect
		UVSetMap				mUVSetMap;

		// vertex data
		DVertexStreams			mVertexStream;
		// IndexBuffer
		DIndexBufferPtr			mIndices;
		// VertexDecration
		DVertexDeclaration		mVBDeclaration;

		uint8					mBoneInfluenceCount;
		BoneAdapter				mBoneAdapter;		

		DMaterialInstancePtr	mMaterial;

	private:
		// not allowed.
		DSubMesh() {}
	};
}

#endif