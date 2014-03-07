//  [11/4/2012 OMEGA] created


#include "DuelCommon.h"
#include "DuelNode.h"
#include "DuelException.h"
#include "DuelKeyFrame.h"
#include "DuelVertexData.h"
#include "DuelAnimation.h"
#include "DuelAnimationTrack.h"

namespace Duel
{

	// Locally key frame search helper
	struct KeyFrameTimeLess
	{
		bool operator() (const DKeyFrame* kf, const DKeyFrame* kf2) const
		{
			return kf->getTimePosition() < kf2->getTimePosition();
		}
	};

	DUEL_IMPLEMENT_RTTI_1(DAnimationTrack, DObject);
	DUEL_IMPLEMENT_RTTI_1(DNumericAnimationTrack, DAnimationTrack);
	DUEL_IMPLEMENT_RTTI_1(DNodeAnimationTrack, DAnimationTrack);
	DUEL_IMPLEMENT_RTTI_1(DMorphAnimationTrack, DAnimationTrack);
//	DUEL_IMPLEMENT_RTTI(PoseAnimationTrack, DAnimationTrack);

	DAnimationTrack::DAnimationTrack( DAnimation* parent, uint16 handle ) :
		mParent(parent),
		mHandle(handle),
		mEasingFunc(EF_Linear),
		mbDirty(false)
	{

	}

	DAnimationTrack::~DAnimationTrack()
	{
		destroyAllKeyFrames();
	}

	DAnimation* DAnimationTrack::getParent() const
	{
		return mParent;
	}

	uint16 DAnimationTrack::getHandle() const
	{
		return mHandle;
	}

	uint16 DAnimationTrack::getKeyFrameCount() const
	{
		return mKeyFrames.size();
	}

	DReal DAnimationTrack::getKeyFramesAtTime( const DTimeIndex& timeIndex, DKeyFrame** keyFrame1, DKeyFrame** keyFrame2, uint16* firstKeyIndex /*= 0*/ ) const
	{
		// Parametric time
		// t1 = time of previous keyframe
		// t2 = time of next keyframe
		DReal t1, t2;

		DReal timePos = timeIndex.getTimePos();
		// Find first keyframe after or on current time
		KeyFrames::const_iterator i;
		if (timeIndex.hasKeyIndex())
		{
			// Global keyframe index available, map to local keyframe index directly.
			assert(timeIndex.getKeyIndex() < mKeyFrameIndexMap.size());
			i = mKeyFrames.begin() + mKeyFrameIndexMap[timeIndex.getKeyIndex()];
#ifdef DUEL_DEBUG
			DKeyFrame timeKey(0, timePos);
			if (i != std::lower_bound(mKeyFrames.begin(), mKeyFrames.end(), &timeKey, KeyFrameTimeLess()))
			{
				DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
					"Optimised key frame search failed",
					"Duel::DAnimationTrack::getKeyFramesAtTime");
			}
#endif
		}
		else
		{
			// wrap time
			DReal totalAnimationLength = mParent->getLength();
			assert(totalAnimationLength > 0.0f && "Invalid animation length!");

			while (timePos > totalAnimationLength && totalAnimationLength > 0.0f)
			{
				timePos -= totalAnimationLength;
			}

			// No global keyframe index, need to search with local keyframes.
			DKeyFrame timeKey(0, timePos);
			i = std::lower_bound(mKeyFrames.begin(), mKeyFrames.end(), &timeKey, KeyFrameTimeLess());
		}

		if (i == mKeyFrames.end())
		{
			// There is no keyframe after this time, wrap back to first
			*keyFrame2 = mKeyFrames.front();
			t2 = mParent->getLength() + (*keyFrame2)->getTimePosition();

			// Use last keyframe as previous keyframe
			--i;
		}
		else
		{
			*keyFrame2 = *i;
			t2 = (*keyFrame2)->getTimePosition();

			// Find last keyframe before or on current time
			if (i != mKeyFrames.begin() && timePos < (*i)->getTimePosition())
			{
				--i;
			}
		}

		// Fill index of the first key
		if (firstKeyIndex)
		{
			*firstKeyIndex = static_cast<uint16>(std::distance(mKeyFrames.begin(), i));
		}

		*keyFrame1 = *i;

		t1 = (*keyFrame1)->getTimePosition();

		if (t1 == t2)
		{
			// Same DKeyFrame (only one)
			return 0.0;
		}
		else
		{
			return (timePos - t1) / (t2 - t1);
		}
	}

	DKeyFrame* DAnimationTrack::createKeyFrame( DReal timePos )
	{
		DKeyFrame* kf = createKeyFrameImpl(timePos);
		KeyFrames::iterator i = std::upper_bound(mKeyFrames.begin(), mKeyFrames.end(), kf, KeyFrameTimeLess());
		mKeyFrames.insert(i,kf);
		mbDirty = true;
		mParent->setDirty();
		return kf;
	}

	DKeyFrame* DAnimationTrack::getKeyFrame( uint16 index ) const
	{
		assert(index < mKeyFrames.size());
		return mKeyFrames[index];
	}

	void DAnimationTrack::destroyKeyFrame( uint16 index )
	{
		assert(index < mKeyFrames.size());
		KeyFrames::iterator i = mKeyFrames.begin() + index;
		delete	(*i);
		mKeyFrames.erase(i);
		mParent->setDirty();
		mbDirty = true;
	}

	void DAnimationTrack::destroyAllKeyFrames( void )
	{
		KeyFrames::iterator i = mKeyFrames.begin();
		for (; i != mKeyFrames.end(); ++i)
		{
			delete (*i);
		}
		mKeyFrames.clear();
		mParent->setDirty();
		mbDirty = true;
	}

	void DAnimationTrack::fillKeyFrameTimeStamp( std::vector<DReal>& destList )
	{
		for (KeyFrames::iterator i = mKeyFrames.begin(); i != mKeyFrames.end(); ++i)
		{
			DReal timePos = (*i)->getTimePosition();

			std::vector<DReal>::iterator it =
				std::lower_bound(destList.begin(), destList.end(), timePos);
			if (it == destList.end() || *it != timePos)
			{
				destList.insert(it, timePos);
			}
		}
	}

	void DAnimationTrack::buildKeyFrameIndexMap( const std::vector<DReal>& keyFrameTimes )
	{
		// Pre-allocate memory
		mKeyFrameIndexMap.resize(keyFrameTimes.size() + 1);

		size_t i = 0, j = 0;
		while (j <= keyFrameTimes.size())
		{
			mKeyFrameIndexMap[j] = static_cast<uint16>(i);
			while (i < mKeyFrames.size() && mKeyFrames[i]->getTimePosition() <= keyFrameTimes[j])
				++i;
			++j;
		}
	}
//////////////////////////////////////////////////////////////////////////
	// easing functions
	// k: weight, current time/total time
	// return the applied blend weight
	DReal	LinearFunc(DReal k){
		return k;
	}
	DReal	DuelInFunc(DReal k)
	{
		return k*k;
	}
	DReal	DuelOutFunc(DReal k)
	{
		k -= 1;
		return -(k*k)+1;
	}
	DReal	DuelInOutFunc(DReal k)
	{
		if(k < 0.5f)
		{
			return DuelInFunc(k);
		}
		else
		{
			return DuelOutFunc(k);
		}
	}
	DReal	CubicInFunc(DReal k)
	{
		return k*k*k;
	}
	DReal	CubicOutFunc(DReal k)
	{
		k -= 1;
		return (k*k*k+1);
	}
	DReal	CubicInOutFunc(DReal k)
	{
		if(k < 0.5f)
		{
			return CubicInFunc(k);
		}
		else
		{
			return CubicOutFunc(k);
		}
	}
	DReal	QuadInFunc(DReal k)
	{
		return k*k*k*k;
	}
	DReal	QuadOutFunc(DReal k)
	{
		k -= 1;
		return -(k*k*k*k)+1;
	}
	DReal	QuadInOutFunc(DReal k)
	{
		if(k < 0.5f)
		{
			return QuadInFunc(k);
		}
		else
		{
			return QuadOutFunc(k);
		}
	}
	DReal	QuintInFunc(DReal k)
	{
		return k*k*k*k*k;
	}
	DReal	QuintOutFunc(DReal k)
	{
		k -= 1;
		return (k*k*k*k*k+1);
	}
	DReal	QuintInOutFunc(DReal k)
	{
		if(k < 0.5f)
		{
			return QuintInFunc(k);
		}
		else
		{
			return QuintOutFunc(k);
		}
	}
	
	DReal	getEasingWeight(DReal k, EasingFunction ef)
	{
		switch(ef)
		{
		case EF_Linear:
			return LinearFunc(k);
		case EF_DualIn:
			return DuelInFunc(k);
		case EF_DualOut:
			return DuelOutFunc(k);
		case EF_DualInOut:
			return DuelInOutFunc(k);
		case EF_CubicIn:
			return CubicInFunc(k);
		case EF_CubicOut:
			return CubicOutFunc(k);
		case EF_CubicInOut:
			return CubicInOutFunc(k);
		case EF_QuadIn:
			return QuadInFunc(k);
		case EF_QuadOut:
			return QuadOutFunc(k);
		case EF_QuadInOut:
			return QuadInOutFunc(k);
		case EF_QuintIn:
			return QuintInFunc(k);
		case EF_QuintOut:
			return QuintOutFunc(k);
		case EF_QuintInOut:
			return QuintInOutFunc(k);
		}
		return LinearFunc(k);
	}

	DNumericAnimationTrack::DNumericAnimationTrack( DAnimation* parent, uint16 handle ) :
		DAnimationTrack(parent, handle),
		mTarget(NULL)
	{

	}

	DNumericAnimationTrack::DNumericAnimationTrack( DAnimation* parent, uint16 handle, DReal* target) :
		DAnimationTrack(parent, handle),
		mTarget(target)
	{

	}

	void DNumericAnimationTrack::bindTargetValue( DReal* target )
	{
		mTarget = target;
	}

	DReal* DNumericAnimationTrack::getTargetValue()
	{
		return mTarget;
	}

	void DNumericAnimationTrack::getInterpolatedKeyFrame( const DTimeIndex& timeIndex, DKeyFrame* kf )
	{
		if (!DUEL_IS_EXACT_KIND(DNumericKeyFrame, kf))
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
				"Type error, expecting NumericKeyFrame",
				"Duel::NumericAnimationTrack::getInderpolatedKeyFrame");
		}
		DNumericKeyFrame* kret = static_cast<DNumericKeyFrame*>(kf);

		DKeyFrame *kBase1, *kBase2;
		DNumericKeyFrame *k1, *k2;
		uint16	firstKeyIndex;

		DReal t = this->getKeyFramesAtTime(timeIndex, &kBase1, &kBase2, &firstKeyIndex);
		k1 = static_cast<DNumericKeyFrame*>(kBase1);
		k2 = static_cast<DNumericKeyFrame*>(kBase2);

		if (t == 0.0)
		{
			// Just use k1
			kret->setValue(k1->getValue());
		}
		else
		{
			// Interpolate by t
			DReal w = getEasingWeight(t, mEasingFunc);
			
			kret->setValue(w*(k2->getValue() - k1->getValue())+k1->getValue());
		}
	}

	void DNumericAnimationTrack::apply( const DTimeIndex& timeIndex, DReal weight /*= 1.0*/, DReal scale /*= 1.0f*/ )
	{
		if (!mTarget)
		{
			return;
		}
		applyToValue(mTarget, timeIndex, weight, scale);
	}

	void DNumericAnimationTrack::applyToValue( DReal* target, const DTimeIndex& timeIndex, DReal weight /*= 1.0*/, DReal scale /*= 1.0f*/ )
	{
		// Nothing to do if no keyframes or zero weight, scale
		if (mKeyFrames.empty() || !weight || !scale || !target)
			return;

		DNumericKeyFrame kf(0, timeIndex.getTimePos());
		getInterpolatedKeyFrame(timeIndex, &kf);
		// add to existing. Weights are not relative, but treated as
		// absolute multipliers for the animation
		DReal val = kf.getValue() * (weight * scale);

		*target = val;
	}

	DKeyFrame* DNumericAnimationTrack::createKeyFrameImpl( DReal time )
	{
		return new DNumericKeyFrame(this, time);
	}

	//////////////////////////////////////////////////////////////////////////
	DNodeAnimationTrack::DNodeAnimationTrack( DAnimation* parent, uint16 handle ) :
		DAnimationTrack(parent, handle),
		mTargetNode(NULL),
		mIMode(IM_Linear),
		mRIMode(RIM_Linear)
	{

	}

	DNodeAnimationTrack::DNodeAnimationTrack( DAnimation* parent, uint16 handle, DNode* targetNode, InterpolationMode im /*= IM_LINEAR*/, RotationInterpolationMode rim /*= RIM_LINEAR*/ ) :
		DAnimationTrack(parent, handle),
		mTargetNode(targetNode),
		mIMode(im),
		mRIMode(rim)
	{

	}


	void DNodeAnimationTrack::getInterpolatedKeyFrame( const DTimeIndex& timeIndex, DKeyFrame* kf )
	{
		if (!DUEL_IS_EXACT_KIND(DTransformKeyFrame, kf))
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
				"Type error, expecting NodeKeyFrame",
				"Duel::NodeAnimationTrack::getInderpolatedKeyFrame");
		}
		DTransformKeyFrame* kret = static_cast<DTransformKeyFrame*>(kf);

		DKeyFrame *kBase1, *kBase2;
		DTransformKeyFrame *k1, *k2;
		uint16	firstKeyIndex;

		DReal t = this->getKeyFramesAtTime(timeIndex, &kBase1, &kBase2, &firstKeyIndex);
		k1 = static_cast<DTransformKeyFrame*>(kBase1);
		k2 = static_cast<DTransformKeyFrame*>(kBase2);

		if (t == 0.0)
		{
			// Just use k1
			kret->setOrient(k1->getOrient());
			kret->setTranslate(k1->getTranslate());
			kret->setScale(k1->getScale());
		}
		else
		{
			DVector3	base;
			switch(mIMode)
			{
			case IM_Linear:
				// Interpolate linearly
				// Rotation
				// Interpolate to nearest rotation if mUseShortestRotationPath set
				t = getEasingWeight(t,mEasingFunc);
				if (mRIMode == RIM_Linear)
				{
					kret->setOrient( DQuaternion::nLerp(t, k1->getOrient(),
						k2->getOrient(), mbNearestRot) );
				}
				else //if (rim == DAnimation::RIM_SPHERICAL)
				{
					kret->setOrient( DQuaternion::Slerp(t, k1->getOrient(),
						k2->getOrient(), mbNearestRot) );
				}

				// Translation
				base = k1->getTranslate();
				kret->setTranslate( base + ((k2->getTranslate() - base) * t) );

				// Scale
				base = k1->getScale();
				kret->setScale( base + ((k2->getScale() - base) * t) );
				break;
			case IM_Spline:
				// update spline
				mPositionSpline.update();
				mScaleSpline.update();
				mRotationSpline.update();

				kret->setOrient(mRotationSpline.interpolateFrom(firstKeyIndex, t, mbNearestRot));
				kret->setTranslate(mPositionSpline.interpolateFrom(firstKeyIndex, t));
				kret->setScale(mScaleSpline.interpolateFrom(firstKeyIndex,t));
				break;

			}
		}
	}

	void DNodeAnimationTrack::apply( const DTimeIndex& timeIndex, DReal weight /*= 1.0*/, DReal scale /*= 1.0f*/ )
	{
		if (!mTargetNode)
		{
			return;
		}
		applyToNode(mTargetNode, timeIndex, weight, scale);
	}

	void DNodeAnimationTrack::bindTargetNode( DNode* target )
	{
		mTargetNode = target;
	}

	DNode* DNodeAnimationTrack::getTargetNode()
	{
		return mTargetNode;
	}

	void DNodeAnimationTrack::setNearestRotation( bool flag )
	{
		mbNearestRot = flag;
	}

	bool DNodeAnimationTrack::isNearestRotation() const
	{
		return mbNearestRot;
	}

	void DNodeAnimationTrack::applyToNode( DNode* node, const DTimeIndex& timeIndex, DReal weight, DReal scale /*= 1.0f*/ )
	{
		// Nothing to do if no keyframes or zero weight or no node
		if (mKeyFrames.empty() || !weight || !node)
			return;

		DTransformKeyFrame kf(0, timeIndex.getTimePos());
		getInterpolatedKeyFrame(timeIndex, &kf);

		// add to existing. Weights are not relative, but treated as absolute multipliers for the animation
		DVector3 trans = kf.getTranslate() * weight * scale;
		node->translate(trans);

		// interpolate between no-rotation and full rotation, to point 'weight', so 0 = no rotate, 1 = full
		DQuaternion rotate;
		
		if (mRIMode == RIM_Linear)
		{
			rotate = DQuaternion::nLerp(weight, DQuaternion::IDENTITY, kf.getOrient(), mbNearestRot);
		}
		else //if (rim == DAnimation::RIM_SPHERICAL)
		{
			rotate = DQuaternion::Slerp(weight, DQuaternion::IDENTITY, kf.getOrient(), mbNearestRot);
		}
		node->rotate(rotate);

		DVector3 kScale = kf.getScale();
		// Not sure how to modify scale for cumulative anims... leave it alone
		//scale = ((DVector3::UNIT_SCALE - kf.getScale()) * weight) + DVector3::UNIT_SCALE;
		if (kScale != DVector3::UNIT_SCALE)
		{
			if (scale != 1.0f)
				kScale = DVector3::UNIT_SCALE + (kScale - DVector3::UNIT_SCALE) * scale;
			else if (weight != 1.0f)
				kScale = DVector3::UNIT_SCALE + (kScale - DVector3::UNIT_SCALE) * weight;
		}
		node->scale(kScale);
	}

	DKeyFrame* DNodeAnimationTrack::createKeyFrameImpl( DReal time )
	{
		return new DTransformKeyFrame(this, time);
	}

	void DNodeAnimationTrack::buildInterpolationSplines()
	{
		// do not update automatically, we do at end
		mPositionSpline.setAutoUpdate(false);
		mScaleSpline.setAutoUpdate(false);
		mRotationSpline.setAutoUpdate(false);

		mPositionSpline.clear();
		mScaleSpline.clear();
		mRotationSpline.clear();

		KeyFrames::iterator i, iend = mKeyFrames.end();
		for (i = mKeyFrames.begin(); i != iend; ++i)
		{
			DTransformKeyFrame* kf = static_cast<DTransformKeyFrame*>(*i);
			mPositionSpline.addPoint(kf->getTranslate());
			mScaleSpline.addPoint(kf->getScale());
			mRotationSpline.addPoint(kf->getOrient());
		}

		mPositionSpline.update();
		mScaleSpline.update();
		mRotationSpline.update();
	}

	void DNodeAnimationTrack::setInterpolationMode( InterpolationMode im )
	{
		mIMode = im;
	}

	InterpolationMode DNodeAnimationTrack::getInterpolationMode()
	{
		return mIMode;
	}

	void DNodeAnimationTrack::setRoationInterpolationMode( RotationInterpolationMode rim )
	{
		mRIMode = rim;
	}

	RotationInterpolationMode DNodeAnimationTrack::getRotationInterpolationMode()
	{
		return mRIMode;
	}

	DKeyFrame* DNodeAnimationTrack::createKeyFrame( DReal timePos )
	{
		DKeyFrame* ret = DAnimationTrack::createKeyFrame(timePos);
		buildInterpolationSplines();
		return ret;
	}

	void DNodeAnimationTrack::destroyKeyFrame( uint16 index )
	{
		DAnimationTrack::destroyKeyFrame(index);
		buildInterpolationSplines();
	}

	void DNodeAnimationTrack::destroyAllKeyFrames( void )
	{
		DAnimationTrack::destroyAllKeyFrames();
		buildInterpolationSplines();
	}

	void DNodeAnimationTrack::optimise( void )
	{
		if (mbDirty)
		{
			// Eliminate duplicate keyframes from 2nd to penultimate keyframe
			// NB only eliminate middle keys from sequences of 5+ identical keyframes
			// since we need to preserve the boundary keys in place, and we need
			// 2 at each end to preserve tangents for spline interpolation
			DVector3 lasttrans = DVector3::ZERO;
			DVector3 lastscale = DVector3::ZERO;
			DQuaternion lastorientation;
			KeyFrames::iterator i = mKeyFrames.begin();
			DRadian quatTolerance(1e-3f);
			std::list<uint16> removeList;
			uint16 k = 0;
			uint16 dupKfCount = 0;
			for (; i != mKeyFrames.end(); ++i, ++k)
			{
				DTransformKeyFrame* kf = static_cast<DTransformKeyFrame*>(*i);
				DVector3 newtrans = kf->getTranslate();
				DVector3 newscale = kf->getScale();
				DQuaternion neworientation = kf->getOrient();
				// Ignore first keyframe; now include the last keyframe as we eliminate
				// only k-2 in a group of 5 to ensure we only eliminate middle keys
				if (i != mKeyFrames.begin() &&
					newtrans == lasttrans &&
					newscale == lastscale &&
					neworientation.equals(lastorientation, quatTolerance))
				{
					++dupKfCount;

					// 4 indicates this is the 5th duplicate keyframe
					if (dupKfCount == 4)
					{
						// remove the 'middle' keyframe
						removeList.push_back(k-2);
						--dupKfCount;
					}
				}
				else
				{
					// reset
					dupKfCount = 0;
					lasttrans = newtrans;
					lastscale = newscale;
					lastorientation = neworientation;
				}
			}

			// Now remove keyframes, in reverse order to avoid index revocation
			std::list<uint16>::reverse_iterator r = removeList.rbegin();
			for (; r!= removeList.rend(); ++r)
			{
				destroyKeyFrame(*r);
			}

			mbDirty = false;
		}

	}

	//////////////////////////////////////////////////////////////////////////
	DMorphAnimationTrack::DMorphAnimationTrack( DAnimation* parent, uint16 handle ) :
		DAnimationTrack(parent, handle)
	{

	}

	DMorphAnimationTrack::DMorphAnimationTrack( DAnimation* parent, uint16 handle, const DVertexData& target ) :
		DAnimationTrack(parent, handle),
		mTarget(target)
	{

	}

	void DMorphAnimationTrack::setTargetData( DVertexData target )
	{
		mTarget = target;
	}

	DVertexData DMorphAnimationTrack::getTargetData()
	{
		return mTarget;
	}

	void DMorphAnimationTrack::apply( const DTimeIndex& timeIndex, DReal weight /*= 1.0*/, DReal scale /*= 1.0f*/ )
	{
		applyToData(mTarget, timeIndex, weight, scale);
	}


	void DMorphAnimationTrack::applyToData( DVertexData& target, const DTimeIndex& timeIndex, DReal weight /*= 1.0*/, DReal scale /*= 1.0f*/ )
	{
		//////////////////////////////////////////////////////////////////////////
		//  [1/16/2014 OMEGA]
		// TODO 重新修订. 动画中带有normal.
		//////////////////////////////////////////////////////////////////////////
		//  [10/24/2013 OMEGA]
		// TODO : 遍历declaration 找到pos
// 		DVertexDeclaration::VertexElementIterator i = target.getDeclaration()->getVertexElementIterator();
// 		bool hasPosElem = false;
// 		bool hasNormElem = false;
// 		DVertexElement posElem;
// 		DVertexElement normElem;
// 		while (i.hasMoreElements())
// 		{ 
// 			DVertexElement curElem = i.getNext();
// 			if (!hasPosElem)
// 			{
// 				if (curElem.getSemantic() == VES_Position)
// 				{
// 					posElem = curElem;
// 					hasPosElem = true;
// 				}
// 			}
// 			if (!hasNormElem)
// 			{
// 				if (curElem.getSemantic() == VES_Normal)
// 				{
// 					normElem = curElem;
// 					hasNormElem = true;
// 				}
// 			}
// 		}
// 		if (!hasPosElem)
// 		{
// 			return;
// 		}
// 
// 		DVertexBufferPtr posBuf = target.getBufferStreams()->getStream(posElem.getSource());
// 		DVertexBufferPtr normBuf = target.getBufferStreams()->getStream(normElem.getSource());
//  		if (mKeyFrames.empty() || !(posBuf))
// 		{
//  		 	return;
// 		}
//  		// Get keyframes
//  		DKeyFrame *kf1, *kf2;
//  		DReal t = getKeyFramesAtTime(timeIndex, &kf1, &kf2);
//  		 
// 		
// 		// here is the trick, do not modify the frame data, only process on the shadow buffer(if the buffer uses)
//  		float* posDest = NULL;
// 		float* normDest = NULL;
// 		if (posBuf == normBuf)
// 		{
// 			posDest = (float*)posBuf->lock(HBL_NoOverwrite);
// 			normDest = posDest;
// 		}
// 		else
// 		{
// 			posDest = (float*)posBuf->lock(HBL_NoOverwrite);
// 			if (normBuf != NULL)
// 			{
// 				normDest = (float*)normBuf->lock(HBL_NoOverwrite);
// 			}
// 		}
//  		posDest += posElem.getOffset();
//  		size_t posPitch = posBuf->getVertexSize();
// 		size_t normPitch = normBuf->getVertexSize();
//  
//  		MorphKeyFrame* kmf1 = static_cast<MorphKeyFrame*>(kf1);
//  		MorphKeyFrame* kmf2 = static_cast<MorphKeyFrame*>(kf2);
//  
//  		float*  pSrc1 = (float*)kmf1->getPositionBuffer()->lock(HBL_ReadOnly);
//  		float*  pSrc2 = (float*)kmf2->getPositionBuffer()->lock(HBL_ReadOnly);
//  
//  		size_t	vertexCount = posBuf->getVerticesCount();
//  	
//  
//  		// TODO:这部分可以用一些手法优化
// 		// TODO:使用新的格式, 顶点动画上带有normal.
//  		// now do the software blend.
//  		// 3 float as position. 
//  		t = getEasingWeight(t, mEasingFunc);
//  		for (uint32 i = 0; i < vertexCount; ++i)
//  		{
//  			// x
//  			*posDest++ = *pSrc1 + t * (*pSrc2 - *pSrc1);
//  			++pSrc1; ++pSrc2;
//  			// y
//  			*posDest++ = *pSrc1 + t * (*pSrc2 - *pSrc1);
//  			++pSrc1; ++pSrc2;
//  			// z
//  			*posDest++ = *pSrc1 + t * (*pSrc2 - *pSrc1);
//  			++pSrc1; ++pSrc2;
//  
//  			posDest = (float*)((char*)posDest + posPitch);
//  		}
//  		kmf1->getPositionBuffer()->unlock();
//  		kmf2->getPositionBuffer()->unlock();
//  		posBuf->unlock();
	}

	DKeyFrame* DMorphAnimationTrack::createKeyFrameImpl( DReal time )
	{
		return new DMorphKeyFrame(this, time);
	}

// 
// 	PoseAnimationTrack::PoseAnimationTrack( DAnimation* parent, uint16 handle ) :
// 		DAnimationTrack(parent, handle)
// 	{
// 
// 	}
// 
// 	DKeyFrame* PoseAnimationTrack::createKeyFrameImpl( DReal time )
// 	{
// 		return new PoseKeyFrame(this, time);
// 	}

}
