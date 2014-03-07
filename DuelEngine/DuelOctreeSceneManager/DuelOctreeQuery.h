//  [1/31/2013 OMEGA] created

#ifndef _DUELOCTREEQUERY_H_
#define _DUELOCTREEQUERY_H_

#include "DuelOctreeCommon.h"
#include "DuelOctreeSceneManager.h"
#include "DuelSceneQuery.h"

namespace Duel
{

	class DUELOCTREE_API OctreeRayQuery : public RayQuery
	{
	DUEL_DECLARE_RTTI(OctreeRayQuery)
	public:
		OctreeRayQuery(OctreeSceneManager* parent);
		// override : RayQuery-------------------------------
		void	execute(RayQueryProcessor* processor);
	};

	class DUELOCTREE_API OctreeAxisAlignedBoxQuery : public AxisAlignedBoxQuery
	{
	DUEL_DECLARE_RTTI(OctreeAxisAlignedBoxQuery)
	public:
		OctreeAxisAlignedBoxQuery(OctreeSceneManager* parent);
		// override : RegionQuery------------------------
		void		execute(RegionQueryProcessor* processor);
	};

	class DUELOCTREE_API OctreeSphereQuery : public SphereQuery
	{
	DUEL_DECLARE_RTTI(OctreeSphereQuery)
	public:
		OctreeSphereQuery(OctreeSceneManager* parent);
		// override : RegionQuery------------------------
		void		execute(RegionQueryProcessor* processor);
	};

	class DUELOCTREE_API OctreeIntersectionQuery : public IntersectionQuery
	{
	DUEL_DECLARE_RTTI(OctreeIntersectionQuery)
	public:
		OctreeIntersectionQuery(OctreeSceneManager* parent);
		// override : IntersectionQuery----------------------
		void		execute(IntersectionQueryProcessor* processor);
	};
}


#endif