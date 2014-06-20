//  [11/4/2012 OMEGA] created

#ifndef  _DUELANIMATIONTRACK_H_
#define  _DUELANIMATIONTRACK_H_

#include "DuelCommon.h"
#include "DuelVertexData.h"

namespace Duel
{

	// easing function, provide smooth animation.
	enum EasingFunction
	{
		EF_Linear,
		EF_DualIn,
		EF_DualOut,
		EF_DualInOut,
		EF_CubicIn,
		EF_CubicOut,
		EF_CubicInOut,
		EF_QuadIn,
		EF_QuadOut,
		EF_QuadInOut,
		EF_QuintIn,
		EF_QuintOut,
		EF_QuintInOut
	};

	/** Time index object used to search keyframe at the given position.
    */
    class DUEL_API DTimeIndex
    {
    public:
        /** Construct time index object by the given time position.
        */
        DTimeIndex(DReal timePos)
            : mTimePos(timePos)
            , mKeyIndex(INVALID_KEY_INDEX)
        {
        }

        /** Construct time index object by the given time position and
            global keyframe index.
        @note In normally, you don't need to use this constructor directly, use
            DAnimation::_getTimeIndex instead.
        */
        DTimeIndex(DReal timePos, uint32 keyIndex)
            : mTimePos(timePos)
            , mKeyIndex(keyIndex)
        {
        }

        bool hasKeyIndex(void) const
        {
            return mKeyIndex != INVALID_KEY_INDEX;
        }

        DReal getTimePos(void) const
        {
            return mTimePos;
        }

        uint32 getKeyIndex(void) const
        {
            return mKeyIndex;
        }
    protected:
        /** The time position (in relation to the whole animation sequence)
        */
        DReal		mTimePos;
        /** The global keyframe index (in relation to the whole animation sequence)
            that used to convert to local keyframe index, or INVALID_KEY_INDEX which
            means global keyframe index unavailable, and then slight slow method will
            used to search local keyframe index.
        */
        uint32		mKeyIndex;

        /** Indicate it's an invalid global keyframe index.
        */
        static const uint32 INVALID_KEY_INDEX = (uint32)-1;

    };

	// A 'track' in an animation sequence, i.e. a sequence of keyframes which affect a
	// certain type of animable object.
	class DUEL_API DAnimationTrack : public DObject
	{
	DUEL_DECLARE_RTTI(DAnimationTrack)
	public:
		// this class should not be created manually, instead, it should be created
		// by the DAnimation.
		DAnimationTrack(DAnimation* parent, uint16 handle);
		virtual ~DAnimationTrack();
		// get the creator of the track.
		DAnimation*			getParent() const;
		// query the properties of the track.
		uint16				getHandle() const;
		uint16				getKeyFrameCount() const;

		/** Gets the 2 DKeyFrame objects which are active at the time given, and the blend value between them.
        @remarks
            At any point in time  in an animation, there are either 1 or 2 keyframes which are 'active',
            1 if the time index is exactly on a keyframe, 2 at all other times i.e. the keyframe before
            and the keyframe after.
        @par
            This method returns those keyframes given a time index, and also returns a parametric
            value indicating the value of 't' representing where the time index falls between them.
            E.g. if it returns 0, the time index is exactly on keyFrame1, if it returns 0.5 it is
            half way between keyFrame1 and keyFrame2 etc.
        @param timeIndex The time index.
        @param keyFrame1 Pointer to a DKeyFrame pointer which will receive the pointer to the 
            keyframe just before or at this time index.
        @param keyFrame2 Pointer to a DKeyFrame pointer which will receive the pointer to the 
            keyframe just after this time index. 
        @param firstKeyIndex Pointer to an uint16 which, if supplied, will receive the 
            index of the 'from' keyframe in case the caller needs it.
        @returns Parametric value indicating how far along the gap between the 2 keyframes the timeIndex
            value is, e.g. 0.0 for exactly at 1, 0.25 for a quarter etc. By definition the range of this 
            value is:  0.0 <= returnValue < 1.0 .
        */
		virtual DReal		getKeyFramesAtTime(const DTimeIndex& timeIndex, DKeyFrame** keyFrame1, DKeyFrame** keyFrame2,
			uint16* firstKeyIndex = 0) const;

		/** Creates a new DKeyFrame and adds it to this animation at the given time index.
        @remarks
            It is better to create KeyFrames in time order. Creating them out of order can result 
            in expensive reordering processing. Note that a DKeyFrame at time index 0.0 is always created
            for you, so you don't need to create this one, just access it using getKeyFrame(0);
        @param timePos The time from which this DKeyFrame will apply.
        */
        virtual DKeyFrame*	createKeyFrame(DReal timePos);

		virtual DKeyFrame*	getKeyFrame(uint16 index) const;

		/** Removes a DKeyFrame by it's index. */
		virtual void		destroyKeyFrame(uint16 index);
		/** Removes all the KeyFrames from this track. */
		virtual void		destroyAllKeyFrames(void);


		// set/get easing function.
		virtual	void			setEasingFunction(EasingFunction ef) { mEasingFunc = ef; }
		virtual	EasingFunction	getEasingFunction() { return mEasingFunc; }



		/** Gets a DKeyFrame object which contains the interpolated transforms at the time index specified.
        @remarks
            The DKeyFrame objects held by this class are transformation snapshots at 
            discrete points in time. Normally however, you want to interpolate between these
            keyframes to produce smooth movement, and this method allows you to do this easily.
            In animation terminology this is called 'tweening'. 
        @param timeIndex The time (in relation to the whole animation sequence)
        @param kf Keyframe object to store results
        */
        virtual void		getInterpolatedKeyFrame(const DTimeIndex& timeIndex, DKeyFrame* kf) = 0;

        /** Applies an animation track to the designated target.
        @param timeIndex The time position in the animation to apply.
        @param weight The influence to give to this track, 1.0 for full influence, less to blend with
          other animations.
	    @param scale The scale to apply to translations and scalings, useful for 
			adapting an animation to a different size target.
        */
        virtual void		apply(const DTimeIndex& timeIndex, DReal weight = 1.0, DReal scale = 1.0f) = 0;

		/** Optimise the current track by removing any duplicate keyframes. */
		virtual void		optimise(void) { mbDirty = false; }

		// used to traverse all keyframes
		typedef	std::vector<DKeyFrame*>		KeyFrames;
		typedef	VectorIterator<KeyFrames>	KeyFrameIterator;
		KeyFrameIterator	getKeyFrameIterator() { return KeyFrameIterator(mKeyFrames.begin(), mKeyFrames.end()); }

	protected:
		friend	class DAnimation;
		// this method should only be called by the DAnimation.
		// fill the current DKeyFrame time stamp into the given list
		virtual	void		fillKeyFrameTimeStamp(std::vector<DReal>& destList);
		// this method should only be called by the DAnimation.
		// method to build keyframe time index map to translate global lower
		// bound index to local lower bound index. provided keyFrameTimes can be different
		// to those created KeyFrames, this method provide functionality for reusing current keyframes
		// in different time line, and should be called before using the new defined time line.
		virtual	void		buildKeyFrameIndexMap(const std::vector<DReal>& keyFrameTimes);

		/// Create a keyframe implementation - must be overridden
		virtual DKeyFrame*	createKeyFrameImpl(DReal time) = 0;

		KeyFrames			mKeyFrames;
		DAnimation*			mParent;
		uint16				mHandle;

		// easing function.
		EasingFunction		mEasingFunc;
		// flag indicate we have made changes to this track.
		bool				mbDirty;

		/// Map used to translate global keyframe time lower bound index to local lower bound index
		typedef std::vector<uint16>		KeyFrameIndexMap;
		KeyFrameIndexMap	mKeyFrameIndexMap;

	};


	class DUEL_API DNumericAnimationTrack : public DAnimationTrack
	{
	DUEL_DECLARE_RTTI(DNumericAnimationTrack)
	public:
		DNumericAnimationTrack(DAnimation* parent, uint16 handle);
		DNumericAnimationTrack(DAnimation* parent, uint16 handle, DReal* target);

		// set the target value 
		virtual	void	bindTargetValue(DReal* target);
		virtual	DReal*	getTargetValue();
		
		// override:DAnimationTrack-----------------------------------------------------
		void			getInterpolatedKeyFrame(const DTimeIndex& timeIndex, DKeyFrame* kf);
		// override:DAnimationTrack------------------------------------------------------
		void			apply(const DTimeIndex& timeIndex, DReal weight = 1.0, DReal scale = 1.0f);

		// apply to a value, the value can be any value.
		virtual	void	applyToValue(DReal* target, const DTimeIndex& timeIndex, DReal weight = 1.0, DReal scale = 1.0f);

	protected:
		// override:DAnimationTrack-----------------------------------------------------
		DKeyFrame*		createKeyFrameImpl(DReal time);

		// target value.
		DReal*			mTarget;
	};

	enum InterpolationMode
	{
		// values are interpolated along straight lines. 
		IM_Linear,
		// values are interpolated along a spline.
		IM_Spline
	};
	enum RotationInterpolationMode
	{
		// Values are interpolated linearly. This is faster but does not 
		// necessarily give a completely accurate result.
		RIM_Linear,
		// Values are interpolated spherically. This is more accurate but
		// has a higher cost.
		RIM_Spherical
	};

	// Specialised DAnimationTrack for dealing with node transforms.
	class DUEL_API DNodeAnimationTrack : public DAnimationTrack
	{
	DUEL_DECLARE_RTTI(DNodeAnimationTrack)
	public:
		DNodeAnimationTrack(DAnimation* parent, uint16 handle);
		DNodeAnimationTrack(DAnimation* parent, uint16 handle, DNode* targetNode, 
			InterpolationMode im = IM_Linear, RotationInterpolationMode rim = RIM_Linear);

		// override:DAnimationTrack-----------------------------------------------------
		void			getInterpolatedKeyFrame(const DTimeIndex& timeIndex, DKeyFrame* kf);
		// override:DAnimationTrack------------------------------------------------------
		void			apply(const DTimeIndex& timeIndex, DReal weight = 1.0, DReal scale = 1.0f);
		// override:DAnimationTrack------------------------------------------------------
		DKeyFrame*		createKeyFrame(DReal timePos);
		// override:DAnimationTrack------------------------------------------------------
		void			destroyKeyFrame(uint16 index);
		// override:DAnimationTrack------------------------------------------------------
		void			destroyAllKeyFrames(void);

		// set the node to be updated by this track.
		void			bindTargetNode(DNode* target);
		DNode*			getTargetNode();

		// set/get whether we are using nearest rotation interpolate.
		void			setNearestRotation(bool flag);
		bool			isNearestRotation() const;

		void						setInterpolationMode(InterpolationMode im);
		InterpolationMode			getInterpolationMode();
		void						setRoationInterpolationMode(RotationInterpolationMode rim);
		RotationInterpolationMode	getRotationInterpolationMode();
		// apply to a node, the node can be any node.
		virtual	void	applyToNode(DNode* node, const DTimeIndex& timeIndex, DReal weight, DReal scale = 1.0f);

		// override:DAnimationTrack-----------------------------------------------------
		void			optimise(void);

	protected:
		// override:DAnimationTrack-------------------------------------------
		DKeyFrame*		createKeyFrameImpl(DReal time);
		// called to build splines
		virtual	void	buildInterpolationSplines();
		
		DNode*			mTargetNode;
		DSpline			mPositionSpline;
		DSpline			mScaleSpline;
		RotationSpline	mRotationSpline;

		// interpolation mode on points.
		InterpolationMode			mIMode;
		// interpolation mode on orientations
		RotationInterpolationMode	mRIMode;

		bool			mbNearestRot;
	};


	/** Type of vertex animation.
		Vertex animation comes in 2 types, morph and pose. The reason
		for the 2 types is that we have 2 different potential goals - to encapsulate
		a complete, flowing morph animation with multiple keyframes (a typical animation,
		but implemented by having snapshots of the vertex data at each keyframe), 
		or to represent a single pose change, for example a facial expression. 
		Whilst both could in fact be implemented using the same system, we choose
		to separate them since the requirements and limitations of each are quite
		different.
	@par
		Morph animation is a simple approach where we have a whole series of 
		snapshots of vertex data which must be interpolated, e.g. a running 
		animation implemented as morph targets. Because this is based on simple
		snapshots, it's quite fast to use when animating an entire mesh because 
		it's a simple linear change between keyframes. However, this simplistic 
		approach does not support blending between multiple morph animations. 
		If you need animation blending, you are advised to use skeletal animation
		for full-mesh animation, and pose animation for animation of subsets of 
		meshes or where skeletal animation doesn't fit - for example facial animation.
		For animating in a vertex shader, morph animation is quite simple and 
		just requires the 2 vertex buffers (one the original position buffer) 
		of absolute position data, and an interpolation factor. Each track in 
		a morph animation refrences a unique set of vertex data.
	@par
		Pose animation is more complex. Like morph animation each track references
		a single unique set of vertex data, but unlike morph animation, each 
		keyframe references 1 or more 'poses', each with an influence level. 
		A pose is a series of offsets to the base vertex data, and may be sparse - ie it
		may not reference every vertex. Because they're offsets, they can be 
		blended - both within a track and between animations. This set of features
		is very well suited to facial animation.
	@par
		For example, let's say you modelled a face (one set of vertex data), and 
		defined a set of poses which represented the various phonetic positions 
		of the face. You could then define an animation called 'SayHello', containing
		a single track which referenced the face vertex data, and which included 
		a series of keyframes, each of which referenced one or more of the facial 
		positions at different influence levels - the combination of which over
		time made the face form the shapes required to say the word 'hello'. Since
		the poses are only stored once, but can be referenced may times in 
		many animations, this is a very powerful way to build up a speech system.
	@par
		The downside of pose animation is that it can be more difficult to set up.
		Also, since it uses more buffers (one for the base data, and one for each
		active pose), if you're animating in hardware using vertex shaders you need
		to keep an eye on how many poses you're blending at once. You define a
		maximum supported number in your vertex program definition, see the 
		includes_pose_animation material script entry. 
	@par
		So, by partitioning the vertex animation approaches into 2, we keep the
		simple morph technique easy to use, whilst still allowing all 
		the powerful techniques to be used. Note that morph animation cannot
		be blended with other types of vertex animation (pose animation or other
		morph animation); pose animation can be blended with other pose animation
		though, and both types can be combined with skeletal animation. Also note
		that all morph animation can be expressed as pose animation, but not vice
		versa.
	*/
	class DUEL_API DMorphAnimationTrack : public DAnimationTrack
	{
	DUEL_DECLARE_RTTI(DMorphAnimationTrack)
	public:
		DMorphAnimationTrack(DAnimation* parent, uint16 handle);
		DMorphAnimationTrack(DAnimation* parent, uint16 handle, const DVertexData& target);

		// override:DAnimationTrack------------------------------------------------------
		/** This method in fact does nothing, since interpolation is not performed
			inside the keyframes for this type of track. 
		*/
		void				getInterpolatedKeyFrame(const DTimeIndex& timeIndex, DKeyFrame* kf) { (void)timeIndex; (void)kf;}
		// override:DAnimationTrack------------------------------------------------------
		virtual	void		apply(const DTimeIndex& timeIndex, DReal weight = 1.0, DReal scale = 1.0f);
		
		// set the target, must set both set the declaration to extract position element.
		void				setTargetData(DVertexData target);
		DVertexData			getTargetData();
		
		// As the 'apply' method but applies to specified DVertexData instead of 
		// associated data.  we always assume the position located in the source 0
		// of the target vertex data.
		virtual void		applyToData(DVertexData& target, 
			const DTimeIndex& timeIndex, DReal weight = 1.0, DReal scale = 1.0f);

	protected:
		// override:DAnimationTrack-------------------------------------------
		DKeyFrame*			createKeyFrameImpl(DReal time);

		DVertexData			mTarget;
	};

// 	class DUEL_API PoseAnimationTrack : public DAnimationTrack
// 	{
// 	DUEL_DECLARE_RTTI(PoseAnimationTrack)
// 	public:
// 		PoseAnimationTrack(DAnimation* parent, uint16 handle);
// 		// override:DAnimationTrack------------------------------------------------------
// 		/** This method in fact does nothing, since interpolation is not performed
// 			inside the keyframes for this type of track. 
// 		*/
// 		void				getInterpolatedKeyFrame(const TimeIndex& timeIndex, DKeyFrame* kf) { (void)timeIndex; (void)kf;}
// 
// 	protected:
// 		// override:DAnimationTrack-------------------------------------------
// 		DKeyFrame*			createKeyFrameImpl(DReal time);
// 
// 
// 	};
}

#endif



