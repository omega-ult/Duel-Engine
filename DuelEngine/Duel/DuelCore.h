/*
	[7/15/2012 OMEGA] created
	Global control class.
	
	maintain and manage all resources such as window, rendersystem, etc.
*/


#ifndef _DUELCORE_H_
#define _DUELCORE_H_

// compiler will throw a C4661 warning if using
// singleton with forbidden default constructor

#include "DuelCommon.h"
#include "DuelLogManager.h"
#include "DuelRenderSystem.h"
#include "DuelGpuProgramManager.h"
#include "DuelDynLibManager.h"
#include "DuelArchiveManager.h"
#include "DuelResourceGroupManager.h"
#include "DuelSceneManagerEnumerator.h"
#include "DuelRenderEffectManager.h"
#include "DuelMeshManager.h"
#include "DuelTextureManager.h"
#include "DuelSkeletonManager.h"
#include "DuelRenderWorkshop.h"
#include "DuelThreadLib.h"
#include "DuelPlugin.h"
#include "DuelInputManager.h"

namespace Duel {

#define DUEL_VERSION "1.0.0"
#define DUEL_WP8VERSION "wp_1.0.0"

	class DUEL_API DCore : public DObject, public DSingleton<DCore>
	{
	DUEL_DECLARE_RTTI(DCore)
	public:
		/* config file is simply an xml, contains file names which are 
		 prepared to be loaded.
		 <CoreConfig DuelVersion = '1.0.0'>
		 <Locale>
			// optional. if none, the system locale will be used. see ANSI C setlocale(int category(LC_ALL), const char* locale);.
		 </Locale>
		 <RenderSystem>
				D3DXXXXXXX.dll
		 </RenderSystem>
		 <PluginList>
				<DPlugin>
				xxxx.dll
				</DPlugin>
				<DPlugin>
				xxx.dll
				</DPlugin>
		 </PluginList>
		 <ArchiveList> // optional. see ArchiveManager.
		 </ArchiveList>
		 <GroupDescription> // optional. see ResourceGroupManager.
		 </GroupDescription>
		 </CoreConfig>
		 ---------------------------------
		 在wp8模式下使用WP8VERSION定义, 并且配置文件改写为:
		 <CoreConfig DuelVersion = 'wp_1.0.0'>
		 <PluginList>	//可为空
				<Plugin>
				xxxx.dll
				</Plugin>
				<Plugin>
				xxx.dll
				</Plugin>
		 </PluginList>
		 <ArchiveList> // optional. see ArchiveManager.
		 </ArchiveList>
		 <GroupDescription> // optional. see ResourceGroupManager.
		 </GroupDescription>
		 </CoreConfig>
		 */
		DCore(const DString& configFile);
		virtual ~DCore();

		// set render system, NEVER NOT call it yourself!!
		INTERNAL void				setRenderSystem(DRenderSystem* rSys);
		DRenderSystem*				getRenderSystem();
		// set render workshop.
		INTERNAL void				setRenderWorkshop(DRenderWorkshop* rWS);
		DRenderWorkshop*			getRenderWorkshop();

		// get the thread pool .
		DThreadPool*				getMainThreadPool();

		// install a plugin.
		void					installPlugin(DPlugin* plugin);
		// uninstall a plugin
		void					uninstallPlugin(DPlugin* plugin);
		// load a plugin
		void					loadPlugin(const DString& pluginName);

	private:
		DRenderSystem*				mRenderSystem;
		DLogManager*				mLogManager;

		DDynLibManager*				mDynLibManager;
		DSceneManagerEnumerator*	mSceneManagerEnumerator;
		DArchiveManager*			mArchiveManager;
		DResourceGroupManager*		mResourceGroupManager;
		DRenderWorkshop*			mRenderWorkShop;
		// resource managers.
		DRenderEffectManager*		mRenderEffectManager;
		DMeshManager*				mMeshManager;
		DFontManager*				mFontManager;
		DSkeletonManager*			mSkeletonManager;
		
		DThreadPool*				mThreadPool;

		// stores plugins.
		typedef	std::set<DPlugin*>	PluginSet;
		PluginSet					mPluginSet;

		// input sysetm.
		DInputManager*				mInputManager;
		// material system/post effect system
		DMaterialManager*			mMaterialManager;
		DPostEffectManager*			mPostEffectManager;

		DString						mInstallLocation;
		DWString					mInstallLocationW;


	private:
		// not allowed.
		DCore() {}
	};

}



#endif