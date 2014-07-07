//  [2/21/2014 OMEGA] created

#ifndef _DUELDEMOMATERIALBANK_H_
#define _DUELDEMOMATERIALBANK_H_

#include "Duel.h"
#include "DuelMaterial.h"
#include "DuelMaterialManager.h"
#include "DuelDemoRenderWorkshop.h"

namespace Duel
{

	class DSkyDomeMaterial : public DMaterial
	{
		DUEL_DECLARE_RTTI(DSkyDomeMaterial)
	public:
		DSkyDomeMaterial();
		virtual	DRenderTechnique* getRenderTechnique(uint32 stage, DCamera* cam, LightIterator li);

	protected:
		DRenderTechniquePtr	mSkyDomeTech;
		DTextureSampler		mSkyColorSamp;
	};

	class DLambertMaterial : public DMaterial
	{
		DUEL_DECLARE_RTTI(DLambertMaterial)
	public:
		DLambertMaterial();

		virtual DRenderTechnique* getRenderTechnique(uint32 stage, DCamera* cam, LightIterator li);
	protected:
		DRenderTechniquePtr	mDeferGBufferTech;
		DTextureSampler mAlbedoSamp;
		DTextureSampler mNormalSamp;
		DTextureSampler mRoughnessSamp;
	};


	class DDemoMaterialBank : public DSingleton<DDemoMaterialBank>
	{
	public:
		DDemoMaterialBank();
		void	init(DDemoRenderWorkshop* rw);
		void	shutdown();
	protected:
		DMaterialPtr	mSkyDomeMtl;
		DMaterialPtr	mLambertMtl;
	};

}

#endif