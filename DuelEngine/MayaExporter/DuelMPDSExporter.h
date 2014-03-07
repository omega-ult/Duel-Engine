//  [10/24/2013 OMEGA] created

#ifndef _DUELMPDSEXPORTER_H_
#define _DUELMPDSEXPORTER_H_

#include "DuelMPMayaIncludes.h"
#include "DuelMPSkeletonData.h"
#include "DuelMPExporterParameters.h"
#include <fstream>

using namespace std;
// copy from DuelDSCodec.h
// ds = duel skeleton, it contains an bone set and its animations
// .ds file format are designed as chunk-based(for efficency), each chunk 
// has an ID, a LENGTH, and a DATA, looks like:
// chunk(n){
//		header {
//			uint32	ID;
//			uint32	LENGTH (with this chunk header excluded, never bigger than 4gb);
//		}	// the length indicate the data size.
//		void*	DATA; // if the length is 0, here the pointer does noe exist
// }
// chunk ID is defined below
// void* data format is described in comment.
enum DSChunkID
{
	DS_Header		= 0xAAAA,	// magic number indicate this file is .ds file
		// (char*)version		;  this part should contain a version description
		DS_Skeleton		= 0x10000,		// Skeleton chunk.
			// (char*)name	
			DS_SubSkeleton	= 0x11000,	// repeatyly, SubSkeleton contains the bones.
				// (char*)name
				DS_Bone			= 0x11100,		// repeatly
					// (uint32)nameLength
					// (char*)name
					// (uint16)handle
					// (Vector3)pos
					// (Vector3)scale
					// (Quaternion)orient		// w,x,y,z
			DS_Relation		= 0x12000,		// repeatly, the parent-children relationship,
				// (uint16)parentHandle
				// (uint16)childHandle
			DS_Animation	= 0x13000,		// repeatly, optional
				// (float)length
				// (char*)name 
				DS_AnimationTrack	= 0x13100,	// repeatly
					// (uint16)targetBoneHandle
					DS_AnimationKeyFrame	= 0x13110,	// repeatly
						// (float)timePos
						// (Vector3)traslate
						// (Vector3)scale
						// (Quaternion)rotate	
	DS_FORCEUINT	= 0xffffffffUL
};


class DMPDSExporter
{
public:
	DMPDSExporter() {}

	void		exportSkeleton(DMPParameters* param);

protected:
	// create a file, return result of creation.
	// you need to call this method and get "true" result before writing mesh data.
	bool		createFile(const string& fullPath);

	void		fillSkeleton(DMPParameters* param);
	void		traverseSubSkeleton(DMPParameters* param, const MDagPath& dagPath);
	void		fillSubSkeleton(DMPParameters* param, const MDagPath& subSkelDag);
	void		fillBones(DMPSkeletonData::SubSkeletonStruct* subSkel, string parent, DMPParameters* param, MDagPath& jointDag);

	void		writeToFile();

	DMPSkeletonData		mSkelData;
	fstream		mFstream;
};


#endif