//  [11/26/2013 OMEGA] created

#include "DuelGUICommon.h"
#include "Duel.h"
#include "DuelGUISize.h"
#include "DuelGUIAlignment.h"
#include "DuelGUIMathTool.h"
#include "DuelGUIRenderDelegate.h"
#include "DuelGUIShowcasePictureLayer.h"

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(DGSPictureLayer, DRenderable);


	DGSPictureLayer::DGSPictureLayer(DGRenderDelegate* parent) :
		mParent(parent),
		mRenderSize(0.0f, 0.0f),
		mClipLeft(0.0f),
		mClipRight(0.0f),
		mClipTop(0.0f),
		mClipBottom(0.0f),
		mAlignment(0),
		mbScalable(false),
		mbDirty(true)
	{
		mWorldTransform = DMatrix4::IDENTITY;
		mQuadTransform = DMatrix4::IDENTITY;
		mUVTransform = DMatrix3::IDENTITY;

		mRenderEffect = DResourceGroupManager::getSingleton().
			getResouceManager("RenderEffect")->getResource( "_BasicShaderPack", "UI_Basic.dre");
		mRenderEffect->touch();

		mTexture = DResourceGroupManager::getSingleton().
			getResouceManager("Texture")->getResource("_BasicMediaPack", "T_debug_default.dds");
		mTexture->touch();

		DRenderResourceManager* hm = DRenderResourceManager::getSingletonPtr();
		mRenderLayout = hm->createRenderLayout();
		mRenderLayout->setTopologyType(PT_TriangleList);
		mVDecl.addElement(0, 0, VET_Float3, VES_Position);
		mVDecl.addElement(0, sizeof(DReal)*3, VET_Float2, VES_TexCoord);
		mVDecl.sort();
		size_t vertSize = mVDecl.getVertexSize(0);
		DVertexBufferPtr vb = hm->createVetexBuffer(vertSize, 4, HBU_Static, false);
		// a quad.
		float data[] =
		{ 
			/*vert*/ -1.0f, 1.0f, 0.0f, /*uv*/ 0.0f, 0.0f,
			/*vert*/ 1.0f,  1.0f, 0.0f, /*uv*/ 1.0f, 0.0f,
			/*vert*/ 1.0f, -1.0f, 0.0f, /*uv*/ 1.0f, 1.0f,
			/*vert*/ -1.0f,-1.0f, 0.0f, /*uv*/ 0.0f, 1.0f
		};

		vb->writeData(0, vb->getSize(), data, true);
		mIndices = hm->createIndexBuffer(IT_16Bit, 6, HBU_Static, false);
		int16 idata[] =
		{
			0, 3, 2,  0, 2, 1
		};
		mIndices->writeData(0, mIndices->getSize(), idata, true);
		mVStream.setStream(0, vb);
		mRenderLayout->setIndexData(DIndexData(mIndices));
		mRenderLayout->setVertexData(DVertexData(mVStream, mVDecl));
		mRenderLayout->seal();

		mTexSampler.addressU = TAM_Border;
		mTexSampler.addressV = TAM_Border;
		mTexSampler.addressW = TAM_Border;
		mTexSampler.minFilter = FO_Linear;
		mTexSampler.magFilter = FO_Linear;
		mTexSampler.mipFilter = FO_Linear;
		mTexSampler.borderColor = DColor(0.0f, 0.0f, 0.0f, 0.0f);
	}


	void DGSPictureLayer::setTexture( DResourcePtr tex )
	{
		if (tex->getAs<DTexture>(false) != NULL)
		{
			mTexture = tex;
			markDirty();
		}
	}

	DRenderLayout* DGSPictureLayer::getRenderLayout()
	{
		return mRenderLayout.get();
	}

	DRenderTechnique* DGSPictureLayer::getRenderTechnique(uint32 stage)
	{
		if ( stage == RS_Forward)
		{
			if (mRenderEffect->isLoaded())
			{
				return mRenderEffect->getAs<DRenderEffect>()->getTechnique("UI_ShowcasePictureBox_t0").get();
			}
		}
		return NULL;
	}

	void DGSPictureLayer::updateCustomGpuParameter( DShaderObject* so )
	{
		if (!so->isValid())
		{
			return;
		}
		so->getVertexProgramParameters()->setValue("quadTransform", mQuadTransform);
		so->getVertexProgramParameters()->setValue("uvTransform", mUVTransform);
		if (mTexture)
		{
			DTexture* tex = mTexture->getAs<DTexture>(false);
			if (tex != NULL)
			{
				so->getPixelProgramParameters()->setValue("texUnit", tex->getGpuTexutureConstant());
			}
			else
			{
				so->getPixelProgramParameters()->setValue("texUnit", NULL);
			}				
			so->getPixelProgramParameters()->setValue("texUnit", mTexSampler);

		}
		so->getPixelProgramParameters()->setValue("colorOffset", DColor(0.0f,0.0f,0.0f, 1.0f));

	}

	void DGSPictureLayer::getWorldTransform( DMatrix4& outMat )
	{
		outMat = mWorldTransform;
	}

	void DGSPictureLayer::setWorldTransform( const DMatrix4& mat )
	{
		if (mWorldTransform == mat)
		{
			return;
		}
		mWorldTransform = mat;
		markDirty();
	}

	void DGSPictureLayer::preRender()
	{
		if (mParent)
		{
			mParent->preRender();
		}
		if (!mbDirty)
		{
			return;
		}
		DReal boxW = mRenderSize.getWidth();
		DReal boxH = mRenderSize.getHeight();
		//mPicLayer->setWorldTransform(mTargetBox->getTransformNode()->getInheritedTransfrom());
		mQuadTransform = DMatrix4::IDENTITY;
		// 由于是1单位的框, 缩放到实际大小要乘以宽度.
		mQuadTransform.setScale(DVector3(boxW/2, boxH/2, 0.0f));
		DReal cL, cR, cT, cB;
		cL = cR = cT = cB = 0.0f;
		//mTargetBox->getClippedDistance(cL, cR, cT, cB);
		mQuadTransform = mQuadTransform * 
			DGGUIMathTool::getClippedTransform(mRenderSize, mClipLeft, mClipRight, mClipTop, mClipBottom);


		// 下面是贴图的uv..那啥了.
		mUVTransform = DGGUIMathTool::getClippedUVTransform(mRenderSize, mClipLeft, mClipRight, mClipTop, mClipBottom);
		if (!mbScalable)
		{
			DTexture* tex;
			if (mTexture == NULL)
			{
				// 没有意义.
				return;
			}
			tex = mTexture->getAs<DTexture>(false);
			if (tex == NULL)
			{
				return;
			}
			// 计算整块uv按alignment调整以后的位置.
			// 这里tex总是有效的.
			DReal texW = (DReal)tex->getWidth();
			DReal texH = (DReal)tex->getHeight();
			DReal uvClipL, uvClipR, uvClipT, uvClipB;
			uvClipL = uvClipR = uvClipT = uvClipB = 0.0f;

			if (mAlignment & AF_Left)
			{
				uvClipR = texW - boxW;
			}
			else if (mAlignment & AF_Right)
			{
				uvClipL = texW - boxW;
			}
			else if (mAlignment & AF_HCenter)
			{
				uvClipL = uvClipR = (texW - boxW) / 2;
			}
			if (mAlignment & AF_Top)
			{
				uvClipB = texH - boxH;
			}
			else if (mAlignment & AF_Bottom)
			{
				uvClipT = texH - boxH;
			}
			else if (mAlignment & AF_VCenter)
			{
				uvClipT = uvClipB = (texH - boxH) / 2;
			}
			DMatrix3 texClipToBox = DGGUIMathTool::getClippedUVTransform(
				DGSize(texW, texH), uvClipL, uvClipR, uvClipT, uvClipB);
			mUVTransform = texClipToBox * mUVTransform;
			mbDirty = false;
		}
	}

	void DGSPictureLayer::postRender()
	{
		if (mParent)
		{
			mParent->postRender();
		}
	}


	void DGSPictureLayer::setRenderSize( const DGSize& size )
	{
		if (mRenderSize == size)
		{
			return;
		}
		mRenderSize = size;
		markDirty();
	}

	void DGSPictureLayer::setClipping( DReal clipL, DReal clipR, DReal clipT, DReal clipB )
	{
		if (clipL != mClipLeft || clipR != mClipRight || clipT != mClipTop || clipB != mClipBottom)
		{
			mClipLeft = clipL;
			mClipRight = clipR;
			mClipTop = clipT;
			mClipBottom = clipB;
			markDirty();
		}
	}

	void DGSPictureLayer::setAlignment( DGAlignment align )
	{
		mAlignment = align;
		markDirty();
	}
	

	void DGSPictureLayer::setScalable( bool flag )
	{
		if (mbScalable == flag)
		{
			return;
		}
		mbScalable = flag;
		markDirty();
	}

	void DGSPictureLayer::markDirty()
	{
		mbDirty = true;
	}

}
