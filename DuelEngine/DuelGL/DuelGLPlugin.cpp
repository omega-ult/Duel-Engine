//  [4/12/2013 OMEGA] created

#include "DuelGLCommon.h"
#include "DuelGLPlugin.h"
#include "DuelCore.h"
#include "DuelGLRenderSystem.h"
#include "DuelRenderResourceManager.h"
#include "DuelGLRenderResourceFactory.h"
#include "DuelGLTextureManager.h"
#include "DuelGLGpuProgramManager.h"


namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(GLPlugin, DPlugin);

	GLPlugin::GLPlugin() :
		mName("GLPlugin"),
		mRenderSystem(NULL),
		mGpuProgramManager(NULL),
		mTextureManager(NULL),
		mhbManager(NULL),
		mRenderResFactory(NULL)
	{

	}

	void GLPlugin::install()
	{
		mRenderSystem = new GLRenderSystem();
		mRenderResFactory = new GLRenderResourceFactory();
		mTextureManager = new GLTextureManager();
		mGpuProgramManager = new GLGpuProgramManger();
		mhbManager = new DRenderResourceManager(mRenderResFactory);
	}

	void GLPlugin::initialize()
	{
		mRenderResFactory->initialize();
		mRenderSystem->initialize();
		DCore::getSingleton().setRenderSystem(mRenderSystem);
		DResourceGroupManager::getSingletonPtr()->registerManager(mGpuProgramManager);
		DResourceGroupManager::getSingletonPtr()->registerManager(mTextureManager);
	}


	void GLPlugin::shutdown()
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

	void GLPlugin::uninstall()
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

	GLPlugin::~GLPlugin()
	{
		shutdown();
		uninstall();
	}



	extern "C" void DUELGL_API dllStartPlugin( void )
	{
		// Create new plugin
		glPlugin = new GLPlugin();

		// Register
		DCore::getSingleton().installPlugin(glPlugin);

	}
	extern "C" void DUELGL_API dllStopPlugin( void )
	{
		DCore::getSingleton().uninstallPlugin(glPlugin);
		delete glPlugin;
	}
}