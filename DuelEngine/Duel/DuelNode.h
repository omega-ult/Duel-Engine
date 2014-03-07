//  [11/2/2012 OMEGA] created

#ifndef _DUELNODE_H_
#define _DUELNODE_H_

#include "DuelCommon.h"

namespace Duel
{
	// the relative space of the node's transformation.
	enum TransformSpace
	{
		TS_Local,
		TS_Parent
	};

	// a common base class for defining a node in 3D space,
	class DUEL_API DNode : public DObject
	{
	DUEL_DECLARE_RTTI(DNode)
	public:
		//////////////////////////////////////////////////////////////////////////
		// signals block
		DSignal<void(const DNode* node)>
			signalUpdated;
		DSignal<void(const DNode* node)>
			signalDestroyed;
		DSignal<void(const DNode* parent, const DNode* child)>
			signalAttached;
		DSignal<void(const DNode* parent, const DNode* child)>
			signalDetached;
		DNode();
		DNode(const DString& name);
		// notice that this method will delete the whole sub-nodes. try to clone this node
		// if you need it for further use.
		virtual ~DNode();
		// store children.
		typedef	std::set<DNode*>	ChildSet;
		typedef	IteratorWrapper<ChildSet, ChildSet::iterator, ChildSet::value_type>	ChildSetIterator;
		ChildSetIterator	getChildSetIterator() { return ChildSetIterator(mChildren.begin(), mChildren.end()); }

		void				setName(const DString& name);
		const DString&		getName();
		// set the properties of this node, param is relative to it parent or absolute to 
		// the parent.
		void				setRelativePosition(const DVector3& pos);
		void				setRelativeScale(const DVector3& s);
		void				setRelativeOrientation(const DQuaternion& orient);
		
		// get properties
		const DVector3&		getRelativePosition() const;
		const DVector3&		getRelativeScale() const;
		const DQuaternion&	getRelativeOrientation() const;

		// set/query the inherited properties. notice that the inherited scale is not usually
		// used, so it is false in default, oppsite to others.
		// pos inheriting is set false as default;
		void				setPositionInherited(bool inherited) { mbPosInherited = inherited; }
		bool				isPositionInherited() const { return mbPosInherited; }
		// scale inheriting is set false as default;
		void				setScaleInherited(bool inherited) { mbScaleInherited = inherited; }
		bool				isScaleInherited() const { return mbScaleInherited; }
		// orient inheriting is set false as default;
		void				setOrientInherited(bool inherited) { mbOrientInherited = inherited; }
		bool				isOrientInherited() const { return mbOrientInherited; }
		// use these methods to ge derived properties, if specified
		// properties is set inheriting disable, then the value will
		// be its relative position.
		const DVector3&		getInheritedPosition();
		const DVector3&		getInheritedScale();
		const DQuaternion&	getInheritedOrient();
		// form a transformation from derived properties.
		const DMatrix4&		getInheritedTransfrom();


		// create a child
		virtual	DNode*		createChild();
		virtual	DNode*		createChild(const DString& name);

		// add a child to this node, notice that this method will
		// make this node be hold by this DNode, and it will manage
		// the life time of this child, check if you want to clone the
		// child before adding.
		virtual	void		addChild(DNode* child);

		// get the parent node
		virtual DNode*		getParent();

		// get the root node.
		virtual	DNode*		getRootNode();

		// get a child with specified name, if there are 
		// more than one node that hold same name, the first one
		// will be returned. if the specified DNode does not exist,
		// a NULL will be returned.
		virtual	DNode*		getChild(uint32	index);
		virtual	DNode*		getChild(const DString& name);
		
		// get the count of the children.
		size_t				getChildCount();
		// get the children.
		const ChildSet&		getChildSet();

		// remove child with specified index/name, if there were more thand
		// one node share the same name, the first node will be removed.
		// notice that this method will not delete the node.
		virtual	DNode*		removeChild(uint32 index);
		virtual	DNode*		removeChild(const DString& name);
		virtual	DNode*		removeChild(DNode* n);
		virtual	void		removeAllChildren();


		// operations on the node
		virtual void		translate(const DVector3& t, TransformSpace space = TS_Local);
		virtual void		translateX(DReal x, TransformSpace space = TS_Local);
		virtual void		translateY(DReal y, TransformSpace space = TS_Local);
		virtual void		translateZ(DReal z, TransformSpace space = TS_Local);
		virtual void		scale(const DVector3& s);
		virtual void		rotate(const DVector3& axis, const DRadian& r, TransformSpace space = TS_Local);
		virtual void		rotate(const DRotator& r, TransformSpace space = TS_Local);
		virtual void		rotate(const DQuaternion& r, TransformSpace space = TS_Local);
		virtual void		yaw(const DRadian& y, TransformSpace space = TS_Local);
		virtual void		pitch(const DRadian& p, TransformSpace space = TS_Local);
		virtual void		roll(const DRadian& r, TransformSpace space = TS_Local);


		/** To be called in the event of transform changes to this node that require it's recalculation.
        @remarks
            This not only tags the node state as being 'dirty', it also requests it's parent to 
            know about it's dirtiness so it will get an update next time.
		@param forceParentUpdate Even if the node thinks it has already told it's
			parent, tell it anyway
        */
        INTERNAL virtual void	needUpdate(bool forceParentUpdate = false);
        /** Called by children to notify their parent that they need an update. 
		@param forceParentUpdate Even if the node thinks it has already told it's
			parent, tell it anyway
		*/
        INTERNAL virtual void	requestUpdate(DNode* child, bool forceParentUpdate = false);
        /** Called by children to notify their parent that they no longer need an update. */
        INTERNAL virtual void	cancelUpdate(DNode* child);

	protected:

		// inner method to set the parent, should not be called outside
		void				setParent(DNode* p);

		// used for sub-class to create DNode.
		virtual	DNode*		createChildImpl();
		virtual	DNode*		createChildImpl(const DString& name);
		// this method used to copy the properties to the destNode,
		// but sub-class should NEVER copy mListeners or mChildSet to that.
		//virtual	void		copyToNode(DNode* destNode);

		/** Internal method to update the DNode.
            @note
                Updates this node and any relevant children to incorporate transforms etc.
                Don't call this yourself unless you are writing a SceneManager implementation.
            @param
                updateChildren If true, the update cascades down to all children. Specify false if you wish to
                update children separately, e.g. because of a more selective SceneManager implementation.
            @param
                parentHasChanged This flag indicates that the parent xform has changed,
                    so the child should retrieve the parent's xform and combine it with its own
                    even if it hasn't changed itself.
        */
		virtual	void		update(bool updateChildren, bool parentHasChanged);
		/** Triggers the node to update it's combined transforms.
            @par
                This method is called internally by Ogre to ask the node
                to update it's complete transformation based on it's parents
                derived transform.
        */
        virtual void		updateFromParent(void);

		/** Class-specific implementation of _updateFromParent.
		@remarks
			Splitting the implementation of the update away from the update call
			itself allows the detail to be overridden without disrupting the 
			general sequence of updateFromParent (e.g. raising events)
		*/
		virtual void		updateFromParentImpl(void);


		// a set contains children which requested update.
		typedef	std::set<DNode*>	ChildUpdateSet;
		ChildUpdateSet	mChildrenToUpdate;
		/// Flag to indicate own transform from parent is out of date
		bool mbNeedParentUpdate;
		/// Flag indicating that all children need to be updated
		bool mbNeedChildUpdate;
		/// Flag indicating that parent has been notified about update request
		bool mbParentNotified ;

		// pointer to the parent node
		DNode*				mParent;
		// name
		DString				mName;

		// basic properties to locate this node in a 3D space
		DVector3				mRelativePos;
		DVector3				mRelativeScale;
		DQuaternion			mRelativeOrient;

		// inheriting properies' flag.
		bool				mbPosInherited;
		bool				mbScaleInherited;
		bool				mbOrientInherited;
		// inherited properties.
		DVector3				mInheritedPos;
		DVector3				mInheritedScale;
		DQuaternion			mInheritedOrient;
		// flag indicate the transfrom is out of date
		bool				mbInheritedTransformOutOfDate;
		DMatrix4				mInheritedTransform;

		// used to store children.
		ChildSet			mChildren;
	private:
		// forbbiden operation
		DNode&	operator = (const DNode& n) { return *this; }
	};

}
#endif