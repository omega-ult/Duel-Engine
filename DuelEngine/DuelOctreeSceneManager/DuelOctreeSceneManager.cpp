//  [1/18/2013 OMEGA] created

#include "DuelOctreeCommon.h"
#include "DuelOctreeSceneManager.h"
#include "DuelLightSource.h"
#include "DuelOctree.h"
#include "DuelOctreeSceneNode.h"
#include "DuelOctreeQuery.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(OctreeSceneManager, DSceneManager);
	DUEL_IMPLEMENT_RTTI_1(OctreeSceneManagerFactory, DSceneManagerFactory);

	OctreeSceneManager::OctreeSceneManager( DSceneManagerFactory* creator,  const DString& name ) :
		DSceneManager(creator, "OctreeSceneManager", name),
		mNameGen("OctreeNode_"),
		mOctree(NULL)
	{
	}


	OctreeSceneManager::~OctreeSceneManager()
	{
		shutdownOctree(mOctree);
	}

	void OctreeSceneManager::removeOctreeSceneNode( OctreeSceneNode* n )
	{
		if (mOctree == NULL)
		{
			return;
		}

		Octant* oct = n->getOctant();
		if (oct != NULL)
		{
			oct->removeSceneNode(n);
		}
		n->setOctant(NULL);
	}

	void OctreeSceneManager::addOctreeSceneNode( OctreeSceneNode* n, Octant* oct, uint32 depth )
	{
		if (mOctree == NULL)
		{
			return;
		}

		const DAxisAlignedBox& nBox = n->getBoundingBox();
		//if the octree is twice as big as the scene node,
		//we will add it to a child.
		if ((depth < mMaxDepth) && oct->isTwiceSize(nBox))
		{
			uint32 x, y, z;
			oct->getChildIndex(nBox, x, y, z);
			Octant* newOct = oct->getOctant(x, y, z);
			if (newOct == NULL)
			{
				newOct = oct->createOctant(x, y, z);
			}
			addOctreeSceneNode(n, newOct, ++depth);
		}
		else // exceed max depth or octree box fits node box.
		{
			oct->addSceneNode(n);
		}
	}

	void OctreeSceneManager::initialize( DSceneInstance* owner, DAxisAlignedBox region, DReal granularity )
	{
		DSceneManager::initialize(owner, region, granularity);
		if (region.isNull() || region.isInfinite())
		{
			return;
		}
		region.merge(DAxisAlignedBox(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f));
		if (mOctree != NULL)
		{
			shutdownOctree(mOctree);
		}
		mOctree = new Octant(NULL);
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
		mOctree->setRegion(mSceneBox);
	}

	
	void OctreeSceneManager::resize( DAxisAlignedBox region )
	{
		region.merge(DAxisAlignedBox(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f));
		std::list<OctreeSceneNode*> tempList;
		SceneNodeIterator i = getSceneNodeIterator();
		while(i.hasMoreElements())
		{
			OctreeSceneNode* n = static_cast<OctreeSceneNode*>(i.current()->second);
			if (n->isEnabled())
			{
				tempList.push_back(n);
			}
			i.moveNext();
		}
		shutdownOctree(mOctree);
		initialize(mOwner, region, mGranularity);
		std::list<OctreeSceneNode*>::iterator li, iend = tempList.end();
		for(li = tempList.begin(); li != iend; ++li )
		{
			(*li)->setOctant(NULL);
			updateSceneNode(*li);
		}
	}

	void OctreeSceneManager::clearScene()
	{
		initialize(mOwner, mSceneBox, mGranularity);
	}

	void OctreeSceneManager::updateSceneNode( DSceneNode* node )
	{
		assert(DUEL_IS_EXACT_KIND(OctreeSceneNode, node));
		OctreeSceneNode* osn = static_cast<OctreeSceneNode*>(node);
		if (osn->getBoundingBox().isNull())
		{
			return;
		}
		if (mOctree == NULL)
		{
			return;
		}

		if (osn->getOctant() == NULL)
		{
			// if the node is not in root region, force into the root tree,
			if (osn->isIn(mOctree->getRegion()))
			{
				addOctreeSceneNode(osn, mOctree, 0);
			}
			else 
			{
				mOctree->addSceneNode(osn);
			}
			return;
		}
		
		if (!(osn->isIn(osn->getOctant()->getRegion())))
		{
			removeOctreeSceneNode(osn);
			// if the node is not in root region, force into the root tree,
			if (osn->isIn(mOctree->getRegion()))
			{
				addOctreeSceneNode(osn, mOctree, 0);
			}
			else 
			{
				mOctree->addSceneNode(osn);
			}
			return;
		}
	}

	DSceneNode* OctreeSceneManager::createSceneNodeImpl()
	{
		return createSceneNodeImpl(mNameGen.generate());
	}

	DSceneNode* OctreeSceneManager::createSceneNodeImpl( const DString& name )
	{
		return new OctreeSceneNode(this, name);
	}

	void OctreeSceneManager::destroySceneNodeImpl( DSceneNode* node )
	{
		assert(DUEL_IS_EXACT_KIND(OctreeSceneNode, node));
		OctreeSceneNode* n = static_cast<OctreeSceneNode*>(node);
		if (n != NULL)
		{
			removeOctreeSceneNode(n);
		}
		delete n;
	}

	RayQuery* OctreeSceneManager::createRayQuery()
	{
		return new OctreeRayQuery(this);
	}

	AxisAlignedBoxQuery* OctreeSceneManager::createAABBQuery()
	{
		return new OctreeAxisAlignedBoxQuery(this);
	}

	SphereQuery* OctreeSceneManager::createSphereQuery()
	{
		return new OctreeSphereQuery(this);
	}

	IntersectionQuery* OctreeSceneManager::createIntersectionQuery()
	{
		return new OctreeIntersectionQuery(this);
	}

	void OctreeSceneManager::destroyQuery( RayQuery* query )
	{
		// check availablity
		if (query->getParent() == this)
		{
			delete query;
		}
	}

	void OctreeSceneManager::destroyQuery( AxisAlignedBoxQuery* query )
	{
		// check availablity
		if (query->getParent() == this)
		{
			delete query;
		}
	}

	void OctreeSceneManager::destroyQuery( SphereQuery* query )
	{
		// check availablity
		if (query->getParent() == this)
		{
			delete query;
		}
	}

	void OctreeSceneManager::destroyQuery( IntersectionQuery* query )
	{
		// check availablity
		if (query->getParent() == this)
		{
			delete query;
		}
	}

	void OctreeSceneManager::populateLights( LightMap& outMap, DCamera* cam )
	{
		cam->update();
		// if the light is directional light or point light, populate it
		LightMap::iterator i, iend = mLightMap.end();
		for(i = mLightMap.begin(); i != iend; ++i)
		{
			LightType t = i->second->getType();
			if (t == LT_Directional) 
			{
				outMap.insert(std::make_pair(i->first, i->second));
			}
			else if (t == LT_Point)
			{
				DReal outRange;
				i->second->getAttenuation(&outRange, NULL, NULL, NULL);
				DSphere lightSphere(i->second->getPosition(), outRange);
				
				if (cam->isInside(lightSphere) != DCamera::FTS_Out)
				{
					outMap.insert(std::make_pair(i->first, i->second));
				}
			}
			else{
				// TODO: calculate visiblity.
				// using common perpendicular line to calcu late intersection between camera
				// view cone and spot light cone.
				DReal cpLength;
				DRay camRay(cam->getEyePosition(), cam->getDirection());
				DRay spotRay(i->second->getPosition(), i->second->getDirection());
				DRay comPerp = camRay.getCommonPerpendicularTo(spotRay, &cpLength);

				// ensure it is a valid result.
				if (comPerp.getDirection() != DVector3::ZERO)
				{
					// ignore the light if it was in the back side of the camera.
					DVector3 comPerpToCam = (comPerp.getOrigin() - camRay.getOrigin());
					DReal coef = comPerpToCam.dotProduct(cam->getDirection());
					if (coef < 0)
					{
						// check whether the camera's origin is in the spotlight's lighting region
						DVector3 camToSpot = cam->getEyePosition() - spotRay.getOrigin();
						
						DRadian r = DMath::arcCos(
							camToSpot.dotProduct(spotRay.getDirection()) / 
							(camToSpot.length() * spotRay.getDirection().length()));
						if (r < i->second->getSpotlightOuterAngle()/2)
						{
							outMap.insert(std::make_pair(i->first, i->second));
						}
					}
					else
					{
						// calculate intersection between two cone
						DReal camDist = (comPerp.getOrigin()-cam->getEyePosition()).length() * DMath::Tan(cam->getFOV()/2);
						DReal spotDist = ((comPerp.getOrigin()+comPerp.getDirection()*cpLength)
							- spotRay.getOrigin()).length() * DMath::Tan(i->second->getSpotlightOuterAngle()/2);
						if (camDist + spotDist > cpLength)
						{
							outMap.insert(std::make_pair(i->first, i->second));
						}
					}

				}
			}
		}
	}

	void OctreeSceneManager::applyToRenderQueue( DRenderQueue* queue, DCamera* cam )
	{
		cam->update();
		if (mOctree != NULL)
		{
			walkOctree(mOctree, queue, cam);
		}
	}

	void OctreeSceneManager::walkOctree( Octant* octant, DRenderQueue* queue, DCamera* cam )
	{
		if (octant == NULL)
		{
			return;
		}
		
		if (cam->isInside(octant->getRegion()) != DCamera::FTS_Out)
		{
			Octant::OctreeSceneNodeIterator ni = octant->getSceneNodeIterator();
			while (ni.hasMoreElements())
			{
				if(cam->isInside((*(ni.current()))->getBoundingBox()) != DCamera::FTS_Out)
				{
					(*(ni.current()))->applyToRenderQueue(queue);
				}
				ni.moveNext();
			}

			// walk tree
			walkOctree(octant->getOctant(0, 0, 0), queue, cam);
			walkOctree(octant->getOctant(0, 0, 1), queue, cam);
			walkOctree(octant->getOctant(0, 1, 0), queue, cam);
			walkOctree(octant->getOctant(0, 1, 1), queue, cam);
			walkOctree(octant->getOctant(1, 0, 0), queue, cam);
			walkOctree(octant->getOctant(1, 0, 1), queue, cam);
			walkOctree(octant->getOctant(1, 1, 0), queue, cam);
			walkOctree(octant->getOctant(1, 1, 1), queue, cam);
		}
	}

	void OctreeSceneManager::shutdownOctree( Octant* octant )
	{
		if (octant == NULL)
		{
			return;
		}

		shutdownOctree(octant->getOctant(0, 0, 0));
		shutdownOctree(octant->getOctant(0, 0, 1));
		shutdownOctree(octant->getOctant(0, 1, 0));
		shutdownOctree(octant->getOctant(0, 1, 1));
		shutdownOctree(octant->getOctant(1, 0, 0));
		shutdownOctree(octant->getOctant(1, 0, 1));
		shutdownOctree(octant->getOctant(1, 1, 0));
		shutdownOctree(octant->getOctant(1, 1, 1));

		octant->removeOctant(0, 0, 0);
		octant->removeOctant(0, 0, 1);
		octant->removeOctant(0, 1, 0);
		octant->removeOctant(0, 1, 1);
		octant->removeOctant(1, 0, 0);
		octant->removeOctant(1, 0, 1);
		octant->removeOctant(1, 1, 0);
		octant->removeOctant(1, 1, 1);
	}

	OctreeSceneManagerFactory::OctreeSceneManagerFactory() :
		DSceneManagerFactory("OctreeSceneManager")
	{

	}

	DSceneManager* OctreeSceneManagerFactory::createSceneManager( const DString& name )
	{
		return new OctreeSceneManager(this, name);
	}

	void OctreeSceneManagerFactory::destroySceneManager( DSceneManager* sm )
	{
		if (sm->getCreator() == this)
		{
			delete sm;
		}
	}

}