//  [11/4/2012 OMEGA] created


#ifndef _DUELANIMATION_H_
#define _DUELANIMATION_H_

#include "DuelCommon.h"
#include "DuelAnimationTrack.h"
#include "DuelSkeleton.h"

namespace Duel
{


	// a class defines a collection of actions.
	class DUEL_API DAnimation
	{
	public:
		DAnimation(const DString& name, DReal length = 1.0f);
		virtual ~DAnimation();
		// set/get the total length of the animation.
		void					setLength(DReal length);
		DReal					getLength() const;
		
		// query the name of the animation.
		const DString&			getName() const;


		// create a node animation track for this animation, 
		// the handle of the track is generated by the animation.
		DNodeAnimationTrack*		createNodeTrack();		
		// create a numeric animation track for this animation
		// the handle of the track is generated by the animation.
		DNumericAnimationTrack*	createNumericTrack();
		// create a morph animation track for this animation,
		// the handle of the track is generated by the animation.
		DMorphAnimationTrack*	createMorphTrack();
		// create a node animation track for this animation,
		// the handle MUST be unique within this animation.
		// if specified handle existed, current one will be returned.
		DNodeAnimationTrack*		createNodeTrack(uint16 handle);		
		// create a numeric animation track for this animation
		// the handle MUST be unique within this animation.
		// if specified handle existed, current one will be returned.
		DNumericAnimationTrack*	createNumericTrack(uint16 handle);
		// create a morph animation track for this animation,
		// the handle MUST be unique within this animation.
		// if specified handle existed, current one will be returned.
		DMorphAnimationTrack*	createMorphTrack(uint16 handle);
		// create a node animation track with binding target for this animation 
		// the handle MUST be unique within this animation.
		DNodeAnimationTrack*		createNodeTrack(uint16 handle, DNode* target);		
		// create a numeric animation track with binding target for this animation 
		// the handle MUST be unique within this animation.
		DNumericAnimationTrack*	createNumericTrack(uint16 handle, DReal* target);
		// create a morph animation track with binding target for this animation,
		// the handle MUST be unique within this animation.
		DMorphAnimationTrack*	createMorphTrack(uint16 handle, const DVertexData& target);

		// query functions
		size_t					getNodeTrackCount() const;
		size_t					getNumericTrackCount() const;
		size_t					getMorphTrackCount() const;
		// get track using specified handle
		DNodeAnimationTrack*		getNodeTrack(uint16 handle) const;
		DNumericAnimationTrack*	getNumericTrack(uint16 handle) const;
		DMorphAnimationTrack*	getMorphTrack(uint16 handle) const;
		// query existence.
		bool					hasNodeTrack(uint16 handle);
		bool					hasNumericTrack(uint16 handle);
		bool					hasMorphTrack(uint16 handle);

		// remove specified track.
		void					destroyNodeTrack(uint16 handle);
		void					destroyNumericTrack(uint16 handle);
		void					destroyMorphTrack(uint16 handle);
		
		// remove all track
		void					destroyAllNodeTracks();
		void					destroyAllNumericTracks();
		void					destroyAllMorphTracks();
		// use handle as the map key to find specified track.
		typedef	std::map<uint16, DNodeAnimationTrack*>		NodeTrackMap;
		typedef	std::map<uint16, DNumericAnimationTrack*>	NumericTrackMap;
		typedef	std::map<uint16, DMorphAnimationTrack*>		MorphTrackMap;
		typedef	MapIterator<NodeTrackMap>		NodeTrackIterator;
		typedef	MapIterator<NumericTrackMap>	NumericTrackIterator;
		typedef	MapIterator<MorphTrackMap>		MorphTrackIterator;
		NodeTrackIterator		getNodeTrackIterator() { return NodeTrackIterator(mNodeTracks.begin(), mNodeTracks.end()); }
		NumericTrackIterator	getNumericTrackIterator() { return NumericTrackIterator(mNumericTracks.begin(), mNumericTracks.end()); }
		MorphTrackIterator		getMorphTrackIterator() { return MorphTrackIterator(mMorphTracks.begin(), mMorphTracks.end()); }
		
		// this method use current AnimationTrack to build key time, make
		// the animation prepared to work. notice that after any changes 
		// happened in any track of this animation, build the animation explicitly.
		void					buildAnimation();

		// apply animation with specified time.
		void					apply(DReal timePos, DReal weight = 1.0f, DReal scale = 1.0f);

		// apply to a skeleton, the Bone's handle of the skeleton should be identical to the specified NodeTrack's handle
		// this work can be done in initializing the skeleton.
		void					apply(DSkeleton* ske, DReal timePos, DReal weight = 1.0f, DReal scale = 1.0f);

		//TODO:添加骨骼权重mask的支持.
	
		// used to convert a time position to time index, used in applying animation.
		DTimeIndex				getTimeIndex(DReal timepos);

		// set the animation modified will enable the animation auto build before
		// next use.
		INTERNAL void			setDirty();
	protected:
		DString					mName;
		DReal					mLength;

		NodeTrackMap			mNodeTracks;
		NumericTrackMap			mNumericTracks;
		MorphTrackMap			mMorphTracks;

		// flag indicate whether the animation is dirty.
		bool					mbDirty;

		/// Global keyframe time list used to search global keyframe index.
		typedef std::vector<DReal>	KeyFrameTimes;
		KeyFrameTimes			mKeyFrameTimes;
	private:
		DAnimation() {}
	};

}

#endif