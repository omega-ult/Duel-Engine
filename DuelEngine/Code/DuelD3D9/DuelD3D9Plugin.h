//  [6/6/2014 OMEGA] created

#ifndef _DUELD3D9PLUGIN_H_
#define _DUELD3D9PLUGIN_H_

#include "Duel.h"
#include "DuelD3D9Common.h"

namespace Duel
{
	class D3D9Plugin : public DPlugin
	{
	DUEL_DECLARE_RTTI(D3D9Plugin)
	public:
		D3D9Plugin();
		~D3D9Plugin();
		virtual const DString& getName() const
		{
			return mName;
		}

		virtual void install();

		virtual void initialize(const DString& config);

		virtual void shutdown();

		virtual void uninstall();
	protected:
		DString			mName;
		D3D9RenderSystem*	mRenderSystem;
		D3D9RenderResourceFactory*	mRenderResFactory;
		D3D9TextureManager*	mTextureManager;
		D3D9GpuProgramManger*	mGpuProgramManager;
		DRenderResourceManager* mhbManager;

	};
	D3D9Plugin* d3d9Plugin;
}

#endif