//  [3/9/2013 OMEGA] created

#include "DuelQuadtreeCommon.h"
#include "DuelQuadtree.h"
#include "DuelSceneQuery.h"
#include "DuelQuadtreeSceneNode.h"
#include "DuelQuadtreeQuery.h"
#include "DuelQuadtreeSceneManager.h"

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(QuadtreeRayQuery, RayQuery);
	DUEL_IMPLEMENT_RTTI_1(QuadtreeAxisAlignedBoxQuery, AxisAlignedBoxQuery);
	DUEL_IMPLEMENT_RTTI_1(QuadtreeSphereQuery, SphereQuery);
	DUEL_IMPLEMENT_RTTI_1(QuadtreeIntersectionQuery, IntersectionQuery);



	// internal help method used to traverse octree
	void walkQuadtree(Quadrant* quadrant, const DAxisAlignedBox& box, RegionQueryResultList& resultList)
	{
		if (quadrant == NULL)
		{
			return;
		}
		if (quadrant->getRegion().isIntersected(box))
		{
			Quadrant::QuadtreeSceneNodeIterator ni = quadrant->getSceneNodeIterator();
			while (ni.hasMoreElements())
			{
				DSceneNode::MovableIterator mi = (*(ni.current()))->getMovableIterator();
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
			walkQuadtree(quadrant->getQuadrant(0, 0), box, resultList);
			walkQuadtree(quadrant->getQuadrant(0, 1), box, resultList);
			walkQuadtree(quadrant->getQuadrant(1, 0), box, resultList);
			walkQuadtree(quadrant->getQuadrant(1, 1), box, resultList);
		}
	}

	// internal help method used to traverse octree
	void walkQuadtree(Quadrant* quadrant, const DSphere& s, RegionQueryResultList& resultList)
	{
		if (quadrant == NULL)
		{
			return;
		}
		if (DMath::intersect(s, quadrant->getRegion()))
		{
			Quadrant::QuadtreeSceneNodeIterator ni = quadrant->getSceneNodeIterator();
			while (ni.hasMoreElements())
			{
				DSceneNode::MovableIterator mi = (*(ni.current()))->getMovableIterator();
				if (DMath::intersect(s, mi.getNext()->getBoundingBox()))
				{
					resultList.push_back(*mi.current());
				}
				ni.moveNext();
			}

			// walk tree
			walkQuadtree(quadrant->getQuadrant(0, 0), s, resultList);
			walkQuadtree(quadrant->getQuadrant(0, 1), s, resultList);
			walkQuadtree(quadrant->getQuadrant(1, 0), s, resultList);
			walkQuadtree(quadrant->getQuadrant(1, 1), s, resultList);
		}
	}

	// internal help method used to traverse octree
	void walkQuadtree(Quadrant* octant, const DRay& r, RayQueryResults& resultList)
	{
		DReal dist;
		RayQueryResultEntry tmpE;
		if (octant == NULL)
		{
			return;
		}
		if (DMath::intersect(r, octant->getRegion(), &dist))
		{
			Quadrant::QuadtreeSceneNodeIterator ni = octant->getSceneNodeIterator();
			while (ni.hasMoreElements())
			{
				DSceneNode::MovableIterator mi = (*(ni.current()))->getMovableIterator();
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
			walkQuadtree(octant->getQuadrant(0, 0), r, resultList);
			walkQuadtree(octant->getQuadrant(0, 1), r, resultList);
			walkQuadtree(octant->getQuadrant(1, 0), r, resultList);
			walkQuadtree(octant->getQuadrant(1, 1), r, resultList);
		}
	}


	QuadtreeRayQuery::QuadtreeRayQuery( QuadtreeSceneManager* parent ) :
		RayQuery(parent)
	{
		assert(DUEL_IS_EXACT_KIND(QuadtreeSceneManager, parent));
	}

	void QuadtreeRayQuery::execute( RayQueryProcessor* processor )
	{
		mResult.clear();
		Quadrant* octant = static_cast<QuadtreeSceneManager*>(mParent)->getQuadtree();
		walkQuadtree(octant, mRay, mResult);
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


	QuadtreeAxisAlignedBoxQuery::QuadtreeAxisAlignedBoxQuery( QuadtreeSceneManager* parent ) :
		AxisAlignedBoxQuery(parent)
	{
		assert(DUEL_IS_EXACT_KIND(QuadtreeSceneManager, parent));
	}

	void QuadtreeAxisAlignedBoxQuery::execute( RegionQueryProcessor* processor )
	{
		mResult.clear();
		Quadrant* quadrant = static_cast<QuadtreeSceneManager*>(mParent)->getQuadtree();
		walkQuadtree(quadrant, mBox, mResult);
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
	QuadtreeSphereQuery::QuadtreeSphereQuery( QuadtreeSceneManager* parent ) :
		SphereQuery(parent)
	{
		assert(DUEL_IS_EXACT_KIND(QuadtreeSceneManager, parent));
	}

	void QuadtreeSphereQuery::execute( RegionQueryProcessor* processor )
	{
		mResult.clear();
		Quadrant* quadrant = static_cast<QuadtreeSceneManager*>(mParent)->getQuadtree();
		walkQuadtree(quadrant, mSphere, mResult);
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
	QuadtreeIntersectionQuery::QuadtreeIntersectionQuery( QuadtreeSceneManager* parent ) :
		IntersectionQuery(parent)
	{
		assert(DUEL_IS_EXACT_KIND(QuadtreeSceneManager, parent));
	}

	void QuadtreeIntersectionQuery::execute( IntersectionQueryProcessor* processor )
	{
		mResult.clear();
		typedef	std::set<MovablePair>	MovablePairSet;
		MovablePairSet	tmpSet;
		RegionQueryResultList	tmpList;
		DSceneManager::SceneNodeIterator sni = mParent->getSceneNodeIterator();
		while (sni.hasMoreElements())
		{
			DSceneNode::MovableIterator mi = sni.getNext()->getMovableIterator();
			while (mi.hasMoreElements())
			{
				tmpList.clear();
				DMovable* obj = mi.getNext();
				//find the nodes that isIntersected the AAB
				walkQuadtree(static_cast<QuadtreeSceneManager*>(mParent)->getQuadtree(), obj->getBoundingBox(), tmpList);
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