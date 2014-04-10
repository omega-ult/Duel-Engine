//  [6/23/2013 OMEGA] created

#include "pch.h"
#include "DuelCommon.h"
#include "DuelRenderWorkshop.h"
#include "DuelRenderEffect.h"
#include "DuelDemoRenderWorkshop.h"
#include "DuelRenderResourceManager.h"
#include "DuelResourceGroupManager.h"

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(DDemoRenderWorkshop, DRenderWorkshop);

	DDemoRenderWorkshop::DDemoRenderWorkshop()
	{
	}
	
	
	DDemoRenderWorkshop::~DDemoRenderWorkshop()
	{
		DRenderResourceManager* rm = DRenderResourceManager::getSingletonPtr();
		DeferLayerMap::iterator i, iend = mDeferLayerMap.end();
		for (i = mDeferLayerMap.begin(); i != iend; ++i)
		{
			destryDeferLayer(i->second);
		}
	}



	DDemoRenderWorkshop::DeferLayer DDemoRenderWorkshop::getCurrentDeferlayer()
	{
		DeferLayerMap::iterator i = mDeferLayerMap.find(mPresentTarget);
		if (i == mDeferLayerMap.end())
		{
			return DeferLayer();
		}
		return i->second;
	}


	void DDemoRenderWorkshop::renderSingleObject( DFrameBuffer* target, DRenderable* rendObj, DRenderPass* pass )
	{
		DShaderObject* so = pass->getShaderObject().get();
		if (target != NULL && so != NULL && so->isValid())
		{
			if(mRenderSystem->getCurrentFrameBuffer() != target)
			{
				mRenderSystem->bindFrameBuffer(target);
			}
			rendObj->preRender();
			rendObj->updateAutoGpuParameter(so);
			rendObj->updateCustomGpuParameter(so);
			mRenderSystem->setDepthStencilState(pass->getDepthStencilStateObject().get());
			mRenderSystem->setRasterizerState(pass->getRaseterizerStateObject().get());
			mRenderSystem->setBlendState(pass->getBlendStateStateObject().get(), DColor::WHITE);
			mRenderSystem->bindShaderObject(so);
			mRenderSystem->render(rendObj->getRenderLayout());
			rendObj->postRender();
		}

	}

	void DDemoRenderWorkshop::render( DRenderQueue* queue )
	{
		if (queue == NULL || mPresentTarget == NULL)
		{
			return;
		}
		DAutoGpuParameter::getSingleton().setParameterDelegate(queue);
		DRenderQueue::RenderGroupIterator ri = queue->getRenderGroupIterator();;
		// for defer stage.
		DeferLayerMap::iterator gi = mDeferLayerMap.find(mPresentTarget);
		if (gi != mDeferLayerMap.end())
		{
			// we can render defer stage.
			DeferLayer deferlayer = gi->second;

			ri = queue->getRenderGroupIterator();
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
			 			renderSingleObject(mPresentTarget, e.renderable, e.renderPass);
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
					renderSingleObject(mPresentTarget, e.renderable, e.renderPass);
				}
				signalGroupFinishRender(queue, rgrp);
			}
		}
		DAutoGpuParameter::getSingleton().setParameterDelegate(NULL);
	}

	void DDemoRenderWorkshop::setPresentTarget( DFrameBuffer* target )
	{
		if (target == NULL)
		{
			return;
		}
		// check whether this target is in our defer layer, if so,
		// dont't create another defer layer for it.
		DeferLayerMap::iterator i, iend = mDeferLayerMap.end();
		bool isInDeferLayer = false;
		for (i = mDeferLayerMap.begin(); i != iend; ++i)
		{
			DeferLayer ly = i->second;
			if (ly.GBuffer == target || 
				ly.LightAccum == target)
			{
				isInDeferLayer = true;
				break;
			}
		}
		if (!isInDeferLayer && mDeferLayerMap.find(target) == mDeferLayerMap.end())
		{
			mDeferLayerMap[target] = createDeferLayer(target);
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
			DRenderTechnique* tech = rend->getRenderTechnique(renderStage, 
				grp->getParent()->getRenderCamera(), grp->getParent()->getLightIterator());
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

	DDemoRenderWorkshop::DeferLayer DDemoRenderWorkshop::createDeferLayer( DFrameBuffer* target )
	{
		DeferLayer ret;
		ret.GBuffer = DRenderResourceManager::getSingleton().createFrameBuffer(target->getWidth(), target->getHeight(), 32);
// 		ret.GBuffer->enableElement(EA_Color0, PF_A8R8G8B8);
// 		ret.GBuffer->enableElement(EA_Color1, PF_A8R8G8B8);
		return ret;
	}

	void DDemoRenderWorkshop::destryDeferLayer( DeferLayer lay )
	{
		if (lay.GBuffer != NULL)
		{
			DRenderResourceManager::getSingleton().destroyFrameBuffer(lay.GBuffer);
			lay.GBuffer = NULL;
		}
	}



}