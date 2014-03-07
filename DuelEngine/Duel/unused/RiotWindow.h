//  [7/15/2012 OMEGA]

#include "RiotCommon.h"
#include "RiotString.h"


#ifndef _RIOTWINDOW_H_
#define	_RIOTWINDOW_H_

namespace Riot{
	class Window;



	// 待完成: WndProc
	class Window
	{
	public:
		Window();
		~Window();

		//--------------------------------------------
		// 一个window对象可以有多个EventListentner
		// 用于实现回调
		class EventListener
		{
		public:
			EventListener(){}
			virtual ~EventListener();

			virtual void windowActive(Window* win, bool flag){}
			virtual void windowMoved(Window* win){}
			virtual void windowResized(Window* win){}
			virtual bool windowClosing(Window* win){ return true;}
			virtual void windowClosed(Window* win){}
			// virtual函数.
		};


		// Query function
		int32_t		getLeft();
		int32_t		getTop();
		uint32_t	getWidth();
		uint32_t	getHeight();
		HWND		getHWND();
		bool		isActivated();

		
		// operation function
		void creat(String const & name, String setting); // 用这个函数完成创建工作. setting是配置文件. 脚本部分待完成
		void show(void);
		void addEventListentner(EventListener* e);	// 监听者模式 你懂的.
		void deleteEventListener(EventListener* e);

		// CALLBACK function
		LRESULT MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	// 消息函数.
	
	private:
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		void		movedOrResized();
		void		setActive(bool flag);

		typedef std::vector<EventListener*> EventListenerList;
		EventListenerList *mListenerList;	// 事件发生的时候就会逐个使用Listener的函数.
		
	private:
		int32_t	 mLeft;
		int32_t	 mTop;
		uint32_t mWidth;
		uint32_t mHeight;

		bool mActive;
		WString	 mWname;
		HWND	 mhWnd;
	};

}

#endif