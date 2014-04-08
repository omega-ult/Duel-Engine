//  [12/22/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelEntity.h"
#include "DuelSceneNode.h"
#include "DuelRenderQueue.h"
#include "DuelSubEntity.h"
#include "DuelSceneManager.h"
#include "DuelAutoGpuParameter.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DEntity, DMovable);

	DEntity::DEntity( const DString& name ) :
		DMovable(name),
		mRenderGroupID(RG_Main_Opaque)
	{

	}

	Duel::DString DEntity::getTypeName() const
	{
		return "Entity";
	}

	void DEntity::applyToRenderQueue( DRenderQueue* destQueue )
	{
		SubEntityIterator sei = getSubEntityIterator();
		while (sei.hasMoreElements())
		{
			DSubEntityPtr se = sei.getNext();
			if (se->isVisible())
			{
				destQueue->addRenderale(mRenderGroupID, se.get());
			}
		}
	}

	void DEntity::setRenderGroup( uint32 grp )
	{
		mRenderGroupID = grp;
	}

	uint32 DEntity::getRenderGroup()
	{
		return mRenderGroupID;
	}

	void DEntity::loadFromMesh( DResourcePtr mesh )
	{
		removeAllSubEntities();
		mMeshTarget.reset();
		mBoundingBox = DAxisAlignedBox::AAB_NULL;
		DMesh* meshPtr = mesh->getAs<DMesh>(false);
		if (meshPtr != NULL && meshPtr->isLoaded())
		{
			mMeshTarget = mesh;
			DMesh::SubMeshIterator sbi = meshPtr->getSubMeshIterator();
			while (sbi.hasMoreElements())
			{
				DSubMeshPtr sb = sbi.getNext();
				DSubEntity* newSubEntity = new DSubEntity(this, sb);
				mBoundingBox.merge(newSubEntity->getBoundingBox());
				mSubEntities.push_back(DSubEntityPtr(newSubEntity));
			}
		}

	}

	uint32 DEntity::getSubEntityCount() const
	{
		return mSubEntities.size();
	}

	DSubEntityPtr DEntity::getSubEntity( const DString& name ) 
	{
		SubEntityList::iterator i, iend = mSubEntities.end();
		for (i = mSubEntities.begin(); i != iend; ++i)
		{
			if ((*i)->getName() == name)
			{
				return *i;
			}
		}
		return DSubEntityPtr();
	}

	void DEntity::removeSubEntity( const DString& name )
	{
		SubEntityList::iterator i, iend = mSubEntities.end();
		for (i = mSubEntities.begin(); i != iend; ++i)
		{
			if ((*i)->getName() == name)
			{
				mSubEntities.erase(i);
			}
		}
	}

	void DEntity::removeAllSubEntities()
	{
		mSubEntities.clear();
	}

}