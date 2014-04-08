//  [4/7/2014 OMEGA] created
#include "pch.h"
#include "Duel.h"
#include "DuelRenderEffect.h"
#include "DuelRenderResourceManager.h"
#include "DuelResourceGroupManager.h"
#include "DuelDemoDeferRenderHelper.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DDemoDeferHelper, DRenderable);
	DUEL_IMPLEMENT_RTTI_1(DDemoAmbientLightHelper, DRenderable);
	DUEL_IMPLEMENT_RTTI_1(DDemoPointLightHelper, DRenderable);
	DUEL_IMPLEMENT_RTTI_1(DDemoDirectionalLightHelper, DRenderable);
	DUEL_IMPLEMENT_RTTI_1(DDemoSpotLightHelper, DRenderable);

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

	DRenderTechnique* DDemoAmbientLightHelper::getRenderTechnique( uint32 stage )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	DDemoPointLightHelper::DDemoPointLightHelper()
	{

	}

	DRenderTechnique* DDemoPointLightHelper::getRenderTechnique( uint32 stage )
	{
		return NULL;
	}



	DRenderTechnique* DDemoDirectionalLightHelper::getRenderTechnique( uint32 stage )
	{
		return NULL;
	}

	DDemoDirectionalLightHelper::DDemoDirectionalLightHelper()
	{
	}


	DRenderTechnique* DDemoSpotLightHelper::getRenderTechnique( uint32 stage )
	{
		return NULL;
	}

	DDemoSpotLightHelper::DDemoSpotLightHelper()
	{

	}

}