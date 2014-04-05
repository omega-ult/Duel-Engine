//  [3/9/2013 OMEGA] created


#include "DuelQuadtreeCommon.h"
#include "DuelQuadtreeSceneManager.h"
#include "DuelLightSource.h"
#include "DuelQuadtree.h"
#include "DuelQuadtreeSceneNode.h"
#include "DuelQuadtreeQuery.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(QuadtreeSceneManager, DSceneManager);
	DUEL_IMPLEMENT_RTTI_1(QuadtreeSceneManagerFactory, DSceneManagerFactory);

	QuadtreeSceneManager::QuadtreeSceneManager( DSceneManagerFactory* creator,  const DString& name ) :
		DSceneManager(creator, "QuadtreeSceneManager", name),
		mNameGen("QuadtreeNode_"),
		mQuadtree(NULL)
	{
		assert(DUEL_IS_EXACT_KIND(QuadtreeSceneManagerFactory, creator));
	}


	QuadtreeSceneManager::~QuadtreeSceneManager()
	{
		shutdownQuadtree(mQuadtree);
	}

	void QuadtreeSceneManager::removeQuadtreeSceneNode( QuadtreeSceneNode* n )
	{
		if (mQuadtree == NULL)
		{
			return;
		}

		Quadrant* oct = n->getQuadrant();
		if (oct != NULL)
		{
			oct->removeSceneNode(n);
		}
		n->setQuadrant(NULL);
	}

	void QuadtreeSceneManager::addQuadtreeSceneNode( QuadtreeSceneNode* n, Quadrant* oct, uint32 depth )
	{
		if (mQuadtree == NULL)
		{
			return;
		}

		const DAxisAlignedBox& nBox = n->getBoundingBox();
		//if the octree is twice as big as the scene node,
		//we will add it to a child.
		if ((depth < mMaxDepth) && oct->isTwiceSize(nBox))
		{
			uint32 x, z;
			oct->getChildIndex(nBox, x, z);
			Quadrant* newOct = oct->getQuadrant(x, z);
			if (newOct == NULL)
			{
				newOct = oct->createQuadrant(x, z);
			}
			addQuadtreeSceneNode(n, newOct, ++depth);
		}
		else // exceed max depth or octree box fits node box.
		{
			oct->addSceneNode(n);
		}
	}

	void QuadtreeSceneManager::initialize(DSceneInstance* owner,  DAxisAlignedBox region, DReal granularity )
	{
		DSceneManager::initialize(owner, region, granularity);
		if (region.isNull() || region.isInfinite())
		{
			return;
		}
		region.merge(DAxisAlignedBox(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f));
		if (mQuadtree != NULL)
		{
			shutdownQuadtree(mQuadtree);
		}
		mQuadtree = new Quadrant(NULL);
		DVector3 max = region.getMaximum();
		DVector3 min = region.getMinimum();
		DReal size = 0.0f;
		size = DMath::Max<DReal>(max.x - min.x, size);
		size = DMath::Max<DReal>(max.y - min.y, size);
		size = DMath::Max<DReal>(max.z - min.z, size);
		uint32 depth = 1;
		granularity = DMath::Max<DReal>(granularity, 1.0f);
		mGranularity = granularity;
		while (size > granularity)
		{
			depth++;
			size /= 2;
		}
		mMaxDepth = depth;
		mSceneBox = region;
		mQuadtree->setRegion(mSceneBox);
	}


// 
// 	void QuadtreeSceneManager::init( const DAxisAlignedBox& region, uint32 depth )
// 	{
// 		if (mQuadtree != NULL)
// 		{
// 			shutdownQuadtree(mQuadtree);
// 		}
// 		mQuadtree = new Quadrant(NULL);
// 		mMaxDepth = depth;
// 		mSceneBox = region;
// 		mQuadtree->setRegion(mSceneBox);
// 	}

	void QuadtreeSceneManager::resize( DAxisAlignedBox region )
	{
		region.merge(DAxisAlignedBox(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f));
		std::list<QuadtreeSceneNode*> tempList;
		SceneNodeIterator i = getSceneNodeIterator();
		while(i.hasMoreElements())
		{
			QuadtreeSceneNode* n = static_cast<QuadtreeSceneNode*>(i.current()->second);
			if (n->isEnabled())
			{
				tempList.push_back(n);
			}
			i.moveNext();
		}
		shutdownQuadtree(mQuadtree);
		initialize(mOwner, region, mGranularity);
		std::list<QuadtreeSceneNode*>::iterator li, iend = tempList.end();
		for(li = tempList.begin(); li != iend; ++li )
		{
			(*li)->setQuadrant(NULL);
			updateSceneNode(*li);
		}
	}

	void QuadtreeSceneManager::clearScene()
	{
		initialize(mOwner, mSceneBox, mGranularity);
	}

	void QuadtreeSceneManager::updateSceneNode( DSceneNode* node )
	{
		assert(DUEL_IS_EXACT_KIND(QuadtreeSceneNode, node));
		QuadtreeSceneNode* osn = static_cast<QuadtreeSceneNode*>(node);
		if (osn->getBoundingBox().isNull() || !(osn->isEnabled()))
		{
			return;
		}
		if (mQuadtree == NULL)
		{
			return;
		}

		if (osn->getQuadrant() == NULL)
		{
			// if the node is not in root region, force into the root tree,
			if (osn->isIn(mQuadtree->getRegion()))
			{
				addQuadtreeSceneNode(osn, mQuadtree, 0);
			}
			else 
			{
				mQuadtree->addSceneNode(osn);
			}
			return;
		}

		if (!(osn->isIn(osn->getQuadrant()->getRegion())))
		{
			removeQuadtreeSceneNode(osn);
			// if the node is not in root region, force into the root tree,
			if (osn->isIn(mQuadtree->getRegion()))
			{
				addQuadtreeSceneNode(osn, mQuadtree, 0);
			}
			else 
			{
				mQuadtree->addSceneNode(osn);
			}
			return;
		}
	}

	DSceneNode* QuadtreeSceneManager::createSceneNodeImpl()
	{
		return createSceneNodeImpl(mNameGen.generate());
	}

	DSceneNode* QuadtreeSceneManager::createSceneNodeImpl( const DString& name )
	{
		return new QuadtreeSceneNode(this, name);
	}

	void QuadtreeSceneManager::destroySceneNodeImpl( DSceneNode* node )
	{
		assert(DUEL_IS_EXACT_KIND(QuadtreeSceneNode, node));
		QuadtreeSceneNode* n = static_cast<QuadtreeSceneNode*>(node);
		if (n != NULL)
		{
			removeQuadtreeSceneNode(n);
		}
		delete n;
	}

	RayQuery* QuadtreeSceneManager::createRayQuery()
	{
		return new QuadtreeRayQuery(this);
	}

	AxisAlignedBoxQuery* QuadtreeSceneManager::createAABBQuery()
	{
		return new QuadtreeAxisAlignedBoxQuery(this);
	}

	SphereQuery* QuadtreeSceneManager::createSphereQuery()
	{
		return new QuadtreeSphereQuery(this);
	}

	IntersectionQuery* QuadtreeSceneManager::createIntersectionQuery()
	{
		return new QuadtreeIntersectionQuery(this);
	}

	void QuadtreeSceneManager::destroyQuery( RayQuery* query )
	{
		// check availablity
		if (query->getParent() == this)
		{
			delete query;
		}
	}

	void QuadtreeSceneManager::destroyQuery( AxisAlignedBoxQuery* query )
	{
		// check availablity
		if (query->getParent() == this)
		{
			delete query;
		}
	}

	void QuadtreeSceneManager::destroyQuery( SphereQuery* query )
	{
		// check availablity
		if (query->getParent() == this)
		{
			delete query;
		}
	}

	void QuadtreeSceneManager::destroyQuery( IntersectionQuery* query )
	{
		// check availablity
		if (query->getParent() == this)
		{
			delete query;
		}
	}

	void QuadtreeSceneManager::applyToRenderQueue( DRenderQueue* queue, DCamera* cam )
	{
		cam->update();
		if (mQuadtree != NULL)
		{
			walkQuadtree(mQuadtree, queue, cam);
		}
	}

	void QuadtreeSceneManager::walkQuadtree( Quadrant* quadrant, DRenderQueue* queue, DCamera* cam )
	{
		if (quadrant == NULL)
		{
			return;
		}

		if (cam->isInside(quadrant->getRegion()) != DCamera::FTS_Out)
		{
			Quadrant::QuadtreeSceneNodeIterator ni = quadrant->getSceneNodeIterator();
			while (ni.hasMoreElements())
			{
				if ((*(ni.current()))->isEnabled())
				{
					if(cam->isInside((*(ni.current()))->getInheritedBox()) != DCamera::FTS_Out)
					{
						(*(ni.current()))->applyToRenderQueue(queue);
					}
				}
				ni.moveNext();
			}

			// walk tree
			walkQuadtree(quadrant->getQuadrant(0, 0), queue, cam);
			walkQuadtree(quadrant->getQuadrant(0, 1), queue, cam);
			walkQuadtree(quadrant->getQuadrant(1, 0), queue, cam);
			walkQuadtree(quadrant->getQuadrant(1, 1), queue, cam);
		}
	}

	void QuadtreeSceneManager::shutdownQuadtree( Quadrant* quadrant )
	{
		if (quadrant == NULL)
		{
			return;
		}

		shutdownQuadtree(quadrant->getQuadrant(0, 0));
		shutdownQuadtree(quadrant->getQuadrant(0, 1));
		shutdownQuadtree(quadrant->getQuadrant(1, 0));
		shutdownQuadtree(quadrant->getQuadrant(1, 1));

		quadrant->removeQuadrant(0, 0);
		quadrant->removeQuadrant(0, 1);
		quadrant->removeQuadrant(1, 0);
		quadrant->removeQuadrant(1, 1);
	}
	QuadtreeSceneManagerFactory::QuadtreeSceneManagerFactory() :
		DSceneManagerFactory("QuadtreeSceneManager")
	{

	}

	DSceneManager* QuadtreeSceneManagerFactory::createSceneManager( const DString& name )
	{
		return new QuadtreeSceneManager(this, name);
	}

	void QuadtreeSceneManagerFactory::destroySceneManager( DSceneManager* sm )
	{
		if (sm->getCreator() == this)
		{
			delete sm;
		}
	}

}