//  [11/1/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelSubMesh.h"
#include "DuelVertexData.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DSubMesh, DObject);

	DSubMesh::DSubMesh(DMesh* parent, const DString& name) :
		mParent(parent),
		mName(name)
	{
	}


	DSubMesh::~DSubMesh()
	{
	
	}



	void DSubMesh::adaptToBone( uint16 boneIndex, const DString& boneName )
	{
		BoneInfo info;
		info.boneIndex = boneIndex;
		info.boneName = boneName;
		mBoneAdapter[boneIndex] = info;
	}

	void DSubMesh::addUVSet( const DString& uvSetName, uint32 elem )
	{
		UVSet addr;
		addr.UVSetName = uvSetName;
		addr.UVaddress = elem;
		if (elem >= 0 && elem < 8) 
		{
			mUVSetMap[uvSetName] = addr; 
		}
	}

	void DSubMesh::setUVSet( const DString& uvSetName, uint32 elem )
	{
		UVSetMap::iterator i = mUVSetMap.find(uvSetName);
		if (i != mUVSetMap.end())
		{
			if (elem >= 0 && elem < 8)
			{
				i->second.UVaddress = elem;
			}
		}
	}

	void DSubMesh::removeUVSet( const DString& uvSetName )
	{
		UVSetMap::iterator i = mUVSetMap.find(uvSetName);
		if (i != mUVSetMap.end())
		{
			mUVSetMap.erase(i);
		}
	}

	void DSubMesh::removeAllUVSets()
	{
		mUVSetMap.clear();
	}

	void DSubMesh::addTexture( const DString& uvSet, const DString& name )
	{
		UVSetMap::iterator i = mUVSetMap.find(uvSet);
		if (i != mUVSetMap.end())
		{
			i->second.textureNames.push_back(name);
		}
	}

	const DStringVector& DSubMesh::getTexture( const DString& uvSet )
	{
		UVSetMap::iterator i = mUVSetMap.find(uvSet);
		if (i != mUVSetMap.end())
		{
			return i->second.textureNames;
		}
		// empty object.
		static const DStringVector ret;
		return ret;
	}

	void DSubMesh::removeTextures( const DString& uvSet )
	{
		UVSetMap::iterator i = mUVSetMap.find(uvSet);
		if (i != mUVSetMap.end())
		{
			i->second.textureNames.clear();
		}
	}

	void DSubMesh::removeTexture( const DString& uvSet, const DString& name )
	{
		UVSetMap::iterator i = mUVSetMap.find(uvSet);
		if (i != mUVSetMap.end())
		{
			DStringVector::iterator ni = std::find(	i->second.textureNames.begin(),
													i->second.textureNames.end(),
													name);
			if (ni != i->second.textureNames.end())
			{
				i->second.textureNames.erase(ni);
			}
		}
	}

	void DSubMesh::removeAllTextures()
	{
		UVSetMap::iterator i, iend = mUVSetMap.end();
		for (i = mUVSetMap.begin(); i != iend; ++i)
		{
			i->second.textureNames.clear();
		}
	}

	void DSubMesh::adaptToSkeleton( DSubSkeleton* skel )
	{
		if (skel == NULL)
		{
			return;
		}
	}

}