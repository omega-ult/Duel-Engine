//  [12/4/2012 OMEGA] created

#ifndef _DUELTAGPOINT_H_
#define _DUELTAGPOINT_H_

#include "DuelCommon.h"
#include "DuelBone.h"
#include "DuelEntity.h"
#include "DuelMovable.h"

namespace Duel
{
	// A tagged point on a skeleton, which can be used to attach entities to on specific
    // other entities.
	class DUEL_API DTagPoint : public DBone
	{
	DUEL_DECLARE_RTTI(DTagPoint)
	public:
		// TagPoint should be created in DSkeleton.
		DTagPoint(uint16	handle, DSkeleton* creator);
		DTagPoint(uint16	handle, const DString& name, DSkeleton* creator);
		virtual ~DTagPoint() {}

		// get/set the parent entity, 
		DEntity*			getParentEntity() const;
		void			setParentEntity(DEntity* entity);

		// get/set the child object
		void			setChildObject(DMovable* obj);
		DMovable*	getChildObject() const;


	};
}

#endif