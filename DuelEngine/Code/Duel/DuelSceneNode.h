/*
	[7/15/2012 OMEGA] created
	DSceneNode
*/


#ifndef _DUELSCENENODE_H_
#define _DUELSCENENODE_H_

#include "DuelCommon.h"
#include "DuelNode.h"
#include "DuelMovable.h"

namespace Duel {

	class DUEL_API DSceneNode : public DNode
	{
	DUEL_DECLARE_RTTI(DSceneNode)
	public:
		// DSceneNode Should never be created manually, it can be created
		// from DSceneManager only.
		DSceneNode(DSceneManager* creator, const DString& name);
		virtual ~DSceneNode();

		// get the scene manager who created this.
		DSceneManager*			getCreator() const { return mCreator; }

		// destroy a child.
		virtual	void			destroyChild(uint32 index);
		virtual	void			destroyChild(const DString& name);
		virtual	void			destroyChild(DSceneNode* n);
		virtual	void			destroyAllChildren();

		// attach a movable object to this DNode, notice that the DSceneNode
		// use name as the key to accessing object, so the obj's name should
		// be unique.
		virtual	void			attachMovable(DMovable* obj);
		// get the count of objects that attached to this node
		size_t					getAttachedMovableCount() const;
		// query specified object in a more accurate way, slower than
		// the hasObject(const DString& name);
		virtual	bool			hasMovable(DMovable* obj);
		// detach an object in a more accurate way, slower than
		// the detachObject(const DString& name);
		virtual	void			detachMovable(DMovable* obj);
		// detach all objects 
		virtual	void			detachAllMovables();
		// used to traverse objects
		typedef	std::list<DMovable*>		MovableList;
		typedef	ListIterator<MovableList>	MovableIterator;
		MovableIterator			getMovableIterator() { return MovableIterator(mMovableList.begin(), mMovableList.end()); }
		
		// update the boundary of this node, this method will merge all
		// the movable objects' bounding box that attached to this node.
		virtual	void			updateBound();

		// set whether the scene node is available for rendering
		virtual	void			setEnable(bool flag) { mbEnable = flag;}
		// query whether the scene node is in scene graph
		virtual	bool			isEnabled() const { return mbEnable; }

		// get the bouding box that contains all the objects attached to this node,
		// and children's bounding box, this method will give a cached box, meanning
		// that you should updateBound() before you call this method, be careful with
		// efficiency.
		virtual	DAxisAlignedBox	getBoundingBox();
		// get Box that located in world space.
		virtual DAxisAlignedBox	getInheritedBox();

		// apply the attached Movables to the render queue, it is designed as an internal method,
		// DO NOT call it directly.
		INTERNAL virtual void	applyToRenderQueue(DRenderQueue* destQueue, DCamera* cam);


	protected:
		// the DSceneManager who create this DNode.
		DSceneManager*	mCreator;
		// the unique name of this node.
		DString			mName;

		// the bounding box represent the exisitance of the node in world space,
		// it contains all the movable objects.
		DAxisAlignedBox	mWorldAABB;
		DAxisAlignedBox	mBoundingBox;

		// indicate whether the node participate the culling process in DSceneManager
		bool			mbEnable;

		// use hash map to store objects.
		MovableList		mMovableList;


	private:
		// not- allowed.
		DSceneNode() {}
	};

}
#endif
