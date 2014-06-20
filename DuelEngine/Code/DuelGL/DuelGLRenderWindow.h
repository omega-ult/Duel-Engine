//   [4/12/2013 OMEGA] created

#ifndef _DUELGLRENDERWINDOW_H_
#define _DUELGLRENDERWINDOW_H_

#include "Duel.h"
#include "DuelGLCommon.h"
#include "DuelFrameBuffer.h"

namespace Duel
{
	
	class DUELGL_API GLRenderWindow : public DRenderWindow
	{
	DUEL_DECLARE_RTTI(GLRenderWindow)
	public:
		GLRenderWindow(DRenderResourceFactory* parent, 
			//GLRenderSystem* rSys,
			const DString& name);
		virtual ~GLRenderWindow();
		// override : DRenderWindow-----------------------
		virtual	const DString&	getName() { return mName; }
		virtual void	create( const DString& name, const RenderWindowSetting& setting, uint32 winHandle );
		virtual void	setFullScreen( bool flag, const DisplayMode& fullScreenMode );
		virtual bool	isFullScreen() const;
		virtual void	reposition( uint32 top, uint32 left );
		virtual uint32	getTop();
		virtual uint32	getLeft();
		virtual	uint32	getWindowHandle();
		virtual	void	update();
		//////////////////////////////////////////////////////////////////////////
		// override : DFrameBuffer-------------------------
		virtual void	attachRenderColorView( ElementAttachment elem, DRenderColorView* v );
		virtual void	detachRenderColorView( ElementAttachment elem );
		virtual void	detachAllRenderColorViews();
		virtual void	attachRenderDepthStencilView( DRenderDepthStencilView* v );
		virtual void	detachRenderDepthStencilView();
		virtual DRenderColorView*	getRenderColorView( ElementAttachment elem );
		virtual DRenderDepthStencilView*	getRenderDepthStencilView();
		virtual void	resize( uint32 width, uint32 height );
		virtual void	clear( uint32 flags, const DColor& clr, DReal depth, int32 stencil );
		//////////////////////////////////////////////////////////////////////////

		GLuint			getFBO() { return mFBO; }
		GLuint			cacheFBO();
		//////////////////////////////////////////////////////////////////////////

	protected:
#ifdef DUEL_PLATFORM_WINDOWS
		HWND		mHWND;
		HDC			mHDC;


#endif // DUEL_PLATFORM_WINDOWS
		DString			mName;
		//GLRenderSystem*	mTargetRSys;
		GLuint			mFBO;
		typedef	std::vector<GLRenderColorView*>	RenderColorViewList;
		RenderColorViewList	mViewList;
		GLRenderDepthStencilView*	mCurDepthView;
		GLRenderColorView*	mMainSurface;
		GLRenderDepthStencilView*	mMainDepthView;
	};

}
#endif