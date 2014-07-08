//  [6/6/2014 OMEGA] created

#include "DuelD3D9Common.h"
#include "Duel.h"
#include "DuelD3D9Plugin.h"
#include "DuelD3D9RenderSystem.h"
#include "DuelD3D9RenderResourceFactory.h"
#include "DuelD3D9TextureManager.h"
#include "DuelD3D9GpuProgramManager.h"


namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(D3D9Plugin, DPlugin);


	D3D9Plugin::D3D9Plugin() :
		mName("D3D9Plugin"),
		mRenderSystem(NULL),
		mGpuProgramManager(NULL),
		mTextureManager(NULL),
		mhbManager(NULL),
		mRenderResFactory(NULL)
	{

	}

	D3D9Plugin::~D3D9Plugin()
	{
		shutdown();
		uninstall();
	}

	void D3D9Plugin::install()
	{
		mRenderResFactory = new D3D9RenderResourceFactory();
		mRenderSystem = new D3D9RenderSystem(mRenderResFactory);
		mGpuProgramManager = new D3D9GpuProgramManger();
		mhbManager = new DRenderResourceManager(mRenderResFactory);
		mTextureManager = new D3D9TextureManager(mRenderResFactory);
	}

	void D3D9Plugin::initialize(const DString& config)
	{
		mRenderResFactory->initialize();
		mRenderSystem->initialize();
		DCore::getSingleton().setRenderSystem(mRenderSystem);
		DResourceGroupManager::getSingletonPtr()->registerManager(mGpuProgramManager);
		DResourceGroupManager::getSingletonPtr()->registerManager(mTextureManager);
	}

	void D3D9Plugin::shutdown()
	{
		mRenderSystem->shutdown();
		if (DCore::getSingleton().getRenderSystem() == mRenderSystem)
		{
			DCore::getSingleton().setRenderSystem(NULL);
		}
		DResourceGroupManager::getSingletonPtr()->unregisterManager(mGpuProgramManager);
		DResourceGroupManager::getSingletonPtr()->unregisterManager(mTextureManager);
		mRenderResFactory->shutdown();
	}

	void D3D9Plugin::uninstall()
	{

		if (mRenderSystem)
		{
			delete mRenderSystem;
			mRenderSystem = NULL;
		}
		if (mGpuProgramManager)
		{
			delete mGpuProgramManager;
			mGpuProgramManager = NULL;
		}
		if (mTextureManager)
		{
			delete mTextureManager;
			mTextureManager = NULL;
		}
		if (mhbManager)
		{
			delete mhbManager;
			mhbManager = NULL;
		}
		if (mRenderResFactory)
		{
			delete mRenderResFactory;
			mRenderResFactory = NULL;
		}
	}


	extern "C" void DUELD3D9_API dllStartPlugin( const DString& config )
	{
		// Create new plugin
		d3d9Plugin = new D3D9Plugin();

		// Register
		DCore::getSingleton().installPlugin(d3d9Plugin, config);

	}
	extern "C" void DUELD3D9_API dllStopPlugin( void )
	{
		DCore::getSingleton().uninstallPlugin(d3d9Plugin);
		delete d3d9Plugin;
	}
}