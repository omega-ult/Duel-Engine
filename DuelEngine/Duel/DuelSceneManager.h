/*
	[7/15/2012 OMEGA] created
	DSceneManager 
*/

#ifndef _DUELSCENEMANAGER_H_
#define _DUELSCENEMANAGER_H_

#include "DuelCommon.h"
#include "DuelSceneNode.h"
#include "DuelRenderQueue.h"
#include "DuelSceneQuery.h"
#include "DuelRenderWorkshop.h"


namespace Duel
{

	/** Manages the organisation and rendering of a 'scene' i.e. a collection 
		of objects and potentially world geometry.
    @remarks
		This class defines the interface and the basic behaviour of a 
		'Scene Manager'. A DSceneManager organises the culling and rendering of
		the scene, in conjunction with the DRenderQueue. This class is designed 
		to be extended through subclassing in order to provide more specialised
		scene organisation structures for particular needs. The default 
		DSceneManager culls based on a hierarchy of node bounding boxes, other
		implementations can use an octree (@see OctreeSceneManager), a BSP
		tree (@see BspSceneManager), and many other options. New DSceneManager
		implementations can be added at runtime by plugins, see 
		SceneManagerEnumerator for the interfaces for adding new DSceneManager
		types.
	@par
		There is a distinction between 'objects' (which subclass MovableObject, 
		and are movable, discrete objects in the world), and 'world geometry',
		which is large, generally static geometry. World geometry tends to 
		influence the DSceneManager organisational structure (e.g. lots of indoor
		static geometry might result in a spatial tree structure) and as such
		world geometry is generally tied to a given DSceneManager implementation,
		whilst MovableObject instances can be used with any DSceneManager.
		Subclasses are free to define world geometry however they please.
	@par
		Multiple DSceneManager instances can exist at one time, each one with 
		a distinct scene. Which DSceneManager is used to render a scene is
		dependent on the DCamera, which will always call back the DSceneManager
		which created it to render the scene. 
     */
	class DUEL_API DSceneManager : public DObject
	{
	DUEL_DECLARE_RTTI(DSceneManager)
	public:		
		DSceneManager(DSceneManagerFactory* creator, const DString& type, const DString& name);
		virtual ~DSceneManager();

		// the region defines the managing area of the scene, param granularity decide
		// the smallest size of an area that can be handled. the granularity affect 
		// computation cost by the scene manager, a porper value should be tested by the 
		// client program.
		virtual void		initialize(DAxisAlignedBox region, DReal granularity) {}
		// change the size of the scene manager, this method takes affect after initialization;
		virtual	void		ResizePalette(DAxisAlignedBox region) {}

		const DString&		getType() const {  return mType; }
		const DString&		getName() const { return mName; }
		DSceneManagerFactory*	getCreator() { return mCreator; }

		// scene node operations. they are used in scene graph
		virtual DSceneNode*	createSceneNode();
		virtual	DSceneNode*	createSceneNode(const DString& name);
		virtual	DSceneNode*	getSceneNode(const DString& name);
		virtual	bool		hasSceneNode(const DString& name);
		virtual	void		destroySceneNode(DSceneNode* node);
		virtual	void		destroyAllSceneNodes();

		// used to retrieve all nodes in scene, including those whose value of
		// isInSceneGraph() is false.
		typedef	std::map<DString, DSceneNode*>	SceneNodeMap;
		typedef MapIterator<SceneNodeMap>		SceneNodeIterator;
		SceneNodeIterator	getSceneNodeIterator() { return SceneNodeIterator(mSceneNodeMap.begin(), mSceneNodeMap.end()); }

		// call this method to update every scene nodes.
		virtual void		updateScene();
		// 用来更新节点在manager中的位置而提供的公共接口.
		virtual	void		updateSceneNode(DSceneNode* node);
		// clear all scene node/lights attached to this manager.
		virtual	void		clearScene();

		// the only interface for creating/destroying lights. 
		typedef	std::map<DString, DLightSource*>	LightMap;
		typedef	MapIterator<LightMap>		LightIterator;
		LightIterator		getLightIterator() { return LightIterator(mLightMap.begin(), mLightMap.end()); }
		virtual	DLightSource*		createLight(const DString& name);
		virtual	DLightSource*		getLight(const DString& name);
		virtual	size_t		getLightCount() const { return mLightMap.size(); }
		virtual	bool		hasLight(const DString& name);
		virtual	void		destroyLight(const DString& name);
		virtual	void		destroyLight(DLightSource* light);
		virtual	void		destroyAllLights();

		// populate lights which are affecting current camera's frustum to the render group..
		virtual void		populateLights(DRenderQueue* queue, DCamera* cam);
		// populate lights to a target lightmap, whicha are affecting current camera's frustum.
		INTERNAL virtual void	populateLights(LightMap& outMap, DCamera* cam) { /* leave to sub-class */ }


		// apply current scene to the render queue.
		virtual	void		applyToRenderQueue(DRenderQueue* queue, DCamera* cam) { /* leave to sub-class */ }

		// use this method to provide auto gpu parameters for renderables.
		// such as world matrix for current scene.
		virtual	void		updateAutoGpuParameterMaps();

		// create/destroy scene queries.
		virtual	RayQuery*				createRayQuery() = 0;
		virtual	AxisAlignedBoxQuery*	createAABBQuery() = 0;
		virtual	SphereQuery*			createSphereQuery() = 0;
		virtual	IntersectionQuery*		createIntersectionQuery() = 0;
		virtual	void		destroyQuery(RayQuery* query) = 0;
		virtual	void		destroyQuery(AxisAlignedBoxQuery* query) = 0;
		virtual	void		destroyQuery(SphereQuery* query) = 0;
		virtual	void		destroyQuery(IntersectionQuery* query) = 0;



	protected:
		// sub-class implement these methods
		virtual	DSceneNode*	createSceneNodeImpl() = 0;
		virtual	DSceneNode*	createSceneNodeImpl(const DString& name) = 0;

		virtual	void		destroySceneNodeImpl(DSceneNode* node) = 0;

		// creator of this sceneManager.
		DSceneManagerFactory*	mCreator;
		// the type of the manager.
		DString				mType;
		// name for the ScneneManager.
		DString				mName;

		// scene node storage
		SceneNodeMap		mSceneNodeMap;

		LightMap			mLightMap;


	};

}

#endif