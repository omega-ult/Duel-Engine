//  [7/6/2014 OMEGA] created

#ifndef _DUELSKYDOME_H_
#define _DUELSKYDOME_H_

#include "DuelCommon.h"
#include "DuelRenderable.h"
#include "DuelSubMesh.h"

namespace Duel
{
	class DUEL_API DSkyComponent : public DRenderable
	{
		DUEL_DECLARE_RTTI(DSkyComponent)
	public:
		DSkyComponent(DSkyDome* parent, DSubMeshPtr subMesh);

		const DString& getName();

		DMaterialInstancePtr	getMaterialInstance();

		void			setCameraPosition(DVector3 pos);
		// override : DRenderable-----------------------------
		virtual DRenderTechnique* getRenderTechnique( uint32 stage, DCamera* cam, LightIterator li );
		// override : DRenderable-----------------------------
		virtual void	updateCustomGpuParameter(DShaderObject* so);

		virtual void	getWorldTransform( DMatrix4& outMat );

		virtual DReal	getSquaredViewDepthToCamera( const DCamera* cam );

	protected:
		DSkyDome*		mParent;
		DString			mName;
		DSubMeshPtr		mTargetSubMesh;

		DMaterialInstancePtr	mMtlInst;
		DSkeletonInstancePtr	mSkelInst;
		
		DVertexData			mVData;
		DIndexData			mIData;

		DVector3			mCamPos;
	};

	class DUEL_API DSkyDome : public DObject
	{
		DUEL_DECLARE_RTTI(DSkyDome)
	public:
		DSkyDome();

		void	loadFromMesh(DResourcePtr mesh);
		DSkyComponent*	getSkyComponent(const DString& name);

		// indicate the radius of the sky dome, default value is 10000.0f;
		void	setSkyCompacity(DReal comp);
		DReal	getSkyCompacity();

		void	applyToRenderQueue(DRenderQueue* destQueue, DCamera* cam);

		typedef std::vector<DSkyComponent*>	SkyComponentList;
		typedef VectorIterator<SkyComponentList>	SkyComponentIterator;
		SkyComponentIterator	getSkyComponentIterator() { return SkyComponentIterator(mCompList); }

	protected:
		SkyComponentList mCompList;

		DResourcePtr	mMeshTarget;

		DReal	mSkyCompacity;
	};

}

#endif