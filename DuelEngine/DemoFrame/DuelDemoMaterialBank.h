//  [2/21/2014 OMEGA] created

#ifndef _DUELDEMOMATERIALBANK_H_
#define _DUELDEMOMATERIALBANK_H_

#include "Duel.h"
#include "DuelMaterial.h"
#include "DuelMaterialManager.h"
#include "DuelDemoRenderWorkshop.h"

namespace Duel
{

	class DLambertMaterial : public DMaterial
	{
		DUEL_DECLARE_RTTI(DLambertMaterial)
	public:
		DLambertMaterial();

		virtual DRenderTechnique* getRenderTechnique(uint32 stage, DCamera* cam, LightIterator li);
		void	reload();
	protected:
		DRenderTechniquePtr	mDeferGBufferTech;
		
	};


	class DDemoMaterialBank : public DSingleton<DDemoMaterialBank>
	{
	public:
		DDemoMaterialBank();
		void	init(DDemoRenderWorkshop* rw);
		void	shutdown();
		// provide for debug;
		void	debugReload();
	protected:
		DMaterialPtr	mLambertMtl;
	};

}

#endif