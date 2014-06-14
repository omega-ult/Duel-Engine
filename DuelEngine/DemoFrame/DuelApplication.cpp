//  [6/27/2013 OMEGA] created

#include "pch.h"
#include "DuelApplication.h"
#include "DuelCore.h"
#include "DuelStateMachine.h"
#include "DuelDemoRenderWorkshop.h"
#include "DuelDemoMaterialBank.h"
#include "DuelVertexBuffer.h"
#include "DuelFont.h"
#include "DuelInputManager.h"
#include "DuelGUISystem.h"
//////////////////////////////////////////////////////////////////////////
#include "TestState.h"
#include <fstream>

namespace Duel
{
	
LRESULT CALLBACK _DefaultWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if(uMsg == WM_CREATE)
	{
	 	// store pointer to Win32Window in user data area
	 	// so we can reach corresponding user data later.
	 	SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)(((LPCREATESTRUCT)lParam)->lpCreateParams));
	 	return 0;
	}
	 
	// get DRenderWindow instance that hang to the hWnd
	// note: it is possible to get a WM_SIZE before WM_CREATE
	DApplication* app = (DApplication*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if(!app)
	{
	 	return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	 
	switch(uMsg)
	{
	case WM_ACTIVATE:
	 	{
// 	 		bool active = (LOWORD(wParam) != WM_ACTIVATE);
// 	 		if(active)
// 	 		{
// 	 			win->setActive(true);
// 	 		}
// 	 		else
// 	 		{
// 	 			if(win->tryLostFocus())
// 	 			{
// 	 				win->setActive(false);
// 	 			}
// 	 		}
	 		break;
	 	}
	 			
	case WM_ENTERSIZEMOVE:
	 	break;
	case WM_EXITSIZEMOVE:
	 	break;
	case WM_MOVE:
//	 	win->onReposition();
	 	break;
	case WM_DISPLAYCHANGE:
//	 	win->onResize();
	case WM_SIZE:
//	 	win->onResize();
	case WM_GETMINMAXINFO:
	 	break;
	case WM_CLOSE:
	 	{
			app->invokeClose();

			return	0;
	 		break;
	 	}
	 			
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


	DApplication::DApplication() :
		mMainWindow(NULL),
		mEngineCore(NULL),
		mTestState(NULL),
		mbStopped(false)
	{
		mStateManager = new DAStateManager(this);
		mGSystem = new DGUISystem;

	}

	DApplication::~DApplication()
	{
		delete mRws;
		delete mStateManager;
		if (mTestState)
		{
			delete mTestState;
		}
	}



	DAStateManager* DApplication::getStateManager()
	{
		return mStateManager;
	}

	DRenderWindow* DApplication::getMainWindow()
	{
		return mMainWindow;
	}


	void DApplication::run()
	{

		if (!mbStopped)
		{
			DInputManager::getSingleton().captureEvent();
			mStateManager->transactionStart();
			mStateManager->parseInput();
			mStateManager->update();
			mStateManager->render();
			mStateManager->present();
			mStateManager->transactionFinish();
		}
	}

	void DApplication::stop(bool flag)
	{
		mbStopped = flag;
	}

	void DApplication::shutdown()
	{
		// default
		DInputManager::getSingleton().unregisterWindow(mMainWindow);
		PostQuitMessage(0);
	}

	void DApplication::initialize(const DString& appCfgFile)
	{
		HINSTANCE hInst = GetModuleHandle(NULL);
		WNDCLASSEX wc;

		wc.cbSize			= sizeof(wc);
		wc.style			= CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc		= _DefaultWndProc;
		wc.cbClsExtra		= 0;
		wc.cbWndExtra		= 0;
		wc.hInstance		= hInst;
		wc.hIcon			= NULL;
		wc.hCursor			= NULL;
		wc.hbrBackground	= NULL;
		wc.lpszMenuName		= NULL;
		// TODO: from config
		wc.lpszClassName	= appCfgFile.c_str();
		wc.hIconSm			= NULL;

		DWORD	styleWord;
		styleWord = WS_OVERLAPPEDWINDOW;
		RECT rc = { 0, 0, 800, 600 };
		AdjustWindowRectEx( &rc, styleWord, FALSE, 0 );
		RegisterClassEx(&wc);
		mHwnd = CreateWindowEx(0, appCfgFile.c_str(), appCfgFile.c_str(), 
			styleWord, 200,100,  rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInst, this);

		ShowWindow(mHwnd, SW_SHOW);
		UpdateWindow(mHwnd);


		mEngineCore = new Duel::DCore("coreConfig.xml");
		Duel::DRenderSystem* rSys = mEngineCore->getRenderSystem();
		Duel::DRenderSystem::InitializationOptions initOp;
		rSys->initialize(initOp);
		Duel::RenderWindowSetting wndSetting;
		wndSetting.left = 200;
		wndSetting.top = 100;
		wndSetting.width = 800;
		wndSetting.height = 600;
		wndSetting.samplerCount = 1;
		wndSetting.samplerQuality = 1;
		wndSetting.verticalSync = false;
		wndSetting.useGamma = false;
		wndSetting.surfaceFormat = Duel::PF_A8R8G8B8;
		mMainWindow = DRenderResourceManager::getSingleton().createRenderWindow(appCfgFile,wndSetting, (Duel::uint32)mHwnd );
#ifdef GLPLUGIN_DEBUG
		//////////////////////////////////////////////////////////////////////////
		// basic resources.
		if(rSys->getName() == "OpenGL")
		{
			DArchivePtr ShaderArchive = Duel::DArchiveManager::getSingleton().
				getArchive("Assets\\_BasicShaderPack");
			if (ShaderArchive != NULL)
			{		
				// for shaders.
				Duel::DDataStreamPtr descFilePtf = 
					ShaderArchive->open("BasicShadersGLSL.xml");
				if (descFilePtf != NULL)
				{
					ResourceDescriptionList resList =  DResourceGroupManager::getSingleton().
						getResouceManager("GpuProgram")->parseParameter(descFilePtf.get());
					ResourceDescriptionList::iterator i, iend = resList.end();
					for (i = resList.begin(); i != iend; ++i)
					{
						DResourceGroupManager::getSingleton().declareResource(*i);
					}
				}
				// for render effects
				descFilePtf = ShaderArchive->open("BasicDRE.xml");
				if (descFilePtf != NULL)
				{
					ResourceDescriptionList resList =  DResourceGroupManager::getSingleton().
						getResouceManager("RenderEffect")->parseParameter(descFilePtf.get());
					ResourceDescriptionList::iterator i, iend = resList.end();
					for (i = resList.begin(); i != iend; ++i)
					{
						DResourceGroupManager::getSingleton().declareResource(*i);
					}
				}				
			}
		}

		Duel::DArchivePtr mediaArchive = Duel::DArchiveManager::getSingleton().
			getArchive("Assets\\_BasicMediaPack");
		if (mediaArchive != NULL)
		{
			// for textures.
			Duel::DDataStreamPtr descFilePtf = 
				mediaArchive->open("BasicTexture.xml");
			if (descFilePtf != NULL)
			{
				ResourceDescriptionList resList =  DResourceGroupManager::getSingleton().
					getResouceManager("Texture")->parseParameter(descFilePtf.get());
				ResourceDescriptionList::iterator i, iend = resList.end();
				for (i = resList.begin(); i != iend; ++i)
				{
					DResourceGroupManager::getSingleton().declareResource(*i);
				}
			}				

			// for fonts.
			descFilePtf = 
				mediaArchive->open("BasicFont.xml");
			if (descFilePtf != NULL)
			{
				ResourceDescriptionList resList =  DResourceGroupManager::getSingleton().
					getResouceManager("Font")->parseParameter(descFilePtf.get());
				ResourceDescriptionList::iterator i, iend = resList.end();
				for (i = resList.begin(); i != iend; ++i)
				{
					DResourceGroupManager::getSingleton().declareResource(*i);
				}
			}				
			//////////////////////////////////////////////////////////////////////////
			// mesh test
			descFilePtf = 
				mediaArchive->open("BasicMesh.xml");
			if (descFilePtf != NULL)
			{
				Duel::ResourceDescriptionList resList =  Duel::DResourceGroupManager::getSingleton().
					getResouceManager("Mesh")->parseParameter(descFilePtf.get());
				Duel::ResourceDescriptionList::iterator i, iend = resList.end();
				for (i = resList.begin(); i != iend; ++i)
				{
					Duel::DResourceGroupManager::getSingleton().declareResource(*i);
				}
			}
			///////////////////////////////////////////////////////////////////////////
			// skel test
			descFilePtf = 
				mediaArchive->open("BasicSkeleton.xml");
			if (descFilePtf != NULL)
			{
				Duel::ResourceDescriptionList resList =  Duel::DResourceGroupManager::getSingleton().
					getResouceManager("Skeleton")->parseParameter(descFilePtf.get());
				Duel::ResourceDescriptionList::iterator i, iend = resList.end();
				for (i = resList.begin(); i != iend; ++i)
				{
					Duel::DResourceGroupManager::getSingleton().declareResource(*i);
				}
			}	
		}
		DResourceGroupManager::getSingleton().createDeclaredResource("_BasicShaderPack");
		DResourceGroupManager::getSingleton().createDeclaredResource("_BasicMediaPack");
#else
#endif
//////////////////////////////////////////////////////////////////////////
#ifdef GLPLUGIN_DEBUG
		mRws = new DDemoRenderWorkshop();
		mEngineCore->setRenderWorkshop(mRws);
		new DDemoMaterialBank();
		DDemoMaterialBank::getSingleton().init(mRws);
#endif
		//////////////////////////////////////////////////////////////////////////
 		DInputManager::getSingleton().registerWindow(mMainWindow, false);

		//////////////////////////////////////////////////////////////////////////
		mTestState = new DemoKit::TestState();
		mStateManager->registerState(mTestState);
		mStateManager->requestStateChange(NULL, "TestState");
		//////////////////////////////////////////////////////////////////////////
// 		std::cout << "hello world" << std::endl;
// 
// 		DResourcePtr fRes = DResourceGroupManager::getSingleton().getResouceManager("Font")->getResource("_BasicMediaPack", "F_msyh.ttc");
// 		fRes->load();
// 		// 来试试创建文字吧.
// 		DFont* fnt = fRes->getAs<DFont>();
// 		DString str = "雪特, 这是一段很长的文字, 如果你能够静下心来看完, 那也没有什么奇怪的嘛. 我只是来做个测试的, 你们不要这样看着我";
// 		DWString wstr = DStringTool::convert(str);
// 		
// 		for (uint32 i = 0; i < wstr.length(); ++i)
// 		{
// 			DFont::GlyphInfo info = fnt->getGlyphInfo(wstr[i]);
// 		}
// 
// 		DVertexBufferPtr videoDiscard = DRenderResourceManager::getSingleton().createVetexBuffer(12, 12, HBU_Discardable, false);
// 		DVertexBufferPtr agpVBDiscard = DRenderResourceManager::getSingleton().createVetexBuffer(12, 12, HBU_Dynamic, false);
// 		DVertexBufferPtr agpVBShadow = DRenderResourceManager::getSingleton().createVetexBuffer(12, 12, HBU_Dynamic, false);
// 		DReal* memoryBuf = new DReal[12*12];
// 		DTimer timer;
// 		DReal writeValue[3] = {0.0, 0.0, 0.0};
// 		uint64 val;
// 		timer.reset();
// 		for (uint32 i = 0; i < 10000; ++i)
// 		{
// 			char* bufPtr = (char*)videoDiscard->lock(HBL_Discard);
// 			for (uint32 j = 0; j < 12; ++j)
// 			{
// 				memcpy(bufPtr, &writeValue, sizeof(DReal)*3); bufPtr += sizeof(DReal)*3;
// 				memcpy(bufPtr, &writeValue, sizeof(DReal)*3); bufPtr += sizeof(DReal)*3;
// 				memcpy(bufPtr, &writeValue, sizeof(DReal)*3);
// 
// 			}
// 			videoDiscard->unlock();
// 		}
// 		val = timer.getMicroseconds();
// // 		std::cout <<"video memory discard " << timer.getMicroseconds() << std::endl;
// // 
// 
// 		timer.reset();
// 		for (uint32 i = 0; i < 100; ++i)
// 		{
// 			char* bufPtr = (char*)agpVBShadow->lock(HBL_NoOverwrite);
// 			for (uint32 j = 0; j < 12; ++j)
// 			{
// 				memcpy(bufPtr, &writeValue, sizeof(DReal)*3); bufPtr += sizeof(DReal)*3;
// 				memcpy(bufPtr, &writeValue, sizeof(DReal)*3); bufPtr += sizeof(DReal)*3;
// 				memcpy(bufPtr, &writeValue, sizeof(DReal)*3);
// 
// 			}
// 			agpVBShadow->unlock();
// 		}
// 		val = timer.getMicroseconds();
// 		//std::cout <<"agp shadow no overwrite " << timer.getMicroseconds() << std::endl;
// 
// 		timer.reset();
// 		for (uint32 i = 0; i < 10000; ++i)
// 		{
// 			char* bufPtr = (char*)memoryBuf;
// 			for (uint32 j = 0; j < 12; ++j)
// 			{
// 				memcpy(bufPtr, &writeValue, sizeof(DReal)*3); bufPtr += sizeof(DReal)*3;
// 				memcpy(bufPtr, &writeValue, sizeof(DReal)*3); bufPtr += sizeof(DReal)*3;
// 				memcpy(bufPtr, &writeValue, sizeof(DReal)*3);
// 
// 			}
// 		}
		//std::cout <<"main memory normal " << timer.getMicroseconds() << std::endl;


	}

}