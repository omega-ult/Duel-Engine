//  [6/23/2013 OMEGA] created

#ifndef _DUELAPPLICATION_H_
#define _DUELAPPLICATION_H_

#include "pch.h"
#include "DuelCommon.h"
#include "DuelStateMachine.h"
#include "DuelInputManager.h"
#include "DuelGUISystem.h"
#include "DuelDemoRenderWorkshop.h"

//////////////////////////////////////////////////////////////////////////
// 加入状态.

namespace Duel
{
	class DAStateManager;
	class DApplication : public DSingleton<DApplication>
	{
	public:
		DApplication();
		~DApplication();
			
		void				initialize(const DString& appCfgFile);
		void				run();
		void				stop(bool flag);
		void				shutdown();

		// used as callback function, called from other function.
		void				invokeClose() {}

		DAStateManager*		getStateManager();
		DRenderWindow*		getMainWindow();

		const DString&		getApplicationDirectory() const;

#ifdef DUEL_PLATFORM_WINDOWS
		HINSTANCE			getApplicationInstance();
#endif // DUEL_PLATFORM_WINDOWS


	protected:
		DCore*				mEngineCore;
		DTimer*				mAppTimer;
		// 狀態控制.
		DAStateManager*		mStateManager;
		// the main window used as representation window
		DRenderWindow*		mMainWindow;

		// using duel's gui system
		DGUISystem*			mGSystem;

		// 
		bool				mbStopped;

		// platform related resources.
		HWND				mHwnd;

		DString				mAppDir;

		//////////////////////////////////////////////////////////////////////////
		// 状态机存在这里.
		DemoKit::TestState*	mTestState;

		DDemoRenderWorkshop*	mRws;
	};

}


#endif