//  [12/24/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelDMCodec.h"
#include "DuelBitTool.h"
#include "DuelVertexData.h"
#include "DuelMesh.h"
#include "DuelSubMesh.h"
#include "DuelDataStream.h"
#include "DuelLogManager.h"
#include "DuelException.h"
#include "DuelSkeletonManager.h"
#include "DuelCore.h"
#include "DuelRenderResourceManager.h"
#include "DuelAnimationTrack.h"
#include "DuelKeyFrame.h"
#include "DuelMaterialManager.h"
#include "DuelMaterial.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DMCodec_v1_0, DResourceCodec);

	MaterialParameterType parseParamType(const DString& typeName)
	{
		DString lowTypeName = typeName;
		DStringTool::toLowerCase(lowTypeName);
		if (lowTypeName == "color") return MPT_Color;
		if (lowTypeName == "texture") return MPT_Texture;
		if (lowTypeName == "int1") return MPT_Int1;
		if (lowTypeName == "int2") return MPT_Int2;
		if (lowTypeName == "int3") return MPT_Int3;
		if (lowTypeName == "int4") return MPT_Int4;
		if (lowTypeName == "float1") return MPT_Float1;
		if (lowTypeName == "float2") return MPT_Float2;
		if (lowTypeName == "float3") return MPT_Float3;
		if (lowTypeName == "float4") return MPT_Float4;
		if (lowTypeName == "matrix2x2") return MPT_Matrix2x2;
		if (lowTypeName == "matrix2x3") return MPT_Matrix2x3;
		if (lowTypeName == "matrix2x4") return MPT_Matrix2x4;
		if (lowTypeName == "matrix3x2") return MPT_Matrix3x2;
		if (lowTypeName == "matrix3x3") return MPT_Matrix3x3;
		if (lowTypeName == "matrix3x4") return MPT_Matrix3x4;
		if (lowTypeName == "matrix4x2") return MPT_Matrix4x2;
		if (lowTypeName == "matrix4x3") return MPT_Matrix4x3;
		if (lowTypeName == "matrix4x4") return MPT_Matrix4x4;
		return MPT_Color;
	}
	// utility function used for setting material parameters.
	void	setMaterialParameter(DMaterialInstancePtr mtl, const DString& paramName, const DString& paramType, const DString& paramVal)
	{
		MaterialParameterType ptype = parseParamType(paramType);
		switch (ptype)
		{
		case Duel::MPT_Color:
			{
				DVector4 val = DVector3::ZERO;
				DStringTool::parseVector4(paramVal, val);
				DColor c;
				c.r = val.x; c.g = val.y; c.b = val.z; c.a = val.w;
				mtl->setValue(paramName, c);
			}
			break;
		case Duel::MPT_Texture:
			{
				DStringVector vals = DStringTool::split(paramVal, "/");
				if (vals.size() == 2)
				{
					DMaterialInstance::TextureConstant c;
					c.first = vals[0]; c.second = vals[1];
					mtl->setValue(paramName, c);
				}
			}
			break;
		case Duel::MPT_Int1:
			{
				int32 c = 0;
				DStringTool::parseString(paramVal, c);
				mtl->setValue(paramName, c);
			}
			break;
		case Duel::MPT_Int2:
			{
				int32 c[2] = {0};
				DStringVector vals = DStringTool::split(paramVal, " ");
				if (vals.size() == 2)
				{
					DStringTool::parseString(vals[0], c[0]);
					DStringTool::parseString(vals[1], c[1]);
				}
				mtl->setValue(paramName, c, 2);
			}
			break;
		case Duel::MPT_Int3:
			{
				int32 c[3] = {0};
				DStringVector vals = DStringTool::split(paramVal, " ");
				if (vals.size() == 3)
				{
					DStringTool::parseString(vals[0], c[0]);
					DStringTool::parseString(vals[1], c[1]);
					DStringTool::parseString(vals[2], c[2]);
				}
				mtl->setValue(paramName, c, 3);
			}
			break;
		case Duel::MPT_Int4:
			{
				int32 c[4] = {0};
				DStringVector vals = DStringTool::split(paramVal, " ");
				if (vals.size() == 4)
				{
					DStringTool::parseString(vals[0], c[0]);
					DStringTool::parseString(vals[1], c[1]);
					DStringTool::parseString(vals[2], c[2]);
					DStringTool::parseString(vals[3], c[3]);
				}
				mtl->setValue(paramName, c, 4);
			}
			break;
		case Duel::MPT_Float1:
			{
				float c = 0;
				DStringTool::parseString(paramVal, c);
				mtl->setValue(paramName, c);
			}
			break;
		case Duel::MPT_Float2:
			{
				DVector2 val = DVector2::ZERO;
				DStringTool::parseVector2(paramVal, val);
				float c[2];
				c[0] = val.x; c[1] = val.y;
				mtl->setValue(paramName, c, 2);
			}
			break;
		case Duel::MPT_Float3:
			{
				DVector3 val = DVector3::ZERO;
				DStringTool::parseVector3(paramVal, val);
				float c[3];
				c[0] = val.x; c[1] = val.y; c[2] = val.z;
				mtl->setValue(paramName, c, 3);
			}
			break;
		case Duel::MPT_Float4:
			{
				DVector4 c = DVector3::ZERO;
				DStringTool::parseVector4(paramVal, c);
				mtl->setValue(paramName, c);
			}
			break;
		case Duel::MPT_Matrix2x2:
			{
				float c[4] = {0.0f};
				DStringVector vals = DStringTool::split(paramVal, " ", 4);
				if (vals.size() == 4)
				{
					for (uint32 i = 0; i < 4; ++i)
					{
						DStringTool::parseString(vals[i], c[i]);
					}
				}
				mtl->setValue(paramName, c, 4);
			}
			break;
		case Duel::MPT_Matrix2x3:
		case Duel::MPT_Matrix3x2:
			{
				float c[6] = {0.0f};
				DStringVector vals = DStringTool::split(paramVal, " ", 6);
				if (vals.size() == 6)
				{
					for (uint32 i = 0; i < 6; ++i)
					{
						DStringTool::parseString(vals[i], c[i]);
					}
				}
				mtl->setValue(paramName, c, 6);
			}
			break;
		case Duel::MPT_Matrix2x4:
		case Duel::MPT_Matrix4x2:
			{
				float c[8] = {0.0f};
				DStringVector vals = DStringTool::split(paramVal, " ", 8);
				if (vals.size() == 8)
				{
					for (uint32 i = 0; i < 8; ++i)
					{
						DStringTool::parseString(vals[i], c[i]);
					}
				}
				mtl->setValue(paramName, c, 8);
			}
			break;
		case Duel::MPT_Matrix3x3:
			{
				float c[9] = {0.0f};
				DStringVector vals = DStringTool::split(paramVal, " ", 9);
				if (vals.size() == 9)
				{
					for (uint32 i = 0; i < 9; ++i)
					{
						DStringTool::parseString(vals[i], c[i]);
					}
				}
				mtl->setValue(paramName, c, 9);
			}
			break;
		case Duel::MPT_Matrix3x4:
		case Duel::MPT_Matrix4x3:
			{
				float c[12] = {0.0f};
				DStringVector vals = DStringTool::split(paramVal, " ", 12);
				if (vals.size() == 12)
				{
					for (uint32 i = 0; i < 12; ++i)
					{
						DStringTool::parseString(vals[i], c[i]);
					}
				}
				mtl->setValue(paramName, c, 12);
			}
			break;
		case Duel::MPT_Matrix4x4:
			{
				float c[16] = {0.0f};
				DStringVector vals = DStringTool::split(paramVal, " ", 16);
				if (vals.size() == 16)
				{
					for (uint32 i = 0; i < 16; ++i)
					{
						DStringTool::parseString(vals[i], c[i]);
					}
				}
				mtl->setValue(paramName, c, 16);
			}
			break;
		default:
			break;
		}
	}

	// this method will parse open parameters of the material a vector, each element of the
	// vector will be [paramName][paramType][paramValue]
	std::vector<DStringVector>	convertParamToString(DMaterialInstancePtr mtl)
	{
		std::vector<DStringVector> ret;
		DMaterialInstance::ParameterIterator pi = mtl->getParameterIterator();
		while (pi.hasMoreElements())
		{
			DMaterialParameter& param = pi.getNext();
			if (param.bUserAccess && param.arraySize == 1) // only accept  parameter with arraysize 1.
			{
				DStringVector paramResult;
				paramResult.push_back(param.paramName);
				paramResult.push_back(DMaterialParameter::getParameterTypeName(param.paramType));
				DString paramVal;
				if (param.isFloat())
				{
					float* fPtr = mtl->getFloatValuePtr(param.physicalIndex);
					for (uint32 i = 0; i < param.elemSize; ++i)
					{
						paramVal += DStringTool::toString(*(fPtr+i));
						if (i + 1 < param.elemSize)
						{
							paramVal += " ";
						}
					}
				}
				else if (param.isInt())
				{
					int32* iPtr = mtl->getIntValuePtr(param.physicalIndex);
					for (uint32 i = 0; i < param.elemSize; ++i)
					{
						paramVal += DStringTool::toString(*(iPtr+i));
						if (i + 1 < param.elemSize)
						{
							paramVal += " ";
						}
					}
				}
				else if (param.isTexture())
				{
					DMaterialInstance::TextureConstant tex = mtl->getTextureValue(param.paramName);
					paramVal = tex.first + "/" + tex.second;
				}
				paramResult.push_back(paramVal);
				ret.push_back(paramResult);
			}
		}
		return ret;
	}

	void DMCodec_v1_0::decode( DDataStream* input, DResource* targetRes )
	{
		DMesh*	targetMesh = targetRes->getAs<DMesh>(false);
		if (targetMesh == NULL)
		{
			return;
		}
		if (peekHeader(input) == DM_Header)
		{
			// clear current mesh.
			targetMesh->removeAllSubMeshes();
			targetMesh->destroyAllAnimations();

			ChunkHeader	header;
			readHeader(input, header);
			DString version = readString(input, header.length);
			if (version == "1.0")
			{
				readMesh(input, targetMesh);
			}
		}		
	}


	DDataStreamPtr DMCodec_v1_0::encode( DResource* targetRes )
	{
		DMesh*	targetMesh = targetRes->getAs<DMesh>(false);
		if (targetMesh == NULL)
		{
			return DDataStreamPtr();
		}
		// calculate the total data size of the mesh, then we can create memory stream that can hold all data.
		size_t totalSize = 0;
		size_t headerSize = sizeof(ChunkHeader);
		// first, version size.
		DString version = "1.0";
		totalSize += headerSize + version.length();

		// calculate first mesh part's size
		totalSize += headerSize + targetMesh->getName().length();

		// calculate submesh's data size
		DMesh::SubMeshIterator sbi = targetMesh->getSubMeshIterator();
		while(sbi.hasMoreElements())
		{
 			// geometry part.
 			DSubMeshPtr	curSubMesh = sbi.getNext();
 			totalSize += headerSize + curSubMesh->getName().length(); // DM_SUBMESH


 			DIndexBufferPtr ib = curSubMesh->getIndices();
 			IndexType	indexType = ib->getType();
 			size_t indexCount = ib->getIndexCount();
 			size_t iBufSize = indexCount * (indexType == IT_16Bit ? 2 : 4);
 			totalSize += headerSize + sizeof(indexType) + sizeof(size_t); // DM_INDEX
 			totalSize += headerSize + iBufSize; // DM_INDEX_BUFFER
 
			DVertexDeclaration& vd = curSubMesh->getVertexDeclaration();
 			size_t vElemCount = vd.getElementCount();
 			totalSize += headerSize + vElemCount*(sizeof(uint16) * 2 + sizeof(uint32) * 2 + sizeof(size_t)); // DM_VERTEX_DECLARATION

			DVertexStreams vbding = curSubMesh->getVertexStreams();
			DVertexStreams::VertexStreamIterator vbdi = vbding.getVertexStreamIterator();
			while(vbdi.hasMoreElements())
			{
				totalSize += headerSize + sizeof(uint32)*2 + sizeof(uint16); // DM_VERTEX
				totalSize += headerSize + vbdi.current()->second->getVertexSize() * vbdi.current()->second->getVerticesCount(); // DM_VERTEX_BUFFER
				vbdi.moveNext();
			}
			// subskeleton
			if (curSubMesh->getTargetSubSkeleton() != DStringTool::BLANK)
			{
				totalSize += headerSize + sizeof(uint32) + curSubMesh->getTargetSubSkeleton().size();
				DSubMesh::BoneAdapterIterator bi = curSubMesh->getBoneAdapterIterator();
				while (bi.hasMoreElements())
				{
					DSubMesh::BoneInfo& ba = bi.getNext();
					totalSize += headerSize + sizeof(uint16) + ba.boneName.size();
				}
			}
			if (curSubMesh->getMaterialInstance() != NULL)
			{
				totalSize += headerSize + curSubMesh->getMaterialInstance()->getParent()->getMaterialName().size();
				std::vector<DStringVector>	mtlParam = convertParamToString(curSubMesh->getMaterialInstance());
				std::vector<DStringVector>::iterator i, iend = mtlParam.end();
				for (i = mtlParam.begin();  i != iend; ++i)
				{
					DStringVector& val = *i;
					totalSize += headerSize + sizeof(uint32) * 3 + val[0].size() + val[1].size() + val[2].size();
				}
			}
			DSubMesh::UVSetIterator uvi = curSubMesh->getUVSetIterator();
			while (uvi.hasMoreElements())
			{
				DSubMesh::UVSet& uvset = uvi.getNext();
				totalSize += headerSize + sizeof(uint32) * 2 + uvset.UVSetName.size();
			}
			uvi = curSubMesh->getUVSetIterator();
			while (uvi.hasMoreElements())
			{
				DSubMesh::UVSet& uvset = uvi.getNext();
				DStringVector& tex = uvset.textureNames;
				for (uint32 i = 0; i < tex.size(); ++i)
				{
					totalSize += headerSize + sizeof(uint32) + uvset.UVSetName.size() + sizeof(uint32) + tex[i].size();
				}
			}
		}
 
 		// skeleton data.
 		if (targetMesh->hasSkeleton())
 		{
 			totalSize += headerSize + sizeof(float) + targetMesh->getSkeleton().size();
 		}

		// animation data.
		DMesh::AnimationIterator ai = targetMesh->getAnimationIterator();
		while(ai.hasMoreElements())
		{
			DAnimation* curAnim = ai.current()->second;

			DAnimation::MorphTrackIterator mti = curAnim->getMorphTrackIterator();
			if (mti.hasMoreElements()) // only available when this animation contains tracks.
			{
				totalSize += headerSize + sizeof(float) + curAnim->getName().length(); // DM_ANIMATION
				while(mti.hasMoreElements())
				{
					totalSize += headerSize + sizeof(uint16); // DM_ANIMATION_TRACK
					DAnimationTrack::KeyFrameIterator kfi = mti.current()->second->getKeyFrameIterator();
					while (kfi.hasMoreElements())
					{
						totalSize += headerSize + sizeof(float) + sizeof(uint32); // DM_ANIMATION_MORPH_KEYFRAME
						DMorphKeyFrame* kf = static_cast<DMorphKeyFrame*>(*(kfi.current()));

						totalSize += headerSize + kf->getPositionBuffer()->getSize(); // DM_ANIMATION_MORPH_KEYFRAME_BUFFER
						if (kf->getNormalBuffer() != NULL)
						{
							totalSize += headerSize + kf->getNormalBuffer()->getSize();
						}
						kfi.moveNext();
					}
					mti.moveNext();
				}
			}
			ai.moveNext();
		}
		DMemoryStream* targetStream = new DMemoryStream(totalSize, DA_Write);
		writeHeader(targetStream, DM_Header, version.length());
		writeString(targetStream, version);
		writeMesh(targetStream, targetMesh);
		return DDataStreamPtr(targetStream);
	}

	void DMCodec_v1_0::encodeToFile( const DString& fileName, DResource* targetRes )
	{
		DMesh*	targetMesh = targetRes->getAs<DMesh>(false);
		if (targetMesh == NULL)
		{
			return;
		}
		DFileStream* fs = new DFileStream(fileName, DA_Write);
		if (!fs->isValid())
		{
			return;
		}
		DString version = "1.0";
		writeHeader(fs, DM_Header, version.length());
		writeString(fs, version);

		writeMesh(fs, targetMesh);

		delete fs;
	}

	Duel::uint32 DMCodec_v1_0::peekHeader( DDataStream* stream )
	{
		uint32 oldPos = stream->tell();
		uint32 headerID = DM_FORCEUINT;
		stream->read(&headerID, sizeof(uint32));
		stream->seek(oldPos);
		return headerID;
	}

	DString DMCodec_v1_0::readString( DDataStream* stream, uint32 length )
	{
		DString ret;
		assert(length > 0 );
		// just a speed-up method.
		if (length < 128)
		{
			char buf[128];
			stream->read(buf, length);
			ret.assign(buf, length);
		}
		else
		{
			char* buf = new char[length];
			stream->read(buf, length);
			ret.assign(buf, length);
			delete buf;
		}
		return ret;
	}

	void DMCodec_v1_0::readHeader( DDataStream* stream, ChunkHeader& header )
	{
		if (stream->eof())
		{
			header.id = DM_FORCEUINT;
			header.length = 0;
		}
		stream->read(&header.id, sizeof(uint32));
		stream->read(&header.length, sizeof(uint32));
	}


	void DMCodec_v1_0::readMesh( DDataStream* stream, DMesh* mesh )
	{
		if (peekHeader(stream) == DM_Mesh)
		{
			ChunkHeader header;
			readHeader(stream, header);
			DString name = readString(stream, header.length);
			mesh->setMeshName(name);
			while (peekHeader(stream) == DM_SubMesh)
			{
				readSubMesh(stream, mesh);
			}
			if (peekHeader(stream) == DM_Skeleton)
			{
				readHeader(stream, header);
				DString skelName = readString(stream, header.length);
				mesh->setSkeleton(mesh->getGroupName() + "/" + skelName);
			}
			while (peekHeader(stream) == DM_Animation)
			{
				readAnimation(stream, mesh);
			}
		}
	}

	void DMCodec_v1_0::readSubMesh( DDataStream* stream, DMesh* mesh )
	{
		if (peekHeader(stream) == DM_SubMesh)
		{
			ChunkHeader header;
			readHeader(stream, header);
			DString subMeshName = readString(stream, header.length);
			DSubMeshPtr sbMesh = mesh->createSubMesh(subMeshName);
			sbMesh->setName(subMeshName);
			DRenderResourceManager* rm = DRenderResourceManager::getSingletonPtr();
			
			if (peekHeader(stream) == DM_Index)
			{
				readHeader(stream, header);
				uint32	indexType;
				size_t	indexCount;
				stream->read(&indexType, sizeof(indexType));
				stream->read(&indexCount, sizeof(indexCount));

				if (peekHeader(stream) == DM_IndexBuffer)
				{
					readHeader(stream, header);
					DIndexBufferPtr iBuf;
					switch (indexType)
					{
					case IT_16Bit:
						iBuf = rm->createIndexBuffer(IT_16Bit, indexCount, HBU_Static, mesh->isShadowBufferEnable());
						break;
					case IT_32Bit:
						iBuf = rm->createIndexBuffer(IT_32Bit, indexCount, HBU_Static,  mesh->isShadowBufferEnable());
						break;
					default:
						DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
							"Encounter an unknown index buffer type: " + mesh->getGroupName() + "/" + mesh->getName(),
							"Duel::DMCodec::readSubMesh");
						break;
					}
					void* bufPtr = iBuf->lock(HBL_Discard);
					stream->read(bufPtr, header.length);
					iBuf->unlock();
					// set the index buffer anyway.
					sbMesh->setIndices(iBuf);
				}
				else
				{
					DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
						"No index buffer contained in the file:"  + mesh->getGroupName() + "/" + mesh->getName(),
						"Duel::DMCodec::readSubMesh");
				}
			}
			else
			{
				DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
					"No index data contained in the file: " + mesh->getGroupName() + "/" + mesh->getName(),
					"Duel::DMCodec::readSubMesh");
			}

			if (peekHeader(stream) == DM_VertexDeclaration)
			{
				uint16	src;
				uint32	offset;
				uint32	vtxElemType;
				uint32	vtxElemSemantic;
				uint16	index;
				
				DVertexDeclaration vd;
				while (peekHeader(stream) == DM_VertexDeclaration)
				{
					readHeader(stream, header);
					stream->read(&src, sizeof(src));
					stream->read(&offset, sizeof(offset));
					stream->read(&vtxElemType, sizeof(vtxElemType));
					stream->read(&vtxElemSemantic, sizeof(vtxElemSemantic));
					stream->read(&index, sizeof(index));
					// validate the semantic and elemtype.
					if (vtxElemType <= VET_UInt4 && vtxElemType >= VET_ARGB && vtxElemSemantic <= VES_Tangent && vtxElemSemantic>= VES_Position)
					{
						vd.addElement(src, offset, (VertexElementType)vtxElemType, (VertexElementSemantic)vtxElemSemantic, index);
					}
				}
				sbMesh->setVertexDeclaration(vd);
			}
			else
			{
				DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
					"No vertex declaration contained in the file: " + mesh->getGroupName() + "/" + mesh->getName(),
					"Duel::DMCodec::readSubMesh");
			}

			if (peekHeader(stream) == DM_Vertex)
			{
				uint16	source;
				uint32	vtxSize;
				uint32	vtxCount;
				DVertexStreams	vs;
				while (peekHeader(stream) == DM_Vertex)
				{
					readHeader(stream, header);
 					stream->read(&source, sizeof(source));
 					stream->read(&vtxSize, sizeof(vtxSize));
 					stream->read(&vtxCount, sizeof(vtxCount));

					DVertexBufferPtr vb = rm->createVetexBuffer(vtxSize, vtxCount, HBU_Dynamic, mesh->isShadowBufferEnable());
					readHeader(stream, header);
					if (header.id == DM_VertexBuffer)
					{
						void* bufPtr = vb->lock(HBL_Discard);
						stream->read(bufPtr, header.length);
						vb->unlock();
					}
					else
					{
						DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
							"No vertex data contained in the file: " + mesh->getGroupName() + "/" + mesh->getName(),
							"Duel::DMCodec::readSubMesh");
					}

					vs.setStream(source, vb);
				}
				sbMesh->setVertexStreams(vs);
			}
			else
			{
				DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
					"no vertex data contained in the file",
					"Duel::DMCodec::readSubMesh");
			}
			if (peekHeader(stream) == DM_TargetSubSkeleton)
			{
				readHeader(stream, header);
				uint32 boneInfluence;
				stream->read(&boneInfluence, sizeof(boneInfluence));
				DString targetSubSkel = readString(stream, header.length - sizeof(boneInfluence));
				sbMesh->setBoneInfluenceCount(boneInfluence);
				sbMesh->setTargetSubSkeleton(targetSubSkel);
				if (peekHeader(stream) == DM_BoneIndexAdapter)
				{
					while (peekHeader(stream) == DM_BoneIndexAdapter)
					{
						readHeader(stream, header);
						uint16 idxInSubmesh;
						stream->read(&idxInSubmesh, sizeof(idxInSubmesh));
						DString boneName = readString(stream, header.length - sizeof(idxInSubmesh));
						sbMesh->adaptToBone(idxInSubmesh, boneName);
					}
				}
				else
				{
					DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
						"Skeleton information specified, but no bone information found: " + mesh->getGroupName() + "/" + mesh->getName(),
						"Duel::DMCodec::readSubMesh");
				}
			}
			if (peekHeader(stream) == DM_Material)
			{
				readHeader(stream, header);
				DString mtlName = readString(stream, header.length);
				DMaterialInstancePtr mtl = DMaterialManager::getSingleton().createMaterialInstance(mtlName);
				
				if (peekHeader(stream) == DM_MaterialParameter)
				{
					while (peekHeader(stream) == DM_MaterialParameter)
					{
						readHeader(stream, header);
						uint32 paramNameLength;
						stream->read(&paramNameLength, sizeof(paramNameLength));
						DString paramName = readString(stream, paramNameLength);
						uint32 paramTypeLength;
						stream->read(&paramTypeLength, sizeof(paramTypeLength));
						DString paramType = readString(stream, paramTypeLength);
						uint32 paramValueLength;
						stream->read(&paramValueLength, sizeof(paramValueLength));
						DString paramVal = readString(stream, paramValueLength);

						if (mtl != NULL)	// skip the parameter if the specified material has not been registered.
						{
							setMaterialParameter(mtl, paramName, paramType, paramVal);
						}
					}
				}
				sbMesh->setMaterialInstance(mtl);
			}
			if (peekHeader(stream) == DM_UVSet)
			{
				while (peekHeader(stream) == DM_UVSet)
				{
					readHeader(stream, header);
					uint32 inputIdx;
					uint32 uvSetNameLength;
					stream->read(&inputIdx, sizeof(inputIdx));
					stream->read(&uvSetNameLength, sizeof(uvSetNameLength));
					DString uvSetName = readString(stream, uvSetNameLength);
					sbMesh->addUVSet(uvSetName, inputIdx);
				}
			}
			if (peekHeader(stream) == DM_BindedTextures)
			{
				while (peekHeader(stream) == DM_BindedTextures)
				{
					readHeader(stream, header);
					uint32 uvSetNameLength;
					uint32 textureNameLength;
					stream->read(&uvSetNameLength, sizeof(uvSetNameLength));
					DString uvSetName = readString(stream, uvSetNameLength);
					stream->read(&textureNameLength, sizeof(textureNameLength));
					DString texName = readString(stream, textureNameLength);
					sbMesh->addTexture(uvSetName, texName);
				}
			}
		}
	}

	void DMCodec_v1_0::readAnimation( DDataStream* stream, DMesh* mesh )
	{
		ChunkHeader	header;
		DRenderResourceManager* rm = DRenderResourceManager::getSingletonPtr();
		if (peekHeader(stream) == DM_Animation)
		{
			while (peekHeader(stream) == DM_Animation)
			{
				readHeader(stream, header);
				float animLength;
				stream->read(&animLength, sizeof(animLength));
				DString animName = readString(stream, header.length - sizeof(animLength));
				DAnimation* anim = mesh->createAnimation(animName);
				anim->setLength(animLength);
				if (peekHeader(stream) == DM_AnimationTrack)
				{
					while (peekHeader(stream) == DM_AnimationTrack)
					{
						readHeader(stream, header);
						DString targetSubMesh = readString(stream, header.length);
						DMorphAnimationTrack* track = anim->createMorphTrack();
						DMesh::AnimationTarget animTarget;
						animTarget.subMeshName = targetSubMesh;
						animTarget.trackHandle = track->getHandle();
						mesh->addAnimationTarget(animName, animTarget);
						if (peekHeader(stream) == DM_AnimationMorphKeyframe)
						{
							while (peekHeader(stream) == DM_AnimationMorphKeyframe)
							{
								readHeader(stream, header);
								float timePos;
								uint32 vtxCount;
								stream->read(&timePos, sizeof(timePos));
								stream->read(&vtxCount, sizeof(vtxCount));
								DMorphKeyFrame* keyframe = track->createKeyFrame(timePos)->getAs<DMorphKeyFrame>();
								DRawBufferPtr posBuf = rm->createRawBuffer(vtxCount * sizeof(float) * 3);
								DRawBufferPtr normBuf = rm->createRawBuffer(vtxCount * sizeof(float) * 3);
								readHeader(stream, header);
								if (header.id == DM_AnimationMorphKeyframePosBuffer)
								{
									char* posBufPtr = (char*)posBuf->lock(HBL_Discard);
									stream->read(posBufPtr, header.length);
									posBuf->unlock();
								}
								else
								{
									DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
										"Morph keyframe's data not found: " + mesh->getGroupName() + "/" + 
											mesh->getName() + "->" + anim->getName() + " track:" + DStringTool::toString(track->getHandle()),
										"Duel::DMCodec::readAnimation");
								}
								if (peekHeader(stream) == DM_AnimationMorphKeyframeNormBuffer)
								{
									readHeader(stream, header);
									char* normBufPtr = (char*)normBuf->lock(HBL_Discard);
									stream->read(normBufPtr, header.length);
									normBuf->unlock();
								}
								else
								{
									char* normBufPtr = (char*)normBuf->lock(HBL_Discard);
									memset(normBufPtr, 0, normBuf->getSize());
									normBuf->unlock();
								}
								keyframe->setPositionBuffer(posBuf);
								keyframe->setNormalBuffer(normBuf);
							}
						} // end of keyframes
					}
				} // end of tracks
			}
		} // end of anims
	}

	void DMCodec_v1_0::writeHeader( DDataStream* stream, DMChunkID chunkID, size_t chunkLenght )
	{
		DMChunkID tmpID = chunkID;
		size_t tmpLength = chunkLenght;
		stream->write(&tmpID, sizeof(DMChunkID));
		stream->write(&tmpLength, sizeof(size_t));
	}

	void DMCodec_v1_0::writeString( DDataStream* stream, const DString& str )
	{
		stream->write(str.c_str(), str.length());
	}

	void DMCodec_v1_0::writeMesh( DDataStream* stream, DMesh* mesh )
	{
		writeHeader(stream, DM_Mesh, mesh->getName().length());
		writeString(stream, mesh->getMeshName());
		DMesh::SubMeshIterator sbi = mesh->getSubMeshIterator();
		while(sbi.hasMoreElements())
		{
			DSubMeshPtr curSubMesh = sbi.getNext();

		}
	}

	void DMCodec_v1_0::writeSubMesh( DDataStream* stream, DMesh* mesh )
	{
// 		DMesh::SubMeshIterator mi = mesh->getSubMeshIterator();
// 		while(mi.hasMoreElements())
// 		{
// 			DSubMeshPtr curSubMesh = *(mi.current());
// 			writeHeader(stream, DM_SubMesh, curSubMesh->getName().length());
// 			writeString(stream, curSubMesh->getName());
// 
// 			// write geometry data
// 			// index buffer.
// 			writeHeader(stream, DM_Geometry, (uint32)0);
// 			writeHeader(stream, DM_Index, sizeof(uint32) + sizeof(size_t));
// 			DIndexBufferPtr ib = curSubMesh->getIndices();
// 			IndexType	indexType = ib->getType();
// 			size_t		indexCount = ib->getIndexCount();
// 			NATIVETOLITTLE(&indexType);
// 			NATIVETOLITTLE(&indexCount);
// 			stream->write(&indexType, sizeof(indexType));
// 			stream->write(&indexCount, sizeof(indexCount));
// 
// 			size_t bufSize = indexCount * (indexType == IT_16Bit ? 16 : 32);
// 			writeHeader(stream, DM_IndexBuffer, bufSize);
// 			void* bufPtr = ib->lock(HBL_ReadOnly);
// 			stream->write(bufPtr, bufSize);
// 			ib->unlock();
// 
// 			// vertex declaration
// 			DVertexDeclarationPtr vd = curSubMesh->getVertexDeclaration();
// 			uint32 elemCount = vd->getElementCount();
// 			if (elemCount == 0)
// 			{
// 				DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
// 					"No valid vertex declaration contains in the mesh, failed to write",
// 					"Duel::DMCodec::writeSubMesh");
// 			}
// 			const size_t vdLength = sizeof(uint16) * 2 + sizeof(size_t) + sizeof(uint32) * 2;
// 			
// 			uint16 source;
// 			size_t offset;
// 			uint32 vertexElementType;
// 			uint32 vertexElementSemantic;
// 			uint16 index;
// 			DVertexDeclaration::VertexElementIterator i = vd->getVertexElementIterator();
// 			while(i.hasMoreElements())
// 			{
// 				DVertexElement velem = i.getNext();
// 				source = velem.getSource();
// 				offset = velem.getOffset();
// 				vertexElementType = velem.getElementType();
// 				vertexElementSemantic = velem.getSemantic();
// 				index = velem.getIndex();
// 
// 				writeHeader(stream, DM_VertexDeclaration, vdLength);
// 				stream->write(&source, sizeof(uint16));
// 				stream->write(&offset, sizeof(size_t));
// 				stream->write(&vertexElementType, sizeof(uint32));
// 				stream->write(&vertexElementSemantic, sizeof(uint32));
// 				stream->write(&index, sizeof(uint16));
// 			}
// 
// 			// vertex buffer
// 			const size_t vbLength = sizeof(uint16) + sizeof(uint32) + sizeof(uint32);
// 			writeHeader(stream, DM_Vertex,  vbLength);
// 			uint16 vsource;
// 			uint32 vertexSize;
// 			uint32 vertexCount;
// 			DVertexStreams* vbbinding = curSubMesh->getVertexStreams();
// 			DVertexStreams::VertexStreamIterator vbi = vbbinding->getVertexStreamIterator();
// 			while(vbi.hasMoreElements())
// 			{
// 				DVertexBufferPtr vb = vbi.current()->second;
// 				vsource = vbi.current()->first;
// 				vertexSize = vb->getVertexSize();
// 				vertexCount = vb->getVerticesCount();
// 
// 				writeHeader(stream, DM_Vertex, vbLength);
// 				stream->write(&vsource, sizeof(vsource));
// 				stream->write(&vertexSize, sizeof(vertexSize));
// 				stream->write(&vertexCount, sizeof(vertexCount));
// 
// 				writeHeader(stream, DM_VertexBuffer, vertexCount*vertexSize);
// 				void* bufPtr = vb->lock(HBL_ReadOnly);
// 				stream->write(bufPtr, vertexCount*vertexSize);
// 				vb->unlock();
// 
// 				vbi.moveNext();
// 			}
// 
// 			DSubMesh::BoneAttachMapIterator bmi = curSubMesh->getBoneAttachMapIterator();
// 			uint32	vertexIndex;
// 			uint16	boneIndex;
// 			float	weight;
// 			while(bmi.hasMoreElements())
// 			{
// 				writeHeader(stream, DM_BoneAssignment, sizeof(VertexBoneAttachment));
// 				vertexIndex = bmi.current()->second.vertexIndex;
// 				boneIndex = bmi.current()->second.boneIndex;
// 				weight = bmi.current()->second.weight;
// 				
// 				NATIVETOLITTLE(&vertexIndex);
// 				NATIVETOLITTLE(&boneIndex);
// 				NATIVETOLITTLE(&weight);
// 
// 				stream->write(&vertexIndex, sizeof(vertexIndex));
// 				stream->write(&boneIndex, sizeof(boneIndex));
// 				stream->write(&weight, sizeof(weight));
// 
// 				bmi.moveNext();
// 			}

			// write render effect data
// 			if (curSubMesh->getRenderEffect() != "")
// 			{
// 				writeHeader(stream, DM_RenderEffect, curSubMesh->getRenderEffect().length());
// 				writeString(stream,curSubMesh->getRenderEffect());
// 			}
// 
// 			// write texture data
// 			DSubMesh::UVSetIterator tmi = curSubMesh->getTextureMapIterator();
// 			if (tmi.hasMoreElements())
// 			{
// 				DString texName = tmi.current()->second;
// 				uint32 texIndex = tmi.current()->first;
// 
// 				NATIVETOLITTLE(&texIndex);
// 
// 				writeHeader(stream, DM_Texture, sizeof(uint32) + texName.length());
// 				stream->write(&texIndex, sizeof(uint32));
// 				writeString(stream, texName);
// 
// 				tmi.moveNext();
// 			}
// 
// 			mi.moveNext();
// 		}

		
		
	}

	void DMCodec_v1_0::writeSkeleton( DDataStream* stream, DMesh* mesh )
	{
		if (mesh->hasSkeleton())
		{
// 			writeHeader(stream, DM_Skeleton, mesh->getSkeleton()->getName().length());
// 			writeString(stream, mesh->getSkeleton()->getName());
		}
	}

	void DMCodec_v1_0::writeAnimation( DDataStream* stream, DMesh* mesh )
	{
		float length;
		DMesh::AnimationIterator ai = mesh->getAnimationIterator();
		while(ai.hasMoreElements())
		{
			DAnimation* curAnim = ai.current()->second;
			writeHeader(stream, DM_Animation, sizeof(float)+curAnim->getName().length());
			length = curAnim->getLength();
			NativeToLittle(&length);
			stream->write(&length, sizeof(length));
			writeString(stream, curAnim->getName());

			DAnimation::MorphTrackIterator mti = curAnim->getMorphTrackIterator();
			if (mti.hasMoreElements())
			{
				while(mti.hasMoreElements())
				{
					uint16 targetSubmeshIndex = mti.current()->second->getHandle();
					writeHeader(stream, DM_AnimationTrack, sizeof(uint16));
					NativeToLittle(&targetSubmeshIndex);
					stream->write(&targetSubmeshIndex, sizeof(uint16));

					DAnimationTrack::KeyFrameIterator kfi = mti.current()->second->getKeyFrameIterator();
// 					float time;
// 					uint32 vertexCount;
					while (kfi.hasMoreElements())
					{
// 						MorphKeyFrame* kf = static_cast<MorphKeyFrame*>(*(kfi.current()));
// 
// 						DVertexBufferPtr vb = kf->getPositionBuffer();
// 						time = kf->getTimePosition();
// 						vertexCount = vb->getVerticesCount();
// 						writeHeader(stream, DM_AnimationMorphKeyframe, sizeof(float) + sizeof(uint32));
// 
// 						NATIVETOLITTLE(&time);
// 						NATIVETOLITTLE(&vertexCount);
// 						stream->write(&time, sizeof(float));
// 						stream->write(&vertexCount, sizeof(uint32));
// 						
// 						writeHeader(stream, DM_AnimationMorphKeyframePosBuffer, vertexCount*3*sizeof(float));
// 						void* bufPtr = vb->lock(HBL_ReadOnly);
// 						stream->write(bufPtr, vertexCount*3*sizeof(float));
// 						vb->unlock();

						kfi.moveNext();
					}

					mti.moveNext();
				}
			}
			else
			{
				DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
					"No animation with empty track allowed, failed to write",
					"Duel::DMCodec::writeAnimation");
			}
			
			ai.moveNext();
		}
	}

}