//  [11/4/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelKeyFrame.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DKeyFrame, DObject);
	DUEL_IMPLEMENT_RTTI_1(DNumericKeyFrame, DKeyFrame);
	DUEL_IMPLEMENT_RTTI_1(DTransformKeyFrame, DKeyFrame);
	DUEL_IMPLEMENT_RTTI_1(DMorphKeyFrame, DKeyFrame);
//	DUEL_IMPLEMENT_RTTI(PoseKeyFrame, DKeyFrame);


	DKeyFrame::DKeyFrame( DAnimationTrack* parent, DReal timePos ) :
		mParent(parent),
		mTimePos(timePos)
	{

	}

	DKeyFrame::~DKeyFrame()
	{

	}

	DAnimationTrack* DKeyFrame::getParent() const
	{
		return mParent;
	}

	DReal DKeyFrame::getTimePosition() const 
	{
		return mTimePos;
	}


	DNumericKeyFrame::DNumericKeyFrame( DAnimationTrack* parent, DReal timePos ) :
		DKeyFrame(parent, timePos)
	{

	}

	DReal DNumericKeyFrame::getValue() const
	{
		return mValue;
	}

	void DNumericKeyFrame::setValue( DReal val )
	{
		mValue = val;
	}


	DTransformKeyFrame::DTransformKeyFrame( DAnimationTrack* parent, DReal timePos ) :
		DKeyFrame(parent, timePos)
	{

	}

	const DVector3& DTransformKeyFrame::getTranslate() const
	{
		return mTranslate;
	}

	const DVector3& DTransformKeyFrame::getScale() const
	{
		return mScale;
	}

	const DQuaternion& DTransformKeyFrame::getOrient() const
	{
		return mOrient;
	}

	void DTransformKeyFrame::setTranslate( const DVector3& trans )
	{
		mTranslate = trans;
	}

	void DTransformKeyFrame::setScale( const DVector3& scale )
	{
		mScale = scale;
	}

	void DTransformKeyFrame::setOrient( const DQuaternion& orient )
	{
		mOrient = orient;
	}

	DMatrix4 DTransformKeyFrame::getTransformMatrix()
	{
		DMatrix4 ret;
		ret.compose(mTranslate,mScale,mOrient);
		return ret;
	}


	DMorphKeyFrame::DMorphKeyFrame( DAnimationTrack* parent, DReal timePos ) : 
		DKeyFrame(parent, timePos)
	{

	}

	void DMorphKeyFrame::setPositionBuffer( const DRawBufferPtr& buf )
	{
		mPosBuf = buf;
	}

	DRawBufferPtr DMorphKeyFrame::getPositionBuffer()
	{
		return mPosBuf;
	}

	void DMorphKeyFrame::setNormalBuffer( const DRawBufferPtr& buf )
	{
		mNormBuf = buf;
	}

	Duel::DRawBufferPtr DMorphKeyFrame::getNormalBuffer()
	{
		return mNormBuf;
	}

// 
// 	PoseKeyFrame::PoseKeyFrame( DAnimationTrack* parent, DReal time ) :
// 		DKeyFrame(parent, time)
// 	{
// 
// 	}
// 
// 	void PoseKeyFrame::addPoseReference( uint16 poseIndex, DReal influence )
// 	{
// 		mPoseRefs.push_back(PoseRef(poseIndex, influence));
// 	}
// 
// 	void PoseKeyFrame::setPoseReference( uint16 poseIndex, DReal influence )
// 	{
// 		for (PoseRefList::iterator i = mPoseRefs.begin(); i != mPoseRefs.end(); ++i)
// 		{
// 			if (i->poseIndex == poseIndex)
// 			{
// 				i->influence = influence;
// 				return;
// 			}
// 		}
// 		// if we got here, we didn't find it
// 		addPoseReference(poseIndex, influence);
// 
// 	}
// 
// 	void PoseKeyFrame::removePoseReference( uint16 poseIndex )
// 	{
// 		for (PoseRefList::iterator i = mPoseRefs.begin(); i != mPoseRefs.end(); ++i)
// 		{
// 			if (i->poseIndex == poseIndex)
// 			{
// 				mPoseRefs.erase(i);
// 				return;
// 			}
// 		}
// 	}
// 
// 	void PoseKeyFrame::removeAllPoseReferences( void )
// 	{
// 		mPoseRefs.clear();
// 	}
// 
// 
// 	PoseKeyFrame::PoseKeyFrame( DAnimationTrack* parent, DReal timePos ) :
// 		DKeyFrame(parent, timePos)
// 	{
// 
// 	}
// 
// 	void PoseKeyFrame::addPoseReference( uint16 poseIndex, DReal influence )
// 	{
// 		mPoseRefMap[poseIndex] = PoseRef(poseIndex, influence);
// 	}
// 
// 	void PoseKeyFrame::setInfluence( uint16 poseIndex, DReal influence )
// 	{
// 		PoseRefMap::iterator i = mPoseRefMap.find(poseIndex);
// 		if (i != mPoseRefMap.end())
// 		{
// 			i->second.influence = influence;
// 		}
// 	}
// 
// 	void PoseKeyFrame::removePoseReference( uint16 poseIndex )
// 	{
// 		PoseRefMap::iterator i = mPoseRefMap.find(poseIndex);
// 		if (i != mPoseRefMap.end())
// 		{
// 			mPoseRefMap.erase(i);
// 		}
// 	}
// 
// 	void PoseKeyFrame::removeAllPoseReference()
// 	{
// 		mPoseRefMap.clear();
// 	}

}