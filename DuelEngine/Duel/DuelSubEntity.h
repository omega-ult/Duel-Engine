//  [12/2/2012 OMEGA] created

#ifndef _DUELSUBENTITY_H_
#define _DUELSUBENTITY_H_

#include "DuelCommon.h"
#include "DuelRenderable.h"
#include "DuelSubMesh.h"

namespace Duel
{

	// each sub-entity is a renderable object, 
	class DUEL_API DSubEntity : public DRenderable
	{
	DUEL_DECLARE_RTTI(DSubEntity)
	public:
		// do not call it directly, it is constructed by its parent.
		DSubEntity(DEntity* parent, DSubMeshPtr subMesh);
		virtual	~DSubEntity();

		const DString&		getName();

		// the skeleton instance will be created when constructed. if specified
		// skeleton linked to the submesh existed.
		DSkeletonInstancePtr	getSkeletonInstance();

		DSubMeshPtr			getTargetSubMesh();
		// get the parent entity of this sub-entity.
		DEntity*			getParent() const;

		// query whether this sub-entity is visible.
		bool				isVisible() const;
		// set the visible property of this sub-entity.
		void				setVisible(bool visible);

		DAxisAlignedBox		getBoundingBox() const;

		DMaterialInstancePtr	getMaterialInstance();

		// override : DRenderable-----------------------------
		virtual DRenderTechnique* getRenderTechnique( uint32 stage, DCamera* cam, LightIterator li );
		// override : DRenderable-----------------------------
		virtual void		updateCustomGpuParameter(DShaderObject* so);

	protected:
		// a pointer to its parent.
		DEntity*		mParent;
		DString			mName;
		DSubMeshPtr		mTargetSubMesh;
		// whether the entity is visible.
		bool			mbVisible;
		DMaterialInstancePtr	mMtlInst;
		DSkeletonInstancePtr	mSkelInst;

		DAxisAlignedBox		mBoundingBox;

		DVertexData			mVData;
		DIndexData			mIData;
	};

}
#endif