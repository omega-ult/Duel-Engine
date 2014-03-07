//  [11/8/2013 OMEGA] created

#include "DuelGUICommon.h"
#include "DuelGUICursor.h"
#include "DuelGUIMathTool.h"
#include "Duel.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DGCursor, DObject);
	
	DGCursor::DGCursor() :
		mHostWindow(NULL),
		mPoint(0.0f, 0.0f),
		mCurAction(CA_Idle),
		mOpacity(1.0f),
		mbVisible(true),
		mSize(20.0f, 20.0f),
		mRenderDelegate(NULL)
	{

	}

	DGCursor::~DGCursor()
	{

	}

	void DGCursor::setCursorAction( uint32 m )
	{
		mCurAction = m;
	}

	uint32 DGCursor::getCursorAction()
	{
		return mCurAction;
	}

	void DGCursor::pointToScreen( const DVector2& pos)
	{
		DVector2 p;
		if (mHostWindow)
		{
			DReal w = (DReal)mHostWindow->getWidth();
			DReal h = (DReal)mHostWindow->getHeight();
			p.x = DMath::Min<DReal>(pos.x, w * 0.5f);
			p.x = DMath::Max<DReal>(pos.x, -w * 0.5f);
			p.y = DMath::Min<DReal>(pos.y, h * 0.5f);
			p.y = DMath::Max<DReal>(pos.y, -h * 0.5f);
			setOSCursorImpl(p);
		}
		setPoint(p);
	}

	void DGCursor::pointToWindow( uint32 left, uint32 top)
	{
		DVector2 p;
		if (mHostWindow)
		{
			DReal w = (DReal)mHostWindow->getWidth();
			DReal h = (DReal)mHostWindow->getHeight();
			p = DGGUIMathTool::getScreenPoint(left, top, DGSize(w,h));
			setOSCursorImpl(p);
		}
		setPoint(p);
	}

	const DVector2& DGCursor::getPointInScreen()
	{
		return mPoint;
	}

	void DGCursor::setVisible( bool flag )
	{
		mbVisible = flag;
	}

	bool DGCursor::isVisible()
	{
		return mbVisible;
	}

	void DGCursor::setOpacity( DReal val )
	{
		mOpacity = val;
	}

	DReal DGCursor::getOpacity()
	{
		return mOpacity;
	}

	void DGCursor::setHostWindow( DRenderWindow* wind )
	{
		mHostWindow = wind;
	}

	INTERNAL DRenderWindow* DGCursor::getHostWindow()
	{
		return mHostWindow;
	}

	void DGCursor::applyToRenderQueue( DRenderQueue* queue, uint32 groupID )
	{
		if (mRenderDelegate)
		{
			mRenderDelegate->applyToRenderQueue(queue, groupID);
		}
	}

	void DGCursor::update()
	{
		if (mRenderDelegate)
		{
			mRenderDelegate->update();
		}
	}

	void DGCursor::setPoint( const DVector2& p )
	{
		mPoint = p;
	}

	void DGCursor::setSize( const DGSize& size )
	{
		mSize = size;
	}

	const DGSize& DGCursor::getSize()
	{
		return mSize;
	}

	void DGCursor::setRenderDelegate( DGRenderDelegate* dele )
	{
		mRenderDelegate = dele;
	}

	DGRenderDelegate* DGCursor::getRenderDelegate()
	{
		return mRenderDelegate;
	}

	void DGCursor::setOSCursorImpl( const DVector2& p )
	{
#ifdef DUELGUI_PLATFORM_WINDOWS
		POINT pt;
		uint32 l = 0;
		uint32 t = 0;
		DGGUIMathTool::getWindowPoint(l, t, p, DGSize((DReal)mHostWindow->getWidth(),(DReal)mHostWindow->getHeight()));
		pt.x = (int32)l;
		pt.y = (int32)t;
		RECT rc;
		::GetWindowRect((HWND)mHostWindow->getWindowHandle(), &rc);
		pt.x += rc.left;
		pt.y += rc.top;
		::SetCursorPos(pt.x,pt.y);
#endif // DUELGUI_PLATFORM_WINDOWS
	}

}