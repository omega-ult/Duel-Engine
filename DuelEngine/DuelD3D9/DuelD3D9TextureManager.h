//  [6/6/2014 OMEGA] created

#ifndef _DUELD3D9TEXTUREMANAGER_H_
#define _DUELD3D9TEXTUREMANAGER_H_

#include "DuelD3D9Common.h"
#include "DuelTextureManager.h"

namespace Duel
{
	class DUELD3D9_API D3D9TextureManager : public DTextureManager
	{
		DUEL_DECLARE_RTTI(D3D9TextureManager)
	public:
		D3D9TextureManager();

	protected:
		virtual DResource* createImpl( DResourceDescription* createParam );

	};

}

#endif