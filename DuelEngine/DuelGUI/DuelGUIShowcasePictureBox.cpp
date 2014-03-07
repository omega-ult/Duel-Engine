//  [11/21/2013 OMEGA] created

#include "DuelGUICommon.h"
#include "Duel.h"
#include "DuelGUIShowcasePictureBox.h"
#include "DuelGUIHotSpot.h"
#include "DuelRenderable.h"
#include "DuelGUIMathTool.h"
#include "DuelGUIManager.h"


namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DGSPictureBoxHS, DGHotspot);
	DUEL_IMPLEMENT_RTTI_1(DGSPictureBoxRD, DGRenderDelegate);
	DUEL_IMPLEMENT_RTTI_1(DGSPictureBox, DGWidget);

	DGSPictureBoxHS::DGSPictureBoxHS( DGSPictureBox* targetBox ) :
		mTargetBox(targetBox)
	{

	}

	void DGSPictureBoxHS::update()
	{
		// 在这里计算新的平面.
		DVector3 origin(0.0f, 0.0f, 0.0f);
		DVector3 normalPnt(0.0f, 0.0f, -1.0f);
		// calculate transformed origin, and normal point
		DMatrix4 debugMat = mTargetBox->getTransformNode()->getInheritedTransfrom();
		origin = debugMat * DVector4(origin, 1.0f);
		normalPnt = debugMat * DVector4(normalPnt, 1.0f);
		mWidgetPlane = DPlane(normalPnt - origin, origin);
	}

	bool DGSPictureBoxHS::response( const DEventBase* e )
	{
// 		if (e->getAs<DGUIMouseEvent>(false) != NULL)
// 		{					
// 			DGUIManager* gm = mTargetBox->getRootManager();
// 
// 			if (gm == NULL)
// 			{
// 				return false;
// 			}
// 			const DGUIMouseEvent* ge = e->getAs<DGUIMouseEvent>();
// 			DRay ray = ge->ray;
// 			DReal dist = 0.0f;
// 			bool bIntersected = false;
// 			DVector3 pInWidget;
// 			if (DMath::intersect(ray, mWidgetPlane, &dist))
// 			{
// 				pInWidget = ray.getPoint(dist);		
// 				// calculate the position in the widget's space.
// 				pInWidget = mTargetBox->getTransformNode()->getInheritedTransfrom().getInverse() * pInWidget;		
// 				// 这里没有做那个啥.. clip的计算.
// 				if (DMath::Abs(pInWidget.x) < mTargetBox->getWidth()/2 &&
// 					DMath::Abs(pInWidget.y) < mTargetBox->getHeight()/2)
// 				{	
// 					bIntersected = true;
// 				}	
// 			}
// 			if (bIntersected)
// 			{
// 
// 				gm->requestCursorAction(DGCursor::CA_Drag);
// 				if (ge->buttonID == MBID_Left && ge->action == IEA_Pressed)
// 				{
// 					mTargetBox->getTransformNode()->yaw(DMath::HALF_PI/20, TS_Parent);
// //						mTargetBox->getTransformNode()->translateZ(DMath::HALF_PI);
// 
// 					if (!mTargetBox->isFocused())
// 					{
// 						gm->pushFocusWidget(mTargetBox);
// 						std::cout << "focused" << std::endl;
// 					}
// 					return true;
// 				}
// 			}
// 			else
// 			{
// 				if (mTargetBox->isFocused())
// 				{
// 					gm->popFocusWidget();
// 					std::cout << "unfocused" << std::endl;
// 				}
// 			}
// 		}
		return false;
	}

	DGWidget* DGSPictureBoxHS::getParent()
	{
		return mTargetBox;
	}



	DGSPictureBoxRD::DGSPictureBoxRD( DGSPictureBox* targetBox ) :
		mTargetBox(targetBox),
		mbClipped(false)
	{
		mPicLayer = new DGSPictureLayer();
	}

	void DGSPictureBoxRD::applyToRenderQueue( DRenderQueue* queue, uint32 groupID )
	{
		if (mTargetBox)
		{
			if (!mbClipped)
			{
				queue->addRenderale(groupID, mPicLayer);
			}
		}
	}

	void DGSPictureBoxRD::update()
	{
		if (mTargetBox)
		{
			mPicLayer->setWorldTransform(mTargetBox->getTransformNode()->getInheritedTransfrom());
			DReal boxW = mTargetBox->getSize().getWidth();
			DReal boxH = mTargetBox->getSize().getHeight();
			mPicLayer->setRenderSize(DGSize(boxW, boxH));
			DReal cL, cR, cT, cB;
			cL = cR = cT = cB = 0.0f;
			mTargetBox->getClippedDistance(cL, cR, cT, cB);
			mPicLayer->setClipping(cL, cR, cT, cB);
			if (cL + cR > boxW || cT + cB > boxH)
			{
				mbClipped = true;
				// no need further calculation.
				return;
			}
			else
			{
				mbClipped = false;
			}

			// 确保贴图有效, 如果贴图无效, 使用defaultTexture然后重新设置
			DTexture* tex = NULL;
			bool bResetTex = false;
			if (mTargetBox->getTexture() != NULL)
			{
				tex = mTargetBox->getTexture()->getAs<DTexture>(false);
				if (tex == NULL)
				{
					bResetTex = true;
				}
				mPicLayer->setTexture(mTargetBox->getTexture());
			}
			else
			{
				bResetTex = true;
			}

			if (bResetTex)
			{
				DResourcePtr defaultTex = DResourceGroupManager::getSingleton().
					getResouceManager("Texture")->getResource("_BasicMediaPack", "T_debug_default.dds");
				defaultTex->touch();
				mPicLayer->setTexture(defaultTex);
				tex = defaultTex->getAs<DTexture>();
				mTargetBox->setScalable(true);
			}

			mPicLayer->setAlignment(mTargetBox->getAlignment());
			mPicLayer->setScalable(mTargetBox->isScalable());
		}
	}

	DGWidget* DGSPictureBoxRD::getParent()
	{
		return mTargetBox;
	}


	DGSPictureBox::DGSPictureBox( DGWidget* parent ) :
		DGWidget(parent),
		mAlign(AF_HCenter | AF_VCenter),
		mbScalable(false)
	{
		mHS = new DGSPictureBoxHS(this);
		mRD = new DGSPictureBoxRD(this);
		mDefaultHotspot = mHS;
		mDefaultRenderDelegate = mRD;
	}

	DGSPictureBox::~DGSPictureBox()
	{
		delete mRD;
		delete mHS;
	}

	void DGSPictureBox::setTexture( const DString& texName, const DString& grpName )
	{
		DResourceManager* resMgr = DResourceGroupManager::getSingleton().getResouceManager("Texture");
		if (resMgr)
		{
			mTex = resMgr->getResource(grpName, texName);
			if (mTex)
			{
				mTex->touch();
			}
		}
	}

	void DGSPictureBox::setTexture( DResourcePtr tex )
	{
		mTex = tex;
	}

	DResourcePtr DGSPictureBox::getTexture()
	{
		return mTex;
	}

	void DGSPictureBox::setAlignment( DGAlignment align)
	{
		mAlign = align;
	}

	DGAlignment DGSPictureBox::getAlignment()
	{
		return mAlign;
	}

	void DGSPictureBox::setScalable( bool flag )
	{
		mbScalable = flag;
	}

	bool DGSPictureBox::isScalable()
	{
		return mbScalable;
	}


}