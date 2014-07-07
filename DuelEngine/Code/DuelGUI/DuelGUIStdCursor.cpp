//  [11/11/2013 OMEGA] created

#include "DuelGUICommon.h"
#include "Duel.h"
#include "DuelGUIStdCursor.h"
#include "DuelGUIMathTool.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DGStdCursorPictureLayer, DRenderable);
	DUEL_IMPLEMENT_RTTI_1(DGStdCursorRD, DGRenderDelegate);
	DUEL_IMPLEMENT_RTTI_1(DGStdCursor, DGCursor);


	DGStdCursorPictureLayer::DGStdCursorPictureLayer(DGCursor* hostCursor) :
		mHostCursor(hostCursor)
	{

		mRenderEffect = DResourceGroupManager::getSingleton().
			getResouceManager("RenderEffect")->getResource("_BasicShaderPack", "UI_Basic.dre");
		mRenderEffect->touch();

		mTexture = DResourceGroupManager::getSingleton().
			getResouceManager("Texture")->getResource("_BasicMediaPack", "T_Std_Cursor.dds");
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

	}

	DRenderTechnique* DGStdCursorPictureLayer::getRenderTechnique(uint32 stage, DCamera* cam, LightIterator li)
	{
		 if ( stage == RS_Forward)
		 {
			 if (mRenderEffect->isLoaded())
			 {
				 return mRenderEffect->getAs<DRenderEffect>()->getTechnique("UI_StdCursor_t0").get();
			 }
		 }
		 return NULL;
	}

	void DGStdCursorPictureLayer::updateCustomGpuParameter(DShaderObject* so)
	{
		if (!so->isValid())
		{
			return;
		}
		// we assume the so is the only so named "UI_StdCursor_t0_p0"
		DMatrix4 vertTransform;
		DMatrix3 uvTransform = DMatrix3::IDENTITY;
		DGCursor::CursorAction action;
		DVector2 posOffset(0.0f, 0.0f);
		DReal cw = mHostCursor->getSize().getWidth() * 0.5f;
		DReal ch = mHostCursor->getSize().getHeight() * 0.5f;
		bool makeOffset = false;
		switch (mHostCursor->getCursorAction())
		{
		case Duel::DGCursor::CA_Idle:
			action = DGCursor::CA_Idle;
			makeOffset = true;
			break;
		case Duel::DGCursor::CA_Busy:
			action = DGCursor::CA_Busy;
			makeOffset = true;
			break;
		case Duel::DGCursor::CA_Pressed:
			action = DGCursor::CA_Pressed;
			makeOffset = true;
			break;
		case Duel::DGCursor::CA_Drag:
			action = DGCursor::CA_Drag;
			makeOffset = true;
			break;
		case Duel::DGCursor::CA_ScaleHorizontal:
			action = DGCursor::CA_ScaleHorizontal;
			break;
		case Duel::DGCursor::CA_ScaleVertical:
			action = DGCursor::CA_ScaleVertical;
			break;
		case Duel::DGCursor::CA_Scale_LeftCorner:
			action = DGCursor::CA_Scale_LeftCorner;
			break;
		case Duel::DGCursor::CA_Scale_RightCorner:
			action = DGCursor::CA_Scale_RightCorner;
			break;
		case Duel::DGCursor::CA_Link:
			action = DGCursor::CA_Link;
			makeOffset = true;
			break;
		case Duel::DGCursor::CA_Scroll:
			action = DGCursor::CA_Scroll;
			break;
		case Duel::DGCursor::CA_Edit:
			action = DGCursor::CA_Edit;
			break;
		case Duel::DGCursor::CA_Invalid:
			action = DGCursor::CA_Invalid;
			break;
		default:
			action = DGCursor::CA_Idle;
			makeOffset = true;
			break;
		}
		if (makeOffset)
		{
			posOffset.x += cw;
			posOffset.y -= ch;
		}
		// 因为标准ui的HotPoint根据不同的动作会有不同的坐标, 因此需要重新计算.
		DVector2 pos = mHostCursor->getPointInScreen();
		pos += posOffset;
		DGSize winSize((DReal)mHostCursor->getHostWindow()->getWidth(),
			(DReal)mHostCursor->getHostWindow()->getHeight());
		vertTransform = DGGUIMathTool::getScreenSpaceTransform(pos, mHostCursor->getSize(), winSize);
		so->getVertexProgramParameters()->setValue("vertTransform", vertTransform);

		// 处理UV, 反正标准Cursor图里只有12个图标. 直接计算.
		DReal uvXScale = 1.0f / 12.0f;
		uvTransform[0][0] = uvXScale;
		uint32 iconId = (uint32)action;
		DReal uvXOffset = uvXScale * iconId;
		// 列主序, 列主存储, x 位移.
		uvTransform[2][0] = uvXOffset;
		so->getVertexProgramParameters()->setValue("uvTransform", uvTransform);
	
		so->getPixelProgramParameters()->setValue("texUnit", mTexture->getAs<DTexture>()->getGpuTexutureConstant());

	}



	DGStdCursorRD::DGStdCursorRD( DGCursor* hostCursor ) :
		mHostCursor(hostCursor)
	{
		assert(mHostCursor != NULL);
		mPicLayer = new DGStdCursorPictureLayer(mHostCursor);
	}

	DGStdCursorRD::~DGStdCursorRD()
	{
		delete mPicLayer;
	}

	void DGStdCursorRD::applyToRenderQueue( DRenderQueue* queue, uint32 groupID )
	{
		queue->addRenderable(groupID, mPicLayer);
	}

	DGWidget* DGStdCursorRD::getParent()
	{
		return NULL;
	}

	DGStdCursor::DGStdCursor()
	{
		mStdRenderDelegate = new DGStdCursorRD(this);
		mRenderDelegate = mStdRenderDelegate;
	}

	DGStdCursor::~DGStdCursor()
	{
		delete mStdRenderDelegate;
	}


}