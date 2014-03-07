//  [6/23/2013 OMEGA] created

#ifndef _DUELDEMORENDERWORKSHOP_H_
#define _DUELDEMORENDERWORKSHOP_H_

#include "DuelCommon.h"
#include "DuelRenderWorkshop.h"

namespace Duel
{

	class DDemoRenderWorkshop : public DRenderWorkshop
	{
	DUEL_DECLARE_RTTI(DDemoRenderWorkshop)
	public:
		DDemoRenderWorkshop();
		~DDemoRenderWorkshop();


		// override : DRenderWorkshop---------------------
		void			renderSingleObject(DRenderable* rendObj, DRenderPass* pass);

		// override : DRenderWorkshop-----------------------
		virtual void	setPresentTarget(DFrameBuffer* target);

		virtual void	render( DRenderQueue* queue );

	protected:
		void			populateRenderables(DRenderGroup* grp, uint32 renderStage);
		// because we are in a multi render target environment,
		// we need manage gbuffer for each present target, here we
		// can do some memory management to reduce the cost.
		typedef std::map<DFrameBuffer*, DFrameBuffer*>	GBufferMap;
		GBufferMap		mGbufferMap;

		struct RenderElement
		{
			DRenderable* renderable;
			DRenderPass* renderPass;
		};
		typedef std::vector<RenderElement>	RenderElementList;
		RenderElementList	mRenderList;
	};
}

#endif