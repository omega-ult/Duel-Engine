//  [6/23/2013 OMEGA] created

#include "pch.h"
#include "DuelCommon.h"
#include "DuelCore.h"
#include "DuelRenderView.h"
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
			destroyDeferLayer(i->second);
		}
	}



	DDemoRenderWorkshop::DeferLayer* DDemoRenderWorkshop::getCurrentDeferlayer()
	{
		DeferLayerMap::iterator i = mDeferLayerMap.find(mPresentTarget);
		if (i != mDeferLayerMap.end())
		{
			return i->second;
		}
		return NULL;
	}


	void DDemoRenderWorkshop::renderSingleObject( DFrameBuffer* target, DRenderable* rendObj, DRenderPass* pass )
	{
		DRenderSystem* rsys = DCore::getSingleton().getRenderSystem();
		if (rsys == NULL || pass == NULL)
		{
			return;
		}
		DShaderObject* so = pass->getShaderObject().get();
		if (target != NULL && so != NULL && so->isValid())
		{
			rsys->bindFrameBuffer(target);

			rendObj->preRender();
			rendObj->updateAutoGpuParameter(so);
			rendObj->updateCustomGpuParameter(so);
			rsys->setDepthStencilState(pass->getDepthStencilStateObject().get());
			rsys->setRasterizerState(pass->getRaseterizerStateObject().get());
			rsys->setBlendState(pass->getBlendStateStateObject().get(), DColor::WHITE);
			rsys->bindShaderObject(so);
			rsys->render(rendObj->getRenderLayout());
			rendObj->postRender();
		}

	}

	void DDemoRenderWorkshop::render( DRenderQueue* queue )
	{
		DRenderSystem* rsys = DCore::getSingleton().getRenderSystem();
		if (queue == NULL || mPresentTarget == NULL || rsys == NULL)
		{
			return;
		}
		// we use the present target's depth stencil buffer, 
		DAutoGpuParameter::getSingleton().setParameterDelegate(queue);
		DRenderQueue::RenderGroupIterator ri = queue->getRenderGroupIterator();;
		// for defer stage.
		DeferLayerMap::iterator gi = mDeferLayerMap.find(mPresentTarget);
		bool deferProcessing = false;	// a flag indicate whether we need to merge defer output.
		while(ri.hasMoreElements())
		{
			DRenderGroup* rgrp = ri.getNext();
			populateRenderables(rgrp, RS_Defer_GBuffer);
			if(!mRenderList.empty())
			{
				deferProcessing = true;
				break;
			}
		}
		if (gi != mDeferLayerMap.end() && deferProcessing)
		{
			// we can render defer stage.
			DeferLayer* deferlayer = gi->second;
			if (deferlayer != NULL)
			{
				deferlayer->getGeoStageFrameBuffer()->resize(mPresentTarget->getWidth(), mPresentTarget->getHeight());
				// GBuffer---------------------------------------------
				deferlayer->prepareGBufferStage();
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
			 				renderSingleObject(deferlayer->getGeoStageFrameBuffer(), e.renderable, e.renderPass);
			 			}
			 			signalGroupFinishRender(queue, rgrp);
			 		}
				}
 				// light accumulation.-------------------------------	
				deferlayer->getLightStageFrameBuffer()->resize(mPresentTarget->getWidth(), mPresentTarget->getHeight());					
				deferlayer->prepareLightStage();
				DRenderQueue::LightIterator li = queue->getLightIterator();
				while (li.hasMoreElements())
				{
					DLightSource* l = li.getNext();
				}
				// merge---------------------------------------------
				deferlayer->getMergeStageFrameBuffer()->resize(mPresentTarget->getWidth(), mPresentTarget->getHeight());
				deferlayer->prepareMergeStage();
				{
					mMergeHelper.setInputAlbedo(deferlayer->getAlbedoMap()->
						getGpuTexutureConstant());
					mMergeHelper.setInputLightAccumulationMap(deferlayer->getLightAccumulationMap()->
						getGpuTexutureConstant());
					mMergeHelper.setInputDepth(deferlayer->getDepthMap()->
						getGpuTexutureConstant());
					DRenderTechnique* mergetTech = mMergeHelper.getRenderTechnique(
						DDemoMergeHelper::RS_DeferMerge, queue->getRenderCamera(), queue->getLightIterator());
					if (mergetTech)
					{
						DRenderTechnique::RenderPassIterator p = mergetTech->getRenderPassIterator();
						while (p.hasMoreElements())
						{
							DRenderPass* pass = p.getNext().get();
							renderSingleObject(deferlayer->getMergeStageFrameBuffer(), &mMergeHelper, pass);
						}
					}
				}
				// transfer color to the present target.
				mMergeHelper.setTransferSource(deferlayer->getMergedColorMap()->
					getGpuTexutureConstant());
				DRenderTechnique* transTech = mMergeHelper.getRenderTechnique(
					DDemoMergeHelper::RS_ScreenTransfer, queue->getRenderCamera(), queue->getLightIterator());
				if (transTech)
				{
					DRenderTechnique::RenderPassIterator p = transTech->getRenderPassIterator();
					while (p.hasMoreElements())
					{
						DRenderPass* pass = p.getNext().get();
						renderSingleObject(mPresentTarget, &mMergeHelper, pass);
					}
				}
			}

		}
		// forward stage now;
		// re-attach depth stencil view to the present target, the drawing result
		// in the defer stage will be re-used.
		rsys->bindFrameBuffer(mPresentTarget);
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

		// post processing stage.

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
			DeferLayer* ly = i->second;
			if (ly->getGeoStageFrameBuffer() == target)
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

	DDemoRenderWorkshop::DeferLayer* DDemoRenderWorkshop::createDeferLayer( DFrameBuffer* target )
	{
		DeferLayer* ret = new DeferLayer();
		ret->initialize(target->getWidth(), target->getHeight());
		return ret;
	}

	void DDemoRenderWorkshop::destroyDeferLayer( DeferLayer* lay )
	{
		if (lay != NULL)
		{
			delete lay;
		}
	}




	DDemoRenderWorkshop::DeferLayer::DeferLayer() : 
		mGeoFrameBuffer(NULL),
		mLightFrameBuffer(NULL),
		mMergeFrameBuffer(NULL),
		mAlbedoRT(NULL),
		mDepthRT(NULL),
		mViewSpaceNormalRT(NULL),
		mGeoDepthStencil(NULL),
		mLightAccumRT(NULL),
		mMergeRT(NULL)
	{

	}

	DDemoRenderWorkshop::DeferLayer::~DeferLayer()
	{
		shutdown();
	}

	void DDemoRenderWorkshop::DeferLayer::initialize(uint32 w, uint32 h)
	{
		assert(w != 0 && h != 0);
		shutdown();
		DRenderResourceManager* rm = DRenderResourceManager::getSingletonPtr();
		mGeoFrameBuffer = rm->createFrameBuffer(w, h, 32);
		mLightFrameBuffer = rm->createFrameBuffer(w, h, 32);
		mMergeFrameBuffer = rm->createFrameBuffer(w, h, 32);

		mAlbedoRT = rm->createRenderColorView(PF_A8R8G8B8);
		mDepthRT = rm->createRenderColorView(PF_A8R8G8B8);
		mViewSpaceNormalRT = rm->createRenderColorView(PF_A8R8G8B8);
		mGeoDepthStencil = rm->createRenderDepthStencilView();

		mLightAccumRT = rm->createRenderColorView(PF_A8R8G8B8);
		mMergeRT = rm->createRenderColorView(PF_A8R8G8B8);

		mGeoFrameBuffer->attachRenderColorView(EA_Color0, mAlbedoRT);
		mGeoFrameBuffer->attachRenderColorView(EA_Color1, mViewSpaceNormalRT);
		mGeoFrameBuffer->attachRenderColorView(EA_Color2, mDepthRT);
		mGeoFrameBuffer->attachRenderDepthStencilView(mGeoDepthStencil);

		mLightFrameBuffer->attachRenderColorView(EA_Color0, mLightAccumRT);
		
		mMergeFrameBuffer->attachRenderColorView(EA_Color0, mMergeRT);
	}

	void DDemoRenderWorkshop::DeferLayer::prepareGBufferStage()
	{
		mGeoFrameBuffer->clear(CBM_Color|CBM_Depth|CBM_Stencil, DColor::ZERO, 1.0f, 0);
	}

	void DDemoRenderWorkshop::DeferLayer::prepareLightStage()
	{
		mLightFrameBuffer->clear(CBM_Color, DColor::ZERO, 1.0f, 0);
	}

	void DDemoRenderWorkshop::DeferLayer::prepareMergeStage()
	{
		mMergeFrameBuffer->clear(CBM_Color, DColor::ZERO, 1.0f, 0);
	}

	DFrameBuffer* DDemoRenderWorkshop::DeferLayer::getGeoStageFrameBuffer()
	{
		return mGeoFrameBuffer;
	}

	DFrameBuffer* DDemoRenderWorkshop::DeferLayer::getLightStageFrameBuffer()
	{
		return mLightFrameBuffer;
	}

	DFrameBuffer* DDemoRenderWorkshop::DeferLayer::getMergeStageFrameBuffer()
	{
		return mMergeFrameBuffer;
	}

	DRenderColorView* DDemoRenderWorkshop::DeferLayer::getAlbedoMap()
	{
		return mAlbedoRT;
	}

	DRenderColorView* DDemoRenderWorkshop::DeferLayer::getDepthMap()
	{
		return mDepthRT;
	}

	DRenderColorView* DDemoRenderWorkshop::DeferLayer::getViewSpaceNormalMap()
	{
		return mViewSpaceNormalRT;
	}

	DRenderColorView* DDemoRenderWorkshop::DeferLayer::getLightAccumulationMap()
	{
		return mLightAccumRT;
	}

	DRenderColorView* DDemoRenderWorkshop::DeferLayer::getMergedColorMap()
	{
		return mMergeRT;
	}


	void DDemoRenderWorkshop::DeferLayer::shutdown()
	{
		DRenderResourceManager* rm = DRenderResourceManager::getSingletonPtr();
		if (mGeoFrameBuffer != NULL)
		{
			rm->destroyFrameBuffer(mGeoFrameBuffer);
			mGeoFrameBuffer = NULL;
		}
		if (mLightFrameBuffer != NULL)
		{
			rm->destroyFrameBuffer(mLightFrameBuffer);
			mLightFrameBuffer = NULL;
		}
		if (mMergeFrameBuffer != NULL)
		{
			rm->destroyFrameBuffer(mMergeFrameBuffer);
			mMergeFrameBuffer = NULL;
		}
		
		if (mAlbedoRT != NULL)
		{
			rm->destroyRenderColorView(mAlbedoRT);
			mAlbedoRT = NULL;
		}
		if (mDepthRT != NULL)
		{
			rm->destroyRenderColorView(mDepthRT);
			mDepthRT = NULL;
		}
		if (mViewSpaceNormalRT != NULL)
		{
			rm->destroyRenderColorView(mViewSpaceNormalRT);
			mViewSpaceNormalRT = NULL;
		}
		if (mGeoDepthStencil != NULL)
		{
			rm->destroyRenderDepthStencilView(mGeoDepthStencil);
			mGeoDepthStencil = NULL;
		}
		
		if (mLightAccumRT != NULL)
		{
			rm->destroyRenderColorView(mLightAccumRT);
			mLightAccumRT = NULL;
		}
		if (mMergeRT != NULL)
		{
			rm->destroyRenderColorView(mMergeRT);
			mMergeRT = NULL;
		}

	}

}