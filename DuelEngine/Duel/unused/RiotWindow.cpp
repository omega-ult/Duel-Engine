//  [7/15/2012 OMEGA]

#include "RiotWindow.h"
#include "RiotString.h"

namespace Riot {




	//Window----------------------------------------
	Window::Window()
	{
		mActive = false;
		mTop = 0;
		mLeft = 0;
		mWidth = 0;
		mHeight = 0;
		mListenerList = new EventListenerList;

		mhWnd = NULL;//填
	}
	Window::~Window()
	{
		if (mhWnd != NULL)
		{
			::DestroyWindow(mhWnd);
			mhWnd = NULL;
		}
	}
	
	//Functions called by system---------------------
	LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		
		Window* win = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

		if(win)
		{
			return win->MsgProc(hWnd, uMsg, wParam, lParam);
		}
		else 
		{
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}
	LRESULT Window::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		//填- -.....
		EventListenerList::iterator	start = mListenerList->begin(),
									end	  = mListenerList->end();
		switch( uMsg )
		{
		case WM_ACTIVATE:
			if(WA_INACTIVE == LOWORD(wParam))
			{
				this->setActive(false);
			}
			else
			{
				this->setActive(true);
			}
			for( ; start != end; ++start )
				(*start)->windowActive(this, this->isActivated());
			break;
		case WM_ENTERSIZEMOVE:
			//-------------------------------
			break;
		case WM_EXITSIZEMOVE:
			//-------------------------------
			break;
		case WM_MOVE:
			this->movedOrResized();
			for( ; start != end; ++start )
				(*start)->windowMoved(this);
			break;
			//...........填
		case WM_GETMINMAXINFO:
			// 防止窗口过小
			((MINMAXINFO*)lParam)->ptMinTrackSize.x = 100;
			((MINMAXINFO*)lParam)->ptMinTrackSize.y = 100;
			break;
		case WM_CLOSE:
			{
				bool close = true;
				for( ; start != end; ++start)
				{
					if(!(*start)->windowClosing(this))
						close = false;
				}
				if (!close) return 0;

				for( ; start != end; ++start)
				{
					(*start)->windowClosed(this);
				}

				// TODO: remove this line into a better place
				PostQuitMessage(0);

				return 0;
			}

		}
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	}
	
	//////////////////////////////
	void Window::creat(String const & name, String setting)
	{
		// 脚本待填坑
		HINSTANCE hInst = GetModuleHandle(NULL);

		StringTool::Convert(mWname,name);
		WNDCLASSEXW wc;

		wc.cbSize			= sizeof(wc);
		wc.style			= CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc		= Window::WndProc;
		wc.cbClsExtra		= 0;
		wc.cbWndExtra		= sizeof(this);
		wc.hInstance		= hInst;
		wc.hIcon			= NULL;
		wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground	= static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
		wc.lpszMenuName		= NULL;
		wc.lpszClassName	= mWname.c_str();
		wc.hIconSm			= NULL;

		RegisterClassExW(&wc);

		//  测试情况只用实际大小填坑. 记得改
		mhWnd = CreateWindowW( mWname.c_str(), mWname.c_str(),WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT, CW_USEDEFAULT, 200, 200, NULL, NULL, hInst, NULL);
        
		SetWindowLongPtrW(mhWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));


		ShowWindow(mhWnd,SW_SHOWNORMAL);
		UpdateWindow(mhWnd);
	}
	//////////////////////////////
	void Window::show(void)
	{
		if(!mhWnd)
		{
			ShowWindow(this->mhWnd,SW_SHOW);
			this->mActive = true;
		}
	}
	//////////////////////////////
	void Window::addEventListentner(EventListener* e)
	{
		mListenerList->push_back(e);
	}
	//////////////////////////////
	void Window::deleteEventListener(EventListener* e)
	{
		EventListenerList::iterator	i	= mListenerList->begin(),
									end	= mListenerList->end();
		for( ; i != end; ++i )
		{
			if(*i == e)
			{
				mListenerList->erase(i);
				break;
			}
		}
	}
	//////////////////////////////
	int32_t	Window::getLeft()
	{
		return mLeft;
	}
	int32_t Window::getTop()
	{
		return mTop;
	}
	uint32_t Window::getWidth()
	{
		return mWidth;
	}
	uint32_t Window::getHeight()
	{
		return mHeight;
	}
	void Window::setActive(bool flag)
	{
		this->mActive = flag;
	}
	bool Window::isActivated()
	{
		return this->mActive;
	}
	void Window::movedOrResized()
	{
		RECT rc;
		GetWindowRect(this->mhWnd, &rc);
		this->mTop = rc.top;
		this->mLeft = rc.left;
		this->mWidth = rc.right - rc.left;
		this->mHeight = rc.bottom - rc.top;
	}

	HWND Window::getHWND()
	{
		return mhWnd;
	}

	//Window----------------------------------------


}