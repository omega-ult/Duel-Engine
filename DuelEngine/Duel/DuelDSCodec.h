//  [12/30/2012 OMEGA] created

#ifndef _DUELDSCODEC_H_
#define _DUELDSCODEC_H_

#include "DuelCommon.h"
#include "DuelCodec.h"

namespace Duel
{
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



	class DUEL_API DSCodec_v1_0 : public DResourceCodec
	{
	DUEL_DECLARE_RTTI(DSCodec_v1_0)
	public:
		struct	ChunkHeader
		{
			uint32	id;
			uint32	length;
		};
		// override : DSkeletonCodec----------------------------
		void			decode(DDataStream* input, DResource* targetSkel);
		// override : DSkeletonCodec----------------------------
		DDataStreamPtr	encode(DResource* targetSkel);
		// override : DSkeletonCodec----------------------------
		void			encodeToFile(const DString& fileName, DResource* targetSkel);

		CodecType		getCodecType() { return CT_Skeleton; }

		// peek the next header's id, this method will not affect the streams' pos
		uint32			peekHeader(DDataStream* stream);
		// internal method used to read string.
		DString			readString(DDataStream* stream, uint32 length);
		// internal method used to read ChunkHeader
		void			readHeader(DDataStream* stream, ChunkHeader& header);
		// internal method used to read Skeleton
		void			readSkeleton(DDataStream* stream, DSkeleton* skel);
		// internal method used to read Skeleton
		void			readSubSkeleton(DDataStream* stream, DSkeleton* skel);

		// internal method used to write ChunkHeader
		void			writeHeader(DDataStream* stream, DSChunkID chunkID, size_t length);
		// internal method used to write DString
		void			writeString(DDataStream* stream, const DString& str);
		// internal method used to write Bone
		void			writeBone(DDataStream* stream, DSkeleton* skel);
		// internal method used to write relationship information
		void			writeRelation(DDataStream* stream, DSkeleton* skel);
		// internal method used to write animations
		void			writeAnimation(DDataStream* stream, DSkeleton* skel);
	};

}

#endif