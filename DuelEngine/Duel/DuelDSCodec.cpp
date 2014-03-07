//  [1/17/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelDSCodec.h"
#include "DuelSkeleton.h"
#include "DuelBitTool.h"
#include "DuelDataStream.h"
#include "DuelLogManager.h"
#include "DuelException.h"
#include "DuelSkeletonManager.h"
#include "DuelCore.h"
#include "DuelRenderResourceManager.h"
#include "DuelAnimationTrack.h"
#include "DuelKeyFrame.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DSCodec_v1_0, DResourceCodec);

	void DSCodec_v1_0::decode( DDataStream* input, DResource* targetRes )
	{
		DSkeleton* targetSkel = targetRes->getAs<DSkeleton>(false);
		if (targetRes == NULL)
		{
			return;
		}
		if (peekHeader(input) == DS_Header)
		{
			// clear current mesh.
			targetSkel->removeAllSubskeletons();

			ChunkHeader	header;
			readHeader(input, header);
			DString version = readString(input, header.length);
			if (version == "1.0")
			{
				readSkeleton(input, targetSkel);
			}
		}
	}

	DDataStreamPtr DSCodec_v1_0::encode( DResource* targetRes )
	{
		DSkeleton* targetSkel = targetRes->getAs<DSkeleton>(false);
		if (targetRes == NULL)
		{
			return DDataStreamPtr();
		}
		//////////////////////////////////////////////////////////////////////////
		//  [10/24/2013 OMEGA] 
		// Ê©¹¤ÖÐ
		/*
		// calculate the total data size of the skeleton.
		size_t totalSize = 0;
		size_t headerSize = sizeof(ChunkHeader);
		// first, version size.
		DString version = "1.0";
		totalSize += headerSize + version.length();

		// data length contains the length of handle, pos, scale and orient, this part
		// is invariant.
		const size_t boneDataSize = sizeof(uint16) + sizeof(DReal) * 10;
		DSkeleton::BoneIterator bi = targetSkel->getBoneIterator();
		while(bi.hasMoreElements())
		{
			DBone* curBone = bi.current()->second;
			totalSize += headerSize + boneDataSize + curBone->getName().length();
			// if the DBone has a parent, it's relationship will be written, so here
			// must calculate it.
			if (curBone->getParent() != NULL)
			{
				totalSize += headerSize + sizeof(uint16) * 2;
			}
			bi.moveNext();
		}
		// calculate animation data size.
		DSkeleton::AnimationIterator ai = targetSkel->getAnimationIterator();
		while(ai.hasMoreElements())
		{
			DAnimation* curAnim = ai.current()->second;
			totalSize += headerSize + sizeof(float) + curAnim->getName().length();
			DAnimation::NodeTrackIterator nti = curAnim->getNodeTrackIterator();
			while(nti.hasMoreElements())
			{
				totalSize += headerSize + sizeof(uint16);
				// here just get the keyframe count, and we can calculate it manually.
				size_t keyframeCount = nti.current()->second->getKeyFrameCount();
				// including timepos, translate, scale and rotate.
				const size_t keyframeDataLength = sizeof(float) * 11;
				
				totalSize += keyframeDataLength * keyframeCount;

				nti.moveNext();
			}

			ai.moveNext();
		}

		// now we get the totalsize, create a new stream.
		DMemoryStream* ms = new DMemoryStream(totalSize, DA_Write);

		writeHeader(ms, DS_Header, version.length());
		writeString(ms, version);

		writeBone(ms, targetSkel);
		writeRelation(ms, targetSkel);
		writeAnimation(ms, targetSkel);

		DDataStreamPtr ret(ms);
		return ret;
		*/
		DDataStreamPtr ret;
		return ret;
	}

	void DSCodec_v1_0::encodeToFile( const DString& fileName, DResource* targetRes )
	{
		DSkeleton* targetSkel = targetRes->getAs<DSkeleton>(false);
		if (targetRes == NULL)
		{
			return;
		}
		DFileStream* fs = new DFileStream(fileName, DA_Write);
		if (!fs->isValid())
		{
			return;
		}
		DString version = "1.0";
		writeHeader(fs, DS_Header, version.length());
		writeString(fs, version);
		
		writeBone(fs, targetSkel);
		writeRelation(fs, targetSkel);
		writeAnimation(fs, targetSkel);

		delete fs;
	}

	void DSCodec_v1_0::readHeader( DDataStream* stream, ChunkHeader& header )
	{
		if (stream->eof())
		{
			header.id = DS_FORCEUINT;
			header.length = 0;
		}
		stream->read(&header.id, sizeof(uint32));
		stream->read(&header.length, sizeof(uint32));
	}

	uint32 DSCodec_v1_0::peekHeader( DDataStream* stream )
	{
		uint32 oldPos = stream->tell();
		uint32 headerID = DS_FORCEUINT;
		stream->read(&headerID, sizeof(uint32));
		stream->seek(oldPos);
		return headerID;
	}



	DString DSCodec_v1_0::readString( DDataStream* stream, uint32 length )
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

	void DSCodec_v1_0::readSkeleton( DDataStream* stream, DSkeleton* skel )
	{
		if (peekHeader(stream) == DS_Skeleton)
		{
			ChunkHeader header;
			readHeader(stream, header);
			DString name = readString(stream, header.length);
			while (peekHeader(stream) == DS_SubSkeleton)
			{
				readSubSkeleton(stream, skel);
			}
		}
	}

	void DSCodec_v1_0::readSubSkeleton( DDataStream* stream, DSkeleton* skel )
	{
		if (peekHeader(stream) == DS_SubSkeleton)
		{
			ChunkHeader header;
			readHeader(stream, header);
			DString name = readString(stream, header.length);
			DSubSkeletonPtr subSkel = skel->createSubSkeleton(name);
			while (peekHeader(stream) == DS_Bone)
			{
				readHeader(stream, header);
				uint32		boneNameLength;
				DString		boneName;
				uint16		handle;
				DVector3	pos;
				DVector3	scale;
				DQuaternion	orient;
				stream->read(&boneNameLength, sizeof(boneNameLength));
				boneName = readString(stream, boneNameLength);
				stream->read(&handle, sizeof(uint16));
				stream->read(pos.ptr(), sizeof(DReal)*3);
				stream->read(scale.ptr(), sizeof(DReal)*3);
				stream->read(orient.ptr(), sizeof(DReal)*4);
				DBone*	newB = subSkel->createBone(handle, boneName);
				newB->setDefaultProperties(pos, scale, orient);
				newB->reset();
			}
			while (peekHeader(stream) == DS_Relation)
			{
				readHeader(stream, header);
				uint16	parent;
				uint16	child;
				stream->read(&parent, sizeof(uint16));
				stream->read(&child, sizeof(uint16));
				DBone* bChil = subSkel->getBone(child);
				DBone* bParent = subSkel->getBone(parent);
				if (bChil == NULL || bParent == NULL)
				{
					DString parentHandle = DStringTool::toString(parent);
					DString childHandle = DStringTool::toString(child);
					DUEL_EXCEPT_BRIEF(DException::ET_InvalidStatus,
						"Corrupted file: specified bone not found, parent: " + parentHandle + "child: " + childHandle,
						"Duel::DSCodec_v1_0::readSubSkeleton");
				}
				bChil->setParent(bParent);
			}
			while (peekHeader(stream) == DS_Animation)
			{
				readHeader(stream, header);
				float animLength;
				stream->read(&animLength, sizeof(animLength));
				DString animName = readString(stream, header.length - sizeof(animLength));
				DAnimation* anim = subSkel->createAnimation(animName);
				anim->setLength(animLength);
				while (peekHeader(stream) == DS_AnimationTrack)
				{
					readHeader(stream, header);
					uint16 targetBoneHandle;
					stream->read(&targetBoneHandle, sizeof(uint16));
					DBone* targetBone = subSkel->getBone(targetBoneHandle);
					DNodeAnimationTrack* newTrack = NULL;
					if (targetBone != NULL)
					{
						newTrack = anim->createNodeTrack(targetBoneHandle);
					}
					while (peekHeader(stream) == DS_AnimationKeyFrame)
					{
						readHeader(stream, header);
						float timePos;
						DVector3 translate;
						DVector3 scale;
						DQuaternion rotate;
						stream->read(&timePos, sizeof(float));
						stream->read(translate.ptr(), sizeof(DReal)*3);
						stream->read(scale.ptr(), sizeof(DReal)*3);
						stream->read(rotate.ptr(), sizeof(DReal)*4);
						if (newTrack != NULL)
						{
							DTransformKeyFrame* kf = newTrack->createKeyFrame(timePos)->getAs<DTransformKeyFrame>();
							kf->setTranslate(translate);
							kf->setScale(scale);
							kf->setOrient(rotate);
						}
					}
					
				}
			}
		}
	}

// 	void DSCodec_v1_0::readAnimation( DDataStream* stream, DSkeleton* skel )
//	{
// 		float	length;
// 		stream->read(&length, sizeof(float));
// 		DString	animName = readString(stream);
// 		DAnimation*	anim = skel->createAnimation(animName);
// 
// 		ChunkHeader header;
// 		size_t streamPos = stream->tell();
// 		readHeader(stream, header);
// 		if (header.id == DS_AnimationTrack)
// 		{
// 			uint16 targetBoneHandle;
// 			float timePos;
// 			DVector3 translate;
// 			DVector3 scale;
// 			DQuaternion rotate;
// 			while(header.id == DS_AnimationTrack)
// 			{
// 				// record the position, used to read another chunk.
// 				stream->read(&targetBoneHandle, sizeof(uint16));
// 
// 				DBone* targetBone = skel->getBone(targetBoneHandle);
// 				if (targetBone == NULL)
// 				{
// 					DUEL_EXCEPT_BRIEF(DException::ET_InvalidStatus,
// 						"Cannot find DBone with handle: " + DStringTool::toString(targetBoneHandle),
// 						"Duel::DSCodec_v1_0::readAnimation");
// 				}
// 				NodeAnimationTrack* newTrack = anim->createNodeTrack(targetBoneHandle, static_cast<DNode*>(targetBone));
// 				
// 				streamPos = stream->tell();
// 				// read keyframe part.
// 				readHeader(stream, header);
// 				while(header.id == DS_AnimationKeyFrame)
// 				{
// 					// record the position, used to read another chunk.
// 					size_t streamPosKF = stream->tell() + header.length;
// 
// 					stream->read(&timePos, sizeof(float));
// 					stream->read(translate.ptr(), sizeof(DReal)*3);
// 					stream->read(scale.ptr(), sizeof(DReal)*3);
// 					stream->read(rotate.ptr(), sizeof(DReal)*4);
// 
// 					TransformKeyFrame* kf = static_cast<TransformKeyFrame*>(newTrack->createKeyFrame(timePos));
// 					kf->setTranslate(translate);
// 					kf->setScale(scale);
// 					kf->setOrient(rotate);
// 
// 					streamPos = stream->tell();
// 					readHeader(stream, header);
// 				}
// 				
// 				stream->seek(streamPos);
// 			}
// 		}
//	}

	void DSCodec_v1_0::writeHeader( DDataStream* stream, DSChunkID chunkID, size_t length )
	{
		DSChunkID tmpID = chunkID;
		size_t tmpLength = length;
		NativeToLittle(&tmpID);
		NativeToLittle(&tmpLength);
		stream->write(&tmpID, sizeof(DSChunkID));
		stream->write(&tmpLength, sizeof(size_t));
	}

	void DSCodec_v1_0::writeString( DDataStream* stream, const DString& str )
	{
		stream->write(str.c_str(), str.length());
	}

	void DSCodec_v1_0::writeBone( DDataStream* stream, DSkeleton* skel )
	{
// 		// ensure the skeleton is in default state.
// 		skel->resetPose();
// 		DSkeleton::BoneIterator i = skel->getBoneIterator();
// 		// data length contains the length of handle, pos, scale and orient, this part
// 		// is invariant.
// 		const size_t boneDataLength = sizeof(uint16) + sizeof(DReal) * 10;
// 		DVector3	tmpPos;
// 		DVector3	tmpScale;
// 		DQuaternion tmpOrient;
// 		while(i.hasMoreElements())
// 		{
// 			DBone* curBone = i.current()->second;
// 			uint16 handle = curBone->getHandle();
// 			size_t chunkLength = curBone->getName().length() + boneDataLength;
// 			
// 			writeHeader(stream, DS_Bone, chunkLength);
// 			writeString(stream, curBone->getName());
// 			stream->write(&handle, sizeof(uint16));
// 
// 			tmpPos = curBone->getRelativePosition();
// 			tmpScale = curBone->getRelativeScale();
// 			tmpOrient = curBone->getRelativeOrientation();
// 
// 			stream->write(tmpPos.ptr(), sizeof(DReal)*3);
// 			stream->write(tmpScale.ptr(), sizeof(DReal)*3);
// 			stream->write(tmpOrient.ptr(), sizeof(DReal)*4);
// 
// 			i.moveNext();
// 		}
	}

	void DSCodec_v1_0::writeRelation( DDataStream* stream, DSkeleton* skel )
	{
// 		DSkeleton::BoneIterator i = skel->getBoneIterator();
// 		// data length contains the length of handle, pos, scale and orient, this part
// 		// is invariant.
// 		const size_t boneDataLength = sizeof(uint16) * 2;
// 
// 		while(i.hasMoreElements())
// 		{
// 			DBone* curBone = i.current()->second;
// 			DBone* parentBone = static_cast<DBone*>(curBone->getParent());
// 			if (parentBone != NULL)
// 			{
// 				uint16 parentHandle = parentBone->getHandle();
// 				uint16 childHandle = curBone->getHandle();
// 				size_t chunkLength =  boneDataLength;
// 
// 				writeHeader(stream, DS_Relation, chunkLength);
// 				stream->write(&parentHandle, sizeof(uint16));
// 				stream->write(&childHandle, sizeof(uint16));
// 			}
// 
// 			i.moveNext();
// 		}
	}

	void DSCodec_v1_0::writeAnimation( DDataStream* stream, DSkeleton* skel )
	{
// 		DSkeleton::AnimationIterator i = skel->getAnimationIterator();
// 		// data length contains the length of handle, pos, scale and orient, this part
// 		// is invariant.
// 		const size_t animDataLength = sizeof(float);
// 		float animLength;
// 		while(i.hasMoreElements())
// 		{
// 			DAnimation* curAnim = i.current()->second;
// 			animLength = curAnim->getLength();
// 			size_t chunkLength = curAnim->getName().length() + animDataLength;
// 
// 			writeHeader(stream, DS_Animation, chunkLength);
// 			stream->write(&animLength, sizeof(float));
// 			writeString(stream, curAnim->getName());
// 
// 			// write track.
// 			DAnimation::NodeTrackIterator ni = curAnim->getNodeTrackIterator();
// 			uint16 targetBoneHandle;
// 			while(ni.hasMoreElements())
// 			{
// 				NodeAnimationTrack* curNTrack = ni.current()->second;
// 				targetBoneHandle = curNTrack->getHandle();
// 
// 				writeHeader(stream, DS_AnimationTrack, sizeof(uint16));
// 				stream->write(&targetBoneHandle, sizeof(uint16));
// 
// 				// write keyframes
// 				DAnimationTrack::KeyFrameIterator ki = curNTrack->getKeyFrameIterator();
// 				float timePos;
// 				DVector3 translate;
// 				DVector3 scale;
// 				DQuaternion rotate;
// 				while(ki.hasMoreElements())
// 				{
// 					TransformKeyFrame* curKf = static_cast<TransformKeyFrame*>(*(ki.current()));
// 
// 					// including timepos, translate, scale and rotate.
// 					const size_t keyframeDataLength = sizeof(float) * 11;
// 
// 					timePos = curKf->getTimePosition();
// 					NATIVETOLITTLE(&timePos);
// 					translate = curKf->getTranslate();
// 					scale = curKf->getScale();
// 					rotate = curKf->getOrient();
// 
// 					writeHeader(stream, DS_AnimationKeyFrame, keyframeDataLength);
// 					stream->write(&timePos, sizeof(float));
// 					stream->write(translate.ptr(), sizeof(float)*3);
// 					stream->write(scale.ptr(), sizeof(float)*3);
// 					stream->write(rotate.ptr(), sizeof(float)*4);
// 
// 					ki.moveNext();
// 				}
// 
// 				ni.moveNext();
// 			}
// 
// 			i.moveNext();
// 		}
	}

}