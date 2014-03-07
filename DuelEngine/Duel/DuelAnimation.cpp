//  [11/4/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelAnimationTrack.h"
#include "DuelAnimation.h"

namespace Duel
{



	DAnimation::DAnimation( const DString& name, DReal length /*= 1.0f*/ ) :
		mName(name),
		mLength(length),
		mbDirty(false)
	{

	}

	DAnimation::~DAnimation()
	{
		destroyAllMorphTracks();
		destroyAllNumericTracks();
		destroyAllNodeTracks();
	}

	void DAnimation::setLength( DReal length )
	{
		mLength = length;
	}

	DReal DAnimation::getLength() const
	{
		return mLength;
	}

	const DString& DAnimation::getName() const
	{
		return mName;
	}

	DNodeAnimationTrack* DAnimation::createNodeTrack()
	{
		uint32 handle;
		DNodeAnimationTrack* na;
		// find an unused handle.
		NodeTrackMap::iterator ifirst = mNodeTracks.begin();
		if (ifirst == mNodeTracks.end())
		{
			na = new DNodeAnimationTrack(this, 0);
			mNodeTracks[0] = na;
			return na;
		}
		NodeTrackMap::iterator isecond = ifirst++;
		while (isecond != mNodeTracks.end())
		{
			// search a gap in the map, use it to create the handle.
			// rely on map's sort order.
			if (isecond->first - ifirst->first > 1)
			{
				handle = ifirst->first + 1;
				na = new DNodeAnimationTrack(this, handle);
				mNodeTracks[handle] = na;
				return na;
			}
			ifirst++; isecond++;
		}
		// end traverse, no gap found.
		handle = mNodeTracks.size();
		na = new DNodeAnimationTrack(this, handle);
		mNodeTracks[handle] = na;
		return na;
	}

	DNodeAnimationTrack* DAnimation::createNodeTrack( uint16 handle )
	{
		// if the handle have been used, return the current one.
		NodeTrackMap::iterator i = mNodeTracks.find(handle);
		if (i != mNodeTracks.end())
		{
			return i->second;
		}
		// else create one.
		DNodeAnimationTrack* na = new DNodeAnimationTrack(this, handle);
		mNodeTracks.insert(std::make_pair(handle, na));
		return na;
	}

	DNodeAnimationTrack* DAnimation::createNodeTrack( uint16 handle, DNode* target )
	{

		// if the handle have been used, return the current one.
		NodeTrackMap::iterator i = mNodeTracks.find(handle);
		if (i != mNodeTracks.end())
		{
			return i->second;
		}
		// else create one.
		DNodeAnimationTrack* na = new DNodeAnimationTrack(this, handle, target);
		mNodeTracks.insert(std::make_pair(handle, na));
		return na;
	}

	DNumericAnimationTrack* DAnimation::createNumericTrack()
	{
		uint32 handle;
		DNumericAnimationTrack* na;
		// find an unused handle.
		NumericTrackMap::iterator ifirst = mNumericTracks.begin();
		if (ifirst == mNumericTracks.end())
		{
			na = new DNumericAnimationTrack(this, 0);
			mNumericTracks[0] = na;
			return na;
		}
		NumericTrackMap::iterator isecond = ifirst++;
		while (isecond != mNumericTracks.end())
		{
			// search a gap in the map, use it to create the handle.
			// rely on map's sort order.
			if (isecond->first - ifirst->first > 1)
			{
				handle = ifirst->first + 1;
				na = new DNumericAnimationTrack(this, handle);
				mNumericTracks[handle] = na;
				return na;
			}
			ifirst++; isecond++;
		}
		// end traverse, no gap found.
		handle = mNumericTracks.size();
		na = new DNumericAnimationTrack(this, handle);
		mNumericTracks[handle] = na;
		return na;
	}

	DNumericAnimationTrack* DAnimation::createNumericTrack( uint16 handle )
	{

		// if the handle have been used, return the current one.
		NumericTrackMap::iterator i = mNumericTracks.find(handle);
		if (i != mNumericTracks.end())
		{
			return i->second;
		}
		// else create one.
		DNumericAnimationTrack* na = new DNumericAnimationTrack(this, handle);
		mNumericTracks.insert(std::make_pair(handle, na));
		return na;
	}

	DNumericAnimationTrack* DAnimation::createNumericTrack( uint16 handle, DReal* target )
	{
		// if the handle have been used, return the current one.
		NumericTrackMap::iterator i = mNumericTracks.find(handle);
		if (i != mNumericTracks.end())
		{
			return i->second;
		}
		// else create one.
		DNumericAnimationTrack* na = new DNumericAnimationTrack(this, handle, target);
		mNumericTracks.insert(std::make_pair(handle, na));
		return na;
	}

	DMorphAnimationTrack* DAnimation::createMorphTrack()
	{
		uint32 handle;
		DMorphAnimationTrack* na;
		// find an unused handle.
		MorphTrackMap::iterator ifirst = mMorphTracks.begin();
		if (ifirst == mMorphTracks.end())
		{
			na = new DMorphAnimationTrack(this, 0);
			mMorphTracks[0] = na;
			return na;
		}
		MorphTrackMap::iterator isecond = ifirst++;
		while (isecond != mMorphTracks.end())
		{
			// search a gap in the map, use it to create the handle.
			// rely on map's sort order.
			if (isecond->first - ifirst->first > 1)
			{
				handle = ifirst->first + 1;
				na = new DMorphAnimationTrack(this, handle);
				mMorphTracks[handle] = na;
				return na;
			}
			ifirst++; isecond++;
		}
		// end traverse, no gap found.
		handle = mMorphTracks.size();
		na = new DMorphAnimationTrack(this, handle);
		mMorphTracks[handle] = na;
		return na;
	}

	DMorphAnimationTrack* DAnimation::createMorphTrack( uint16 handle )
	{
		// if the handle have been used, return the current one.
		MorphTrackMap::iterator i = mMorphTracks.find(handle);
		if (i != mMorphTracks.end())
		{
			return i->second;
		}
		// else create one.
		DMorphAnimationTrack* ma = new DMorphAnimationTrack(this, handle);
		mMorphTracks.insert(std::make_pair(handle, ma));
		return ma;
	}

	DMorphAnimationTrack* DAnimation::createMorphTrack( uint16 handle, const DVertexData& target)
	{
		// if the handle have been used, return the current one.
		MorphTrackMap::iterator i = mMorphTracks.find(handle);
		if (i != mMorphTracks.end())
		{
			return i->second;
		}
		// else create one.
		DMorphAnimationTrack* ma = new DMorphAnimationTrack(this, handle, target);
		mMorphTracks.insert(std::make_pair(handle, ma));
		return ma;
	}

	size_t DAnimation::getNodeTrackCount() const
	{
		return mNodeTracks.size();
	}

	size_t DAnimation::getNumericTrackCount() const
	{
		return mNumericTracks.size();
	}

	size_t DAnimation::getMorphTrackCount() const
	{
		return mMorphTracks.size();
	}

	DNodeAnimationTrack* DAnimation::getNodeTrack( uint16 handle ) const
	{
		// if the handle have been used, return the current one.
		NodeTrackMap::const_iterator i = mNodeTracks.find(handle);
		if (i != mNodeTracks.end())
		{
			return i->second;
		}
		// no track found.
		assert(false);
		return NULL;
	}

	DNumericAnimationTrack* DAnimation::getNumericTrack( uint16 handle ) const
	{
		// if the handle have been used, return the current one.
		NumericTrackMap::const_iterator i = mNumericTracks.find(handle);
		if (i != mNumericTracks.end())
		{
			return i->second;
		}
		// no track found.
		assert(false);
		return NULL;
	}

	DMorphAnimationTrack* DAnimation::getMorphTrack( uint16 handle ) const
	{
		// if the handle have been used, return the current one.
		MorphTrackMap::const_iterator i = mMorphTracks.find(handle);
		if (i != mMorphTracks.end())
		{
			return i->second;
		}
		// no track found.
		assert(false);
		return NULL;
	}


	bool DAnimation::hasNodeTrack( uint16 handle )
	{
		return (mNodeTracks.find(handle) != mNodeTracks.end());
	}

	bool DAnimation::hasNumericTrack( uint16 handle )
	{
		return (mNumericTracks.find(handle) != mNumericTracks.end());
	}

	bool DAnimation::hasMorphTrack( uint16 handle )
	{
		return (mMorphTracks.find(handle) != mMorphTracks.end());
	}

	void DAnimation::destroyNodeTrack( uint16 handle )
	{
		NodeTrackMap::iterator i = mNodeTracks.find(handle);
		if (i != mNodeTracks.end())
		{
			delete (i->second);
		}
		mNodeTracks.erase(i);
	}

	void DAnimation::destroyNumericTrack( uint16 handle )
	{
		NumericTrackMap::iterator i = mNumericTracks.find(handle);
		if (i != mNumericTracks.end())
		{
			delete (i->second);
		}
		mNumericTracks.erase(i);

	}

	void DAnimation::destroyMorphTrack( uint16 handle )
	{

		MorphTrackMap::iterator i = mMorphTracks.find(handle);
		if (i != mMorphTracks.end())
		{
			delete (i->second);
		}
		mMorphTracks.erase(i);
	}

	void DAnimation::destroyAllNodeTracks()
	{
		NodeTrackMap::iterator i, iend = mNodeTracks.end();
		for (i = mNodeTracks.begin(); i != iend; ++i)
		{
			delete (i->second);
		}
		mNodeTracks.clear();
	}

	void DAnimation::destroyAllNumericTracks()
	{
		NumericTrackMap::iterator i, iend = mNumericTracks.end();
		for (i = mNumericTracks.begin(); i != iend; ++i)
		{
			delete (i->second);
		}
		mNumericTracks.clear();
	}

	void DAnimation::destroyAllMorphTracks()
	{
		MorphTrackMap::iterator i, iend = mMorphTracks.end();
		for (i = mMorphTracks.begin(); i != iend; ++i)
		{
			delete (i->second);
		}
		mMorphTracks.clear();
	}


	void DAnimation::buildAnimation()
	{
		if (mbDirty)
		{	
			NodeTrackMap::const_iterator i, iend = mNodeTracks.end();
			NumericTrackMap::const_iterator j, jend = mNumericTracks.end();
			MorphTrackMap::const_iterator k, kend = mMorphTracks.end();
		
			mKeyFrameTimes.clear();

			// Collect all keyframe times from each track
			for (i = mNodeTracks.begin(); i != iend; ++i)
			{
				i->second->optimise();
				i->second->fillKeyFrameTimeStamp(mKeyFrameTimes);
			}
			for (j = mNumericTracks.begin(); j != jend; ++j)
			{
				j->second->optimise();
				j->second->fillKeyFrameTimeStamp(mKeyFrameTimes);
			}
			for (k = mMorphTracks.begin(); k != kend; ++k)
			{
				k->second->optimise();
				k->second->fillKeyFrameTimeStamp(mKeyFrameTimes);
			}
			// build index map.
			for (i = mNodeTracks.begin(); i != iend; ++i)
			{
				i->second->buildKeyFrameIndexMap(mKeyFrameTimes);
			}
			for (j = mNumericTracks.begin(); j != jend; ++j)
			{
				j->second->buildKeyFrameIndexMap(mKeyFrameTimes);
			}
			for (k = mMorphTracks.begin(); k != kend; ++k)
			{
				k->second->buildKeyFrameIndexMap(mKeyFrameTimes);
			}

			mbDirty = false;
		}
	}

	void DAnimation::apply( DReal timePos, DReal weight/* = 1.0f*/, DReal scale/* = 1.0f*/ )
	{
		DTimeIndex timeIndex = getTimeIndex(timePos);

		NodeTrackMap::const_iterator i, iend = mNodeTracks.end();
		NumericTrackMap::const_iterator j, jend = mNumericTracks.end();
		MorphTrackMap::const_iterator k, kend = mMorphTracks.end();

		for (i = mNodeTracks.begin(); i != iend; ++i)
		{
			i->second->apply(timeIndex,weight,scale);
		}
		for (j = mNumericTracks.begin(); j != jend; ++j)
		{
			j->second->apply(timeIndex,weight,scale);
		}
		for (k = mMorphTracks.begin(); k != kend; ++k)
		{
			k->second->apply(timeIndex,weight,scale);
		}
	}

	void DAnimation::apply( DSkeleton* ske, DReal timePos, DReal weight /*= 1.0f*/, DReal scale /*= 1.0f*/ )
	{
// 		TimeIndex timeIndex = getTimeIndex(timePos);
// 
// 		NodeTrackMap::const_iterator i, iend = mNodeTracks.end();
// 		for (i = mNodeTracks.begin(); i != iend; ++i)
// 		{
// 			// get bone
// 			DBone* b = ske->getBone(i->first);
// 			i->second->applyToNode(b, timeIndex,weight,scale);
// 		}
 	}


	DTimeIndex DAnimation::getTimeIndex( DReal timepos )
	{
		if (mbDirty)
		{
			buildAnimation();
		}
		// wrap time
		DReal totalAnimationLength = mLength;
		while(timepos > totalAnimationLength && totalAnimationLength > 0.0f)
		{
			timepos -= totalAnimationLength;
		}
		// search for globalindex
		KeyFrameTimes::iterator i = std::lower_bound(mKeyFrameTimes.begin(), mKeyFrameTimes.end(),timepos);

		return DTimeIndex(timepos, std::distance(mKeyFrameTimes.begin(), i));
	}

	void DAnimation::setDirty()
	{
		mbDirty = true;
	}

}