//  [1/5/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelAnimationClip.h"

namespace Duel
{


	const DString& DAnimationClip::getAnimation() const
	{
		return mAnimation;
	}

	DReal DAnimationClip::getTimePosition( void ) const
	{
		return mTimePos;
	}

	void DAnimationClip::setTimePosition( DReal timePos )
	{
		mTimePos = timePos;
	}

	DReal DAnimationClip::getLength() const
	{
		return mLength;
	}

	void DAnimationClip::setLength( DReal len )
	{
		mLength = len;
	}

	DReal DAnimationClip::getWeight( void ) const
	{
		return mLength;
	}

	void DAnimationClip::setWeight( DReal weight )
	{
		mWeight = weight;
	}

	bool DAnimationClip::isEnabled() const
	{
		return mbEnable;
	}

	void DAnimationClip::setEnable( bool flag )
	{
		mbEnable = flag;
	}

	bool DAnimationClip::isLooping() const
	{
		return mbLoop;
	}

	void DAnimationClip::setLoop( bool flag )
	{
		mbLoop = flag;
	}

}