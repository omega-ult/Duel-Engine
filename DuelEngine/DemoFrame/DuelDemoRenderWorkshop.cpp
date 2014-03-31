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
	DUEL_IMPLEMENT_RTTI_1(DDemoDeferHelper, DObject);


	DDemoDeferHelper::DDemoDeferHelper()
	{
		DResourceGroupManager* rg = DResourceGroupManager::getSingletonPtr();
		DRenderResourceManager* re = DRenderResourceManager::getSingletonPtr(); 
		DResourcePtr renderEffect = rg->getResouceManager("RenderEffect")->getResource("_BasicShaderPack", "Demo_RenderWorkshop.dre");
		renderEffect->touch();
		mCopyTexTech = renderEffect->getAs<DRenderEffect>()->getTechnique("DemoRenderWorkshop_CopyTexture");
		mComposeTech = renderEffect->getAs<DRenderEffect>()->getTechnique("DemoRenderWorkshop_DeferCompose");
		mGBufferTech = renderEffect->getAs<DRenderEffect>()->getTechnique("DemoRenderWorkshop_DeferGBuffer");
		mRenderLayout = re->createRenderLayout();
		DVertexDeclaration vd;
		mRenderLayout->setTopologyType(PT_TriangleList);
		vd.addElement(0, 0, VET_Float3, VES_Position);
		vd.addElement(0, sizeof(DReal)*3, VET_Float2, VES_TexCoord);
		vd.sort();
		size_t vertSize = vd.getVertexSize(0);
		DVertexBufferPtr vb = re->createVetexBuffer(vertSize, 4, HBU_Static, false);
		// a quad.
		float data[] =
		{ 
			/*vert*/ -1.0f, 1.0f, 0.0f, /*uv*/ 0.0f, 0.0f,
			/*vert*/ 1.0f,  1.0f, 0.0f, /*uv*/ 1.0f, 0.0f,
			/*vert*/ 1.0f, -1.0f, 0.0f, /*uv*/ 1.0f, 1.0f,
			/*vert*/ -1.0f,-1.0f, 0.0f, /*uv*/ 0.0f, 1.0f
		};
		vb->writeData(0, vb->getSize(), data, true);
		DIndexBufferPtr vi = re->createIndexBuffer(IT_16Bit, 6, HBU_Static, false);
		int16 idata[] =
		{
			0, 3, 2,  0, 2, 1
		};
		vi->writeData(0, vi->getSize(), idata, true);
		DVertexStreams vs;
		vs.setStream(0, vb);
		mRenderLayout->setIndexData(DIndexData(vi));
		mRenderLayout->setVertexData(DVertexData(vs, vd));
		mRenderLayout->seal();
	}
	DRenderLayout* DDemoDeferHelper::getRenderLayout()
	{
		return mRenderLayout.get();
	}

	DRenderTechnique* DDemoDeferHelper::getRenderTechnique( uint32 stage )
	{
		if (stage == RS_ScreenQuadTransfer)
		{
			return mCopyTexTech.get();
		}
		return NULL;
	}

	void DDemoDeferHelper::updateCustomGpuParameter( DShaderObject* so )
	{
		if (so->getPassName() == "DemoRenderWorkshop_ClearGBuffer")
		{

		}
		else if (so->getPassName() == "DemoRenderWorkshop_CopyTexture")
		{
			so->getVertexProgramParameters()->setValue("targetTexture", mInputTex);
		}
		else if (so->getPassName() == "DemoRenderWorkshop_Compose")
		{

		}
	}


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


	void DDemoRenderWorkshop::renderSingleObject( DRenderable* rendObj, DRenderPass* pass )
	{
		DShaderObject* so = pass->getShaderObject().get();
		if (so != NULL && so->isValid())
		{
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
		DRenderQueue::RenderGroupIterator ri = queue->getRenderGroupIterator();;
		// for defer stage.
		DeferLayerMap::iterator gi = mDeferLayerMap.find(mPresentTarget);
		if (gi != mDeferLayerMap.end())
		{
			// we can render defer stage.
			DeferLayer deferlayer = gi->second;
			mRenderSystem->bindFrameBuffer(deferlayer.GBuffer);
			// debug;
			mRenderSystem->bindFrameBuffer(mPresentTarget);
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

	DDemoRenderWorkshop::DeferLayer DDemoRenderWorkshop::createDeferLayer( DFrameBuffer* target )
	{
		DeferLayer ret;
		ret.GBuffer = DRenderResourceManager::getSingleton().createFrameBuffer(target->getWidth(), target->getHeight(), 32);
		ret.GBuffer->enableElement(EA_Color0, PF_A8R8G8B8);
		ret.GBuffer->enableElement(EA_Color1, PF_A8R8G8B8);
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