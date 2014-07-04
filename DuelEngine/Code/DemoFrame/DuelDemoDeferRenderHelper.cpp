//  [4/7/2014 OMEGA] created
#include "pch.h"
#include "Duel.h"
#include "DuelRenderEffect.h"
#include "DuelRenderResourceManager.h"
#include "DuelResourceGroupManager.h"
#include "DuelDemoDeferRenderHelper.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DDemoMergeHelper, DRenderable);
	DUEL_IMPLEMENT_RTTI_1(DDemoAmbientLightHelper, DRenderable);
	DUEL_IMPLEMENT_RTTI_1(DDemoPointLightHelper, DRenderable);
	DUEL_IMPLEMENT_RTTI_1(DDemoDirectionalLightHelper, DRenderable);
	DUEL_IMPLEMENT_RTTI_1(DDemoSpotLightHelper, DRenderable);

	DDemoAmbientLightHelper::DDemoAmbientLightHelper()
	{
		DResourceGroupManager* rg = DResourceGroupManager::getSingletonPtr();
		DRenderResourceManager* re = DRenderResourceManager::getSingletonPtr(); 
		// 		DResourcePtr renderEffect = rg->getResouceManager("RenderEffect")->getResource("_BasicShaderPack", "Demo_RenderWorkshop.dre");
		// 		renderEffect->touch();
		// 		mCopyTexTech = renderEffect->getAs<DRenderEffect>()->getTechnique("DemoRenderWorkshop_CopyTexture");
		// 		mComposeTech = renderEffect->getAs<DRenderEffect>()->getTechnique("DemoRenderWorkshop_DeferCompose");
		// 		mGBufferTech = renderEffect->getAs<DRenderEffect>()->getTechnique("DemoRenderWorkshop_DeferGBuffer");
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

	DRenderTechnique* DDemoAmbientLightHelper::getRenderTechnique(uint32 stage, DCamera* cam, LightIterator li)
	{
		throw std::exception("The method or operation is not implemented.");
	}

	DDemoPointLightHelper::DDemoPointLightHelper()
	{

	}

	DRenderTechnique* DDemoPointLightHelper::getRenderTechnique(uint32 stage, DCamera* cam, LightIterator li)
	{
		return NULL;
	}



	DRenderTechnique* DDemoDirectionalLightHelper::getRenderTechnique(uint32 stage, DCamera* cam, LightIterator li)
	{
		return NULL;
	}

	DDemoDirectionalLightHelper::DDemoDirectionalLightHelper()
	{
	}


	DRenderTechnique* DDemoSpotLightHelper::getRenderTechnique(uint32 stage, DCamera* cam, LightIterator li)
	{
		return NULL;
	}

	DDemoSpotLightHelper::DDemoSpotLightHelper()
	{

	}

	DDemoMergeHelper::DDemoMergeHelper()
	{
		DResourceGroupManager* rg = DResourceGroupManager::getSingletonPtr();
		DRenderResourceManager* re = DRenderResourceManager::getSingletonPtr(); 
		DResourcePtr renderEffect = rg->getResouceManager("RenderEffect")->getResource("_BasicShaderPack", "Demo_RenderWorkshop.dre");
		renderEffect->touch();
		mMergeTech = renderEffect->getAs<DRenderEffect>()->getTechnique("DeferMerge");
		mScrnTransTech = renderEffect->getAs<DRenderEffect>()->getTechnique("ScreenTransfer");
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

		mTextureSampler = re->createTextureSamplerObject(DTextureSampler());
	}

	DRenderTechnique* DDemoMergeHelper::getRenderTechnique(uint32 stage, DCamera* cam, LightIterator li)
	{
		if (stage == RS_DeferMerge)
		{
			return mMergeTech.get();
		}
		else if (stage == RS_ScreenTransfer)
		{
			return mScrnTransTech.get();
		}
		return NULL;
	}

	void DDemoMergeHelper::updateCustomGpuParameter( DShaderObject* so )
	{
		if (so->getPassName() == "DeferMerge_p0")
		{
			so->getPixelProgramParameters()->setValue("albedoMap", mAlbedoTex);
			so->getPixelProgramParameters()->setValue("albedoMapSamp", mTextureSampler);
			so->getPixelProgramParameters()->setValue("lightAccumMap", mLightAccumTex);
			so->getPixelProgramParameters()->setValue("lightAccumMapSamp", mTextureSampler);
		}
		if (so->getPassName() == "ScreenTransfer_p0")
		{
			so->getPixelProgramParameters()->setValue("srcTex", mTransSrc);
			so->getPixelProgramParameters()->setValue("srcTexSamp", mTextureSampler);
		}
	}

	void DDemoMergeHelper::setInputAlbedo( DGpuTextureConstantPtr tex )
	{
		mAlbedoTex = tex;
	}

	void DDemoMergeHelper::setInputLightAccumulationMap( DGpuTextureConstantPtr tex )
	{
		mLightAccumTex = tex;
	}

	void DDemoMergeHelper::setInputDepth( DGpuTextureConstantPtr tex )
	{
		mDepthTex = tex;
	}

	void DDemoMergeHelper::setTransferSource( DGpuTextureConstantPtr tex )
	{
		mTransSrc = tex;
	}


}