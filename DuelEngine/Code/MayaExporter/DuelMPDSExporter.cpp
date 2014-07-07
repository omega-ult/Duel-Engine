//  [1/16/2014 OMEGA] created


#include "DuelMPMayaIncludes.h"
#include "DuelMPDSExporter.h"
#include "DuelMPSkeletonData.h"
#include "DuelMPExporterParameters.h"
#include <cstdint>

#pragma warning (disable : 4267) // size_t to uint32_t

bool DMPDSExporter::createFile( const string& fullPath )
{
	mFstream.open(fullPath, ios_base::out | ios_base::binary);
	if (mFstream.fail())
	{
		return false;
	}
	return true;

}

void DMPDSExporter::fillSkeleton( DMPParameters* param )
{
	MStatus stat;
	mSkelData.clear();
	if (!param->bExportSkeleton)
	{
		return;
	}
	mSkelData.skeleton.name = param->skeletonFileName.asUTF8();


	// Get the selection list
	MSelectionList activeList;
	stat = MGlobal::getActiveSelectionList(activeList);
	if(param->bExportAll)
	{
		// We are exporting the whole scene
		MItDag dagIter;
		MFnDagNode worldDag (dagIter.root());
		MDagPath worldPath;
		worldDag.getPath(worldPath);
		traverseSubSkeleton(param, worldPath);
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
			traverseSubSkeleton(param, dagPath); 
		}	
	}
	// may lose selection while exporting, so reset it.
	MGlobal::setActiveSelectionList(activeList);
}

void DMPDSExporter::traverseSubSkeleton( DMPParameters* param, const MDagPath& dagPath )
{
	MStatus stat; 
	fillSubSkeleton(param, dagPath); 
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
		fillSubSkeleton(param, childPath);
		if (MStatus::kSuccess != stat)
		{
			return;
		}
	}
}

void DMPDSExporter::fillSubSkeleton( DMPParameters* param, const MDagPath& subSkelDag )
{
	if (subSkelDag.apiType() != MFn::kJoint)
	{
		return; // early out.
	}
	MStatus status; 
	// try to retrieve the root path, otherwise ignore it.
	MDagPath rootJntDag, tmpJointPath;
	tmpJointPath = subSkelDag;
	rootJntDag = tmpJointPath;
	while (tmpJointPath.length() > 0)
	{
		tmpJointPath.pop();
		if (tmpJointPath.hasFn(MFn::kJoint) && (tmpJointPath.length() > 0))
		{
			rootJntDag = tmpJointPath;
		}
	}
	// check whether we have processed this joint.
	for (unsigned int i = 0; i < mSkelData.skeleton.subSkeletons.size(); ++i)
	{
		if (mSkelData.skeleton.subSkeletons[i].name == rootJntDag.partialPathName().asUTF8())
		{		
			return;
		}
	}
	DMPSkeletonData::SubSkeletonStruct newSkel;
	newSkel.name = rootJntDag.partialPathName().asUTF8();

	//set the skeleton to the desired neutral pose
	if (param->neutralPoseType == DMPParameters::NPT_SkinBindPose)
	{
		MGlobal::executeCommand("select " + rootJntDag.fullPathName(),true);
		//disable constraints, IK, etc...
		MGlobal::executeCommand("doEnableNodeItems false all",true);
		// Note: we reset to the bind pose
		MGlobal::executeCommand("dagPose -r -g -bp",true);
	}

 	// build animation clip info, for animation exporting.
 	if (param->bExportSkelAnimation)
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
 			DMPSkeletonData::TransformAnimation newAnim;
 			newAnim.name = param->clipList[i].clipName.asUTF8();
 			newAnim.startTime = clipStart;
			newAnim.endTime = clipEnd;
			newSkel.animations.push_back(newAnim);
 		}
 	}


	fillBones(&newSkel, "", param, rootJntDag);

	mSkelData.skeleton.subSkeletons.push_back(newSkel);
}

void DMPDSExporter::fillBones( DMPSkeletonData::SubSkeletonStruct* subSkel, string parent, DMPParameters* param, MDagPath& jointDag )
{
	MStatus status;
	if (jointDag.apiType() != MFn::kJoint)
	{
		return; // early out.
	}
	DMPSkeletonData::BoneStruct newBone;
	newBone.boneHandle = (unsigned int)subSkel->bones.size();
	newBone.name = jointDag.partialPathName().asUTF8();
	newBone.parentName = parent;

	MFnIkJoint fnJoint(jointDag, &status);
	//	matrix = [S] * [RO] * [R] * [JO] * [IS] * [T]
	/*
		These matrices are defined as follows:
		•[S] : scale
		•[RO] : rotateOrient (attribute name is rotateAxis)
		•[R] : rotate
		•[JO] : jointOrient
		•[IS] : parentScaleInverse
		•[T] : translate

		The methods to get the value of these matrices are:
		•[S] : getScale
		•[RO] : getScaleOrientation
		•[R] : getRotation
		•[JO] : getOrientation
		•[IS] : (the inverse of the getScale on the parent transformation matrix)
		•[T] : translation

	*/
	MVector trans = fnJoint.getTranslation(MSpace::kTransform);
	double scale[3];
	fnJoint.getScale(scale);
	MQuaternion R, RO, JO;
	fnJoint.getScaleOrientation(RO);
	fnJoint.getRotation(R);
	fnJoint.getOrientation(JO);
	MQuaternion rot = RO * R * JO; 
	
	newBone.translate[0] = trans.x * param->lum;
	newBone.translate[1] = trans.y * param->lum;
	newBone.translate[2] = trans.z * param->lum;

	newBone.orientation[0] = rot.w;
	newBone.orientation[1] = rot.x;
	newBone.orientation[2] = rot.y;
	newBone.orientation[3] = rot.z;

	newBone.scale[0] = scale[0];
	newBone.scale[1] = scale[1];
	newBone.scale[2] = scale[2];

	subSkel->bones.push_back(newBone);
	// Load child joints
	for (unsigned int i=0; i<jointDag.childCount();i++)
	{
		MObject child;
		child = jointDag.child(i);
		MDagPath childDag = jointDag;
		childDag.push(child);
		fillBones(subSkel, newBone.name, param, childDag);
	}
	// now go for animations
	if (param->bExportSkelAnimation)
	{
		for (unsigned int i = 0; i < subSkel->animations.size(); ++i)
		{
			DMPSkeletonData::TransformAnimation& anim = subSkel->animations[i];
			DMPSkeletonData::TransformTrack subTrack;
			subTrack.targetBone = newBone.name;

			MPlug		plugT;	// translate
 			MPlug		plugR;	// R
 			MPlug		plugRO;	// RO
 			MPlug		plugJO;	// JO
			MPlug		plugS;	// scale
			double		dataT[3];
			double		dataR[3];
			double		dataRO[3];
			double		dataJO[3];
			double		dataS[3];
			MFnDependencyNode	fnDependNode( jointDag.node(), &status );
			
			plugT = fnDependNode.findPlug("translate", false, &status);
 			plugR = fnDependNode.findPlug("rotate", false, &status);
 			plugRO = fnDependNode.findPlug("rotateAxis", false, &status);
 			plugJO = fnDependNode.findPlug("jointOrient", false, &status);
			plugS = fnDependNode.findPlug("scale", false, &status);

			float timeStep = param->samplerRate;
			if (param->animSampleType == DMPParameters::AST_Frame)
			{
				timeStep /= param->fps;
			}
			for (float curTime = anim.startTime; curTime <= anim.endTime; curTime += timeStep)
			{
				MTime		mayaTime;
				DMPSkeletonData::TransformKeyFrame keyframe;
				keyframe.time = curTime - anim.startTime;
				mayaTime.setUnit(MTime::kSeconds);
				mayaTime.setValue(curTime);

				// Get its value at the specified Time.
				plugT.child(0).getValue(dataT[0], MDGContext(mayaTime));
				plugT.child(1).getValue(dataT[1], MDGContext(mayaTime));
				plugT.child(2).getValue(dataT[2], MDGContext(mayaTime));

				plugR.child(0).getValue(dataR[0], MDGContext(mayaTime));
				plugR.child(1).getValue(dataR[1], MDGContext(mayaTime));
				plugR.child(2).getValue(dataR[2], MDGContext(mayaTime));

				plugRO.child(0).getValue(dataRO[0], MDGContext(mayaTime));
				plugRO.child(1).getValue(dataRO[1], MDGContext(mayaTime));
				plugRO.child(2).getValue(dataRO[2], MDGContext(mayaTime));

				plugJO.child(0).getValue(dataJO[0], MDGContext(mayaTime));
				plugJO.child(1).getValue(dataJO[1], MDGContext(mayaTime));
				plugJO.child(2).getValue(dataJO[2], MDGContext(mayaTime));

				plugS.child(0).getValue(dataS[0], MDGContext(mayaTime));
				plugS.child(1).getValue(dataS[1], MDGContext(mayaTime));
				plugS.child(2).getValue(dataS[2], MDGContext(mayaTime));

				// fill the frame.
				keyframe.translate[0] = dataT[0] * param->lum;
				keyframe.translate[1] = dataT[1] * param->lum;
				keyframe.translate[2] = dataT[2] * param->lum;
				// calculate quaternion.
				MEulerRotation	rotR(dataR[0], dataR[1], dataR[2]);
				MEulerRotation	rotRO(dataRO[0], dataRO[1], dataRO[2]);
				MEulerRotation	rotJO(dataJO[0], dataJO[1], dataJO[2]);

				MQuaternion finalRot = rotRO.asQuaternion()*rotR.asQuaternion()*rotJO.asQuaternion();
				
				keyframe.orientation[0] = finalRot.w;
				keyframe.orientation[1] = finalRot.x;
				keyframe.orientation[2] = finalRot.y;
				keyframe.orientation[3] = finalRot.z;

				keyframe.scale[0] = dataS[0];
				keyframe.scale[1] = dataS[1];
				keyframe.scale[2] = dataS[2];

				subTrack.frames.push_back(keyframe);
			}
			anim.tracks.push_back(subTrack);
		}
	}
}

void DMPDSExporter::exportSkeleton( DMPParameters* param )
{
	// 先尝试创建文件吧. 如果不成功那么就算了...
	if (param->bExportSkeleton)
	{
		// try create file.	
		// try create file.
		if (createFile(string(
			MString(param->outputDir + param->skeletonFileName).asUTF8())))
		{
			fillSkeleton(param);
			writeToFile();
		}
		else
		{
			MGlobal::executeCommand("print \"fail to create file: " + param->outputDir + param->skeletonFileName + "\\n\"");
			MGlobal::executeCommand("print \"Skeleton will not be exported\\n\"");
		}

	}

}

struct DSHeader
{
	uint32_t id;
	uint32_t length;
};

void writeDSHeader(DSHeader& header, fstream& fs)
{
	fs.write((const char*)(&header.id),sizeof(uint32_t));
	fs.write((const char*)(&header.length),sizeof(uint32_t));
}

void DMPDSExporter::writeToFile()
{
	if (!mFstream.is_open())
	{
		return;
	}

	if (mSkelData.skeleton.subSkeletons.size() == 0)
	{
		MGlobal::executeCommand("print \"fail to export skeleton, skeleton data not found\\n\"");
		mFstream.close();
		return;
	}
	// write header.
	DSHeader fileHeader;
	fileHeader.id = DS_Header;
	string version = "1.0";
	fileHeader.length = version.length();
	writeDSHeader(fileHeader, mFstream);
	mFstream.write(version.c_str(), version.length());

	// write skel
	DSHeader skelHeader;
	skelHeader.id = DS_Skeleton;
	skelHeader.length = mSkelData.skeleton.name.length();
	writeDSHeader(skelHeader, mFstream);
	mFstream.write(mSkelData.skeleton.name.c_str(), skelHeader.length);

	// write subskelton
	for (unsigned int i = 0; i < mSkelData.skeleton.subSkeletons.size(); ++ i)
	{
		DMPSkeletonData::SubSkeletonStruct& subSkel = mSkelData.skeleton.subSkeletons[i];

		DSHeader ssHeader;
		ssHeader.id = DS_SubSkeleton;
		ssHeader.length = subSkel.name.length();
		writeDSHeader(ssHeader, mFstream);
		mFstream.write(subSkel.name.c_str(), subSkel.name.length());

		for (unsigned int j = 0; j < subSkel.bones.size(); ++j)
		{
			DMPSkeletonData::BoneStruct& bone = subSkel.bones[j];

			DSHeader bHeader;
			bHeader.id = DS_Bone;
			uint32_t nameLength = bone.name.length();
			uint16_t handle = bone.boneHandle;
			float pos[3];
			float scale[3];
			float orient[4];	// w, x, y, z
			bHeader.length = sizeof(nameLength) + 
				bone.name.length() + 
				sizeof(handle)  +
				sizeof(float) * 3 +
				sizeof(float) * 3 +
				sizeof(float) * 4;
			writeDSHeader(bHeader, mFstream);

			pos[0] = (float)bone.translate[0];
			pos[1] = (float)bone.translate[1];
			pos[2] = (float)bone.translate[2];
			scale[0] = (float)bone.scale[0];
			scale[1] = (float)bone.scale[1];
			scale[2] = (float)bone.scale[2];
			orient[0] = (float)bone.orientation[0];
			orient[1] = (float)bone.orientation[1];
			orient[2] = (float)bone.orientation[2];
			orient[3] = (float)bone.orientation[3];
			
			mFstream.write((const char*)&nameLength, sizeof(nameLength));
			mFstream.write(bone.name.c_str(), bone.name.length());
			mFstream.write((const char*)&handle, sizeof(handle));
			mFstream.write((const char*)pos, sizeof(float) * 3);
			mFstream.write((const char*)scale, sizeof(float) * 3);
			mFstream.write((const char*)orient, sizeof(float) * 4);
		}
		for (unsigned int j = 0; j < subSkel.bones.size(); ++j)
		{
			DMPSkeletonData::BoneStruct& bone = subSkel.bones[j];
			if (bone.parentName != "")
			{
				// for relation.
				DSHeader rHeader;
				rHeader.id = DS_Relation;
				rHeader.length = sizeof(uint16_t) * 2;
				writeDSHeader(rHeader, mFstream);
				// find the parent bone.
				uint16_t cHandle = bone.boneHandle;
				uint16_t pHandle = 0;
				for (unsigned int k = 0; k < subSkel.bones.size(); ++k)
				{
					if (subSkel.bones[k].name == bone.parentName)
					{
						pHandle = subSkel.bones[k].boneHandle;
						break;
					}
				}
				mFstream.write((const char*)&pHandle, sizeof(pHandle));
				mFstream.write((const char*)&cHandle, sizeof(cHandle));
			}
		}
		if(subSkel.animations.size() > 0)
		{
			for (unsigned int k = 0; k < subSkel.animations.size(); ++k)
			{
				DMPSkeletonData::TransformAnimation& anim = subSkel.animations[k];
				DSHeader animHeader;
				animHeader.id = DS_Animation;
				float length = anim.endTime - anim.startTime;
				animHeader.length = sizeof(float) + anim.name.length();
				writeDSHeader(animHeader, mFstream);
				mFstream.write((const char*)&length, sizeof(length));
				mFstream.write(anim.name.c_str(), anim.name.length());
				vector<DMPSkeletonData::TransformTrack>::iterator ti, tiend = anim.tracks.end();
				for (ti = anim.tracks.begin(); ti != tiend; ++ti)
				{
					DMPSkeletonData::TransformTrack& track = (*ti);
					DSHeader tHeader;
					uint16_t targetHandle = 0;
					for (unsigned int n = 0; n < subSkel.bones.size(); ++n)
					{
						if (subSkel.bones[n].name == track.targetBone)
						{
							targetHandle = subSkel.bones[n].boneHandle;
							break;
						}
					}
					tHeader.id = DS_AnimationTrack;
					tHeader.length = sizeof(targetHandle);
					writeDSHeader(tHeader, mFstream);
					mFstream.write((const char*)&targetHandle, sizeof(targetHandle));
					vector<DMPSkeletonData::TransformKeyFrame>::iterator fi, fiend = track.frames.end();
					for (fi = track.frames.begin(); fi != fiend; ++fi)
					{
						DMPSkeletonData::TransformKeyFrame& frame = (*fi);
						DSHeader fHeader;
						fHeader.id = DS_AnimationKeyFrame;
						float timePos = frame.time;
						float fPos[3];
						float fScale[3];
						float fOrient[4];
						fHeader.length = sizeof(timePos) +
							sizeof(float) * 3 +
							sizeof(float) * 3 +
							sizeof(float) * 4;

						fPos[0] = (float)frame.translate[0];
						fPos[1] = (float)frame.translate[1];
						fPos[2] = (float)frame.translate[2];
						fScale[0] = (float)frame.scale[0];
						fScale[1] = (float)frame.scale[1];
						fScale[2] = (float)frame.scale[2];
						fOrient[0] = (float)frame.orientation[0];
						fOrient[1] = (float)frame.orientation[1];
						fOrient[2] = (float)frame.orientation[2];
						fOrient[3] = (float)frame.orientation[3];
						writeDSHeader(fHeader, mFstream);
						mFstream.write((const char*)&timePos, sizeof(timePos));
						mFstream.write((const char*)fPos, sizeof(float) * 3);
						mFstream.write((const char*)fScale, sizeof(float) * 3);
						mFstream.write((const char*)fOrient, sizeof(float) * 4);
					}
				}
			}
		}
	}
	MGlobal::executeCommand("print \"success to export skeleton file: " + MString(mSkelData.skeleton.name.c_str()) + "\\n\"");
	mFstream.flush();
	mFstream.close();
}
