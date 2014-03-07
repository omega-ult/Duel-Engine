//  [8/14/2013 OMEGA] created

#include "DuelGUICommon.h"
#include "Duel.h"
#include "DuelGUI.h"
#include "DuelGUIMathTool.h"
#include "DuelRenderable.h"
#include "DuelGUIRenderDelegate.h"
#include "DuelGUI2DButtonRD.h"
#include "DuelGUI2DButton.h"
#include "DuelResourceGroupManager.h"


namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DG2DButtonRenderable, DRenderable);
	DUEL_IMPLEMENT_RTTI_1(DG2DButtonRD, DGRenderDelegate);

	DG2DButtonRenderable::DG2DButtonRenderable()
	{
		mRenderEffect = DResourceGroupManager::getSingleton().
			getResouceManager("RenderEffect")->getResource("UI_Basic.dre", "_BasicShaderPack");
		mRenderEffect->touch();

		mTexture = DResourceGroupManager::getSingleton().
			getResouceManager("Texture")->getResource("T_debug_default.dds", "_BasicMediaPack");
		mTexture->touch();

		mbDeferRender = false;
		mbForwardRender = true;
		DRenderResourceManager* hm = DRenderResourceManager::getSingletonPtr();
		mRenderLayout = hm->createRenderLayout();
		mRenderLayout->setTopologyType(PT_TriangleList);
		mVDecl = hm->createVertexDeclaration();
		mVDecl->addElement(0, 0, VET_Float3, VES_Position);
		mVDecl->addElement(0, sizeof(DReal)*3, VET_Float2, VES_TexCoord);
		mVDecl->sort();
		size_t vertSize = mVDecl->getVertexSize(0);
		DVertexBufferPtr vb = hm->createVetexBuffer(vertSize, 4, HBU_Static, false);
		// a quad.
		float data[] =
		{ 
			-1.0f, 1.0f, 0.0f,  0.0f, 0.0f,
			1.0f,  1.0f, 0.0f,  1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,  1.0f, 1.0f,
			-1.0f,-1.0f, 0.0f,  0.0f, 1.0f
		};

		vb->writeData(0, vb->getSize(), data, true);
		mIndices = hm->createIndexBuffer(IT_16Bit, 6, HBU_Static, false);
		int16 idata[] =
		{
			0, 3, 2,  0, 2, 1
		};
		mIndices->writeData(0, mIndices->getSize(), idata, true);
		mVStream.setStream(0, vb);
		mRenderLayout->setIndexBuffer(mIndices.get());
		mRenderLayout->setVertexData(DVertexData(&mVStream, mVDecl.get()));
		mRenderLayout->seal();
	}

	void DG2DButtonRenderable::update( DG2DButton* parent )
	{
		mTransform = DMatrix4::IDENTITY;
		IRect widRect = parent->getGeometry();
		DGSize windSize = parent->getHostWindowSize();

		DVector2 scrPos = parent->getPosition();
		widRect.left = (int32)scrPos.x; 
		widRect.top  = (int32)scrPos.y;

		mTransform = DGGUIMathTool::getScreenSpaceTransform(widRect, windSize);
	}

	DRenderLayout* DG2DButtonRenderable::getRenderLayout()
	{
		return mRenderLayout.get();
	}

	DRenderTechnique* DG2DButtonRenderable::getForwardRenderTechnique()
	{
		return mRenderEffect->getAs<DRenderEffect>()->getTechnique("plain_texture_t0").get();

	}

	Duel::DReal DG2DButtonRenderable::getSquaredViewDepthToCamera( const DCamera* cam )
	{
		return 0.0f;
	}

	void DG2DButtonRenderable::getWorldTransform( DMatrix4& outMat )
	{
		outMat = mTransform;
	}

	void DG2DButtonRenderable::updateCustomGpuParameter()
	{
		DRenderPass* pass = mRenderEffect->getAs<DRenderEffect>()->getTechnique("plain_texture_t0")->getPass(0).get();
		pass->getVertexProgram()->getGpuParameters()->setValue("gtransform", mTransform);
		pass->getPixelProgram()->getGpuParameters()->setValue("texUnit", mTexture->getAs<DTexture>()->getGpuTexutureConstant());

	}

	DG2DButtonRD::DG2DButtonRD( DG2DButton* parent ) :
		mParent(parent)
	{
		
	}


	void DG2DButtonRD::update()
	{
		mRenderable.update(mParent);
	}


	void DG2DButtonRD::applyToRenderQueue( DRenderQueue* queue, uint32 groupID )
	{
		queue->addRenderale(groupID, &mRenderable);
	}



}