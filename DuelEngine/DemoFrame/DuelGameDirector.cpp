//  [3/19/2013 OMEGA] created

#include "pch.h"
#include "DuelGameDirector.h"
#include "NullState.h"
#include "DuelCore.h"

namespace Duel
{

	// 絕對不要new出2個GameDirector, 絕對報錯.
	DAGameDirector::DAGameDirector() : 
		m_dpi(-0.1f),
		mRenderWindow(NULL),
		mNULLState(NULL),
		mDemoState(NULL)
	{
		mEngineCore = new Duel::DCore("Assets\\CoreConfig.xml");
		mQuadtreePlugin = new Duel::QuadtreePlugin();
		mQuadtreePlugin->install();
		mQuadtreePlugin->initialize();
		
		mWPDXPlugin = new Duel::WPDXPlugin();
		mWPDXPlugin->install();
		mWPDXPlugin->initialize();


	}

	DAGameDirector::~DAGameDirector()
	{

		mEngineCore->getMovableManager()->unregisterMovableFactory(mActorFactory);
		delete	mActorFactory;

		if (mStateManager)
		{
			delete	mStateManager;
			mStateManager = NULL;
		}
		if (mNULLState)
		{
			delete	mNULLState;
			mNULLState = NULL;
		}
		if (mDemoState)
		{
			delete	mDemoState;
			mDemoState = NULL;
		}

		mQuadtreePlugin->shutdown();
		mQuadtreePlugin->uninstall();
		delete mQuadtreePlugin;
		mQuadtreePlugin = NULL;
		mWPDXPlugin->shutdown();
		mWPDXPlugin->uninstall();
		delete mWPDXPlugin;
		mWPDXPlugin = NULL;
	}

	void DAGameDirector::Initialize( Windows::UI::Core::CoreWindow^ window, Windows::UI::Xaml::Controls::SwapChainBackgroundPanel^ panel, float dpi )
	{


		m_window = window;
		m_panel = panel;

		CreateDeviceResources();
		SetDpi(dpi);

		mActorFactory = new ActorFactory();
		mEngineCore->getMovableManager()->registerMovableFactory(mActorFactory);

		//////////////////////////////////////////////////////////////////////////
		// 資源的加載就這樣. 這裡只是做一個資源聲明. 正式的資源載入時機交給程序.
		Duel::FileStreamWP8* fs = new Duel::FileStreamWP8("Assets\\MeshResources.xml");
		Duel::ResourceParametersPtr resP = mEngineCore->getResourceGroupManager()->getResouceManager("Mesh")->parseParameter(Duel::DDataStreamPtr(fs));
		Duel::ResourceParameters::iterator i, iend = resP->end();
		for (i = resP->begin(); i != iend; ++i)
		{
			mEngineCore->getResourceGroupManager()->declareResource(*i);
		}

		Duel::FileStreamWP8* gs = new Duel::FileStreamWP8("Assets\\GpuProgramResources.xml");
		resP = mEngineCore->getResourceGroupManager()->getResouceManager("GpuProgram")->parseParameter(Duel::DDataStreamPtr(gs));
		iend = resP->end();
		for (i = resP->begin(); i != iend; ++i)
		{
			mEngineCore->getResourceGroupManager()->declareResource(*i);
		}

		Duel::FileStreamWP8* rs = new Duel::FileStreamWP8("Assets\\RenderEffectResources.xml");
		resP = mEngineCore->getResourceGroupManager()->getResouceManager("RenderEffect")->parseParameter(Duel::DDataStreamPtr(rs));
		iend = resP->end();
		for (i = resP->begin(); i != iend; ++i)
		{
			mEngineCore->getResourceGroupManager()->declareResource(*i);
		}
		Duel::FileStreamWP8* ts = new Duel::FileStreamWP8("Assets\\TextureResources.xml");
		resP = mEngineCore->getResourceGroupManager()->getResouceManager("Texture")->parseParameter(Duel::DDataStreamPtr(ts));
		iend = resP->end();
		for (i = resP->begin(); i != iend; ++i)
		{
			mEngineCore->getResourceGroupManager()->declareResource(*i);
		}
		mEngineCore->getResourceGroupManager()->createDeclaredResource("Test");
		mEngineCore->getResourceGroupManager()->createDeclaredResource("General");


		mStateManager = new DAStateManager(this);
		mNULLState = new DANullState();
		mDemoState = new DemoState();
		mStateManager->registerState(mNULLState);
		mStateManager->registerState(mDemoState);
		mStateManager->requestStateChange(mNULLState, "Null");
		mStateManager->requestStateChange(mDemoState, "DemoState");


		//////////////////////////////////////////////////////////////////////////
		// test////// 這都通過了.... 沒問題-v-
		//////////////////////////////////////////////////////////////////////////

		Duel::DResourcePtr res = mEngineCore->getResourceGroupManager()->getResouceManager("Mesh")->getResource("box1box2.dm");
		res->load();
		//res = mCore->getResourceGroupManager()->getResouceManager("GpuProgram")->getResource("SimpleVertexShader.cso");
		//res->load();
		res = mEngineCore->getResourceGroupManager()->getResouceManager("RenderEffect")->getResource("testRE.dre");
		res->load();

		res = mEngineCore->getResourceGroupManager()->getResouceManager("Texture")->getResource("tree.dds");
		static_cast<DTexture*>(res.get())->load();

	}

	void DAGameDirector::HandleDeviceLost()
	{
		// 重置这些成员变量，以确保 SetDpi 重新创建所有资源。
		float dpi = m_dpi;
		m_dpi = -1.0f;
		m_windowBounds.Width = 0;
		m_windowBounds.Height = 0;
		mSwapChain = nullptr;

		CreateDeviceResources();
		SetDpi(dpi);
	}

	void DAGameDirector::CreateDeviceResources()
	{
		// 此标志为与 API 默认设置具有不同颜色渠道顺序的图面
		// 添加支持。要与 Direct2D 兼容，必须满足此要求。
		UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG)
		// 如果项目处于调试生成过程中，请通过带有此标志的 SDK 层启用调试。
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		// 此数组定义此应用程序将支持的 DirectX 硬件功能级别组。
		// 请注意，应保留顺序。
		// 请不要忘记在应用程序的说明中声明其所需的
		// 最低功能级别。除非另行说明，否则假定所有应用程序均支持 9.1。
		D3D_FEATURE_LEVEL featureLevels[] = 
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1
		};

		// 创建 Direct3D 11 API 设备对象和对应的上下文。
		ComPtr<ID3D11Device> d3dDevice;
		ComPtr<ID3D11DeviceContext> d3dContext;
		DX::ThrowIfFailed(
			D3D11CreateDevice(
			nullptr, // 指定 nullptr 以使用默认适配器。
			D3D_DRIVER_TYPE_HARDWARE,
			0,
			creationFlags, // 设置调试和 Direct2D 兼容性标志。
			featureLevels, // 此应用程序可以支持的功能级别的列表。
			ARRAYSIZE(featureLevels),
			D3D11_SDK_VERSION, // 对于 Windows 应用商店应用程序，始终将此值设置为 D3D11_SDK_VERSION。
			&d3dDevice, // 返回创建的 Direct3D 设备。
			&mD3DFeatureLevel, // 返回所创建设备的功能级别。
			&d3dContext // 返回设备的即时上下文。
			)
			);

		// 获取 Direct3D 11.1 API 设备和上下文接口。
		DX::ThrowIfFailed(
			d3dDevice.As(&mD3DDevice)
			);

		DX::ThrowIfFailed(
			d3dContext.As(&mD3DContext)
			);

		// 和引擎掛鉤的遊戲窗口.
		static_cast<WPDXRenderSystem*>(mEngineCore->getRenderSystem())->setDevice(mD3DDevice.Get());
		mRenderWindow = static_cast<WPDXRenderWindow*>(mEngineCore->getRenderSystem()->createRenderWindow("GameWindow"));
		
	}

	void DAGameDirector::UpdateForWindowSizeChange()
	{
		// 如果没有挂起的 DPI 更改，则仅处理窗口大小更改。
		if (m_dpi != DisplayProperties::LogicalDpi)
		{
			return;
		}

		if (m_window->Bounds.Width  != m_windowBounds.Width ||
			m_window->Bounds.Height != m_windowBounds.Height ||
			m_orientation != DisplayProperties::CurrentOrientation)
		{
			ID3D11RenderTargetView* nullViews[] = {nullptr};
			mD3DContext->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
			m_d3dRenderTargetView = nullptr;
			mD3DContext->Flush();
			CreateWindowSizeDependentResources();
		}
	}

	void DAGameDirector::SetDpi( float dpi )
	{
		if (dpi != m_dpi)
		{
			// 保存更新的 DPI 值。
			m_dpi = dpi;


			// 通常 DPI 更改意味着窗口大小更改。某些情况下，Windows 会发出
			// 大小更改事件和 DPI 更改事件。在这种情况下，生成的绑定
			// 将不会更改，并且窗口调整大小代码仅执行一次。
			UpdateForWindowSizeChange();
		}
	}

	float DAGameDirector::ConvertDipsToPixels( float dips )
	{	
		static const float dipsPerInch = 96.0f;
		return floor(dips * m_dpi / dipsPerInch + 0.5f); // 舍入到最接近的整数。
	}

	void DAGameDirector::CreateWindowSizeDependentResources()
	{
		// 存储窗口绑定，以便下次我们获取 SizeChanged 事件时，可避免
		// 在大小相同的情况下重新生成所有内容。
		m_windowBounds = m_window->Bounds;

		// 计算必要的交换链并呈现目标大小(以像素为单位)。
		float windowWidth = ConvertDipsToPixels(m_windowBounds.Width);
		float windowHeight = ConvertDipsToPixels(m_windowBounds.Height);

		// 交换链的宽度和高度必须基于窗口的
		// 横向宽度和高度。如果窗口在纵向
		// 方向，则必须使尺寸反转。
		m_orientation = DisplayProperties::CurrentOrientation;
		bool swapDimensions =
			m_orientation == DisplayOrientations::Portrait ||
			m_orientation == DisplayOrientations::PortraitFlipped;
		m_d3dRenderTargetSize.Width = swapDimensions ? windowHeight : windowWidth;
		m_d3dRenderTargetSize.Height = swapDimensions ? windowWidth : windowHeight;



		if (mSwapChain != nullptr)
		{
			// 如果交换链已存在，请调整其大小。
			// 不管. 遊戲就樣.
// 			HRESULT hr = mSwapChain->ResizeBuffers(
// 				2, // 双缓冲交换链。
// 				static_cast<UINT>(m_d3dRenderTargetSize.Width),
// 				static_cast<UINT>(m_d3dRenderTargetSize.Height),
// 				DXGI_FORMAT_B8G8R8A8_UNORM,
// 				0
// 				);
// 
// 			if (hr == DXGI_ERROR_DEVICE_REMOVED)
// 			{
// 				HandleDeviceLost();
// 				return;
// 			}
// 			else
// 			{
// 				DX::ThrowIfFailed(hr);
// 			}
		}
		else
		{
			// 否则，使用与现有 Direct3D 设备相同的适配器新建一个。
			DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {0};
			swapChainDesc.Width = static_cast<UINT>(m_d3dRenderTargetSize.Width); // 匹配窗口的大小。
			swapChainDesc.Height = static_cast<UINT>(m_d3dRenderTargetSize.Height);
			swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // 这是最常用的交换链格式。
			swapChainDesc.Stereo = false; 
			swapChainDesc.SampleDesc.Count = 1; // 请不要使用多采样。
			swapChainDesc.SampleDesc.Quality = 0;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = 2; // 使用双缓冲最大程度地减小延迟。
			swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // 所有 Windows 应用商店应用程序都必须使用此 SwapEffect。
			swapChainDesc.Flags = 0;

			ComPtr<IDXGIDevice1> dxgiDevice;
			DX::ThrowIfFailed(
				mD3DDevice.As(&dxgiDevice)
				);

			ComPtr<IDXGIAdapter> dxgiAdapter;
			DX::ThrowIfFailed(
				dxgiDevice->GetAdapter(&dxgiAdapter)
				);

			ComPtr<IDXGIFactory2> dxgiFactory;
			DX::ThrowIfFailed(
				dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory))
				);

			DX::ThrowIfFailed(
				dxgiFactory->CreateSwapChainForComposition(
				mD3DDevice.Get(),
				&swapChainDesc,
				nullptr,
				&mSwapChain
				)
				);

			// 将新交换链与 SwapChainBackgroundPanel 元素关联。
			ComPtr<ISwapChainBackgroundPanelNative> panelNative;
			DX::ThrowIfFailed(
				reinterpret_cast<IUnknown*>(m_panel)->QueryInterface(IID_PPV_ARGS(&panelNative))
				);

			DX::ThrowIfFailed(
				panelNative->SetSwapChain(mSwapChain.Get())
				);

			// 确保 DXGI 不会一次对多个帧排队。这样既可以减小延迟，
			// 又可以确保应用程序将只在每个 VSync 之后呈现，从而最大程度地降低功率消耗。
			DX::ThrowIfFailed(
				dxgiDevice->SetMaximumFrameLatency(1)
				);
		}
		// 为交换链设置正确方向，并生成 2D 和 3D 矩阵
		// 转换以呈现到旋转交换链。
		// 请注意，用于 2D 和 3D 转换的旋转角度不同。
		// 这是由坐标空间的差异引起的。此外，
		// 显式指定 3D 矩阵可以避免舍入误差。
		DXGI_MODE_ROTATION rotation = DXGI_MODE_ROTATION_UNSPECIFIED;
		switch (m_orientation)
		{
		case DisplayOrientations::Landscape:
			rotation = DXGI_MODE_ROTATION_IDENTITY;
			m_orientationTransform2D = D2D1::Matrix3x2F::Identity();
			m_orientationTransform3D = XMFLOAT4X4( // 0 度 Z 旋转
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
				);
			break;

		case DisplayOrientations::Portrait:
			rotation = DXGI_MODE_ROTATION_ROTATE270;
			m_orientationTransform2D = 
				D2D1::Matrix3x2F::Rotation(270.0f) *
				D2D1::Matrix3x2F::Translation(0.0f, m_windowBounds.Width);
			m_orientationTransform3D = XMFLOAT4X4( // 90 度 Z 旋转
				0.0f, 1.0f, 0.0f, 0.0f,
				-1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
				);
			break;

		case DisplayOrientations::LandscapeFlipped:
			rotation = DXGI_MODE_ROTATION_ROTATE180;
			m_orientationTransform2D = 
				D2D1::Matrix3x2F::Rotation(180.0f) *
				D2D1::Matrix3x2F::Translation(m_windowBounds.Width, m_windowBounds.Height);
			m_orientationTransform3D = XMFLOAT4X4( // 180 度 Z 旋转
				-1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, -1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
				);
			break;

		case DisplayOrientations::PortraitFlipped:
			rotation = DXGI_MODE_ROTATION_ROTATE90;
			m_orientationTransform2D = 
				D2D1::Matrix3x2F::Rotation(90.0f) *
				D2D1::Matrix3x2F::Translation(m_windowBounds.Height, 0.0f);
			m_orientationTransform3D = XMFLOAT4X4( // 270 度 Z 旋转
				0.0f, -1.0f, 0.0f, 0.0f,
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
				);
			break;

		default:
			throw ref new Platform::FailureException();
		}

		DX::ThrowIfFailed(
			mSwapChain->SetRotation(rotation)
			);

		ComPtr<ID3D11Texture2D> backBuffer;
		DX::ThrowIfFailed(
			mSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer))
			);

		DX::ThrowIfFailed(
			mD3DDevice->CreateRenderTargetView(
			backBuffer.Get(),
			nullptr,
			&m_d3dRenderTargetView
			)
			);

		// 嗯 綁定了.
		mRenderWindow->updateRenderTargetView(m_d3dRenderTargetView.Get());

	}

	void DAGameDirector::ValidateDevice()
	{
		ComPtr<IDXGIDevice1> dxgiDevice;
		ComPtr<IDXGIAdapter> deviceAdapter;
		DXGI_ADAPTER_DESC deviceDesc;
		DX::ThrowIfFailed(mD3DDevice.As(&dxgiDevice));
		DX::ThrowIfFailed(dxgiDevice->GetAdapter(&deviceAdapter));
		DX::ThrowIfFailed(deviceAdapter->GetDesc(&deviceDesc));


		ComPtr<IDXGIFactory2> dxgiFactory;
		ComPtr<IDXGIAdapter1> currentAdapter;
		DXGI_ADAPTER_DESC currentDesc;

		DX::ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory)));
		DX::ThrowIfFailed(dxgiFactory->EnumAdapters1(0, &currentAdapter));
		DX::ThrowIfFailed(currentAdapter->GetDesc(&currentDesc));

		if (deviceDesc.AdapterLuid.LowPart != currentDesc.AdapterLuid.LowPart ||
			deviceDesc.AdapterLuid.HighPart != currentDesc.AdapterLuid.HighPart ||
			FAILED(mD3DDevice->GetDeviceRemovedReason()))
		{
			dxgiDevice = nullptr;
			deviceAdapter = nullptr;
			HandleDeviceLost();
		}
	}

	void DAGameDirector::update()
	{
		// hehe
		mStateManager->transactionStart();
		mStateManager->parseInput();
		mStateManager->update();

	}
	void DAGameDirector::render()
	{
		mStateManager->render();
	}


	void DAGameDirector::present()
	{
		// 应用程序可以选择指定“已更新”或“滚动”
		// 矩形，以改进特定方案的效率。
		DXGI_PRESENT_PARAMETERS parameters = {0};
		parameters.DirtyRectsCount = 0;
		parameters.pDirtyRects = nullptr;
		parameters.pScrollRect = nullptr;
		parameters.pScrollOffset = nullptr;

		// 第一个参数指示 DXGI 进行阻止直到 VSync，这使应用程序
		// 在下一个 VSync 前进入休眠。这将确保我们不会浪费任何周期呈现将
		// 从不会在屏幕上显示的帧。
		HRESULT hr = mSwapChain->Present1(1, 0, &parameters);

		// 放弃呈现目标的内容。
		// 这是仅在完全覆盖现有内容时
		// 有效的操作。如果使用已更新或滚动矩形，则应将本调用删除。
		mD3DContext->DiscardView(m_d3dRenderTargetView.Get());

		// 放弃深度模具的内容。
		mD3DContext->DiscardView(mRenderWindow->getDepthStencilView());

		// 如果通过断开连接或进行驱动程序升级的方法删除了设备，则必须
		// 必须重新创建所有设备资源。
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			HandleDeviceLost();
		}
		else
		{
			DX::ThrowIfFailed(hr);
		}
		// 結束本輪循環.
		mStateManager->transactionFinish();
	}

}