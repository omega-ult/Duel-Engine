//  [12/24/2012 OMEGA] created

#ifndef _DUELDMCODEC_H_
#define _DUELDMCODEC_H_

#include "DuelCommon.h"
#include "DuelCodec.h"

namespace Duel
{
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
						// (uint32)IndexType
						// (uint32)IndexCount
						DM_IndexBuffer		= 0x4211,	// buffer type is decided by IndexType
							// (uint16*/uint32*)Data
					DM_VertexDeclaration	= 0X4220,	// declaration part, repeatly
						// (uint16)source
						// (uint32)offset
						// (uint32)VertexElementType
						// (uint32)VertexElementSemantic
						// (uint16)index
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
					DM_UVSet	= 0x4500,	// optional, repeatly
						// (uint32)inputIndex,	// coresponding to n in vertex declaration's texture[n] 
						// (uint32)uvSetNameLength
						// (char*)uvSetName,
						DM_BindedTextures	= 0x4501,	// optional, repeatly,  DM_UVSet required.
							// (uint32)uvSetNameLenght
							// (char*)uvSetName
							// (uint32)textureNameLength
							// (char*)textureName
			DM_Skeleton		= 0x5000,	// optional
				// (char*)skeletonName
			DM_Animation	= 0x6000,	// optional, repeatly
				// (float)length
				// (char*)name
				DM_AnimationTrack	= 0x6100,
					// (char*)targetSubMeshName	
					DM_AnimationMorphKeyframe		= 0x6110,	// repeatly by the count of the corresponding submesh's vertices
						// (float)timePos
						// (uint32)vertexCount
						DM_AnimationMorphKeyframePosBuffer		= 0x6111, // only float position value x, y, z existed.
							// (void*)buffer
						DM_AnimationMorphKeyframeNormBuffer		= 0x6112, // optional, float position value x, y, z.
							// (void*)buffer
		DM_FORCEUINT = 0xffffffffUL	// never used.
	};


	// DMCodec, used for .dm file. version i '1.0'
	class DUEL_API DMCodec_v1_0 : public DResourceCodec
	{
	DUEL_DECLARE_RTTI(DMCodec_v1_0);
	public:
		DMCodec_v1_0() {}

		// override:DMeshCodec-------------------------------------
		void			decode(DDataStream* input, DResource* targetMesh);
		// override:DMeshCodec-------------------------------------
		DDataStreamPtr	encode(DResource* targetMesh);
		// override:DMeshCodec-------------------------------------
		void			encodeToFile(const DString& fileName, DResource* targetMesh);

		CodecType		getCodecType() { return CT_Mesh; }

		struct ChunkHeader
		{
			DMChunkID	id;
			uint32		length;
		};
		// peek the next header's id, this method will not affect the streams' pos
		uint32			peekHeader(DDataStream* stream);
		// internal method used to read string.
		DString			readString(DDataStream* stream, uint32 length);
		// internal method used to read header.
		void			readHeader(DDataStream* stream, ChunkHeader& header);
		// internal method used to read mesh
		void			readMesh(DDataStream* stream, DMesh* mesh);
		// internal method used to read submesh
		void			readSubMesh(DDataStream* stream, DMesh* mesh);
		// internal method used to read animations
		void			readAnimation(DDataStream* stream, DMesh* mesh);

		// internal method used to write header
		void			writeHeader(DDataStream* stream, DMChunkID chunkID, size_t chunkLenght);
		// internal method used to write string
		void			writeString(DDataStream* stream, const DString& str);
		// internal method used to write DMesh
		void			writeMesh(DDataStream* stream, DMesh* mesh);
		// internal method used to write submesh
		void			writeSubMesh(DDataStream* stream, DMesh* mesh);
		// internal method used to write skeleton
		void			writeSkeleton(DDataStream* stream, DMesh* mesh);
		// internal method used to write animations.
		void			writeAnimation(DDataStream* stream, DMesh* mesh);
	};

}
#endif