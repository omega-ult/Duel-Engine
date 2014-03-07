//  [8/13/2013 OMEGA] created

#ifndef _DUELGUI2DBUTTONRD_H_
#define _DUELGUI2DBUTTONRD_H_

#include "Duel.h"
#include "DuelGUI.h"
#include "DuelGUICommon.h"
#include "DuelGUIRenderDelegate.h"
#include "DuelGUI2DButton.h"


namespace Duel
{
	class DUELGUI_API DG2DButtonRenderable : public DRenderable
	{
		DUEL_DECLARE_RTTI(DG2DButtonRenderable)
	public:
		DG2DButtonRenderable();
		// override : DGRenderDelegate-------------------------------
		virtual void	update(DG2DButton* parent);
		virtual DRenderLayout* getRenderLayout();
		virtual DRenderTechnique* getForwardRenderTechnique();
		//	virtual DRenderTechnique* getDeferRenderTechinique( DeferRenderStage stage );
		virtual DReal	getSquaredViewDepthToCamera( const DCamera* cam );
		virtual void	getWorldTransform( DMatrix4& outMat );
		virtual void	setDeferRenderEnabled( bool flag ) { /*omit it */ } 
		virtual void	setForwardRenderEnabled( bool flag ) { /*omit it */ } 
		void			updateCustomGpuParameter();
		//////////////////////////////////////////////////////////////////////////

	protected:
		// renderable's required data.
		DRenderLayoutPtr		mRenderLayout;
		DVertexStreams			mVStream;
		DIndexBufferPtr			mIndices;
		DVertexDeclarationPtr	mVDecl;
		DResourcePtr			mRenderEffect;
		// trans form data used in rendering.
		DMatrix4				mTransform;
		DResourcePtr			mTexture;
	};

	class DUELGUI_API DG2DButtonRD : public DGRenderDelegate
	{
	DUEL_DECLARE_RTTI(DG2DButtonRD)
	public:
		DG2DButtonRD(DG2DButton* parent);

		// override : DGRenderDelegate-------------------------------
		virtual void	update();
		// override : DGRenderDelegate-------------------------------
		virtual	void	applyToRenderQueue(DRenderQueue* queue, uint32 groupID);

		
	protected:

		DG2DButton*				mParent;
		DG2DButtonRenderable	mRenderable;
	};
	
}

#endif