//  [11/4/2012 OMEGA] created

#ifndef _DUELKEYFRAME_H_
#define _DUELKEYFRAME_H_

#include "DuelCommon.h"


namespace Duel
{
	class DUEL_API DKeyFrame : public DObject
	{
	DUEL_DECLARE_RTTI(DKeyFrame)
	public:
		// DKeyFrame should only be created by the DAnimationTrack.
		DKeyFrame(DAnimationTrack* parent, DReal timePos);
		virtual ~DKeyFrame();

		// query the parent.
		DAnimationTrack*	getParent() const;
		// get the time position of the DKeyFrame.
		DReal			getTimePosition() const;

	protected:
		DAnimationTrack*	mParent;
		DReal			mTimePos;
	};

	class DUEL_API DNumericKeyFrame : public DKeyFrame
	{
	DUEL_DECLARE_RTTI(DNumericKeyFrame)
	public:
		DNumericKeyFrame(DAnimationTrack* parent, DReal timePos);
		
		// set/get value.
		DReal	getValue() const;
		void	setValue(DReal val);

	protected:
		DReal	mValue;
	};

	class DUEL_API DTransformKeyFrame : public DKeyFrame
	{
	DUEL_DECLARE_RTTI(DTransformKeyFrame)
	public:
		DTransformKeyFrame(DAnimationTrack* parent, DReal timePos);

		// set/get value.
		const DVector3&		getTranslate() const;
		const DVector3&		getScale() const;
		const DQuaternion&	getOrient() const; 
		void				setTranslate(const DVector3& trans);
		void				setScale(const DVector3& scale);
		void				setOrient(const DQuaternion& orient);
		DMatrix4				getTransformMatrix();

	protected:
		DVector3				mTranslate;
		DVector3				mScale;
		DQuaternion			mOrient;
	};

	class DUEL_API DMorphKeyFrame : public DKeyFrame
	{
	DUEL_DECLARE_RTTI(DMorphKeyFrame)
	public:
		DMorphKeyFrame(DAnimationTrack* parent, DReal timePos);

		// We assume that positions are the first 3 float elements in this buffer,
		// although we don't necessarily assume they're the only ones in there.
		void				setPositionBuffer(const DRawBufferPtr& buf);
		DRawBufferPtr		getPositionBuffer();
	
		void				setNormalBuffer(const DRawBufferPtr& buf);
		DRawBufferPtr		getNormalBuffer();

	protected:
		DRawBufferPtr		mPosBuf;
		DRawBufferPtr		mNormBuf;	
	};

// 
// 	/** Reference to a pose at a given influence level 
// 	@remarks
// 		Each keyframe can refer to many poses each at a given influence level.
// 	**/
// 	struct DUEL_API PoseRef
// 	{
// 		/** The linked pose index.
// 		@remarks
// 			The Mesh contains all poses for all vertex data in one list, both 
// 			for the shared vertex data and the dedicated vertex data on submeshes.
// 			The 'target' on the parent track must match the 'target' on the 
// 			linked pose.
// 		*/
// 		uint16	poseIndex;
// 		/** Influence level of the linked pose. 
// 			1.0 for full influence (full offset), 0.0 for no influence.
// 		*/
// 		DReal	influence;
// 		PoseRef() : poseIndex(0), influence(0) {}
// 		PoseRef(uint16 p, DReal i) : poseIndex(p), influence(i) {}
// 	};
// 
// 	class DUEL_API PoseKeyFrame : public DKeyFrame
// 	{
// 	DUEL_DECLARE_RTTI(PoseKeyFrame)
// 	public:
// 		PoseKeyFrame(DAnimationTrack* parent, DReal timePos);
// 
// 	
// 		typedef	std::map<uint16, PoseRef>	PoseRefMap;
// 		typedef	MapIterator<PoseRefMap>	PoseRefsIterator;
// 		PoseRefsIterator	getPoseRefsIterator() { return PoseRefsIterator(mPoseRefMap.begin(), mPoseRefMap.end()); }
// 
// 		// add a pose reference
// 		void		addPoseReference(uint16 poseIndex, DReal influence);
// 		// set the influence of specified pose index.
// 		void		setInfluence(uint16 poseIndex, DReal influence);
// 		// remove specified pose reference
// 		void		removePoseReference(uint16 poseIndex);
// 		// remove all pose references
// 		void		removeAllPoseReference();
// 
// 	protected:
// 		PoseRefMap	mPoseRefMap;
// 	};
// 	
}

#endif