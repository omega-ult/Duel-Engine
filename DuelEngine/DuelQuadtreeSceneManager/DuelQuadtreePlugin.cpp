//  [3/10/2013 OMEGA] created

#include "DuelQuadtreeCommon.h"
#include "DuelQuadtreeSceneManager.h"
#include "DuelQuadtreePlugin.h"
#include "DuelCore.h"

namespace Duel
{

	const DString sPluginName = "QuadtreePlugin";

	QuadtreePlugin::QuadtreePlugin() :
		mFactory(NULL)
	{

	}

	const DString& QuadtreePlugin::getName() const
	{
		return sPluginName;
	}

	void QuadtreePlugin::install()
	{
		mFactory = new QuadtreeSceneManagerFactory();
	}

	void QuadtreePlugin::initialize()
	{
		DSceneManagerEnumerator::getSingletonPtr()->registerSceneManagerFactory(mFactory);
	}

	void QuadtreePlugin::shutdown()
	{
		DSceneManagerEnumerator::getSingletonPtr()->unregisterSceneManagerFactory(mFactory);
	}

	void QuadtreePlugin::uninstall()
	{
		delete mFactory;
		mFactory = NULL;
	}

	QuadtreePlugin* quadtreePlugin;

	extern "C" void DUELQUADTREE_API dllStartPlugin( void )
	{
		// Create new scene manager
		quadtreePlugin = new QuadtreePlugin();

		// Register
		DCore::getSingleton().installPlugin(quadtreePlugin);

	}
	extern "C" void DUELQUADTREE_API dllStopPlugin( void )
	{
		DCore::getSingleton().uninstallPlugin(quadtreePlugin);
		delete quadtreePlugin;
	}

}