//  [11/26/2013 OMEGA] created

#ifndef _DUELGUISHOWCASEPICTURELAYER_H_
#define _DUELGUISHOWCASEPICTURELAYER_H_

#include "DuelGUICommon.h"
#include "Duel.h"

namespace Duel
{

	class DUELGUI_API DGSPictureLayer : public DRenderable
	{
		DUEL_DECLARE_RTTI(DGSPictureLayer)
	public:
		DGSPictureLayer(DGRenderDelegate* parent);
		// override : DRenderable-----------------
		virtual DRenderLayout*	getRenderLayout();
		virtual DRenderTechnique*	getRenderTechnique(uint32 stage);
		virtual void	updateCustomGpuParameter(DShaderObject* so);
		virtual	void	getWorldTransform(DMatrix4& outMat);
		virtual void	preRender(); // calculate matrix;
		virtual void	postRender();

		void			setRenderSize(const DGSize& size);
		void			setClipping(DReal clipL, DReal clipR, DReal clipT, DReal clipB);

		void			setTexture(DResourcePtr tex);
		// 好吧我现在就只是show看看. 什么都不说了
		void			setAlignment(DGAlignment align);

		// whether to extend to the widget's geometry.
		void			setScalable(bool flag);

		// for upper level, setting the global transform.
		void			setWorldTransform(const DMatrix4& mat);

	protected:
		void			markDirty();
		DGRenderDelegate*		mParent;
		// renderable's required data.
		DRenderLayoutPtr		mRenderLayout;
		DVertexStreams			mVStream;
		DIndexBufferPtr			mIndices;
		DVertexDeclaration		mVDecl;
		DResourcePtr			mRenderEffect;
		// trans form data used in rendering.
		DMatrix4				mWorldTransform;
		DMatrix4				mQuadTransform;
		DMatrix3				mUVTransform;

		DResourcePtr			mTexture;
		DTextureSamplerState	mTexSampler;

		DGSize				mRenderSize;
		DReal				mClipLeft;
		DReal				mClipRight;
		DReal				mClipTop;
		DReal				mClipBottom;

		DGAlignment			mAlignment;

		bool				mbScalable;
		bool				mbDirty;
	};
}

#endif // !_DUELGUISHOWCASEPICTURELAYER_H_
