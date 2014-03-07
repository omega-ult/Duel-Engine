//  [4/12/2013 OMEGA] created

#include "DuelGLCommon.h"
#include "DuelGLTextureManager.h"
#include "DuelGLTexture.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(GLTextureManager, DTextureManager);



	DResource* GLTextureManager::createImpl( DResourceDescription* createParam )
	{
		DTextureDescription* resDesc = createParam->getAs<DTextureDescription>(false);
		GLTexture* ret = NULL;
		if (resDesc != NULL)
		{
			ret = new GLTexture(this, resDesc->getName(), resDesc->getGroupName()); 
			ret->setCodecFormat(resDesc->codecFormat);
		}
		return ret;
		
	}

}