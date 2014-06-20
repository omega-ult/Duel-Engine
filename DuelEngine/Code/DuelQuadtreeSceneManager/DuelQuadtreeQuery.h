//  [3/7/2013 OMEGA] created

#ifndef _DUELQUADTREEQUERY_H_
#define _DUELQUADTREEQUERY_H_

#include "DuelQuadtreeCommon.h"
#include "DuelQuadtreeSceneManager.h"
#include "DuelSceneQuery.h"

namespace Duel
{

	class DUELQUADTREE_API QuadtreeRayQuery : public RayQuery
	{
		DUEL_DECLARE_RTTI(QuadtreeRayQuery)
	public:
		QuadtreeRayQuery(QuadtreeSceneManager* parent);
		// override : RayQuery-------------------------------
		void	execute(RayQueryProcessor* processor);
	};

	class DUELQUADTREE_API QuadtreeAxisAlignedBoxQuery : public AxisAlignedBoxQuery
	{
		DUEL_DECLARE_RTTI(QuadtreeAxisAlignedBoxQuery)
	public:
		QuadtreeAxisAlignedBoxQuery(QuadtreeSceneManager* parent);
		// override : RegionQuery------------------------
		void		execute(RegionQueryProcessor* processor);
	};

	class DUELQUADTREE_API QuadtreeSphereQuery : public SphereQuery
	{
		DUEL_DECLARE_RTTI(QuadtreeSphereQuery)
	public:
		QuadtreeSphereQuery(QuadtreeSceneManager* parent);
		// override : RegionQuery------------------------
		void		execute(RegionQueryProcessor* processor);
	};

	class DUELQUADTREE_API QuadtreeIntersectionQuery : public IntersectionQuery
	{
		DUEL_DECLARE_RTTI(QuadtreeIntersectionQuery)
	public:
		QuadtreeIntersectionQuery(QuadtreeSceneManager* parent);
		// override : IntersectionQuery----------------------
		void		execute(IntersectionQueryProcessor* processor);
	};
}

#endif // !_DUELQUADTREEQUERY_H_
