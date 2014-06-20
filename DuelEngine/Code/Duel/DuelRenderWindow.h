//  [7/15/2012 OMEGA] created
//  [4/11/2013 OMEGA] 移除2个参数较少的create函数, 统一使用完整的参数.

#ifndef _DUELRENDERWINDOW_H_
#define _DUELRENDERWINDOW_H_


#include "DuelCommon.h"
#include "DuelPixelFormat.h"
#include "DuelFrameBuffer.h"
#include "DuelRenderDeviceCaps.h"

namespace Duel
{

	// TODO: 增加/删除必要的参数型
	// used to describe a render window, act as a parameter
	// for DRenderWindow creation
	struct RenderWindowSetting
	{
		// whether the window is fixed size.
		//bool		fixedSize;
		//bool			fullScreen;
		uint32			left;
		uint32			top;
		uint32			width;
		uint32			height;

		uint32			samplerCount;
		uint32			samplerQuality;
		uint32			verticalSync;
		bool			useGamma;
		DPixelFormat	surfaceFormat;
	};

	// because window acts differntly in differnt OS, so here our
	// render window is just an interface class whichi provide basic
	// functions to simulate a window. 
	// resources such as DX or OpenGL need a window as its host, 
	// usually you must create a window before you use the rendersystem
	class DUEL_API DRenderWindow : public DFrameBuffer
	{
	DUEL_DECLARE_RTTI(DRenderWindow)
	public:
		DRenderWindow(DRenderResourceFactory* parent, const DString& name)
		 : DFrameBuffer(parent, 0, 0, 0) {}
		virtual ~DRenderWindow() {}

		//////////////////////////////////////////////////////////////////////////
		// signals block
		// not complete.
		DSignal<void(DRenderWindow* wind, bool flag)>	
			signalActive;
		DSignal<void(DRenderWindow* wind)>				
			signalMoved;
		DSignal<void(DRenderWindow* wind)>				
			signalResized;
		DSignal<bool(DRenderWindow* wind), ResultCombiner<bool>>	
			signalLosingFocus;
		DSignal<void(DRenderWindow* wind)>	
			signalLostFocus;
		DSignal<bool(DRenderWindow* wind), ResultCombiner<bool>>	
			signalClosing;
		DSignal<void(DRenderWindow* wind)>	
			signalClosed;

		virtual	const DString&	getName() = 0;
		// create a window with platform-specified handle, it will probably creates a child window.
		virtual void	create(const DString& name, const RenderWindowSetting& setting, uint32 winHandle) = 0;

		// param: flag
		//		true:  make the window become fullscreen
		//		false: make the window become original size.
		virtual void	setFullScreen(bool flag, const DisplayMode& fullScreenMode) = 0;
		virtual	bool	isFullScreen() const = 0;


		// get/set properties of the window
		virtual void	reposition(uint32 top, uint32 left) = 0;
		virtual uint32	getTop() = 0;
		virtual uint32	getLeft() = 0;

		// update the content of the window.
		virtual	void	update() = 0;

		// this method is designed for different platform
		// for windows, it should be HWND, for linux, it is Window, etc.
		virtual	uint32	getWindowHandle() = 0;

	};


	// render window interface, used as render target, 
	// should be implement in specified render system
	// notice that the render window is a lazy class, it will
	// not resize/reposition according to actual window with 
	// in different system, implementing a wrapper class in
	// specified OS to call "onXxxx()" youself.
// 	class DUEL_API DRenderWindow : public DRenderTarget
// 	{
// 	DUEL_DECLARE_RTTI(DRenderWindow)
// 	public:
// 		//////////////////////////////////////////////////////////////////////////
// 		// signals block
// 		// not complete.
// 		DSignal<void(DRenderWindow* wind, bool flag)>	
// 			signalActive;
// 		DSignal<void(DRenderWindow* wind)>				
// 			signalMoved;
// 		DSignal<void(DRenderWindow* wind)>				
// 			signalResized;
// 		DSignal<bool(DRenderWindow* wind), ResultCombiner<bool>>	
// 			signalLosingFocus;
// 		DSignal<void(DRenderWindow* wind)>	
// 			signalLostFocus;
// 		DSignal<bool(DRenderWindow* wind), ResultCombiner<bool>>	
// 			signalClosing;
// 		DSignal<void(DRenderWindow* wind)>	
// 			signalClosed;
// 		//////////////////////////////////////////////////////////////////////////
// 
// 		DRenderWindow(DRenderSystem* parent, const DString& name);
// 		virtual ~DRenderWindow();
// 
// 		//--------------------------------------------
// 		// TODO: 废弃这两个选项.
// 		// default creation, without specified setting
// 		//virtual void	create(const DString& name) {}
// 		// advanced creation, with specified setting
// 		//virtual void	create(const DString& name, const RenderWindowSetting& setting) {}
// 
// 		// create a window with platform-specified handle, it will probably creates a child window.
// 		virtual void	create(const DString& name, const RenderWindowSetting& setting, uint32 winHandle) = 0;
// 
// 		// param: flag
// 		//		true:  make the window become fullscreen
// 		//		false: make the window become original size.
// 		virtual void	setFullScreen(bool flag, const DisplayMode& fullScreenMode);
// 		virtual	bool	isFullScreen() const { return mbFullScreen; }
// 
// 		// do something when the window destroys.
// 		virtual void	onDestroy();
// 		// do something when the window resizes.
// 		virtual void	onResize();
// 		// do something when the window moved..
// 		virtual void	onReposition();
// 
// 		virtual void	setVisible(bool flag);
// 		virtual bool	isVisible();
// 
// 		virtual bool	isClosed();
// 		// this method will notify all listeners, if all listeners accept(i.e. return true)
// 		// the window will actually be closed.
// 		virtual bool	tryClosing();
// 
// 
// 		// called when lost focus.
// 		virtual void	onLostFocus();
// 
// 		// get/set properties of the window
// 		virtual void	reposition(uint32 top, uint32 left);
// 		virtual uint32	getTop() { return mTop; }
// 		virtual uint32	getLeft() { return mLeft; }
// 
// 		// this method is designed for different platform
// 		// for windows, it should be HWND, for linux, it is Window, etc.
// 		virtual uint32	getWindowHandle() = 0;
// 
// 		// callback function, do when window losting focus, if
// 		// return false, then the window will not be inactive;
// 		virtual bool	tryLostFocus();
// 
// 
// 
// 	protected:
// 		bool		mbFullScreen;
// 		bool		mbFixedSize;
// 		bool		mbEnableFullScreen;
// 		bool		mbVisible;
// 		bool		mbClosed;
// 		uint32		mLeft;
// 		uint32		mTop;
// 
// 
// 	};
}


#endif

