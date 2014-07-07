//  [2/21/2014 OMEGA] created

#include "pch.h"
#include "DuelDemoMaterialBank.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DSkyDomeMaterial, DMaterial);
	DUEL_IMPLEMENT_RTTI_1(DLambertMaterial, DMaterial);


	DSkyDomeMaterial::DSkyDomeMaterial() :
		DMaterial("SkyDome")
	{
		DResourceGroupManager* rg = DResourceGroupManager::getSingletonPtr();
		DResourcePtr renderEffect = rg->getResouceManager("RenderEffect")->getResource("_BasicShaderPack", "Demo_MaterialBank.dre");
		renderEffect->touch();
		mSkyDomeTech = renderEffect->getAs<DRenderEffect>()->getTechnique("SkyDome_Defer_GBuffer");

		mPersistInstance->addParameter("SkyTexture", "Demo_SkyDome.ps", "skyTexture", MPT_Texture, 1, true);
		mPersistInstance->addParameter("SkySampler", "Demo_SkyDome.ps", "skyTextureSamp", MPT_Sampler);

		mPersistInstance->setValue("SkySampler", mSkyColorSamp);
	}

	DRenderTechnique* DSkyDomeMaterial::getRenderTechnique( uint32 stage, DCamera* cam, LightIterator li )
	{
		if (stage == RS_Defer_GBuffer)
		{
			return mSkyDomeTech.get();
		}
		return NULL;
	}


	DRenderTechnique* DLambertMaterial::getRenderTechnique(uint32 stage, DCamera* cam, LightIterator li)
	{
		if (stage == RS_Defer_GBuffer)
		{
			return mDeferGBufferTech.get();
		}
		return NULL;
	}

	DLambertMaterial::DLambertMaterial() :
		DMaterial("Lambert")
	{
		DResourceGroupManager* rg = DResourceGroupManager::getSingletonPtr();
		DResourcePtr renderEffect = rg->getResouceManager("RenderEffect")->getResource("_BasicShaderPack", "Demo_MaterialBank.dre");
		renderEffect->touch();
		mDeferGBufferTech = renderEffect->getAs<DRenderEffect>()->getTechnique("Lambert_Defer_GBuffer");
		
		mPersistInstance->addParameter("DiffuseTexture", "Demo_DeferGBuffer.ps", "diffuseTexture", MPT_Texture, 1, true);
		mPersistInstance->addParameter("DiffuseSampler", "Demo_DeferGBuffer.ps", "diffuseTextureSamp", MPT_Sampler);
		mPersistInstance->addParameter("NormalTexture", "Demo_DeferGBuffer.ps", "normalTexture", MPT_Texture, 1, true);
		mPersistInstance->addParameter("NormalSampler", "Demo_DeferGBuffer.ps", "normalTextureSamp", MPT_Sampler);
		mPersistInstance->addParameter("SpecularTexture", "Demo_DeferGBuffer.ps", "specularTexture", MPT_Texture, 1, true);
		mPersistInstance->addParameter("SpecularSampler", "Demo_DeferGBuffer.ps", "specularTextureSamp", MPT_Sampler);

		mAlbedoSamp.mipFilter = FO_None;
		mNormalSamp.mipFilter = FO_None;
		mPersistInstance->setValue("DiffuseSampler", mAlbedoSamp);
		mPersistInstance->setValue("NormalSampler", mNormalSamp);

	}
// 
// 	void DLambertMaterial::reload()
// 	{
// 		DResourceGroupManager* rg = DResourceGroupManager::getSingletonPtr();
// 		DResourcePtr renderEffect = rg->getResouceManager("RenderEffect")->getResource("_BasicShaderPack", "Demo_MaterialBank.dre");
// 		DRenderTechnique::RenderPassIterator ri = mDeferGBufferTech->getRenderPassIterator();
// 		while (ri.hasMoreElements())
// 		{
// 			DRenderPassPtr p = ri.getNext();
// 			p->getVertexProgram()->reload();
// 			p->getPixelProgram()->reload();
// 		}
// 		renderEffect->reload();
// 		mDeferGBufferTech = renderEffect->getAs<DRenderEffect>()->getTechnique("Lambert_Defer_GBuffer");
// 	}


	DDemoMaterialBank::DDemoMaterialBank()
	{

	}

	void DDemoMaterialBank::init( DDemoRenderWorkshop* rw )
	{
		DResourceGroupManager::getSingleton().createGroup("_DemoMaterial");
		mSkyDomeMtl = DMaterialPtr(new DSkyDomeMaterial());
		mLambertMtl = DMaterialPtr(new DLambertMaterial());

		DMaterialManager::getSingleton().registerMaterial(mLambertMtl);
		DMaterialManager::getSingleton().registerMaterial(mSkyDomeMtl);
	}

	void DDemoMaterialBank::shutdown()
	{
		if (mLambertMtl)
		{
			DMaterialManager::getSingleton().unregisterMaterial("SkyDome");
			DMaterialManager::getSingleton().unregisterMaterial("Lambert");
			mLambertMtl.reset();
		}
	}

}