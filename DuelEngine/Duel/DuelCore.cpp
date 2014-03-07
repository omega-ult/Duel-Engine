//  [7/15/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelCore.h"
#include "DuelLogManager.h"
#include "DuelRenderSystem.h"
#include "DuelGpuProgramManager.h"
#include "DuelAutoGpuParameter.h"
#include "DuelRenderSystem.h"
#include "DuelDynLibManager.h"
#include "DuelArchiveManager.h"
#include "DuelResourceGroupManager.h"
#include "DuelSceneManagerEnumerator.h"
#include "DuelRenderEffectManager.h"
#include "DuelMeshManager.h"
#include "DuelSkeletonManager.h"
#include "DuelFontManager.h"
#include "DuelThreadLib.h"
#include "DuelInputManager.h"
#include "DuelMaterialManager.h"
#include "DuelPostEffectManager.h"
#include "DuelXML.h"


namespace Duel 
{

	DUEL_IMPLEMENT_RTTI_1(DCore, DObject);

	typedef void (*DLL_START_PLUGIN)(void);
	typedef void (*DLL_STOP_PLUGIN)(void);




	DCore::DCore(const DString& configFile) :
		mRenderSystem(NULL),
		mRenderWorkShop(NULL)
	{
		// initialize math lib.
		new Duel::DMath();
		new Duel::DAutoGpuParameter();
		DStringTool::setLocale("");
		// initialize singleton managers. 
		mLogManager			= new DLogManager();
		mSceneManagerEnumerator	= new DSceneManagerEnumerator();
		mArchiveManager		= new DArchiveManager();
		mResourceGroupManager	= new DResourceGroupManager();
		mDynLibManager		= new DDynLibManager();

		mRenderEffectManager	= new DRenderEffectManager();
		mMeshManager		= new DMeshManager();
		mSkeletonManager	= new DSkeletonManager();
		mFontManager		= new DFontManager();

		mInputManager		= new DInputManager();

		mMaterialManager	= new DMaterialManager();
		mPostEffectManager	= new DPostEffectManager();

		mResourceGroupManager->registerManager(mRenderEffectManager);
		mResourceGroupManager->registerManager(mMeshManager);
		mResourceGroupManager->registerManager(mSkeletonManager);
		mResourceGroupManager->registerManager(mFontManager);

		mThreadPool			= new DThreadPool();

		DDataStreamPtr fileStream = DDataStreamPtr(new DFileStream(configFile));
		if (!fileStream->isValid())
		{
			DUEL_EXCEPT_BRIEF(DException::ET_ItemNotFound,
				"Unable to open file: " + configFile,
				"Duel::Core");
		}
		mInstallLocation	= "";
		mInstallLocationW	= L"";


		size_t fileSize = fileStream->getSize();
		DXMLDoc	doc;
		char*	xmlContent;
		xmlContent = new char[fileSize+1];
		fileStream->read(xmlContent, fileSize);
		xmlContent[fileSize] = '\0';

		doc.parse<0>(xmlContent);

		DXMLNode* rootNode = doc.first_node("CoreConfig");
		if (rootNode == NULL)
		{
			DUEL_EXCEPT_BRIEF(DException::ET_ItemNotFound,
				"Incorrect config file: " + configFile,
				"Duel::Core");
		}
		DXMLAttribute* versionAtt = rootNode->first_attribute("DuelVersion");
		DString fileVer = DXMLTool::readValue(versionAtt);

		
		if (DUEL_VERSION == fileVer)
		{

			DString locale = "";
			DXMLNode* localeNode = rootNode->first_node("Locale");
			if (localeNode)
			{
				locale = DXMLTool::readValue(localeNode);
			}
			DStringTool::setLocale(locale);

			DXMLNode* pluginNode = rootNode->first_node("PluginList");
			if (pluginNode == NULL)
			{
				DUEL_EXCEPT_BRIEF(DException::ET_ItemNotFound,
					"Incorrect config file: " + configFile,
					"Duel::Core");
			}
			pluginNode = pluginNode->first_node("Plugin");
			DString pluginName;
			while(pluginNode != NULL)
			{
				pluginName = DXMLTool::readValue(pluginNode);
				bool failed = false;
				try{
					mDynLibManager->load(pluginName);
				}
				catch(...)
				{
					failed = true;
				}
				if (!failed)
				{
					loadPlugin(pluginName);
				}
				
				pluginNode = pluginNode->next_sibling();
			}

			// optional
			DXMLNode* archiveListNode = rootNode->first_node("ArchiveList");
			if (archiveListNode != NULL)
			{
				mArchiveManager->registerFromXML(archiveListNode);
			}
			// optional
			DXMLNode* resourceGroups = rootNode->first_node("GroupDescription");
			if (resourceGroups != NULL)
			{
				mResourceGroupManager->loadFromXML(resourceGroups);
			}

		}
		else if(DUEL_WP8VERSION == fileVer)
		{
			DStringTool::setLocale("");
			DXMLNode* pluginNode = rootNode->first_node("PluginList");
			if (pluginNode != NULL)
			{

				pluginNode = pluginNode->first_node("Plugin");
				DString pluginName;
				while(pluginNode != NULL)
				{
					pluginName.assign(DXMLTool::readValue(pluginNode));
					mDynLibManager->load(pluginName);
					pluginNode = pluginNode->next_sibling();
				}
			}

			// optional
			DXMLNode* archiveListNode = rootNode->first_node("ArchiveList");
			if (archiveListNode != NULL)
			{
				mArchiveManager->registerFromXML(archiveListNode);
			}
			// optional
			DXMLNode* resourceGroups = rootNode->first_node("GroupDescription");
			if (resourceGroups != NULL)
			{
				mResourceGroupManager->loadFromXML(resourceGroups);
			}

		}


		delete []xmlContent;
	}

	DCore::~DCore()
	{
		// usually the Core object exist with the instance all the time,
		// so don't delete it manually. these code is under review.
		// unload plugins firstly.
		
		PluginSet::iterator i, iend = mPluginSet.end();
		for (i = mPluginSet.begin(); i != iend; ++i)
		{
			(*i)->shutdown();
		}

		delete	mDynLibManager;
		delete	mThreadPool;
		mResourceGroupManager->unregisterManager(mFontManager);
		mResourceGroupManager->unregisterManager(mRenderEffectManager);
		mResourceGroupManager->unregisterManager(mMeshManager);
		mResourceGroupManager->unregisterManager(mSkeletonManager);

		delete mPostEffectManager;
		delete mMaterialManager;

		delete mInputManager;

		delete mFontManager;
		delete mRenderEffectManager;
		delete mMeshManager;
		delete mSkeletonManager;

		delete mSceneManagerEnumerator;
		delete mResourceGroupManager;
		delete mArchiveManager;
		delete mLogManager;
	}

	void DCore::setRenderSystem( DRenderSystem* rSys )
	{
		// you can set only once.
		if(mRenderSystem == NULL)
		{
			mRenderSystem = rSys;
		}
	}

	DRenderSystem* DCore::getRenderSystem()
	{
		return mRenderSystem;
	}


	void DCore::setRenderWorkshop( DRenderWorkshop* rWS )
	{
		// you can set only once.
		if(mRenderWorkShop == NULL)
		{
			mRenderWorkShop = rWS;
		}
	}

	DRenderWorkshop* DCore::getRenderWorkshop()
	{
		return mRenderWorkShop;
	}


	void DCore::installPlugin( DPlugin* plugin )
	{
		// if the plugin has installed. do nothing.
		PluginSet::iterator i = mPluginSet.find(plugin);
		if (i == mPluginSet.end())
		{
			plugin->install();
			mPluginSet.insert(plugin);
			plugin->initialize();
		}
	}

	void DCore::uninstallPlugin( DPlugin* plugin )
	{
		// if the plugin has uninstalled. do nothing.
		PluginSet::iterator i = mPluginSet.find(plugin);
		if (i != mPluginSet.end())
		{
			plugin->shutdown();
			plugin->uninstall();
			mPluginSet.erase(i);
		}
	}

	void DCore::loadPlugin( const DString& pluginName )
	{
		DDynLib* rSysLib;
		try
		{ 
			rSysLib = mDynLibManager->load(pluginName);
		}
		catch (...)
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InternalError,
				"Unable to load lib: " + pluginName,
				"Duel::loadPlugin");
		}
		// Call startup function
		DLL_START_PLUGIN pFunc = (DLL_START_PLUGIN)rSysLib->getSymbol("dllStartPlugin");

		if (!pFunc)
			DUEL_EXCEPT_BRIEF(DException::ET_ItemNotFound,
			"Cannot find symbol dllStartPlugin in library " + pluginName,
			"Duel::loadPlugin");
		pFunc();
	}



	DThreadPool* DCore::getMainThreadPool()
	{
		return mThreadPool;
	}

	//------------------------------------
}