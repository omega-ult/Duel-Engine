//  [6/23/2013 OMEGA] created

#include "pch.h"
#include "DuelCommon.h"
#include "DuelRenderWorkshop.h"
#include "DuelDemoRenderWorkshop.h"
#include "DuelRenderResourceManager.h"

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(DDemoRenderWorkshop, DRenderWorkshop);


	DDemoRenderWorkshop::DDemoRenderWorkshop()
	{
	}
	
	
	DDemoRenderWorkshop::~DDemoRenderWorkshop()
	{
		DRenderResourceManager* rm = DRenderResourceManager::getSingletonPtr();
		GBufferMap::iterator i, iend = mGbufferMap.end();
		for (i = mGbufferMap.begin(); i != iend; ++i)
		{
			rm->destroyFrameBuffer(i->second);
		}
	}



	void DDemoRenderWorkshop::renderSingleObject( DRenderable* rendObj, DRenderPass* pass )
	{
		DShaderObject* so = pass->getShaderObject().get();
		if (so != NULL && so->isValid())
		{
			rendObj->updateAutoGpuParameter(so);
			rendObj->updateCustomGpuParameter(so);
			mRenderSystem->setDepthStencilState(pass->getDepthStencilStateObject().get());
			mRenderSystem->setRasterizerState(pass->getRaseterizerStateObject().get());
			mRenderSystem->setBlendState(pass->getBlendStateStateObject().get(), DColor::WHITE);
			mRenderSystem->bindShaderObject(so);
			mRenderSystem->render(rendObj->getRenderLayout());
		}

	}

	void DDemoRenderWorkshop::render( DRenderQueue* queue )
	{
		if (queue == NULL || mPresentTarget == NULL)
		{
			return;
		}
		// for defer stage.
		GBufferMap::iterator gi = mGbufferMap.find(mPresentTarget);
		if (gi == mGbufferMap.end())
		{
			return;
		}
		DFrameBuffer* gbuf = gi->second;
		mRenderSystem->bindFrameBuffer(gbuf);

		DRenderQueue::RenderGroupIterator ri = queue->getRenderGroupIterator();
		while (ri.hasMoreElements())
		{
			DRenderGroup* rgrp = ri.getNext();
			populateRenderables(rgrp, RS_Defer_GBuffer);
			if (!mRenderList.empty())
			{
				signalGroupStartRender(queue, rgrp);
				RenderElementList::iterator i, iend = mRenderList.end();
				for (i = mRenderList.begin(); i != iend; ++i)
				{
					RenderElement& e = *i;
					renderSingleObject(e.renderable, e.renderPass);
				}
				signalGroupFinishRender(queue, rgrp);
			}
			// now go for light accumulation.
			DRenderQueue::LightIterator li = queue->getLightIterator();
			while (li.hasMoreElements())
			{
				DLightSource* l = li.getNext();
			}
		}

		// forward stage now;
		mRenderSystem->bindFrameBuffer(mPresentTarget);
		ri = queue->getRenderGroupIterator();
		while (ri.hasMoreElements())
		{
			DRenderGroup* rgrp = ri.getNext();
			populateRenderables(rgrp, RS_Forward);
			if (!mRenderList.empty())
			{
				signalGroupStartRender(queue, rgrp);
				RenderElementList::iterator i, iend = mRenderList.end();
				for (i = mRenderList.begin(); i != iend; ++i)
				{
					RenderElement& e = *i;
					renderSingleObject(e.renderable, e.renderPass);
				}
				signalGroupFinishRender(queue, rgrp);
			}
		}
	}

	void DDemoRenderWorkshop::setPresentTarget( DFrameBuffer* target )
	{
		if (target == NULL)
		{
			return;
		}
		if (mGbufferMap.find(target) == mGbufferMap.end())
		{
			DFrameBuffer* gbuffer = DRenderResourceManager::getSingleton().createFrameBuffer(target->getWidth(), target->getHeight(), 32);
			gbuffer->enableElement(EA_Color0, PF_A8R8G8B8);
			gbuffer->enableElement(EA_Color1, PF_A8R8G8B8);
			gbuffer->enableElement(EA_Color2, PF_A8R8G8B8);
			gbuffer->enableElement(EA_Color3, PF_R32_Float);
			mGbufferMap[target] = gbuffer;
		}
		DRenderWorkshop::setPresentTarget(target);
	}

	void DDemoRenderWorkshop::populateRenderables(DRenderGroup* grp,  uint32 renderStage )
	{
		mRenderList.clear();
		DRenderGroup::RenderableIterator rendI = grp->getRederableIterator();
		while (rendI.hasMoreElements())
		{
			DRenderable* rend = rendI.getNext();
			DRenderTechnique* tech = rend->getRenderTechnique(renderStage);
			if (tech != NULL)
			{
				DRenderTechnique::RenderPassIterator pi = tech->getRenderPassIterator();
				while (pi.hasMoreElements())
				{
					DRenderPass* p = pi.getNext().get();
					if (p != NULL &&
						p->getVertexProgram() != NULL && p->getVertexProgram()->isLoaded() &&
						p->getPixelProgram() != NULL && p->getPixelProgram()->isLoaded())
					{
						RenderElement elm;
						elm.renderable = rend;
						elm.renderPass = p;
						mRenderList.push_back(elm);
					}
				}
			}
		}
	}

}