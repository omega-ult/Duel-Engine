//  [1/31/2013 OMEGA] created

#include "DuelOctreeCommon.h"
#include "DuelOctree.h"
#include "DuelSceneQuery.h"
#include "DuelOctreeQuery.h"
#include "DuelOctreeSceneManager.h"

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(OctreeRayQuery, RayQuery);
	DUEL_IMPLEMENT_RTTI_1(OctreeAxisAlignedBoxQuery, AxisAlignedBoxQuery);
	DUEL_IMPLEMENT_RTTI_1(OctreeSphereQuery, SphereQuery);
	DUEL_IMPLEMENT_RTTI_1(OctreeIntersectionQuery, IntersectionQuery);



	// internal help method used to traverse octree
	void walkOctree(Octant* octant, const DAxisAlignedBox& box, RegionQueryResultList& resultList)
	{
		if (octant == NULL)
		{
			return;
		}
		if (octant->getRegion().isIntersected(box))
		{
			Octant::OctreeSceneNodeIterator ni = octant->getSceneNodeIterator();
			while (ni.hasMoreElements())
			{
				DSceneNode::MovableIterator mi = (*(ni.current()))->getMovableMapIterator();
				while (mi.hasMoreElements())
				{
					if (mi.getNext()->getBoundingBox().isIntersected(box))
					{
						resultList.push_back(*mi.current());
					}
				}
				ni.moveNext();
			}

			// walk tree
			walkOctree(octant->getOctant(0, 0, 0), box, resultList);
			walkOctree(octant->getOctant(0, 0, 1), box, resultList);
			walkOctree(octant->getOctant(0, 1, 0), box, resultList);
			walkOctree(octant->getOctant(0, 1, 1), box, resultList);
			walkOctree(octant->getOctant(1, 0, 0), box, resultList);
			walkOctree(octant->getOctant(1, 0, 1), box, resultList);
			walkOctree(octant->getOctant(1, 1, 0), box, resultList);
			walkOctree(octant->getOctant(1, 1, 1), box, resultList);
		}
	}

	// internal help method used to traverse octree
	void walkOctree(Octant* octant, const DSphere& s, RegionQueryResultList& resultList)
	{
		if (octant == NULL)
		{
			return;
		}
		if (DMath::intersect(s, octant->getRegion()))
		{
			Octant::OctreeSceneNodeIterator ni = octant->getSceneNodeIterator();
			while (ni.hasMoreElements())
			{
				DSceneNode::MovableIterator mi = (*(ni.current()))->getMovableMapIterator();
				while (mi.hasMoreElements())
				{
					if (DMath::intersect(s, mi.getNext()->getBoundingBox()))
					{
						resultList.push_back(*mi.current());
					}
				}
				ni.moveNext();
			}

			// walk tree
			walkOctree(octant->getOctant(0, 0, 0), s, resultList);
			walkOctree(octant->getOctant(0, 0, 1), s, resultList);
			walkOctree(octant->getOctant(0, 1, 0), s, resultList);
			walkOctree(octant->getOctant(0, 1, 1), s, resultList);
			walkOctree(octant->getOctant(1, 0, 0), s, resultList);
			walkOctree(octant->getOctant(1, 0, 1), s, resultList);
			walkOctree(octant->getOctant(1, 1, 0), s, resultList);
			walkOctree(octant->getOctant(1, 1, 1), s, resultList);
		}
	}


	// internal help method used to traverse octree
	void walkOctree(Octant* octant, const DRay& r, RayQueryResults& resultList)
	{
		DReal dist;
		RayQueryResultEntry tmpE;
		if (octant == NULL)
		{
			return;
		}
		if (DMath::intersect(r, octant->getRegion(), &dist))
		{
			Octant::OctreeSceneNodeIterator ni = octant->getSceneNodeIterator();
			while (ni.hasMoreElements())
			{
				DSceneNode::MovableIterator mi = (*(ni.current()))->getMovableMapIterator();
				while (mi.hasMoreElements())
				{
					if (DMath::intersect(r, octant->getRegion(), &dist))
					{
						tmpE.distance = dist;
						tmpE.movable = mi.getNext();
						resultList.push_back(tmpE);
					}
				}
				ni.moveNext();
			}

			// walk tree
			walkOctree(octant->getOctant(0, 0, 0), r, resultList);
			walkOctree(octant->getOctant(0, 0, 1), r, resultList);
			walkOctree(octant->getOctant(0, 1, 0), r, resultList);
			walkOctree(octant->getOctant(0, 1, 1), r, resultList);
			walkOctree(octant->getOctant(1, 0, 0), r, resultList);
			walkOctree(octant->getOctant(1, 0, 1), r, resultList);
			walkOctree(octant->getOctant(1, 1, 0), r, resultList);
			walkOctree(octant->getOctant(1, 1, 1), r, resultList);
		}
	}


	OctreeRayQuery::OctreeRayQuery( OctreeSceneManager* parent ) :
		RayQuery(parent)
	{

	}

	void OctreeRayQuery::execute( RayQueryProcessor* processor )
	{
		mResult.clear();
		Octant* octant = static_cast<OctreeSceneManager*>(mParent)->getOctree();
		walkOctree(octant, mRay, mResult);
		std::sort(mResult.begin(), mResult.end());
		RayQueryResultIterator ri = getQueryResultIterator();
		uint16 counter = 0;
		RayQueryResultEntry tmpE;
		if (processor != NULL)
		{
			while (ri.hasMoreElements() && counter < mMaxResults)
			{
				tmpE = ri.getNext();
				processor->processResult(tmpE.movable, tmpE.distance);
			}
		}
	}


	OctreeAxisAlignedBoxQuery::OctreeAxisAlignedBoxQuery( OctreeSceneManager* parent ) :
		AxisAlignedBoxQuery(parent)
	{

	}

	void OctreeAxisAlignedBoxQuery::execute( RegionQueryProcessor* processor )
	{
		mResult.clear();
		Octant* octant = static_cast<OctreeSceneManager*>(mParent)->getOctree();
		walkOctree(octant, mBox, mResult);
		RegionQueryResultIterator ri = getQueryResultIterator();
		if (processor != NULL)
		{
			while (ri.hasMoreElements())
			{
				processor->processResult(*(ri.current()));
				ri.moveNext();
			}
		}
	}
	OctreeSphereQuery::OctreeSphereQuery( OctreeSceneManager* parent ) :
		SphereQuery(parent)
	{

	}

	void OctreeSphereQuery::execute( RegionQueryProcessor* processor )
	{
		mResult.clear();
		Octant* octant = static_cast<OctreeSceneManager*>(mParent)->getOctree();
		walkOctree(octant, mSphere, mResult);
		RegionQueryResultIterator ri = getQueryResultIterator();
		if (processor != NULL)
		{
			while (ri.hasMoreElements())
			{
				processor->processResult(*(ri.current()));
				ri.moveNext();
			}
		}
	}
	OctreeIntersectionQuery::OctreeIntersectionQuery( OctreeSceneManager* parent ) :
		IntersectionQuery(parent)
	{

	}

	void OctreeIntersectionQuery::execute( IntersectionQueryProcessor* processor )
	{
		mResult.clear();
		typedef	std::set<MovablePair>	MovablePairSet;
		MovablePairSet	tmpSet;
		RegionQueryResultList	tmpList;
		DSceneManager::SceneNodeIterator sni = mParent->getSceneNodeIterator();
		while (sni.hasMoreElements())
		{
			DSceneNode::MovableIterator mi = sni.getNext()->getMovableMapIterator();
			while (mi.hasMoreElements())
			{
				tmpList.clear();
				DMovable* obj = mi.getNext();
				//find the nodes that intersect the AAB
				walkOctree(static_cast<OctreeSceneManager*>(mParent)->getOctree(), obj->getBoundingBox(), tmpList);
				// TODO:增加QueryMask功能
				RegionQueryResultList::iterator rli = tmpList.begin();
				RegionQueryResultList::iterator rlend = tmpList.end();
				while (rli != rlend)
				{
					// avoid duplication.
					if (tmpSet.find(MovablePair(obj, *rli)) != tmpSet.end() &&
						tmpSet.find(MovablePair(*rli, obj)) != tmpSet.end())
					{
						processor->processResult(obj, *rli);
						tmpSet.insert(MovablePair(obj, *rli));
					}
					++rli;
				}
			}
		}
	}

}