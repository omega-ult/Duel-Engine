//  [10/22/2013 OMEGA] created


#ifndef _DUELMPDMEXPORTER_H_
#define _DUELMPDMEXPORTER_H_

#include "DuelMPMayaIncludes.h"
#include "DuelMPMeshData.h"
#include "DuelMPExporterParameters.h"
#include <fstream>

using std::string;
using std::fstream;

// copy from DuelDMCodec.h.
// dm = duel mesh, it contains an Entity, not just a mesh data
// .dm file format are designed as chunk-based(for efficency), each chunk 
// has an ID, a LENGTH, and a DATA, looks like:
// chunk(n){
//		header {
//			uint32	ID;
//			uint32	LENGTH(with this chunk header excluded, never bigger than 4gb);
//		}	// the length indicate the data size.
//		void*	DATA; // if the length is 0, here the pointer does noe exist
// }
// chunk ID is defined below
// void* data format is described in comment.
enum DMChunkID
{
	DM_Header		= 0xABAB,			// magic number indicate this file is .dm format.
		// (char*)version			;  this part should contain a version description
		DM_Mesh			= 0x4000,			// mesh chunk.
			// (char*)name	
			DM_SubMesh		= 0x4100,		// sub-mesh chunk, repeatly.
				// (char*)name
				DM_Index	= 0x4210,	// index part.
					// (uint32)IndexType		// 
					// (uint32)IndexCount
					DM_IndexBuffer		= 0x4211,	// buffer type is decided by IndexType
						// (uint16*/uint32*)Data
				DM_VertexDeclaration	= 0X4220,	// declaration part, repeatly
					// (uint16)source
					// (uint32)offset
					// (uint32)VertexElementType
					// (uint32)VertexElementSemantic
					// (uint16)index	// texcoord/uv set index
				DM_Vertex		= 0x4230,	// vertices part, repeatly
					// (uint16)source		// bingding source.
					// (uint32)vertexSize
					// (uint32)vertexCount
					DM_VertexBuffer	= 0x4231,
						// (void*)buffer
				DM_TargetSubSkeleton	= 0x4300,	// optionnal, 
					// (uint32)boneInfluence
					// (char*)subSkeletonName
					DM_BoneIndexAdapter	= 0x4310,	// repeatly, required if DM_TargetSubSkeleton specified.
						// (uint16)indexInSubMesh
						// (char*)boneName
				DM_Material = 0x4410,		// optioanl, used to record material parameter
					// (char*)materialName		// name of the material from MaterialSystem.
					DM_MaterialParameter = 0x4411,	// optional, repeately
						// (uint32)paramNameLength	// string length of paramName
						// (char*)paramName
						// (uint32)paramTypeLength	// string length of paramType
						// (char*)paramType
						// (uint32)paramValueLength	// string length of paramValue
						// (char*)paramValue
				DM_UVSet	= 0x4500,	// optional, repeatly.
					// (uint32)inputIndex,	// coresponding to n in vertex declaration's texture[n] 
					// (uint32)uvSetNameLength
					// (char*)uvSetName,
					DM_BindedTextures	= 0x4501,	// optional, repeatly,  DM_UVSet required.
						// (uint32)uvSetNameLenght
						// (char*)uvSetName
						// (uint32)textureNameLength
						// (char*)textureName
		DM_Skeleton		= 0x5000,	// optional, repeatly
			// (char*)skeletonName
		DM_Animation	= 0x6000,	// optional, repeatly
			// (float)length
			// (char*)name
			DM_AnimationTrack	= 0x6100,
				// (char*)targetSubMeshName	
				DM_AnimationMorphKeyframe		= 0x6110,	// repeatly by the count of the corresponding submesh's vertices
					// (float)timePos
					// (uint32)vertexCount
				DM_AnimationMorphKeyframeBuffer		= 0x6111, // only float position value x, y, z existed.
					// (void*)buffer
				DM_AnimationMorphKeyframeNormBuffer		= 0x6112, // optional, float position value x, y, z.
					// (void*)buffer
	DM_FORCEUINT = 0xffffffffUL	// never used.
};


// copy from : DuelVertexDeclaration
// Vertex element semantics, used to identify 
// the meaning of vertex buffer contents
enum VertexElementSemantic
{
	// position, using 3 Reals per vertex
	VES_Position		= 0,
	// blending weight
	VES_BlendWeight		= 1,
	// blending indices
	VES_BlendIndices	= 2,
	// normal, using 3 Reals per vertex
	VES_Normal			= 3,
	// diffuse color
	VES_Diffuse			= 4,
	// specular color
	VES_Specular		= 5,
	// texture coordinates, support up to 7 texcoords(with index in DVertexElement)
	VES_TexCoord		= 6,
	// binormal(Y axis if normal is Z)
	VES_Binormal		= 14,
	// tangent (X axis if normal is Z)
	VES_Tangent			= 15
};

// copy from : DuelVertexDeclaration
// Vertex element type, used to identify the base types of the vertex contents
// notice that there are more available types in specified render system, but
// these types are enough for us.
enum VertexElementType
{
	/// D3D style compact colour	// byte for each chanel.
	VET_ARGB	= 0,
	/// GL style compact colour
	VET_ABGR	= 1,
	VET_Byte4	= 2,
	VET_UByte4	= 3,
	VET_Float1	= 4,
	VET_Float2	= 5,
	VET_Float3	= 6,
	VET_Float4	= 7,
	VET_Short1	= 8,
	VET_Short2	= 9,
	VET_Short3	= 10,
	VET_Short4	= 11,
	VET_UShort1	= 12,
	VET_UShort2	= 13,
	VET_UShort3	= 14,
	VET_UShort4	= 15,
	VET_Int1	= 16,
	VET_Int2	= 17,
	VET_Int3	= 18,
	VET_Int4	= 19,
	VET_UInt1	= 20,
	VET_UInt2	= 21,
	VET_UInt3	= 22,
	VET_UInt4	= 23
};

// copy from DuelIndexBuffer
enum IndexType
{
	IT_16Bit = 0,
	IT_32Bit = 1
};

class DMPDMExporter
{
public:
	DMPDMExporter() {}

	void		exportMesh(DMPParameters* param);

protected:
	// create a file, return result of creation.
	// you need to call this method and get "true" result before writing mesh data.
	bool		createFile(const string& fullPath);

	// try fill mesh with a dag path.
	void		fillMesh(DMPParameters* param);

	void		traverseSubMesh(DMPParameters* param, const MDagPath& dagPath);
	void		fillSubMesh(DMPParameters* param, const MDagPath& subMeshDag);
	void		fillJointMap(DMPMeshData::SubMeshStruct* subMesh, MFnIkJoint& fnJoint, MDagPath& dagPath);

	void		writeToFile();

	DMPMeshData	mMeshData;
	fstream		mFstream;
};


#endif