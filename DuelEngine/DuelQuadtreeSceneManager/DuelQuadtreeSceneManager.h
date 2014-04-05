//  [3/7/2013 OMEGA] created

#ifndef _DUELQUADTREESCENEMANAGER_H_
#define _DUELQUADTREESCENEMANAGER_H_

#include "DuelQuadtreeCommon.h"
#include "DuelQuadtree.h"
#include "DuelSceneManager.h"
#include "DuelSceneManagerEnumerator.h"

namespace Duel
{
	// -- type name == "QuadtreeSceneManager"
	class DUELQUADTREE_API QuadtreeSceneManager : public DSceneManager
	{
	DUEL_DECLARE_RTTI(QuadtreeSceneManager)
	public:
		QuadtreeSceneManager(DSceneManagerFactory* creator, const DString& name);
		~QuadtreeSceneManager();

		// override : DSceneManger------------------------------
		void			initialize(DSceneInstance* owner, DAxisAlignedBox region, DReal granularity);
		// resize the quadtree
		virtual void	resize(DAxisAlignedBox region);
		// get the quadtree
		Quadrant*		getQuadtree() { return mQuadtree; }

		// override : DSceneManager--------------------------
		void			updateSceneNode(DSceneNode* node);
		// override : DSceneManager---------------------------
		void			clearScene();

		// remove an QuadtreeSceneNode from scene quadtree, this method will no affect its children.
		// 将一个QuadtreeSceneNode从场景八叉树中移除, 但不影响其子节点.
		INTERNAL void	removeQuadtreeSceneNode(QuadtreeSceneNode* n);
		// add an QuadtreeSceneNode to the scene quadtree recursivly, without affecting its children.
		// 这个函数将添加一个QuadtreeSceneNode到指定的八叉树子节点中, 并且会递归查找合适的子节点插入, 但不影响
		// QuadtreeSceneNode的子节点.
		INTERNAL void	addQuadtreeSceneNode(QuadtreeSceneNode* n, Quadrant* oct, uint32 depth );

		// override : DSceneManager--------------------------
		RayQuery*				createRayQuery();
		// override : DSceneManager--------------------------
		AxisAlignedBoxQuery*	createAABBQuery();
		// override : DSceneManager--------------------------
		SphereQuery*			createSphereQuery();
		// override : DSceneManager--------------------------
		IntersectionQuery*		createIntersectionQuery();
		// override : DSceneManager--------------------------
		void			destroyQuery(RayQuery* query);
		// override : DSceneManager--------------------------
		void			destroyQuery(AxisAlignedBoxQuery* query);
		// override : DSceneManager--------------------------
		void			destroyQuery(SphereQuery* query);
		// override : DSceneManager--------------------------
		void			destroyQuery(IntersectionQuery* query);

		// override : DSceneManager--------------------------
		void			applyToRenderQueue(DRenderQueue* queue, DCamera* cam);


	protected:
		// override : DSceneManager--------------------------
		DSceneNode*		createSceneNodeImpl();
		// override : DSceneManager--------------------------
		DSceneNode*		createSceneNodeImpl(const DString& name);
		// override : DSceneManager--------------------------
		void			destroySceneNodeImpl(DSceneNode* node);

		// internal method used to traverse the quadtree. and populate objects to 
		// the render queue.
		void			walkQuadtree(Quadrant* quadrant, DRenderQueue* queue, DCamera* cam);

		// shut down the scene manager, delete all quadrants
		void			shutdownQuadtree(Quadrant* quadrant);

		// root quadtree.
		Quadrant*		mQuadtree;
		// the maximun depth of quadtree.
		uint32			mMaxDepth;
		// name generater.
		DNameGenerator	mNameGen;
	};

	class DUELQUADTREE_API QuadtreeSceneManagerFactory : public DSceneManagerFactory
	{
	DUEL_DECLARE_RTTI(QuadtreeSceneManagerFactory)
	public:
		QuadtreeSceneManagerFactory();
		// override:DSceneManagerFactory---------------------------
		DSceneManager*	createSceneManager(const DString& name);
		// override:DSceneManagerFactory---------------------------
		void			destroySceneManager(DSceneManager* sm);

	};
}

#endif // !_DUELQUADTREESCENEMANAGER_H_
