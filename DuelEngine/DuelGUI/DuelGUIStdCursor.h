//  [11/11/2013 OMEGA] created

#ifndef _DUELGUISTDBUTTON_H_
#define _DUELGUISTDBUTTON_H_

#include "DuelGUICommon.h"
#include "DuelGUICursor.h"

namespace Duel
{

	// 特化的东西 - - .
	class DUELGUI_API DGStdCursorRD : public DGRenderDelegate, public DRenderable
	{
		DUEL_DECLARE_RTTI(DGStdCursorRD)
	public:
		DGStdCursorRD(DGCursor* hostCursor);
		 
		// override : DGRenderDelegate------------------
		virtual void	applyToRenderQueue( DRenderQueue* queue, uint32 groupID );

		// override : DRenderable-----------------
		virtual DRenderLayout*	getRenderLayout();
		virtual DRenderTechnique*	getRenderTechnique(uint32 stage);
		virtual void	updateCustomGpuParameter(DShaderObject* so);

		virtual DGWidget* getParent();

	protected:
		DGCursor*		mHostCursor;
		// renderable's required data.
		DRenderLayoutPtr		mRenderLayout;
		DVertexStreams			mVStream;
		DIndexBufferPtr			mIndices;
		DVertexDeclaration		mVDecl;
		DResourcePtr			mRenderEffect;
		// trans form data used in rendering.
		DMatrix4				mTransform;
		DResourcePtr			mTexture;

	};

	class DUELGUI_API DGStdCursor : public DGCursor
	{
		DUEL_DECLARE_RTTI(DGStdCursor)
	public:
		DGStdCursor();
		~DGStdCursor();

	protected:


	private:
		// private render delegate.
		DGRenderDelegate*	mStdRenderDelegate;
	};
}


#endif