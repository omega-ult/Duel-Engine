//  [1/10/2013 OMEGA] created

#ifndef _DUELSCENEQUERY_H_
#define _DUELSCENEQUERY_H_

#include "DuelCommon.h"

namespace Duel
{
	// TODO: 没有地形系统, 地形系统去死吧.

	// there were three types of queries, RayQuery RegionQuery and IntersectionQuery,
	// they are using its own query result formats. query mask is an extensible constant,
	// which can be extended by specfied scene manager, here are some common masks:
// 	enum SceneQueryMask
// 	{
// 		SQM_LIGHT		= 0,
// 		SQM_MOVABLE		= 1,
// 		SQM_FORCEUINT	= 0xffffffffUL	//never used.
// 	};

	// base class for defining scene query objects, which are used to 
	// retrieve testing result in the scene.
	class DUEL_API SceneQuery : public DObject
	{
	DUEL_DECLARE_RTTI(SceneQuery)
	public:
		// scene query objects are created by the scene manager.
		SceneQuery(DSceneManager* parent) : mParent (parent), mQueryMask(0) {}
		// get creator of this scene query.
		DSceneManager*	getParent() const { return mParent; }

		// enalbe specified query mask.
		virtual	void	enableQuery(uint32 mask) { mQueryMask |= mask; }
		// disable specified mask.
		virtual	void	disableQuery(uint32 mask) { mQueryMask &= ~(mask);}
		// set the whole query mask word.
		virtual	void	setQueryMask(uint32 mask) { mQueryMask = mask; }
		// get whether specified query mask is enabled.
		virtual	bool	getQueryMask(uint32 mask) { return (mask & mQueryMask) != 0; }
		// get the whole mask word.
		virtual	uint32	getQueryMask() { return mQueryMask; }

	protected:
		DSceneManager*	mParent;
		uint32			mQueryMask;

	};

	// DRay Query------------------------------------------------------
	class DUEL_API RayQueryProcessor
	{
	public:
		// return false to abandon current query, dist is the distance to the ray origin.
		virtual	bool	processResult(DMovable* mov, DReal dist) { return true; }
	};

	struct DUEL_API RayQueryResultEntry
	{
		DReal		distance;
		DMovable*	movable;
		// comparison operator for sorting.
		bool	operator < (const RayQueryResultEntry& rhs) const
		{
			return this->distance < rhs.distance;
		}

	};
	typedef	std::vector<RayQueryResultEntry>	RayQueryResults;
	typedef	VectorIterator<RayQueryResults>		RayQueryResultIterator;
	class DUEL_API RayQuery : public SceneQuery
	{
	DUEL_DECLARE_RTTI(RayQuery)
	public:
		RayQuery(DSceneManager* parent) : SceneQuery(parent), mRay(DVector3::ZERO, DVector3::UNIT_Z), mbSortByDistance(false), mMaxResults(0xffff) {}

		// set/get ray used in querying.
		virtual	void	setRay(const DRay& ray) { mRay = ray; }
		virtual	const DRay&	getRay() const { return mRay; }

		// set sorting by distance, and the maximum result count.
		virtual	void	setSortByDistance(bool flag, uint16 maxResults) { mbSortByDistance = flag; mMaxResults = maxResults; }
		virtual	bool	isSortByDistance() const { return mbSortByDistance; }
		// get the max count of results
		virtual	uint16	getMaxResultCount() const { return mMaxResults; }

		// sub-class use its own algorithm to complete the querying.
		virtual	void	execute(RayQueryProcessor* processor) = 0;
		
		// optional method to traverse all results, only last query results available.
		RayQueryResultIterator	getQueryResultIterator() { return RayQueryResultIterator(mResult.begin(), mResult.end()); }

		// clear all results, it was performed automatically in execute()
		virtual	void	clearResults() { mResult.clear(); }

	protected:
		DRay				mRay;
		bool			mbSortByDistance;
		uint16			mMaxResults;
		RayQueryResults	mResult;
	};


	// Region Query--------------------------------------------------
	class DUEL_API RegionQueryProcessor
	{
	public:
		// return false to abandon current query.
		virtual	bool	processResult(DMovable* mov) { return true; }
	};

	typedef	std::list<DMovable*>		RegionQueryResultList;
	typedef	ListIterator<RegionQueryResultList>	RegionQueryResultIterator;

	class DUEL_API RegionQuery : public SceneQuery
	{
	DUEL_DECLARE_RTTI(RegionQuery)
	public:
		RegionQuery(DSceneManager* parent) : SceneQuery(parent) {}

		// sub-class use its own algorithm to complete the querying.
		virtual	void		execute(RegionQueryProcessor* processor) = 0;

		// optional method to traverse all results, only last query results available.
		RegionQueryResultIterator	getQueryResultIterator() { return RegionQueryResultIterator(mResult.begin(), mResult.end()); }

		// clear all results, it was performed automatically in execute()
		virtual	void		clearResults() { mResult.clear(); }

	protected:
		RegionQueryResultList	mResult;
	};

	class DUEL_API SphereQuery : public RegionQuery
	{
	DUEL_DECLARE_RTTI(SphereQuery)
	public:
		SphereQuery(DSceneManager* parent) : RegionQuery(parent), mSphere(DVector3::ZERO, 1.0f) {}

		// get/set sphere used in quering.
		virtual	void	setSphere(const DSphere& sphere) { mSphere = sphere; }
		virtual const DSphere&	getSphere() const { return mSphere; }

	protected:
		DSphere			mSphere;
	};

	class DUEL_API AxisAlignedBoxQuery : public RegionQuery
	{
	DUEL_DECLARE_RTTI(AxisAlignedBoxQuery)
	public:
		AxisAlignedBoxQuery(DSceneManager* parent) : RegionQuery(parent), mBox(DAxisAlignedBox::AAB_NULL) {}

		// set/get box used in querying.
		virtual	void	setBox(const DAxisAlignedBox& box) { mBox = box; }
		virtual	const DAxisAlignedBox&	getBox() const { return mBox; }

	protected:
		DAxisAlignedBox	mBox;
	};

	// IntersectionQuery-----------------------------------------------

	class DUEL_API IntersectionQueryProcessor
	{
	public:
		// return false to abandon current query.
		virtual	bool	processResult(DMovable* a, DMovable* b) { return true; }
	};
	
	typedef	std::pair<DMovable*, DMovable*>	MovablePair;
	typedef	std::list<MovablePair>			IntersectionQueryResultList;
	typedef	ListIterator<IntersectionQueryResultList>	IntersectionQueryResultIterator;

	class DUEL_API IntersectionQuery : public SceneQuery
	{
	DUEL_DECLARE_RTTI(IntersectionQuery)
	public:
		IntersectionQuery(DSceneManager* parent) : SceneQuery(parent) {}

		// sub-class use its own algorithm to complete the querying.
		virtual	void		execute(IntersectionQueryProcessor* processor) = 0;

		// clear all results, it was performed automatically in execute()
		virtual	void		clearResults() { mResult.clear(); }

		// optional method to traverse all results, only last query results available.
		IntersectionQueryResultIterator	getResultItrator() { return IntersectionQueryResultIterator(mResult.begin(), mResult.end()); }

	protected:
		IntersectionQueryResultList	mResult;
	};
}

#endif