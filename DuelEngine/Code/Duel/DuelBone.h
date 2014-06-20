//  [11/2/2012 OMEGA] created

#ifndef _DUELBONE_H_
#define _DUELBONE_H_

#include "DuelCommon.h"
#include "DuelNode.h"
#include "DuelSubSkeleton.h"

namespace Duel
{
	// an bone in a skeleton is like a joint that created for DSkeleton.
	// see skeletal animation for more information
	class DUEL_API DBone : public DNode
	{
	DUEL_DECLARE_RTTI(DBone)
	public:
		// this constructor should only be called by DSkeleton.
		DBone(uint16 handle, DSubSkeleton* creator);
		DBone(uint16 handle, const DString& name, DSubSkeleton* creator);
		virtual ~DBone() {}
		// override:DNode------------------------------------------
		void		addChild(DNode* child);
		
		// set the default value of the bone, these value will be set
		// through the calling of reset();
		void		setDefaultProperties(const DVector3& dPos,
										 const DVector3& dScale,
										 const DQuaternion& dOrient);
		
		// use given handle to create a new DBone.
		DBone*		createChild(uint16 handle);	
		// use given handle to create a new DBone.
		DBone*		createChild(uint16 handle, const DString& name);			
		
		// get the handle of the bone.
		uint16		getHandle() { return mHandle; }
		// get a pointer to the skeleton who create this DBone.
		DSubSkeleton*	getCreator() { return mCreator; }
		// reset the bone to the default state.
		void		reset();

	protected:
		// override:DNode------------------------------------------
		// this method is designed for DSCodec.
		void		setParent(DBone* p);
		// override:DNode------------------------------------------
		DNode*		createChildImpl();
		// override:DNode------------------------------------------
		DNode*		createChildImpl(const DString& name);
		// override:DNode------------------------------------------
		// do not copy mHandle. because it is assigned by the skeleton and must keep unique.
		void		copyToNode(DNode* destNode);
		
		// the skeleton who create this bone.
		DSubSkeleton*			mCreator;
		// the handle of this bone.
		uint16				mHandle;

		// the default properties of the bone.
		DVector3			mDefaultPos;
		DVector3			mDefaultScale;
		DQuaternion			mDefaultOrient;

		friend	class DSCodec_v1_0;
	private:
		// forbidden operations.
		DBone(){}
		DBone&	operator = (const DBone& bone) { return *this; }
	};
}
#endif