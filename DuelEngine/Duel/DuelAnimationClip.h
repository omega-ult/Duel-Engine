//  [12/29/2012 OMEGA] created

#ifndef _DUELANIMATIONCLIP_H_
#define _DUELANIMATIONCLIP_H_

#include "DuelCommon.h"
#include "DuelAnimation.h"

namespace Duel
{

	// this class is a running animation, oppsite to the static class Animation.
	class DUEL_API DAnimationClip
	{
	public:
		DAnimationClip() : mTimePos(0.0f), mLength(0.0f), mWeight(1.0f), mbEnable(false), mbLoop(false) {}
		virtual ~DAnimationClip() {}
		// Gets the name of the animation to which this state applies
		const DString&	getAnimation() const;
		// Gets the time position for this animation
		DReal		getTimePosition(void) const;
		// Sets the time position for this animation
		void		setTimePosition(DReal timePos);
		// Gets the total length of this animation (may be shorter than whole animation)
		DReal		getLength() const;
		// Sets the total length of this animation (may be shorter than whole animation)
		void		setLength(DReal len);
		// Gets the weight (influence) of this animation
		DReal		getWeight(void) const;
		// Sets the weight (influence) of this animation
		void		setWeight(DReal weight);

		// set/get the enable state of this clip
		bool		isEnabled() const;
		void		setEnable(bool flag);
		
		// set/get loop state of this clip
		bool		isLooping() const;
		void		setLoop(bool flag);
	
	protected:
		// targeting animation.
		DString		mAnimation;
		DReal		mTimePos;
		DReal		mLength;
		DReal		mWeight;
		bool		mbEnable;
		bool		mbLoop;
	};
}
#endif