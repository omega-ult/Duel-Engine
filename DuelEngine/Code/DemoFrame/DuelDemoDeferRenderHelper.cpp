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
	DUEL_IMPLEMENT_RTTI_1(DDemoSpotlightHelper, DRenderable);

	DDemoAmbientLightHelper::DDemoAmbientLightHelper() :
		mAmbientColor(DColor::ZERO)
	{
		DResourceGroupManager* rg = DResourceGroupManager::getSingletonPtr();
		DRenderResourceManager* re = DRenderResourceManager::getSingletonPtr(); 
		DResourcePtr renderEffect = rg->getResouceManager("RenderEffect")->getResource("_BasicShaderPack", "Demo_RenderWorkshop.dre");
		renderEffect->touch();
		mAmbientLightTech = renderEffect->getAs<DRenderEffect>()->getTechnique("DeferAmbientLight");
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
		if (stage == RS_DeferAmbientLight)
		{
			return mAmbientLightTech.get();
		}
		return NULL;
	}

	void DDemoAmbientLightHelper::updateCustomGpuParameter( DShaderObject* so )
	{
		if (so->getPassName() == "DeferAmbientLight_p0")
		{
			so->getPixelProgramParameters()->setValue("lightDiffuseColor", mAmbientColor);
		}
	}

	void DDemoAmbientLightHelper::setAmbientLightParameter( DLightSource* light )
	{
		if (light)
		{
			mAmbientColor = light->getDiffuseColor();
		}
		else
		{
			mAmbientColor = DColor::ZERO;
		}
	}



	DDemoPointLightHelper::DDemoPointLightHelper() :
		mLightPos(DVector3::ZERO),
		mLightRadius(0.0f),
		mDiffuseColor(DColor::ZERO),
		mSpecularColor(DColor::ZERO)
	{
		DResourceGroupManager* rg = DResourceGroupManager::getSingletonPtr();
		DRenderResourceManager* re = DRenderResourceManager::getSingletonPtr(); 
		DResourcePtr renderEffect = rg->getResouceManager("RenderEffect")->getResource("_BasicShaderPack", "Demo_RenderWorkshop.dre");
		renderEffect->touch();
		mPointLightTech = renderEffect->getAs<DRenderEffect>()->getTechnique("DeferPointLight");

		mRenderLayout = re->createRenderLayout();

		DTextureSampler depthSampler;
		//...texture sampler.
		mDepthSampler = re->createTextureSamplerObject(depthSampler);
		DTextureSampler vnormSampler;
		mVNormalSampler = re->createTextureSamplerObject(vnormSampler);

		DResourcePtr meshModel = rg->getResouceManager("Mesh")->getResource("_BasicMediaPack", "M_PointLightSphereModel.dm");
		if (meshModel != NULL)
		{
			DSubMeshPtr sbmesh = meshModel->getAs<DMesh>()->getSubMesh("PointLightSphereModel");
			if (sbmesh != NULL)
			{
				mRenderLayout->setTopologyType(PT_TriangleList);
				mRenderLayout->setVertexData(DVertexData(sbmesh->getVertexStreams(), sbmesh->getVertexDeclaration()));
				mRenderLayout->setIndexData(DIndexData(sbmesh->getIndices()));
				mRenderLayout->seal();
			}
		}
	}

	DRenderTechnique* DDemoPointLightHelper::getRenderTechnique(uint32 stage, DCamera* cam, LightIterator li)
	{
		if (stage == RS_DeferPointLight)
		{
			return mPointLightTech.get();
		}
		return NULL;
	}

	void DDemoPointLightHelper::updateCustomGpuParameter( DShaderObject* so )
	{
		if (so->getPassName() == "DeferPointLight_p0")
		{
			so->getPixelProgramParameters()->setValue("depthTexture", mDepthTexture);
			so->getPixelProgramParameters()->setValue("depthTextureSamp", mDepthSampler);
			so->getPixelProgramParameters()->setValue("normalTexture", mVNormalTexture);
			so->getPixelProgramParameters()->setValue("normalTextureSamp", mVNormalSampler);

			so->getPixelProgramParameters()->setValue("lightPos", mLightPos);
			so->getPixelProgramParameters()->setValue("lightDiffuseColor", mDiffuseColor);
			so->getPixelProgramParameters()->setValue("lightRadius", mLightRadius);
		}
		mVNormalTexture.reset();
		mDepthTexture.reset();
	}


	void DDemoPointLightHelper::setPointLightParameter( DLightSource* light )
	{
		DPointLight* pLight = light->getAs<DPointLight>(false);
		if (pLight)
		{
			mLightPos = pLight->getPosition();
			mLightRadius = pLight->getRadius();
			mDiffuseColor = pLight->getDiffuseColor();
			mSpecularColor = pLight->getSpecularColor();
		}
		else
		{
			mLightPos = DVector3::ZERO;
			mLightRadius = 0.0f;
			mDiffuseColor = DColor::ZERO;
			mSpecularColor = DColor::ZERO;
		}
	}

	void DDemoPointLightHelper::setViewSpaceNormaTexture( DGpuTextureConstantPtr tex )
	{
		mVNormalTexture = tex;
	}

	void DDemoPointLightHelper::setDepthTexture( DGpuTextureConstantPtr tex )
	{
		mDepthTexture = tex;
	}






	DDemoDirectionalLightHelper::DDemoDirectionalLightHelper()
	{
		DResourceGroupManager* rg = DResourceGroupManager::getSingletonPtr();
		DRenderResourceManager* re = DRenderResourceManager::getSingletonPtr(); 
		DResourcePtr renderEffect = rg->getResouceManager("RenderEffect")->getResource("_BasicShaderPack", "Demo_RenderWorkshop.dre");
		renderEffect->touch();
		mDirectionalLightTech = renderEffect->getAs<DRenderEffect>()->getTechnique("DeferDirectionalLight");

		mRenderLayout = re->createRenderLayout();

		DResourcePtr meshModel = rg->getResouceManager("Mesh")->getResource("_BasicMediaPack", "M_DirectionalLightCylinderModel.dm");
		if (meshModel != NULL)
		{
			DSubMeshPtr sbmesh = meshModel->getAs<DMesh>()->getSubMesh("DirectionalLightCylinderModel");
			if (sbmesh != NULL)
			{
				mRenderLayout->setTopologyType(PT_TriangleList);
				mRenderLayout->setVertexData(DVertexData(sbmesh->getVertexStreams(), sbmesh->getVertexDeclaration()));
				mRenderLayout->setIndexData(DIndexData(sbmesh->getIndices()));
				mRenderLayout->seal();
			}
		}
	}

	DRenderTechnique* DDemoDirectionalLightHelper::getRenderTechnique(uint32 stage, DCamera* cam, LightIterator li)
	{
		if (stage == RS_DeferDirectionalLight)
		{
			return mDirectionalLightTech.get();
		}
		return NULL;
	}

	void DDemoDirectionalLightHelper::updateCustomGpuParameter( DShaderObject* so )
	{

	}

	void DDemoDirectionalLightHelper::setDirectionalLightParameter( DLightSource* light )
	{

	}




	DDemoSpotlightHelper::DDemoSpotlightHelper()
	{

	}

	DRenderTechnique* DDemoSpotlightHelper::getRenderTechnique(uint32 stage, DCamera* cam, LightIterator li)
	{
		return NULL;
	}

	void DDemoSpotlightHelper::updateCustomGpuParameter( DShaderObject* so )
	{

	}

	void DDemoSpotlightHelper::setSpotlightParameter( DLightSource* light )
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
			so->getPixelProgramParameters()->setValue("albedoTexture", mAlbedoTex);
			so->getPixelProgramParameters()->setValue("albedoTextureSamp", mTextureSampler);
			so->getPixelProgramParameters()->setValue("lightAccumTexture", mLightAccumTex);
			so->getPixelProgramParameters()->setValue("lightAccumTextureSamp", mTextureSampler);
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