//  [9/16/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelRenderWindow.h"


namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DRenderWindow, DObject);
	//DUEL_IMPLEMENT_RTTI_1(DRenderWindow, DRenderTarget);
// 
// 	DRenderWindow::DRenderWindow(DRenderSystem* parent, const DString& name) : 
// 		DRenderTarget(parent,name),
// 		mbFullScreen(false),
// 		mbVisible(true),
// 		mbClosed(false),
// 		mLeft(0),
// 		mTop(0)
// 	{
// 		mWidth = 0;
// 		mHeight = 0;
// 	}
// 
// 
// 	DRenderWindow::~DRenderWindow()
// 	{
// 		signalClosed(this);
// 	}
// 
// 	void DRenderWindow::setFullScreen( bool flag, const DisplayMode& fullScreenMode )
// 	{
// 		if(flag == true)
// 		{
// 			mbFullScreen = true;
// 		}
// 		else
// 		{
// 			mbFullScreen = false;
// 		}
// 	}
// 
// 	void DRenderWindow::onDestroy()
// 	{
// 
// 	}
// 
// 	void DRenderWindow::onResize()
// 	{
// 		signalResized(this);
// 	}
// 
// 	void DRenderWindow::onReposition()
// 	{
// 		signalMoved(this);
// 	}
// 
// 	bool DRenderWindow::isVisible()
// 	{
// 		return mbVisible;
// 	}
// 
// 	void DRenderWindow::setVisible( bool flag )
// 	{
// 		mbVisible = flag;
// 	}
// 
// 	bool DRenderWindow::isClosed()
// 	{
// 		return mbClosed;
// 	}
// 
// 	bool DRenderWindow::tryClosing()
// 	{
// 		bool accepted = true;
// 		std::vector<bool> result = signalClosing(this);
// 		std::vector<bool>::iterator i, iend = result.end();
// 		for ( i = result.begin(); i != iend; ++i)
// 		{
// 			if (*i == false)
// 			{
// 				accepted = false;
// 				break;
// 			}
// 		}
// 
// 		if(accepted)
// 		{
// 			signalClosed(this);
// 		}
// 
// 		return accepted;
// 	}
// 
// 	bool DRenderWindow::tryLostFocus()
// 	{
// 		bool accepted = true;
// 		std::vector<bool> result = signalLosingFocus(this);
// 		std::vector<bool>::iterator i, iend = result.end();
// 		for ( i = result.begin(); i != iend; ++i)
// 		{
// 			if (*i == false)
// 			{
// 				accepted = false;
// 				break;
// 			}
// 		}
// 		return accepted;
// 	}
// 
// 	void DRenderWindow::onLostFocus()
// 	{
// 		signalLostFocus(this);
// 	}
// 
// 	void DRenderWindow::reposition( uint32 top, uint32 left )
// 	{
// 		mTop = top, mLeft = left;
// 		onReposition();
// 	}


}

