//  [7/15/2012 OMEGA] created


#ifndef _DUELMOVABLE_H_
#define	_DUELMOVABLE_H_

#include "DuelCommon.h"
#include "DuelNode.h"

namespace Duel 
{
	

	// objects that in the scece are called DMovable Objects, this is the interface
	// defining their actions and properties.
	class DUEL_API DMovable : public DObject
	{
	DUEL_DECLARE_RTTI(DMovable)
	public:
		// notify listeners the mo is destroyed.
		DSignal<void(DMovable*)>	signalDestroyed;
		// notify when the mo is attached to the scene node
		DSignal<void(DMovable*)>	signalAttached;
		// notify when the mo is detached from the node, you can get 
		// the detached node with getParentNode() within this function, it will 
		// return the last node this mo attached to.
		DSignal<void(DMovable*)>	signalDetached;
			// notify when the mo moved.
		DSignal<void(DMovable*)>	signalMoved;

		// the movable object should be created by a factory, this creating method
		// provide the variety for defining new object.
		DMovable(const DString& name);
		// destruction method will check whether the object has attached to a node,
		// detach the object from its parent before deleting it will be more efficient.
		virtual ~DMovable();

		virtual DString			getTypeName() const = 0;
		const DString&			getName() const { return mName; }

		// set a new parent for this object. you'd better not call this method directly,
		// because the parent node may own a pointer to this object, and it still work,
		// use parent node's method to handle that instead.
		INTERNAL virtual void	attachToNode(DSceneNode* parent);
		// movable object is moved through DNode, 
		virtual	DSceneNode*		getAttachedNode() const { return mAttachedNode; }

		virtual	void			setVisible(bool visible) { mbVisible = visible; }
		virtual	bool			isVisible() const { return mbVisible; }

		virtual	bool			isAttachedToNode() const { return mAttachedNode != NULL; }

		// the bounding box is the box that contains all visible objects within this
		// movable object, it is recommanded that the box contains ALL things, otherwise
		// an incorrect culling result could be given in SceneManager.
		virtual	void			setBoundingBox(const DAxisAlignedBox& box) { mBoundingBox = box; }
		virtual const DAxisAlignedBox&	getBoundingBox() const { return mBoundingBox; }
		// set/getÅÐ¶¨Çò, Äã‚ƒ¶¼Ï²šgµÄ
		virtual	void				setBoundingSphere(const DSphere& s) { mBoundingSphere = s; }
		virtual	const DSphere&		getBoundingSphere() const { return mBoundingSphere; }
		

		// apply the renderables to the render queue, it is designed as an internal method,
		// DO NOT call it directly.
		INTERNAL virtual void	applyToRenderQueue(DRenderQueue* destQueue) {}



	protected:

		// name of the object
		DString					mName;
		// the target node this object attached to
		DSceneNode*				mAttachedNode;

		// whether the object is visible
		bool					mbVisible;
		
		// local AABB,
		DAxisAlignedBox			mBoundingBox;
		// local BoundingSphere.
		DSphere					mBoundingSphere;


	private:
		// not allowed.
		DMovable() {}
	};

}

#endif