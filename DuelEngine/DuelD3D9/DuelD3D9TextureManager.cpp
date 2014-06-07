//  [6/6/2014 OMEGA] created

#include "DuelD3D9Common.h"
#include "DuelTextureManager.h"
#include "DuelD3D9TextureManager.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(D3D9TextureManager, DTextureManager);

	D3D9TextureManager::D3D9TextureManager()
	{

	}


	DResource* D3D9TextureManager::createImpl( DResourceDescription* createParam )
	{
		throw std::exception("The method or operation is not implemented.");
	}

}