//  [11/26/2013 OMEGA] created

#ifndef _DUELGUISHOWCASETEXTLAYER_H_
#define _DUELGUISHOWCASETEXTLAYER_H_

#include "DuelGUICommon.h"
#include "Duel.h"

namespace Duel
{
	struct CharacterInfo 
	{
		uint32			code;
		DGGeometry		geometry;
		DFont::UVRect	uvRect;
	};
	typedef	std::vector<CharacterInfo>	CharacterInfoList;

	class DUELGUI_API DGSTextLayer : public DRenderable
	{
		DUEL_DECLARE_RTTI(DGSTextLayer)
	public:
		// override : DRenderable-----------------
		virtual DRenderLayout*	getRenderLayout();
		virtual DRenderTechnique*	getForwardRenderTechnique();
		virtual void	updateCustomGpuParameter(DRenderPass* pass);
		virtual	void	getWorldTransform(DMatrix4& outMat);
		virtual void	preRender(); // calculate matrix;


		void		setTexture(const DString& texName, const DString& groupName);
		void		setTexture(DResourcePtr tex);

		void		setRenderSize(const DGSize& size);
		void		setClipping(DReal clipL, DReal clipR, DReal clipT, DReal clipB);

		void		setWorldTransform(const DMatrix4& mat);

		void		setText(const CharacterInfoList& text);

		void		clearText();

	protected:
		DRenderLayoutPtr		mRenderLayout;
		DVertexStreams			mVStream;
		DIndexBufferPtr			mIndices;
		DVertexDeclarationPtr	mVDecl;
		DResourcePtr			mRenderEffect;


		bool			mbDirty;
	};
}


#endif