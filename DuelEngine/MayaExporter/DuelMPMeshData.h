//  [10/24/2013 OMEGA] created

#ifndef _DUELMPMESHDATA_H_
#define _DUELMPMESHDATA_H_

#include "DuelMPDataTemplate.h"
// unsigned int to size_t
#pragma warning (disable : 4267)

using std::vector;
using std::map;
using std::pair;

class DMPMeshData
{
public:
	DMPMeshData() {}
	void	clear()
	{
		mesh.name = "";
		mesh.targetSkeleton = "";
		mesh.subMeshes.clear();
		mesh.animations.clear();
	}
	typedef	DataTemplate<double, 2>	Point2;
	typedef	DataTemplate<double, 3>	Point3;
	typedef	DataTemplate<double, 4>	Point4;
	typedef Point2 TexCoord;	// maya do not handle 3d texture, so we don't have to deal with w coordinate.

	// joint name : joint weight
	typedef pair<unsigned int, float> JointBinding;

	struct MorphKeyFrame
	{
		float	time;
		vector<Point3>		positions;
		vector<Point3>		normals;
	};
	
	struct MorphTrack
	{
		string targetSubMesh;
		vector<MorphKeyFrame>	frames;
	};

	struct MorphAnimation
	{
		string name;
		float  startTime;
		float  endTime;
		vector<MorphTrack>		tracks;
	};

	struct VertexStruct
	{
		Point3	position;
		Point3	normal;
		Point3	tangent;

		// uvset index : texcoord.
		map<int, TexCoord> texcoords;
		vector<JointBinding> jointBindings;
	};

	struct TriangleStruct
	{
		unsigned int vertexIndex[3];

		bool operator==(TriangleStruct const & rhs) const
		{
			return (vertexIndex[0] == rhs.vertexIndex[0])
				&& (vertexIndex[1] == rhs.vertexIndex[1])
				&& (vertexIndex[2] == rhs.vertexIndex[2]);
		}
	};

	struct SubMeshStruct
	{
		string name;
		string targetSubSkeleton;
		vector<VertexStruct> vertices;
		vector<TriangleStruct> triangles;
		bool	bHasTangent;
		typedef map<string, unsigned int>	UVSetMap;
		typedef map<string, vector<string>>	TextureMap;
		UVSetMap		UVSets;
		TextureMap		textureMap;
		typedef map<string, unsigned int> JointIndexMap;
		JointIndexMap	jointIndexMap;


		// use this function to retrieve unique index for specified joint
		unsigned int getJointIndex(const string& jointName)
		{
			JointIndexMap::iterator i = jointIndexMap.find(jointName);
			if (i == jointIndexMap.end())
			{
				unsigned int indx = jointIndexMap.size();
				jointIndexMap[jointName] = indx;
				return indx;
			}
			return i->second;
		}
		void	addUVSet(const string& uvSetName)
		{
			UVSetMap::iterator i = UVSets.find(uvSetName);
			if (i == UVSets.end())
			{
				UVSets[uvSetName] = UVSets.size();
			}
		}
		unsigned int	getUVSetIndex(const string& uvSetName)
		{
			UVSetMap::iterator i = UVSets.find(uvSetName);
			if (i != UVSets.end())
			{
				return i->second;
			}
			return 0;
		}
	};
	struct MeshStruct
	{
		string name;
		string targetSkeleton;
		vector<SubMeshStruct>	subMeshes;
		vector<MorphAnimation>	animations;
	};

	MeshStruct	mesh;
};

#endif // !_DUELMPDMDATASTORAGE_H_
