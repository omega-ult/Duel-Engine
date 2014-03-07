/* 
	[7/15/2012 OMEGA] created
	Used to manage render window, but can not create,
	use RenderSystem::createRenderWindow to do creation.
*/
//  [4/11/2013 OMEGA] remove this class
// ÒÆ³ý¸ÃÀà.


#ifndef _DUELWINDOWMANAGER_H_
#define _DUELWINDOWMANAGER_H_

// compiler will throw a C4661 warning if using
// singleton with forbidden default constructor


#include "DuelCommon.h"
// //#include "DuelWindow.h"
// #include "DuelSingleton.h"
// #include "DuelString.h"
// #include "DuelRenderWindow.h"
// 
// 
// 
// namespace Duel {
// 
// 
// 
// 
// 
// 	//--------------------------------------------
// 	class DUEL_API WindowManager : public DSingleton<WindowManager>
// 	{
// 	public:
// 		WindowManager();
// 		~WindowManager();
// 
// 
// 
// 		// Call this once per frame  This will update all registered
// 		// RenderWindows (If using external Windows, you can 
// 		// optionally register those yourself)
// 		static void messagePump();
// 
// 		// used to traverse all renderwindows
// 		typedef std::vector<DRenderWindow*>		RenderWindows;
// 		typedef	VectorIterator<RenderWindows>	RenderWindowIterator;
// 		static RenderWindowIterator	getRenderWindowIterator() { return RenderWindowIterator(mRenderWindows.begin(), mRenderWindows.end()); }
// // 
// // 		static void addRenderWindow(DRenderWindow* rWnd);
// // 
// // 		static void removeRenderWindow(DRenderWindow* rWnd);
// // 		
// #ifdef	DUEL_PLATFORM_WINDOWS
// 		// Win32 WndProc
// 		static LRESULT CALLBACK _WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
// #endif
// 
// 	protected:
// 		static RenderWindows mRenderWindows;
// 
// 	};
// 
// }

#endif