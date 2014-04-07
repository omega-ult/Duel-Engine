// [10/22/2013 OMEGA] created

#include "DuelMPMayaIncludes.h"
#include "DuelMPDMExporter.h"
#include "DuelMPMeshData.h"
#include "DuelMPExporterParameters.h"
#include <cstdint>

using namespace std;

struct DMHeader
{
	uint32_t id;
	uint32_t length;
};
void writeHeader(DMHeader header, fstream& fs)
{
	fs.write((const char*)(&header.id),sizeof(uint32_t));
	fs.write((const char*)(&header.length),sizeof(uint32_t));
}
bool DMPDMExporter::createFile( const string& fullPath )
{
	mFstream.open(fullPath, ios_base::out | ios_base::binary);
	if (mFstream.fail())
	{
		return false;
	}
	return true;
}

void DMPDMExporter::fillMesh(DMPParameters* param )
{
	MStatus stat;
	mMeshData.clear();
	if (!param->bExportMesh)
	{
		return;
	}
	mMeshData.mesh.name = param->meshFileName.asUTF8();		
	if (param->skeletonTarget == DMPParameters::ST_UseAssigned)
	{
		mMeshData.mesh.targetSkeleton = param->assignSkeleton.asUTF8();
	}
	else if(param->skeletonTarget == DMPParameters::ST_UseCurrent)
	{
		mMeshData.mesh.targetSkeleton = param->skeletonFileName.asUTF8();
	}

	// first build animation clip info
	if (param->bExportMorphAnimation)
	{
		for (unsigned int i = 0; i < param->clipList.size(); ++i)
		{
			// convert to realtime speed(seconds);
			float sampleRate;
			float clipStart;
			float clipEnd;
			if (param->animSampleType == DMPParameters::AST_Frame)
			{
				sampleRate = param->samplerRate / param->fps;
				clipStart = (param->clipList[i].start - 1.0f) / param->fps;	// include the first clip;
				clipEnd = param->clipList[i].end / param->fps;
			}
			else
			{
				sampleRate = param->samplerRate;
				clipStart = param->clipList[i].start;
				clipEnd = param->clipList[i].end;
			}
			DMPMeshData::MorphAnimation newAnim;
			newAnim.name = param->clipList[i].clipName.asUTF8();
			newAnim.startTime = clipStart;
			newAnim.endTime = clipEnd;
			mMeshData.mesh.animations.push_back(newAnim);
		}
	}

	MSelectionList activeList;
	stat = MGlobal::getActiveSelectionList(activeList);
	if(param->bExportAll)
	{

		// We are exporting the whole scene
		MItDag dagIter;
		MFnDagNode worldDag (dagIter.root());
		MDagPath worldPath;
		worldDag.getPath(worldPath);
		traverseSubMesh(param, worldPath);

	}
	else
	{
		if (MStatus::kSuccess != stat)
		{
			return;
		}
		MItSelectionList iter(activeList);

		for ( ; !iter.isDone(); iter.next())
		{								
			MDagPath dagPath;
			iter.getDagPath(dagPath);
			traverseSubMesh(param, dagPath); 
		}	
	}
	// may lose selection while exporting, so reset it.
	MGlobal::setActiveSelectionList(activeList);
}

void DMPDMExporter::traverseSubMesh( DMPParameters* param, const MDagPath& dagPath )
{
	MStatus stat; 
	fillSubMesh(param, dagPath); 
	// look for meshes and cameras within the node's children
	for (unsigned int i=0; i<dagPath.childCount(); i++)
	{
		MObject child = dagPath.child(i);
		MDagPath childPath = dagPath;
		stat = childPath.push(child);
		if (MStatus::kSuccess != stat)
		{
			std::cout << "Error retrieving path to child " << i << " of: " << dagPath.fullPathName().asChar();
			std::cout.flush();
			return;
		}
		traverseSubMesh(param, childPath);
		if (MStatus::kSuccess != stat)
		{
			return;
		}
	}
}

void DMPDMExporter::fillSubMesh(DMPParameters* param, const MDagPath& subMeshDag)
{
	if (subMeshDag.apiType() != MFn::kMesh)
	{
		return; // early out.
	}
	MStatus status; 
	MFnMesh fnMesh(subMeshDag, &status);
	// 老子艹. tmd终于搞清楚为什么绑定了骨骼的mesh会多导出一份xxxOrig的副本,
	// 原来那货是intermediateObject
	if (fnMesh.isIntermediateObject())
	{
		return;
	}
	//MObject component = MObject::kNullObj;
	MString submeshName = subMeshDag.partialPathName();
	MSpace::Space space = MSpace::kObject;

	if (status != MS::kSuccess)
	{
		std::cout << "Fail to initialize mesh." << std::endl;
		return;
	}

	size_t subMeshId = mMeshData.mesh.subMeshes.size();
	mMeshData.mesh.subMeshes.push_back(DMPMeshData::SubMeshStruct());
	DMPMeshData::SubMeshStruct& subMesh = mMeshData.mesh.subMeshes[subMeshId];
	subMesh.name = submeshName.asChar();
	unsigned int numVertices = fnMesh.numVertices();
	if (numVertices <= 0)
	{
		std::cout << "warning: no vertices found in the mesh : " << submeshName << std::endl;
		return; // no vertices.
	}

	// get the size of vertices, then we can allocate space for our vertices.
	subMesh.vertices.resize(numVertices);
	// notice that the normals and uvs are calculated in triagle's traversal procedure,
	// and their indices are different from the position indices. in the triagle's traversal procedure,
	// specified vertex in each index may appear more than once, so we need a flag vector
	// to indicate whether the vertex is processed.
	vector<bool>	vtxProcessed;
	vtxProcessed.resize(numVertices, false);


	bool hasTexCoords = false;
	MString* uvSetName = NULL;

	MPointArray points;
	fnMesh.getPoints(points, space);
	int size = points.length();
	// Get normals
	MFloatVectorArray normals;
	fnMesh.getVertexNormals(false, normals, space);

	// Get UV set names and associated textures
	// FIXME: At present only the first UV set is recorded and used
	MObjectArray textureObjects;
	MStringArray uvSetNames;
	if (fnMesh.numUVs() > 0)
	{
		hasTexCoords = true;
	}
	// build uv set maps and texture maps.
	fnMesh.getUVSetNames(uvSetNames);
	if (uvSetNames.length() > 0)
	{
		for (unsigned int i = 0; i < uvSetNames.length(); ++i)
		{
			uvSetName = &uvSetNames[i];
			string uvSetNameStr = uvSetName->asChar();
			subMesh.addUVSet(uvSetNameStr);
			// append textures.
			fnMesh.getAssociatedUVSetTextures(*uvSetName, textureObjects);
			if (textureObjects.length() > 0)
			{
				for (unsigned int i = 0; i < textureObjects.length(); ++i)
				{
					MObject* texObj = &textureObjects[i];
					if (texObj->hasFn(MFn::kFileTexture))
					{
						MFnDependencyNode texDn(*texObj);
						MPlug plug = texDn.findPlug("fileTextureName");

						// this method will retrieve full path of the texture,
						// so we must extract base file name 
						MString textureName;
						plug.getValue(textureName);
						string baseFileName = textureName.asChar();
						size_t basePos = baseFileName.find_last_of('/');
						if (basePos == string::npos)
						{
							// try use another form
							basePos = baseFileName.find_last_of('\\');
							if (basePos == string::npos )
								continue;	// skip this texture;
						}
						// +1 to skip the slash.
						baseFileName = baseFileName.substr(basePos+1, baseFileName.size());
						subMesh.textureMap[uvSetNameStr].push_back(baseFileName);
					}
				}
			}
		}
	} // handle uv sets and textures.


	std::map<string, MFloatArray>	uMaps;
	std::map<string, MFloatArray>	vMaps;

	for (unsigned int i = 0; i < uvSetNames.length(); ++i)
	{
		MString uvSetName = uvSetNames[i];

		// Get UV coords of default UV set
		MFloatArray uArray, vArray;
		fnMesh.getUVs(uArray, vArray, &uvSetName);
		uMaps[uvSetName.asChar()] = uArray;
		vMaps[uvSetName.asChar()] = vArray;
	}

	// Get associated joints and weights
	std::vector<MFloatArray> weights(numVertices);
	std::vector<MIntArray> jointIndices(numVertices);


	if (param->skeletonTarget != DMPParameters::ST_NoSkeleton)
	{

		// work on the joints.
		MFnSkinCluster* fnSkinClusterPtr = NULL;
		MItDependencyNodes dn(MFn::kSkinClusterFilter);
		// since only mesh could be affected by only one skin cluster, so we just need 
		// to find it out.
		for (; !dn.isDone() && !fnSkinClusterPtr; dn.next())
		{
			MObject object = dn.item();
			fnSkinClusterPtr = new MFnSkinCluster(object, &status);

			unsigned int numGeometries = fnSkinClusterPtr->numOutputConnections();
			for (unsigned int i=0; i<numGeometries && fnSkinClusterPtr; ++i) 
			{
				unsigned int index = fnSkinClusterPtr->indexForOutputConnection(i);
				MObject outputObject = fnSkinClusterPtr->outputShapeAtIndex(index);

				// Check if skin cluster is invalid
				if (outputObject != fnMesh.object())
				{
					delete fnSkinClusterPtr;
					fnSkinClusterPtr = NULL;
				}
			}
		}

		// build joint maps.
		// build vertex binding weights
		if (fnSkinClusterPtr)
		{
			MDagPathArray influencePaths;
			int numInfluenceObjs = fnSkinClusterPtr->influenceObjects(influencePaths, &status);

			// query the max joint count.


			MDagPath jointPath,rootPath;
			for (int i=0; i<numInfluenceObjs; ++i)
			{
				jointPath = influencePaths[i];
				if (!jointPath.hasFn(MFn::kJoint))
				{
					continue;
				}

				// Try to retrieve the root path
				rootPath = jointPath;
				while (jointPath.length() > 0)
				{
					jointPath.pop();
					if (jointPath.hasFn(MFn::kJoint) && (jointPath.length() > 0))
					{
						rootPath = jointPath;
					}
				}

				if (rootPath.hasFn(MFn::kJoint))
				{
					MFnIkJoint fnRootJoint(rootPath);

					string rootJointName = fnRootJoint.partialPathName().asChar();
					// this method determined the subskeleton name 
					// is identical to the sub-root joint name.
					subMesh.targetSubSkeleton = rootJointName;
					fillJointMap(&subMesh, fnRootJoint, rootPath);
					break;
				}
			}

			unsigned int numWeight;
			MItGeometry geomIterator(subMeshDag);
			for (int i = 0; !geomIterator.isDone(); geomIterator.next(), ++i)
			{
				MObject component = geomIterator.component();
				MFloatArray vertexWeights;
				status = fnSkinClusterPtr->getWeights(subMeshDag, component, vertexWeights, numWeight);
				if (status != MS::kSuccess)
				{
					std::cout << "Fail to retrieve vertex weights." << std::endl;
					break;
				}
				if (numWeight > 4)
				{
					MGlobal::executeCommand("print \"fail to export skin information, influenced joint count bigger than 4:" + MString(subMesh.name.c_str()) + "\\n\"");
					break;
				}
				weights[i] = vertexWeights;
				jointIndices[i].setLength(vertexWeights.length());

				MDagPathArray influenceObjs;
				fnSkinClusterPtr->influenceObjects(influenceObjs, &status);
				if (status != MS::kSuccess)
				{
					std::cout << "Fail to retrieve influence objects for the skin cluster." << std::endl;
					break;
				}
				for (unsigned int j = 0; j < influenceObjs.length(); ++j)
				{
					MString partialPathName = influenceObjs[j].partialPathName();
					unsigned int index = subMesh.getJointIndex(partialPathName.asChar());
					// Record joint index
					jointIndices[i][j] = index;
				}
			}
			delete fnSkinClusterPtr;
		}
	}

	// Traverse polygons
	MItMeshPolygon polyIter(fnMesh.object(), &status);
	if (status != MS::kSuccess)
	{
		std::cout << "Fail to traverse polygons." << std::endl;
		return;
	}
	for (; !polyIter.isDone(); polyIter.next())
	{
		int triangleCount; 
		status = polyIter.numTriangles(triangleCount);
		if (status != MS::kSuccess)
		{
			continue;
		}

		DMPMeshData::TriangleStruct triangle;
		for (int ti = 0; ti < triangleCount; ++ti)
		{
			MPointArray triPoints;	// will not use.
			MIntArray	tmpTriIndices, triVertexIdx;
			status = polyIter.getTriangle(ti, triPoints, tmpTriIndices, space);
			if (status != MS::kSuccess)
			{
				std::cout << "Fail to traverse polygons." << std::endl;
				return;
			}
			// convert indices to face-relative indices
			MIntArray polyIndices;
			polyIter.getVertices(polyIndices);
			for (unsigned int iObj = 0; iObj < tmpTriIndices.length(); ++iObj)
			{
				// iPoly is face-relative vertex index
				for (unsigned int iPoly=0; iPoly < polyIndices.length(); ++iPoly)
				{
					if (tmpTriIndices[iObj] == polyIndices[iPoly]) 
					{
						triVertexIdx.append(iPoly);
						break;
					}
				}
			}

			for (unsigned int idi = 0; idi < 3; ++idi)
			{
				int vtxIndexInPolygon = triVertexIdx[idi];
				unsigned int vertexIndex = polyIter.vertexIndex(vtxIndexInPolygon);

				// build triangle indices.
				triangle.vertexIndex[idi] = tmpTriIndices[idi];

				if (!vtxProcessed[vertexIndex])
				{
					DMPMeshData::VertexStruct& curVtx = subMesh.vertices[vertexIndex];

					// pos
					MPoint pos = points[vertexIndex];
					pos.cartesianize();
					curVtx.position[0] = pos.x * param->lum;
					curVtx.position[1] = pos.y * param->lum;
					curVtx.position[2] = pos.z * param->lum;

					// normal
					MFloatVector norm = normals[vertexIndex];
					curVtx.normal = DMPMeshData::Point3(3, norm.x, norm.y, norm.z);
					
					// weight and joints.
					MFloatArray vWeights = weights[vertexIndex];
					MIntArray vJoints = jointIndices[vertexIndex];
					unsigned int numBinds = std::min<unsigned int>(vWeights.length(), vJoints.length());
 					for (unsigned int n = 0; n < numBinds; ++n)
 					{
 						curVtx.jointBindings.push_back(DMPMeshData::JointBinding(vJoints[n],vWeights[n]));
 					}

 					if (hasTexCoords)
 					{
 						// add uv info
 						for (unsigned int m = 0; m < uvSetNames.length(); ++m)
 						{
 							if (polyIter.hasUVs(uvSetNames[m]))
 							{
 								float2 uv;
 								int uvSetIndex = subMesh.getUVSetIndex(string(uvSetNames[m].asChar()));
 								polyIter.getUV(vtxIndexInPolygon, uv, &uvSetNames[m]);
 								curVtx.texcoords[uvSetIndex] = DMPMeshData::TexCoord(2, uv[0], uv[1]);
 							}
 						}
 					}
					// mark as processed..
					vtxProcessed[vertexIndex] = true;
				}
			}
			// push triangle info
			subMesh.triangles.push_back(triangle);
		}
	}

	// now go for morph animations.
	if (param->bExportMorphAnimation && MAnimUtil::isAnimated(subMeshDag, true))
	{
		for (unsigned int i = 0; i < mMeshData.mesh.animations.size(); ++i)
		{
			// we have build anim map before, so just retrieve its reference here.
			DMPMeshData::MorphAnimation& anim = mMeshData.mesh.animations[i];
			DMPMeshData::MorphTrack	subTrack;
			subTrack.targetSubMesh = subMesh.name;
			MPointArray pos;
			MFloatVectorArray	norms;
			MPlug		plugMesh;
			MObject		meshData;
			MFnDependencyNode	fnDependNode(subMeshDag.node(), &status);
			// Get the .outMesh plug for this mesh
			plugMesh = fnDependNode.findPlug("outMesh", false, &status);
			if (status != MStatus:: kSuccess)
			{
				continue;
			}
			float timeStep = param->samplerRate;
			if (param->animSampleType == DMPParameters::AST_Frame)
			{
				timeStep /= param->fps;
			}
			for (float curTime = anim.startTime; curTime <= anim.endTime; curTime += timeStep)
			{
				MTime		mayaTime;
				DMPMeshData::MorphKeyFrame keyframe;
				keyframe.time = curTime - anim.startTime;
				mayaTime.setUnit(MTime::kSeconds);
				mayaTime.setValue(curTime);
				// Get its value at the specified Time.
				plugMesh.getValue(meshData, MDGContext(mayaTime));
				// Use its MFnMesh function set 
				MFnMesh		fnMesh(meshData, &status);
				// And query the point coordinates
				fnMesh.getPoints(pos);
				fnMesh.getVertexNormals(false, norms);
				for (unsigned int i = 0; i < pos.length(); ++i)
				{
					DMPMeshData::Point3 p;
					p[0] = pos[i].x * param->lum;
					p[1] = pos[i].y * param->lum;
					p[2] = pos[i].z * param->lum;
					keyframe.positions.push_back(p);
				}
				for (unsigned int i = 0; i < norms.length(); ++i)
				{
					DMPMeshData::Point3 n;
					n[0] = norms[i].x;
					n[1] = norms[i].y;
					n[2] = norms[i].z;
					keyframe.normals.push_back(n);
				}
				subTrack.frames.push_back(keyframe);
			}
			anim.tracks.push_back(subTrack);
		}
	}
}

void DMPDMExporter::fillJointMap( DMPMeshData::SubMeshStruct* subMesh, MFnIkJoint& fnJoint, MDagPath& dagPath )
{
	MString jointName = fnJoint.partialPathName();
	std::string jointNameStr = jointName.asChar();
	subMesh->getJointIndex(jointNameStr);
	// Traverse child joints

	for (unsigned int i=0; i<dagPath.childCount(); ++i)
	{
		MObject child = dagPath.child(i);
		MDagPath childPath = dagPath;
		childPath.push(child);
		if (!childPath.hasFn(MFn::kJoint)) continue;

		MFnIkJoint fnChildJoint(childPath);

		fillJointMap(subMesh, fnChildJoint, childPath);
	}
}

void DMPDMExporter::exportMesh( DMPParameters* param )
{
	if (param->bExportMesh)
	{
		// try create file.
		if (createFile(string(
			MString(param->outputDir + param->meshFileName).asUTF8())))
		{
			// ok, 填充数据结构,
			fillMesh(param);
			writeToFile();
		}
		else
		{
			MGlobal::executeCommand("print \"fail to create file: " + param->outputDir + param->meshFileName + "\\n\"");
			MGlobal::executeCommand("print \"mesh will not be exported\\n\"");
		}
	}
}

void DMPDMExporter::writeToFile()
{
	if (!mFstream.is_open())
	{
		return;
	}
	if (mMeshData.mesh.subMeshes.size() == 0)
	{
		MGlobal::executeCommand("print \"fail to export mesh, mesh data not found\\n\"");
		mFstream.close();
		return;
	}
	// write header:
	DMHeader fileHeader;
	fileHeader.id = DM_Header;
	string version = "1.0";
	fileHeader.length = version.size();
	writeHeader(fileHeader, mFstream);
	mFstream.write(version.c_str(), version.length());
	
	// write mesh
	DMHeader meshHeader;
	meshHeader.id = DM_Mesh;
	string meshName = mMeshData.mesh.name;
	meshHeader.length = meshName.length();
	writeHeader(meshHeader, mFstream);
	mFstream.write(meshName.c_str(), meshName.length());

	for (unsigned int i = 0; i < mMeshData.mesh.subMeshes.size(); ++i)
	{
		DMPMeshData::SubMeshStruct& subMesh =  mMeshData.mesh.subMeshes[i];
		if (subMesh.vertices.size() > 0)
		{
			// submesh.
			DMHeader subMeshHeader;
			subMeshHeader.id = DM_SubMesh;
			subMeshHeader.length = subMesh.name.length();
			writeHeader(subMeshHeader, mFstream);
			mFstream.write(subMesh.name.c_str(), subMesh.name.length());
			// index
			DMHeader indexHeader;
			indexHeader.id = DM_Index;
			uint32_t idxType = IT_32Bit;
			uint32_t idxCount = subMesh.triangles.size() * 3;
			indexHeader.length = sizeof(idxType) + sizeof(idxCount);
			writeHeader(indexHeader, mFstream);
			mFstream.write((const char*)&idxType, sizeof(idxType));
			mFstream.write((const char*)&idxCount, sizeof(idxCount));
			DMHeader indexBufHeader;
			indexBufHeader.id = DM_IndexBuffer;
			indexBufHeader.length = sizeof(uint32_t) * 3 * subMesh.triangles.size();
			writeHeader(indexBufHeader, mFstream);
			for (unsigned int j = 0; j < subMesh.triangles.size(); ++j)
			{
				DMPMeshData::TriangleStruct& tri = subMesh.triangles[j];
				mFstream.write((const char*)tri.vertexIndex, sizeof(uint32_t) * 3);
			}
			// vertex declaration
			DMHeader vdHeader;
			vdHeader.id = DM_VertexDeclaration;
			// construct vd, for pos/normal/blendId/blendWeight/texcoord_n/
			// here we use the following format:
			/*
			src0: position
			src1: normal
			src2: tex_0-tex_n
			src3: blendIndex
			src4: blendWeight
			*/
			// pos:
			uint16_t src = 0;
			uint32_t offset = 0;
			uint32_t vType = VET_Float3;
			uint32_t vSemantic = VES_Position;
			uint16_t vIdx = 0;
			vdHeader.length = sizeof(src) + sizeof(offset) + sizeof(vType) + sizeof(vSemantic) + sizeof(vIdx);
			writeHeader(vdHeader, mFstream);
				mFstream.write((const char*)&src, sizeof(src));
				mFstream.write((const char*)&offset, sizeof(offset));
				mFstream.write((const char*)&vType, sizeof(vType));
				mFstream.write((const char*)&vSemantic, sizeof(vSemantic));
				mFstream.write((const char*)&vIdx, sizeof(vIdx));
			// normal:
			src = 1;
			offset = 0;
			vType = VET_Float3;
			vSemantic = VES_Normal;
			vIdx = 0;
			writeHeader(vdHeader, mFstream);
				mFstream.write((const char*)&src, sizeof(src));
				mFstream.write((const char*)&offset, sizeof(offset));
				mFstream.write((const char*)&vType, sizeof(vType));
				mFstream.write((const char*)&vSemantic, sizeof(vSemantic));
				mFstream.write((const char*)&vIdx, sizeof(vIdx));
			// tex:
			for (unsigned int j = 0; j < subMesh.UVSets.size(); ++j)
			{
				if (j < 8)	// we can only export max to 8 texcoords
				{
					src = 2;
					offset = sizeof(float) * 2 * j;
					vType = VET_Float2;
					vSemantic = VES_TexCoord + j;
					vIdx = j;
					writeHeader(vdHeader, mFstream);
						mFstream.write((const char*)&src, sizeof(src));
						mFstream.write((const char*)&offset, sizeof(offset));
						mFstream.write((const char*)&vType, sizeof(vType));
						mFstream.write((const char*)&vSemantic, sizeof(vSemantic));
						mFstream.write((const char*)&vIdx, sizeof(vIdx));
				}
			}
			if (subMesh.targetSubSkeleton != "")
			{
				// blend weight/blend index.
				uint16_t src = 3;
				uint32_t offset = 0;
				uint32_t vType = VET_Short4;
				uint32_t vSemantic = VES_BlendIndices;
				uint16_t vIdx = 0;
				writeHeader(vdHeader, mFstream);
					mFstream.write((const char*)&src, sizeof(src));
					mFstream.write((const char*)&offset, sizeof(offset));
					mFstream.write((const char*)&vType, sizeof(vType));
					mFstream.write((const char*)&vSemantic, sizeof(vSemantic));
					mFstream.write((const char*)&vIdx, sizeof(vIdx));
				src = 4;
				offset = 0;
				vType = VET_Float4;
				vSemantic = VES_BlendWeight;
				vIdx = 0;
				writeHeader(vdHeader, mFstream);
					mFstream.write((const char*)&src, sizeof(src));
					mFstream.write((const char*)&offset, sizeof(offset));
					mFstream.write((const char*)&vType, sizeof(vType));
					mFstream.write((const char*)&vSemantic, sizeof(vSemantic));
					mFstream.write((const char*)&vIdx, sizeof(vIdx));

			}
		
			// now vertex buffer.
			DMHeader vHeader;
			vHeader.id = DM_Vertex;
			DMHeader vbHeader;
			vbHeader.id = DM_VertexBuffer;
			// for pos.
			uint16_t vSrc = 0;
			uint32_t vSize = sizeof(float) * 3;
			uint32_t vCount = subMesh.vertices.size(); 
			vHeader.length = sizeof(vSrc) + sizeof(vSize) + sizeof(vCount);
			writeHeader(vHeader, mFstream);
				mFstream.write((const char*)&vSrc, sizeof(vSrc));
				mFstream.write((const char*)&vSize, sizeof(vSize));
				mFstream.write((const char*)&vCount, sizeof(vCount));
			vbHeader.length = vSize * vCount;
			writeHeader(vbHeader, mFstream);
				for (unsigned int k = 0 ; k < subMesh.vertices.size(); ++k)
				{
					DMPMeshData::VertexStruct& vtx = subMesh.vertices[k];
					float pos[3];
					pos[0] = (float)vtx.position[0];
					pos[1] = (float)vtx.position[1];
					pos[2] = (float)vtx.position[2];
					mFstream.write((const char*)pos, sizeof(float) * 3);
				}
			// for normal
			vSrc = 1;
			writeHeader(vHeader, mFstream);
			mFstream.write((const char*)&vSrc, sizeof(vSrc));
			mFstream.write((const char*)&vSize, sizeof(vSize));
			mFstream.write((const char*)&vCount, sizeof(vCount));
			vbHeader.length = vSize * vCount;
			writeHeader(vbHeader, mFstream);
			for (unsigned int k = 0 ; k < subMesh.vertices.size(); ++k)
			{
				DMPMeshData::VertexStruct& vtx = subMesh.vertices[k];
				float norm[3];
				norm[0] = (float)vtx.normal[0];
				norm[1] = (float)vtx.normal[1];
				norm[2] = (float)vtx.normal[2];
				mFstream.write((const char*)norm, sizeof(float) * 3);
			}
			// for texcoord.
			if (subMesh.UVSets.size() > 0)
			{
				vSrc = 2;
				vSize = sizeof(float) * 2 * (std::min<unsigned int>(8, subMesh.UVSets.size()));
				vCount = subMesh.vertices.size(); 
				writeHeader(vHeader, mFstream);
					mFstream.write((const char*)&vSrc, sizeof(vSrc));
					mFstream.write((const char*)&vSize, sizeof(vSize));
					mFstream.write((const char*)&vCount, sizeof(vCount));
				vbHeader.length = vSize * vCount;
				writeHeader(vbHeader, mFstream);
				for (unsigned int k = 0; k < subMesh.vertices.size(); ++k)
				{
					DMPMeshData::VertexStruct& vtx = subMesh.vertices[k];
					map<int, DMPMeshData::TexCoord>::iterator txI, txIend = vtx.texcoords.end();
					txI = vtx.texcoords.begin();
					uint32_t uvsetCount = std::min<unsigned int>(8, subMesh.UVSets.size());
					for (uint32_t p = 0; p < uvsetCount; ++p)
					{
						// fill zero if the uv is not in the expected uvset.
						float uv[2] = {0.0f};
						if (txI != txIend)
						{
							if (txI->first == p)
							{
								uv[0] = (float)(txI->second)[0];
								uv[1] = (float)(txI->second)[1];
							}
							++txI;
						}
						mFstream.write((const char*)uv, sizeof(float) * 2);
					}
				}
			}
			// for blend index/blend weight
			if (subMesh.targetSubSkeleton != "")
			{
				// for blend index:
				vSrc = 3;
				vSize = sizeof(unsigned short) * 4;
				vCount = subMesh.vertices.size();
				writeHeader(vHeader, mFstream);
					mFstream.write((const char*)&vSrc, sizeof(vSrc));
					mFstream.write((const char*)&vSize, sizeof(vSize));
					mFstream.write((const char*)&vCount, sizeof(vCount));
				vbHeader.length = vSize * vCount;
				writeHeader(vbHeader, mFstream);
				for (unsigned int k = 0; k < subMesh.vertices.size(); ++k)
				{
					DMPMeshData::VertexStruct& vtx = subMesh.vertices[k];
					vector<DMPMeshData::JointBinding>::iterator jbI, jbIend = vtx.jointBindings.end();;
					jbI = vtx.jointBindings.begin();
					uint16_t bId[4];
					for (unsigned int n = 0; n < 4; ++n)
					{
						if (jbI != vtx.jointBindings.end())
						{
							bId[n] = jbI->first;
							jbI++;
						}
						else
						{
							// for invalid bone.
							bId[n] = 0;
						}
					}
					mFstream.write((const char*)bId, sizeof(uint16_t) * 4);
				}
				// for blend weight:
				vSrc = 4;
				vSize = sizeof(float) * 4;
				vCount = subMesh.vertices.size();
				writeHeader(vHeader, mFstream);
					mFstream.write((const char*)&vSrc, sizeof(vSrc));
					mFstream.write((const char*)&vSize, sizeof(vSize));
					mFstream.write((const char*)&vCount, sizeof(vCount));
				vbHeader.length = vSize * vCount;
				writeHeader(vbHeader, mFstream);
				for (unsigned int k = 0; k < subMesh.vertices.size(); ++k)
				{
					DMPMeshData::VertexStruct& vtx = subMesh.vertices[k];
					vector<DMPMeshData::JointBinding>::iterator jbI, jbIend = vtx.jointBindings.end();;
					jbI = vtx.jointBindings.begin();
					float bWeight[4];
					for (unsigned int n = 0; n < 4; ++n)
					{
						if (jbI != vtx.jointBindings.end())
						{
							bWeight[n] = jbI->second;
							jbI++;
						}
						else
						{
							// for invalid bone.
							bWeight[n] = 0.0f;
						}
					}
					mFstream.write((const char*)bWeight, sizeof(float) * 4);
				}
			}
			if (subMesh.targetSubSkeleton != "")
			{
				// write target skeleton.
				DMHeader tsHeader;
				tsHeader.id = DM_TargetSubSkeleton;
				uint32_t boneInfluence = 4;
				string ts = subMesh.targetSubSkeleton;
				tsHeader.length = sizeof(uint32_t) + ts.length();
				writeHeader(tsHeader, mFstream);
				mFstream.write((const char*)&boneInfluence, sizeof(boneInfluence));
				mFstream.write(ts.c_str(), ts.length());
				// bone adapter.
				DMHeader baHeader;
				baHeader.id = DM_BoneIndexAdapter;
				DMPMeshData::SubMeshStruct::JointIndexMap::iterator jiI, jiIend = subMesh.jointIndexMap.end();
				for (jiI = subMesh.jointIndexMap.begin(); jiI != jiIend; ++jiI)
				{
					uint16_t jid = jiI->second;
					baHeader.length = sizeof(jid) + jiI->first.length();
					writeHeader(baHeader, mFstream);
					mFstream.write((const char*)&jid, sizeof(uint16_t));
					mFstream.write(jiI->first.c_str(), jiI->first.length());
				}
			}
			// no material specified.
			// uvset
			if (subMesh.UVSets.size() > 0)
			{
				DMPMeshData::SubMeshStruct::UVSetMap::iterator uvI, uvIend = subMesh.UVSets.end();
				uvI = subMesh.UVSets.begin();
				while (uvI != uvIend)
				{
					DMHeader uvHeader;
					uvHeader.id = DM_UVSet;
					string setName = uvI->first;
					uint32_t ves = uvI->second;
					if (uvI->second < 8)
					{
						uint32_t uvSetLength = setName.length();
						uvHeader.length = sizeof(ves) + 
							sizeof(uvSetLength) +
							setName.length();
						writeHeader(uvHeader, mFstream);
							mFstream.write((const char*)&ves, sizeof(ves));
							mFstream.write((const char*)&uvSetLength, sizeof(uvSetLength));
							mFstream.write(setName.c_str(), setName.length());
					}
					++uvI;
				} // end of uvsets.

				// binded textures.
				uvI = subMesh.UVSets.begin();
				while (uvI != uvIend)
				{
					DMHeader bdTexHeader;
					bdTexHeader.id = DM_BindedTextures;
					string setName = uvI->first;
					DMPMeshData::SubMeshStruct::TextureMap::iterator ti = subMesh.textureMap.find(uvI->first);
					if (ti != subMesh.textureMap.end())
					{
						vector<string>& txList = ti->second;
						vector<string>::iterator txlI, txlIend = txList.end();
						for (txlI = txList.begin(); txlI != txlIend; ++txlI)
						{
							uint32_t uvSetLength = setName.length();
							string txName = *txlI;
							uint32_t txNameLength = txName.length();
							bdTexHeader.length = sizeof(uvSetLength) +
								setName.length() + sizeof(txNameLength) + txName.length();

							writeHeader(bdTexHeader, mFstream);
								mFstream.write((const char*)&uvSetLength, sizeof(uvSetLength));
								mFstream.write(setName.c_str(), setName.length());
								mFstream.write((const char*)&txNameLength, sizeof(txNameLength));
								mFstream.write(txName.c_str(), txName.length());
						}
					}
					++uvI;
				}
			}
		}
	}	// end of submesh.
	// now here is the skeleton.
	if (mMeshData.mesh.targetSkeleton != "")
	{
		DMHeader skelHeader;
		skelHeader.id = DM_Skeleton;
		skelHeader.length = mMeshData.mesh.targetSkeleton.length();
		writeHeader(skelHeader, mFstream);
			mFstream.write(mMeshData.mesh.targetSkeleton.c_str(), mMeshData.mesh.targetSkeleton.length());
	}
	if (mMeshData.mesh.animations.size() > 0)
	{
		DMHeader animHeader;
		animHeader.id = DM_Animation;
		vector<DMPMeshData::MorphAnimation>::iterator ai, aiend = mMeshData.mesh.animations.end();
		for (ai = mMeshData.mesh.animations.begin(); ai != aiend; ++ai)
		{
			DMPMeshData::MorphAnimation& anim = (*ai);
			float length = anim.endTime - anim.startTime;
			string name = anim.name;
			animHeader.length = sizeof(float) + name.length();
			writeHeader(animHeader, mFstream);
				mFstream.write((const char*)&length, sizeof(length));
				mFstream.write(name.c_str(), name.length());
			vector<DMPMeshData::MorphTrack>::iterator ti, tiend = anim.tracks.end();
			for (ti = anim.tracks.begin(); ti != tiend; ++ti)
			{
				DMPMeshData::MorphTrack& track = (*ti);
				DMHeader tkHeader;
				tkHeader.id = DM_AnimationTrack;
				tkHeader.length = track.targetSubMesh.length();
				writeHeader(tkHeader, mFstream);
					mFstream.write(track.targetSubMesh.c_str(), track.targetSubMesh.length());
				vector<DMPMeshData::MorphKeyFrame>::iterator mi, miend = track.frames.end();
				for (mi = track.frames.begin(); mi != miend; ++mi)
				{
					DMPMeshData::MorphKeyFrame& frame = (*mi);
					DMHeader mkHeader;
					mkHeader.id = DM_AnimationMorphKeyframe;
					float time = frame.time;
					uint32_t vcnt = frame.positions.size();
					mkHeader.length = sizeof(time) + sizeof(vcnt);
					writeHeader(mkHeader, mFstream);
						mFstream.write((const char*)&time, sizeof(float));
						mFstream.write((const char*)&vcnt, sizeof(vcnt));
					// posBuf
					DMHeader bufHeader;
					bufHeader.id = DM_AnimationMorphKeyframeBuffer;
					bufHeader.length = vcnt * sizeof(float) * 3;
					writeHeader(bufHeader, mFstream);
					float fPos[3];
					for (unsigned int n = 0; n < vcnt; ++n)
					{
						fPos[0] = (float)frame.positions[n][0];
						fPos[0] = (float)frame.positions[n][0];
						fPos[0] = (float)frame.positions[n][0];
						mFstream.write((const char*)fPos, sizeof(float) * 3);
					}
					// normBuf
					bufHeader.id = DM_AnimationMorphKeyframeNormBuffer;
					bufHeader.length = vcnt * sizeof(float) * 3;
					writeHeader(bufHeader, mFstream);
					for (unsigned int n = 0; n < vcnt; ++n)
					{
						fPos[0] = (float)frame.positions[n][0];
						fPos[0] = (float)frame.positions[n][0];
						fPos[0] = (float)frame.positions[n][0];
						mFstream.write((const char*)fPos, sizeof(float) * 3);
					}
				}
			}
		}
	}


	MGlobal::executeCommand("print \"success to export mesh file: " + MString(mMeshData.mesh.name.c_str()) + "\\n\"");
	mFstream.flush();
	mFstream.close();
}
