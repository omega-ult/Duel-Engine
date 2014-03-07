//  [6/22/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelHelperPlaneGrid.h"
#include "DuelColor.h"
#include "DuelMovable.h"
#include "DuelCore.h"
#include "DuelRenderResourceManager.h"
#include "DuelRenderEffect.h"

namespace Duel
{
// 
// 	DUEL_IMPLEMENT_RTTI_1(DHGrid, DRenderable);
// 	DUEL_IMPLEMENT_RTTI_1(DHPlaneGrid, DMovable);
// 
// 
// 	DHGrid::DHGrid( DHPlaneGrid* parent, DReal longtitude, DReal unit ) :
// 		mParent(parent)
// 	{
// 		DResourcePtr p = Duel::DCore::getSingleton().\
// 			getResourceGroupManager()->getResouceManager("RenderEffect")->getResource("colored_wireframe", "BasicShaderPack");
// 		Duel::DRenderResourceManager* hbm = DRenderResourceManager::getSingletonPtr();
// 		mRenderLayout = hbm->createRenderLayout();
// 		mVDeclare.addElement(0, 0, VET_Float3, VES_Position);
// 		mRenderEffect->touch();
// 		mRenderEffect.reset(static_cast<DRenderEffect*>(p.get()));
// 		reset(longtitude, unit);
// 	}
// 
// 	void DHGrid::setColor( const DColor& color )
// 	{
// 		mColor = color;
// 	}
// 
// 	DRenderLayout* DHGrid::getRenderLayout()
// 	{
// 		return mRenderLayout.get();
// 	}
// 
// 	DRenderTechnique* DHGrid::getForwardRenderTechnique()
// 	{
// 		return mRenderEffect->getTechnique(0).get();
// 	}
// 
// 	void DHGrid::updateCustomGpuParameter()
// 	{
// 		DRenderTechnique* tech = getForwardRenderTechnique();
// 		DRenderPassPtr pass = tech->getPass(0);
// 		pass->getPixelProgram()->getGpuParameters()->setValue("color", mColor);
// 	}
// 
// 	void DHGrid::reset( DReal longtitude, DReal unit )
// 	{
// 		mLength = longtitude;
// 		mUnit = unit;
// 
// 		uint32 step = (uint32)(longtitude / unit);
// 		step += 1;
// 
// 		Duel::DRenderResourceManager* hbm = DRenderResourceManager::getSingletonPtr();
// 		// 构造顶点, 
// 		mIndices = hbm->createIndexBuffer(IT_16Bit, 2*(step*step*8 + 4*step/* + 1*/), HBU_Dynamic, true);
// 		mVStreams.clearAllStreams();
// 		DVertexBufferPtr vb = hbm->createVetexBuffer(sizeof(float)*3, (2*step+1)*(2*step+1) /*+ 1*/, HBU_Dynamic, true);
// 		float* pPtr = (float*)vb->lock(HBL_Discard);
// 
// 		float curX = -unit*step;
// 		float finish = -curX;
// 		float curZ = curX;
// 		while ( curX <= finish && curZ <= finish)
// 		{
// 			// x
// 			pPtr[0] = curX;
// 			// y
// 			pPtr[1] = 0.0f;
// 			// z
// 			pPtr[2] = curZ;
// 
// 			// float point + 3;
// 			pPtr += 3;
// 
// 			curX += unit;
// 			if (curX > finish)
// 			{
// 				curX = - finish;
// 				curZ += unit;
// 			}
// 		}
// 		// 最后一个顶点是y轴的.
// // 		pPtr[0] = 0.0f;
// // 		pPtr[1] = unit*2;
// // 		pPtr[2] = 0.0f;
// 		vb->unlock();
// 
// 		uint32 tmp = 0;
// 		// 构建顶点.
// 		uint16 fI = 0, sI = 1;
// 		uint32 col = 2*step+1;
// 		uint16* iPtr = (uint16*)mIndices->lock(HBL_Discard);
// 		for(uint32 ci = 0; ci < col; ++ci)
// 		{
// 			fI = ci * col;
// 			sI = fI + 1;
// 			for (uint32 ri = 0; ri < col-1; ++ri)
// 			{
// 				iPtr[0] = fI;
// 				iPtr[1] = sI;
// 				iPtr += 2;
// 				tmp+=2;
// 				fI++;
// 				sI++;
// 			}
// 		}
// 		for (uint32 ri = 0; ri < col; ++ri)
// 		{
// 			fI = ri;
// 			sI = ri + col;
// 			for (uint32 ci = 0; ci < col-1; ++ci)
// 			{
// 				iPtr[0] = fI;
// 				iPtr[1] = sI;
// 				tmp+=2;
// 				iPtr += 2;
// 				fI += col;
// 				sI += col;
// 			}
// 		}
// // 		iPtr[0] = step*(step*2+1) + step;
// // 		iPtr[1] = (step*2+1)*(step*2+1) + 1;
// 
// 		mIndices->unlock();
// 
// 		mVStreams.setStream(0, vb);
// 		mRenderLayout->setTopologyType(PT_LineList);
// 		mRenderLayout->setIndexData(DIndexData(mIndices));
// 		mRenderLayout->setVertexData(DVertexData(mVStreams, mVDeclare));
// 		mRenderLayout->seal();
// 	}
// 
// 	void DHGrid::getWorldTransform( DMatrix4& outMat )
// 	{
// 		if (mParent->getParentNode() != NULL)
// 		{
// 			outMat = mParent->getParentNode()->getInheritedTransfrom();
// 		}
// 	}
// 
// 	//////////////////////////////////////////////////////////////////////////
// 	DHPlaneGrid::DHPlaneGrid(DMovableFactory* creator, const DString& name, DReal longtitude, DReal unit /*= 10.0f*/ ) :
// 		DMovable(creator, name)
// 	{
// 		mGrid = new DHGrid(this, longtitude, unit);
// 		mGrid->setColor(DColor::WHITE);
// 	}
// 
// 	DHPlaneGrid::~DHPlaneGrid()
// 	{
// 		delete mGrid;
// 	}
// 
// 	void DHPlaneGrid::setColor( const DColor& color )
// 	{
// 		mGrid->setColor(color);
// 	}
// 
// 	void DHPlaneGrid::reset( DReal longtitude, DReal unit )
// 	{
// 		mAABB.setExtent(-longtitude/2, -longtitude/2, -1.0f,
// 			longtitude/2, longtitude/2, 1.0f );
// 		mGrid->reset(longtitude, unit);
// 	}
// 
// 	void DHPlaneGrid::applyToRenderQueue( DRenderQueue* destQueue )
// 	{
// 		destQueue->addRenderale(RG_Background, mGrid, mGrid->getForwardRenderTechnique());
// 	}

}