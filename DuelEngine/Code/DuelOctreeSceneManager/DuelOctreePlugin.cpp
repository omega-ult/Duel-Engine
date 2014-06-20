//  [4/12/2013 OMEGA] created

#include "DuelOctreeCommon.h"
#include "DuelOctreePlugin.h"
#include "DuelOctreeSceneManager.h"
#include "DuelOctreePlugin.h"
#include "DuelCore.h"

namespace Duel
{

	const DString sPluginName = "QuadtreePlugin";

	OctreePlugin::OctreePlugin() :
		mFactory(NULL)
	{

	}

	const DString& OctreePlugin::getName() const
	{
		return sPluginName;
	}

	void OctreePlugin::install()
	{
		mFactory = new OctreeSceneManagerFactory();
	}

	void OctreePlugin::initialize()
	{
		DSceneManagerEnumerator::getSingletonPtr()->registerSceneManagerFactory(mFactory);
	}

	void OctreePlugin::shutdown()
	{
		DSceneManagerEnumerator::getSingletonPtr()->unregisterSceneManagerFactory(mFactory);
	}

	void OctreePlugin::uninstall()
	{
		delete mFactory;
		mFactory = NULL;
	}


	OctreePlugin* octreePlugin;

	extern "C" void DUELOCTREE_API dllStartPlugin( void )
	{
		// Create new scene manager
		octreePlugin = new OctreePlugin();

		// Register
		DCore::getSingleton().installPlugin(octreePlugin);

	}
	extern "C" void DUELOCTREE_API dllStopPlugin( void )
	{
		DCore::getSingleton().uninstallPlugin(octreePlugin);
		delete octreePlugin;
	}
}