//  [12/2/2012 OMEGA] created


#ifndef	_DUELENTITY_H_
#define _DUELENTITY_H_

#include "DuelCommon.h"
#include "DuelMovable.h"
#include "DuelSubEntity.h"

namespace Duel
{

	class DUEL_API DEntity : public DMovable
	{
		DUEL_DECLARE_RTTI(DEntity)
	public:
		DEntity(const DString& name);
		virtual ~DEntity() {}
		// used to traverse sub-entities.
		typedef	std::vector<DSubEntityPtr>		SubEntityList;
		typedef	VectorIterator<SubEntityList>	SubEntityIterator;
		SubEntityIterator	getSubEntityIterator() { return SubEntityIterator(mSubEntities.begin(), mSubEntities.end()); }

		// construct element from mesh resource.
		void				loadFromMesh(DResourcePtr mesh);
		// get the count of sub entities this entity owned.
		uint32				getSubEntityCount() const;
		// get sub-entity using specified name,
		DSubEntityPtr		getSubEntity(const DString& name);
		// remove specified sub entity.
		void				removeSubEntity(const DString& name);
		// clear all entities
		void				removeAllSubEntities();

		// set destinate render group this entity should be in.
		// default to RenderGroupID:RG_Main.
		void				setRenderGroup(uint32 grp);
		uint32				getRenderGroup();

		// override : DMovable--------------------------
		virtual DString		getTypeName() const;
		// override : DMovable--------------------------
		virtual void		applyToRenderQueue(DRenderQueue* destQueue, DCamera* cam);

	protected:
		SubEntityList		mSubEntities;
		uint32				mRenderGroupID;
		// record the mesh data, used for loading animations.
		DResourcePtr		mMeshTarget;

	};

}

#endif