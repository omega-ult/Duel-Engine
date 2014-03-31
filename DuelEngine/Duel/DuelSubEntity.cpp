//  [12/22/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelSubEntity.h"
#include "DuelSubMesh.h"
#include "DuelSkeleton.h"
#include "DuelCore.h"
#include "DuelResourceGroupManager.h"
#include "DuelMaterial.h"
#include "DuelMaterialManager.h"
#include "DuelVertexDeclaration.h"
#include "DuelVertexData.h"
#include "DuelRenderResourceManager.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DSubEntity, DRenderable);


	DSubEntity::DSubEntity( DEntity* parent, DSubMeshPtr subMesh ) :
		mParent(parent),
		mTargetSubMesh(subMesh),
		mbVisible(true),
		mBoundingBox(DAxisAlignedBox::AAB_NULL)
	{
		mName = subMesh->getName();

		DString skelName = subMesh->getParent()->getSkeleton();
		// split group name and the res name with "/"
		DStringVector tmp = DStringTool::split(skelName, "/");
		DString skelGrpName;
		DString skelResName;
		if (tmp.size() == 2) // meanning the result contains the group name.
		{
			skelGrpName = tmp[0];
			skelResName = tmp[1];
		}
		else if (tmp.size() == 1) // meaning the result only contains res name, try to 
		{
			skelGrpName = subMesh->getParent()->getGroupName();
			skelResName = tmp[0];
		}
		DResourcePtr skelRes = DResourceGroupManager::getSingletonPtr()->getResouceManager("Skeleton")->getResource(skelGrpName, skelResName);
		if (skelRes != NULL && skelRes->isLoaded())
		{
			mSkelInst = skelRes->getAs<DSkeleton>()->createSkeltonInstance(mName);
		}
		if (subMesh->getMaterialInstance() != NULL)
		{
			mMtlInst = DMaterialManager::getSingleton().createMaterialInstance(subMesh->getMaterialInstance()->getParent()->getMaterialName());
			subMesh->getMaterialInstance()->copyTo(mMtlInst.get());
		}
		
		// now go for the vertex data and index data.
		// first copy the index data and the vertex data.
		mIData = subMesh->getIndices();
		mVData = DVertexData(subMesh->getVertexStreams(), subMesh->getVertexDeclaration());
		DVertexDeclaration vdecl = mVData.getDeclaration();
		DVertexStreams	vstream = mVData.getBufferStreams();
		DVertexDeclaration::VertexElementIterator vei = vdecl.getVertexElementIterator();
		// we must copy the postion and the normal buffer for applying morph animation.
		// for other vertex element, use the submesh's buffer is ok.
		DVertexElement posElem;	// record the pos elem for constructing the bounding box.
		uint32	posSrc = 0xffff;
		uint32  normSrc = 0xffff;
		while (vei.hasMoreElements())
		{
			DVertexElement elem = vei.getNext();
			if (elem.getSemantic() == VES_Position)
			{
				posSrc = elem.getSource();
				posElem = elem;
			}
			if (elem.getSemantic() == VES_Normal)
			{
				normSrc = elem.getSource();
			}
		}
		if (posSrc != 0xffff || normSrc != 0xffff)
		{
			DVertexBufferPtr posBuf;
			DVertexBufferPtr normBuf;
			if (posSrc != 0xffff)
			{
				posBuf = vstream.getStream(posSrc);
			}
			if (normSrc != 0x0ffff)
			{
				normBuf = vstream.getStream(normSrc);
			}
			// the case posbuf share with normbuf.
			if (posBuf != NULL)
			{
				DVertexBufferPtr newBuf = DRenderResourceManager::getSingleton().
					createVetexBuffer(posBuf->getVertexSize(), posBuf->getVerticesCount(), HBU_DynamicWriteOnly, true);
				void* newBufPtr = newBuf->lock(HBL_Discard);
				posBuf->readData(0, posBuf->getSize(), newBufPtr);
				newBuf->unlock();
				vstream.setStream(posSrc, newBuf);
				// construct bounding box.
				if(posElem.getElementType() == VET_Float3)
				{
					char* buf = (char*)newBuf->lock(HBL_ReadOnly);
					buf += posElem.getOffset();
					uint32 vcount = newBuf->getVerticesCount();
					DReal min[3] = { DMath::POS_INFINITY };
					DReal max[3] = { DMath::NEG_INFINITY };
					for (uint32 i = 0; i < vcount; ++i)
					{
						float* fPtr = (float*)buf;
						min[0] = DMath::Min<DReal>(*fPtr, min[0]);
						min[1] = DMath::Min<DReal>(*(fPtr+1), min[1]);
						min[2] = DMath::Min<DReal>(*(fPtr+2), min[2]);
						max[0] = DMath::Max<DReal>(*fPtr, max[0]);
						max[1] = DMath::Max<DReal>(*(fPtr+1), max[1]);
						max[2] = DMath::Max<DReal>(*(fPtr+2), max[2]);
						buf += posElem.getSize();
					}
					newBuf->unlock();
					mBoundingBox.setExtent(min[0],min[1],min[2],
						max[0],max[1],max[2]);
				}
			}
			if (normBuf != NULL && posBuf.get() != normBuf.get())
			{
				DVertexBufferPtr newBuf = DRenderResourceManager::getSingleton().
					createVetexBuffer(normBuf->getVertexSize(), normBuf->getVerticesCount(), HBU_DynamicWriteOnly, true);
				void* newBufPtr = newBuf->lock(HBL_Discard);
				normBuf->readData(0, normBuf->getSize(), newBufPtr);
				newBuf->unlock();
				vstream.setStream(normSrc, newBuf);
			}
			mVData.setStreams(vstream);
		} // else no posbuf/normbuf found, skip it.

		mRenderLayout = DRenderResourceManager::getSingleton().createRenderLayout();
		mRenderLayout->setIndexData(mIData);
		mRenderLayout->setVertexData(mVData);
		mRenderLayout->setTopologyType(PT_TriangleList);
		mRenderLayout->seal();
	}

	DSubEntity::~DSubEntity()
	{

	}

	const DString& DSubEntity::getName()
	{
		return mName;
	}

	DSubMeshPtr DSubEntity::getTargetSubMesh()
	{
		return mTargetSubMesh;
	}

	DEntity* DSubEntity::getParent() const
	{
		return mParent;
	}

	bool DSubEntity::isVisible() const
	{
		return mbVisible;
	}

	void DSubEntity::setVisible( bool visible )
	{
		mbVisible = visible;
	}

	DRenderLayout* DSubEntity::getRenderLayout()
	{
		return mRenderLayout.get();
	}

	DRenderTechnique* DSubEntity::getRenderTechnique( uint32 stage )
	{
		if (mMtlInst != NULL)
		{
			return mMtlInst->getRenderTechnique(stage);
		}
		return NULL;
	}

	Duel::DAxisAlignedBox DSubEntity::getBoundingBox() const
	{
		return mBoundingBox;
	}

	void DSubEntity::updateCustomGpuParameter( DShaderObject* so )
	{
		if (mMtlInst != NULL)
		{
			mMtlInst->updateGpuParameter(so);
		}
	}

	Duel::DMaterialInstancePtr DSubEntity::getMaterialInstance()
	{
		return mMtlInst;
	}



}