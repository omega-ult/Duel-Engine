//  [3/19/2013 OMEGA] created

#pragma once
#include "pch.h"
#include "DuelStateMachine.h"
#include "NullState.h"
#include "DemoState.h"
#include "DuelCore.h"
#include "DuelWPDXPlugin.h"
#include "DuelQuadtreePlugin.h"
#include "ActorFactory.h"


namespace Duel
{

	class DAGameDirector : public Duel::DSingleton<DAGameDirector>
	{
	public:
		// 這裡創建D3DX資源並且提供給渲染系統使用.
		DAGameDirector();
		~DAGameDirector();

	public:
		//////////////////////////////////////////////////////////////////////////
		// 這裡加上與XAML交互所需要的接口. 比如查詢遊戲狀態啊.之類之類的.





		// 下面是遊戲引擎相關的. 不需要知道.
		//////////////////////////////////////////////////////////////////////////
		// 和模板程序類似.
		virtual	void		Initialize(Windows::UI::Core::CoreWindow^ window, Windows::UI::Xaml::Controls::SwapChainBackgroundPanel^ panel, float dpi);
		virtual void		HandleDeviceLost();
		virtual void		CreateDeviceResources();
		virtual void		UpdateForWindowSizeChange();
		virtual	void		SetDpi(float dpi);	// dpi是毛線?		
		virtual float		ConvertDipsToPixels(float dips);
		virtual void		CreateWindowSizeDependentResources();
		void				ValidateDevice();

		// - - 調用render函數.
		void				parseInput(){ mStateManager->parseInput(); }
		void				update();
		void				render();

		// 渲染完畢. 展示畫面.
		void				present();

		// get/set 取得遊戲的狀態等信息, 這裡提供接口和頁面通信.
		Duel::DCore*			getEngineCore() { return mEngineCore; }
		DAStateManager*		getStateManager() { return mStateManager; }
		Duel::DRenderWindow*	getGameWindow() { return (Duel::DRenderWindow*)mRenderWindow; }

	protected:
		// 我只是抄下來..
		Platform::Agile<Windows::UI::Core::CoreWindow> m_window;
		Windows::UI::Xaml::Controls::SwapChainBackgroundPanel^ m_panel;

		// d3d 資源, 這裡只做一個創建工作, 并不在實際運行過程中使用, 具體使用的時機交給WPDX渲染系統.
		Microsoft::WRL::ComPtr<ID3D11Device1>			mD3DDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext1>	mD3DContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain1>			mSwapChain;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	mRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	m_d3dRenderTargetView;

		D3D_FEATURE_LEVEL		mD3DFeatureLevel;

		// 用于显示方向的转换。
		D2D1::Matrix3x2F m_orientationTransform2D;
		DirectX::XMFLOAT4X4 m_orientationTransform3D;

		// 引擎核心.
		Duel::DCore*				mEngineCore;	
		Duel::QuadtreePlugin*	mQuadtreePlugin;
		Duel::WPDXPlugin*		mWPDXPlugin;
		Duel::WPDXRenderWindow*	mRenderWindow;

		// 缓存的呈现器属性。
		// 這我只是抄下來.
		Windows::Foundation::Size m_d3dRenderTargetSize;
		Windows::Graphics::Display::DisplayOrientations m_orientation;
		Windows::Foundation::Rect m_windowBounds;
		float m_dpi;

		// 狀態控制.
		DAStateManager*			mStateManager;
		// 這裡做個示範, 載入一個空狀態.
		DANullState*				mNULLState;
		DemoState*				mDemoState;
		// 最基本的角色. 擴展之後在做.
		ActorFactory*			mActorFactory;

	};

}