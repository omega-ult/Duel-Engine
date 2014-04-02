//  [2/21/2014 OMEGA] created

#include "pch.h"
#include "DuelDemoMaterialBank.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DLambertMaterial, DMaterial);



	DRenderTechnique* DLambertMaterial::getRenderTechnique( uint32 stage )
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
		mPersistInstance->addParameter("NormalTexture", "Demo_DeferGBuffer.ps", "normalTexture", MPT_Texture, 1, true);
		mPersistInstance->addParameter("SpecularTexture", "Demo_DeferGBuffer.ps", "specularTexture", MPT_Texture, 1, true);

	}

	void DLambertMaterial::reload()
	{
		DResourceGroupManager* rg = DResourceGroupManager::getSingletonPtr();
		DResourcePtr renderEffect = rg->getResouceManager("RenderEffect")->getResource("_BasicShaderPack", "Demo_MaterialBank.dre");
		DRenderTechnique::RenderPassIterator ri = mDeferGBufferTech->getRenderPassIterator();
		while (ri.hasMoreElements())
		{
			DRenderPassPtr p = ri.getNext();
			p->getVertexProgram()->reload();
			p->getPixelProgram()->reload();
		}
		renderEffect->reload();
		mDeferGBufferTech = renderEffect->getAs<DRenderEffect>()->getTechnique("Lambert_Defer_GBuffer");
	}


	DDemoMaterialBank::DDemoMaterialBank()
	{

	}

	void DDemoMaterialBank::init( DDemoRenderWorkshop* rw )
	{
		DResourceGroupManager::getSingleton().createGroup("_DemoMaterial");
		mLambertMtl = DMaterialPtr(new DLambertMaterial());

		DMaterialManager::getSingleton().registerMaterial(mLambertMtl);
	}

	void DDemoMaterialBank::shutdown()
	{
		if (mLambertMtl)
		{
			DMaterialManager::getSingleton().unregisterMaterial("Lambert");
			mLambertMtl.reset();
		}
	}

	void DDemoMaterialBank::debugReload()
	{
		mLambertMtl->getAs<DLambertMaterial>()->reload();
	}

}