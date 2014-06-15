//  [6/6/2014 OMEGA] created

#include "DuelD3D9Common.h"
#include "DuelTextureManager.h"
#include "DuelD3D9Texture.h"
#include "DuelD3D9TextureManager.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(D3D9TextureManager, DTextureManager);

	D3D9TextureManager::D3D9TextureManager(D3D9RenderResourceFactory* resFact) :
		mResFactory(resFact)
	{

	}


	DResource* D3D9TextureManager::createImpl( DResourceDescription* createParam )
	{
		D3D9Texture* ret = new D3D9Texture(this, createParam->getName(), createParam->getGroupName());
		ret->setHardwareBufferUsage(createParam->getAs<DTextureDescription>()->memoryStrategy);
		return ret;
	}

	D3D9RenderResourceFactory* D3D9TextureManager::getD3DResourceFactory()
	{
		return mResFactory;
	}

}