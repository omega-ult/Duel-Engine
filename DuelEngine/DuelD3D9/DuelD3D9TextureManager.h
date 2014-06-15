//  [6/6/2014 OMEGA] created

#ifndef _DUELD3D9TEXTUREMANAGER_H_
#define _DUELD3D9TEXTUREMANAGER_H_

#include "DuelD3D9Common.h"
#include "DuelTextureManager.h"


namespace Duel
{
	class D3D9TextureManager : public DTextureManager
	{
		DUEL_DECLARE_RTTI(D3D9TextureManager)
	public:
		D3D9TextureManager(D3D9RenderResourceFactory* resFact);
		D3D9RenderResourceFactory*	getD3DResourceFactory();
	protected:
		virtual DResource* createImpl( DResourceDescription* createParam );
		// a pointer to the factory for retrieving d3d9device.
		D3D9RenderResourceFactory*	mResFactory; 
	};

}

#endif