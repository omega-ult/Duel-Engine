//  [2/21/2014 OMEGA] created

#include "pch.h"
#include "DuelDemoMaterialBank.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DLambertMaterial, DMaterial);



	DRenderTechnique* DLambertMaterial::getRenderTechnique( uint32 stage )
	{
		return NULL;
	}

	DLambertMaterial::DLambertMaterial() :
		DMaterial("Lambert")
	{

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

}