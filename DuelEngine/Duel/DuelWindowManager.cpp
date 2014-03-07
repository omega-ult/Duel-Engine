//  [7/15/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelWindowManager.h"
// 
// namespace Duel {
// 
// 	WindowManager::RenderWindows WindowManager::mRenderWindows = RenderWindows();
// 
// // 	//-------------------------------------
// // 	template<> WindowManager* Singleton<WindowManager>::msInstance = 0;
// // 	WindowManager& WindowManager::getSingleton()
// // 	{
// // 		return *msInstance;
// // 	}
// // 	WindowManager* WindowManager::getSingletonPtr()
// // 	{
// // 		return msInstance;
// // 	}
// // 	//-------------------------------------------
// 
// 
// 	//WindowManager---------------------------------
// 	WindowManager::WindowManager()
// 	{
// 	}
// 
// 
// 	WindowManager::~WindowManager(void)
// 	{
// 		// no need to release all windows.
// 	}
// 
// 
// 	void WindowManager::messagePump()
// 	{
// #ifdef DUEL_PLATFORM_WINDOWS
// 
// #ifdef DUEL_PLATFORM_WINDOWS_PHONE_8
// #else
// 		// Windows Message Loop (NULL means check all HWNDs belonging to this context)
// 		MSG  msg;
// 		while( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
// 		{
// 			TranslateMessage( &msg );
// 			DispatchMessage( &msg );
// 		}
// 
// #endif // DUAL_PLATFORM_WINDOWS_PHONE_8
// #endif // DUEL_PLATFORM_WINDOWS
// 
// 
// 	}
// // 
// // 	void WindowManager::addRenderWindow( DRenderWindow* rWnd )
// // 	{
// // 		mRenderWindows.push_back(rWnd);
// // 	}
// // 
// // 	void WindowManager::removeRenderWindow( DRenderWindow* rWnd )
// // 	{
// // 		RenderWindows::iterator i = std::find(mRenderWindows.begin(), mRenderWindows.end(), rWnd);
// // 		if(i != mRenderWindows.end())
// // 		{
// // 			mRenderWindows.erase(i);
// // 		}
// // 	}
// #ifdef DUEL_PLATFORM_WINDOWS
// #ifdef DUEL_PLATFORM_WINDOWS_PHONE_8
// 
// #else
// 	
// 		LRESULT CALLBACK WindowManager::_WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
// 	{
// 		if(uMsg == WM_CREATE)
// 		{
// 			// store pointer to Win32Window in user data area
// 			// so we can reach corresponding user data later.
// 			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)(((LPCREATESTRUCT)lParam)->lpCreateParams));
// 			return 0;
// 		}
// 
// 		// get DRenderWindow instance that hang to the hWnd
// 		// note: it is possible to get a WM_SIZE before WM_CREATE
// 		DRenderWindow* win = (DRenderWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
// 		if(!win)
// 		{
// 			return DefWindowProc(hWnd, uMsg, wParam, lParam);
// 		}
// 
// 		switch(uMsg)
// 		{
// 		case WM_ACTIVATE:
// 			{
// 				bool active = (LOWORD(wParam) != WM_ACTIVATE);
// 				if(active)
// 				{
// 					win->setActive(true);
// 				}
// 				else
// 				{
// 					if(win->tryLostFocus())
// 					{
// 						win->setActive(false);
// 					}
// 				}
// 				break;
// 			}
// 			
// 		case WM_ENTERSIZEMOVE:
// 			break;
// 		case WM_EXITSIZEMOVE:
// 			break;
// 		case WM_MOVE:
// 			win->onReposition();
// 			break;
// 		case WM_DISPLAYCHANGE:
// 			win->onResize();
// 		case WM_SIZE:
// 			win->onResize();
// 		case WM_GETMINMAXINFO:
// 			// prevent window from being too small
// 		/*	((MINMAXINFO*)lParam)->ptMinTrackSize.x = 100;
// 			((MINMAXINFO*)lParam)->ptMinTrackSize.y = 100;*/
// 			break;
// 		case WM_CLOSE:
// 			{
// 				bool closeConfirm = true;
// 				if(win->tryClosing())
// 				{
// 					closeConfirm = true;
// 				}
// 				if(closeConfirm)
// 				{
// 					win->onDestroy();
// 				}
// 				break;
// 			}
// 			
// 		}
// 		return DefWindowProc(hWnd, uMsg, wParam, lParam);
// 	}
// 	//WindowManager---------------------------------
// 
// #endif
// 
// #endif
// }
