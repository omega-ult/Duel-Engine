//  [6/12/2014 OMEGA] created

#include "DuelD3D9Common.h"
#include "DuelD3D9Include.h"
#include "DuelD3D9RenderWindow.h"
#include "DuelD3D9RenderView.h"
#include "DuelD3D9RenderResourceFactory.h"
#include "DuelD3D9RenderSystem.h"
#include "DuelCore.h"
#include "DuelD3D9ShaderObject.h"


namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(D3D9RenderWindow, DRenderWindow);


	D3D9RenderWindow::D3D9RenderWindow( DRenderResourceFactory* creator, const DString& name ) :
		DRenderWindow(creator, name),
		mHWND(NULL),
		mSwapChain(NULL),
		mCurDepthView(NULL),
		mName(name),
		mMainSurface(NULL),
		mMainDepthView(NULL)
	{
		for (int32 i = 0; i < 8; i++)
		{
			mViewList.push_back(NULL);
		}
		ZeroMemory(&mPresentParam, sizeof(D3DPRESENT_PARAMETERS));
	}

	D3D9RenderWindow::~D3D9RenderWindow()
	{
		ReleaseCOM(mSwapChain);

		if (mMainSurface != NULL)
		{
			mCreator->destroyRenderColorView(mMainSurface);
		}
	}

	const DString& D3D9RenderWindow::getName()
	{
		return mName;
	}

	void D3D9RenderWindow::create( const DString& name, const RenderWindowSetting& setting, uint32 winHandle )
	{
		mColorBits = DPixelFormatTool::getFormatBits(setting.surfaceFormat);

		mHWND = (HWND)winHandle;

		mPresentParam.Windowed					= true;		// 暂时先做窗口化, 全屏需要考虑设备丢失问题.
// 		mPresentParam.BackBufferWidth			= mWidth;
// 		mPresentParam.BackBufferHeight			= mHeight;
		mPresentParam.SwapEffect				= D3DSWAPEFFECT_DISCARD;
		mPresentParam.hDeviceWindow				= mHWND;	// 目标窗口, 不一定与device创建窗口相同.
		mPresentParam.EnableAutoDepthStencil	= false;
		mPresentParam.PresentationInterval		= D3DPRESENT_INTERVAL_ONE;	// swapbuffer数决定.
		mPresentParam.FullScreen_RefreshRateInHz= 0;
		mPresentParam.BackBufferFormat			= D3DFMT_X8R8G8B8;

		resize(setting.width, setting.height);

		if (mMainSurface != NULL)
		{
			mCreator->destroyRenderColorView(mMainSurface);
		}
		mMainSurface = mCreator->createRenderColorView(setting.surfaceFormat);
		// there isn't a render color view, use our main surface view instead.
		if (mViewList[0] == NULL)
		{
			attachRenderColorView(EA_Color0, mMainSurface);
		}


		if(mMainDepthView != NULL)
		{
			mCreator->destroyRenderDepthStencilView(mMainDepthView);
		}
		mMainDepthView = mCreator->createRenderDepthStencilView();
		// there isn't a render depth view, use our main depth view view instead.
		if (getRenderDepthStencilView() == NULL)
		{
			attachRenderDepthStencilView(mMainDepthView);
		}
	}

	void D3D9RenderWindow::setFullScreen( bool flag, const DisplayMode& fullScreenMode )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	bool D3D9RenderWindow::isFullScreen() const
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void D3D9RenderWindow::reposition( uint32 top, uint32 left )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	Duel::uint32 D3D9RenderWindow::getTop()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	Duel::uint32 D3D9RenderWindow::getLeft()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void D3D9RenderWindow::update()
	{
		if (mViewList[0] != NULL)
		{
			IDirect3DSurface9* backBuffer = NULL;
			mSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
			DCore::getSingleton().getRenderSystem()->getAs<D3D9RenderSystem>()->blitTexture(
				mViewList[0]->getGpuTexutureConstant()->getAs<D3D9GpuTextureConstant>()->getD3DTexture(),
				backBuffer);
			ReleaseCOM(backBuffer);
		}


		HRESULT hr = mSwapChain->Present(NULL,NULL,NULL,NULL,D3DPRESENT_DONOTWAIT);
		// test device lost state.
		if (hr == D3DERR_DEVICELOST || hr == D3DERR_DRIVERINTERNALERROR)
		{
			D3D9RenderResourceFactory* d3dFact = mCreator->getAs<D3D9RenderResourceFactory>();
			d3dFact->reset();
		}
	}

	void D3D9RenderWindow::resize( uint32 width, uint32 height )
	{
		if (width == mWidth && height == mHeight)
		{
			return;
		}
		assert(width != 0 && height != 0);
		DFrameBuffer::resize(width, height);
		mCurViewport.reset(0,0, width, height);

		onDeviceLost();
		onDeviceReset(mCreator->getAs<D3D9RenderResourceFactory>()->getMainDevice());

		RenderColorViewList::iterator i, iend = mViewList.end();
		for (i = mViewList.begin(); i != iend; ++i)
		{
			if ((*i) != NULL)
			{
				(*i)->resize(width, height);
			}
		}

		if(mCurDepthView != NULL)
		{
			mCurDepthView->resize(width, height);
		}
	}

	Duel::uint32 D3D9RenderWindow::getWindowHandle()
	{
		return (uint32)mHWND;
	}

	void D3D9RenderWindow::attachRenderColorView( ElementAttachment elem, DRenderColorView* v )
	{
		if (v == NULL)
		{
			assert(false);// "Can't attach an empty render color view"
			return;
		}
		if (v->getAttachedFrameBuffer() != NULL && v->getAttachedFrameBuffer() != this)
		{
			assert(false);// "Specified view is attached to another frame buffer, you must detach it before reusing it"
			return;
		}
		// the case specified v is attached to another channel
		if (getRenderColorView(elem) != NULL)
		{
			assert(false);//"Specified attachement is already in use, you must detach it firstly"
			return;
		}
		// if this view has been attached to this frame buffer, change its channel.
		if (v->getAttachedFrameBuffer() == this)
		{
			detachRenderColorView(v->getAttachPoint());
		}

		// chech format compatibility
		if (DPixelFormatTool::getFormatBits(v->getFormat()) != mColorBits)
		{
			return;
		}
		D3D9RenderColorView* rv = v->getAs<D3D9RenderColorView>(false);
		// check type availability.
		if (rv == NULL)
		{
			return;
		}
		rv->resize(mWidth, mHeight);
		mViewList[(uint32)(elem)] = rv;
		rv->setAttachFrameBuffer(this);
		rv->setAttachElement(elem);
	}

	void D3D9RenderWindow::detachRenderColorView( ElementAttachment elem )
	{
		if (mViewList[(uint32)(elem)] != NULL)
		{
			mViewList[(uint32)(elem)]->setAttachFrameBuffer(NULL);
			mViewList[(uint32)(elem)]->setAttachElement(EA_Color0);
			mViewList[(uint32)(elem)] = NULL;
		}
	}

	void D3D9RenderWindow::detachAllRenderColorViews()
	{
		for (uint32 i = 0; i < 8; ++i)
		{
			mViewList[i] = NULL;
		}
	}

	void D3D9RenderWindow::attachRenderDepthStencilView( DRenderDepthStencilView* v )
	{
		if (v == NULL)
		{
			assert(false);//"Can't attach an empty depth stencil view"
			return;
		}
		if (v->getAttachedFrameBuffer() != NULL && v->getAttachedFrameBuffer() != this)
		{
			assert(false);//"Specified view is attached to another frame buffer, you must detach it before reusing it"
			return;
		}
		D3D9RenderDepthStencilView* rv = v->getAs<D3D9RenderDepthStencilView>(false);
		// check type availability.
		if (rv == NULL)
		{
			return;
		}
		detachRenderDepthStencilView();
		rv->resize(mWidth, mHeight);
		mCurDepthView = rv;
		mCurDepthView->setAttachFrameBuffer(this);
	}

	void D3D9RenderWindow::detachRenderDepthStencilView()
	{
		if(mCurDepthView == NULL)
		{
			return;
		}
		mCurDepthView->setAttachFrameBuffer(NULL);
		mCurDepthView = NULL;
	}

	DRenderColorView* D3D9RenderWindow::getRenderColorView( ElementAttachment elem )
	{
		assert((uint32)elem < mViewList.size());
		return mViewList[elem];
	}

	DRenderDepthStencilView* D3D9RenderWindow::getRenderDepthStencilView()
	{
		return mCurDepthView;
	}

	void D3D9RenderWindow::onDeviceLost()
	{
		ReleaseCOM(mSwapChain);
		mSwapChain = NULL;
	}

	void D3D9RenderWindow::onDeviceReset(IDirect3DDevice9* dev)
	{
		mPresentParam.BackBufferWidth	= mWidth;
		mPresentParam.BackBufferHeight	= mHeight;

		dev->CreateAdditionalSwapChain(&mPresentParam,&mSwapChain);
	}

	void D3D9RenderWindow::clear( uint32 flags, const DColor& clr, DReal depth, int32 stencil )
	{

		D3D9RenderSystem* d3dRSys = DCore::getSingleton().getRenderSystem()->getAs<D3D9RenderSystem>();

		d3dRSys->clearFrameBuffer(this, flags, clr, depth, stencil);

	}

}