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
		virtual void	enableElement( ElementAttachment elem, DPixelFormat format );
		virtual void	disableElement( ElementAttachment elem );
		virtual DRenderView* getRenderView( ElementAttachment elem );
		virtual void	resize( uint32 width, uint32 height );
		virtual void	clear( uint32 flags, const DColor& clr, DReal depth, int32 stencil );
		//////////////////////////////////////////////////////////////////////////

		GLuint			getFBO() { return mFBO; }
		HGLRC			getGLRC() { return mHGLRC; }
		GLuint			cacheFBO();
		//////////////////////////////////////////////////////////////////////////

		void			bindContext();

	protected:
#ifdef DUEL_PLATFORM_WINDOWS
		HWND		mHWND;
		HGLRC		mHGLRC;
		HDC			mHDC;


#endif // DUEL_PLATFORM_WINDOWS
		DString			mName;
		//GLRenderSystem*	mTargetRSys;
		GLuint			mFBO;
		GLuint			mDepthStencilBuffer;
		typedef	std::vector<GLRenderView*>	RenderViewList;
		RenderViewList	mViewList;
	};

}
#endif