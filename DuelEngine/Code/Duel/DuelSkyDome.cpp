//  [7/6/2014 OMEGA] created

#include "DuelCommon.h"
#include "DuelSkyDome.h"
#include "DuelMaterial.h"
#include "DuelMaterialManager.h"
#include "DuelRenderResourceManager.h"
#include "DuelRenderQueue.h"
#include "DuelMesh.h"

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(DSkyComponent, DRenderable);
	DUEL_IMPLEMENT_RTTI_1(DSkyDome, DObject);


	DSkyComponent::DSkyComponent(DSkyDome* parent, DSubMeshPtr subMesh ) :
		mParent(parent),
		mTargetSubMesh(subMesh),
		mCamPos(DVector3::ZERO)
	{
		mName = subMesh->getName();
		if (subMesh->getMaterialInstance() != NULL)
		{
			mMtlInst = DMaterialManager::getSingleton().copyMaterialInstance(subMesh->getMaterialInstance());
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
					createVetexBuffer(posBuf->getVertexSize(), posBuf->getVertexCount(), HBU_DynamicWriteOnly, true);
				void* newBufPtr = newBuf->lock(HBL_Discard);
				posBuf->readData(0, posBuf->getSize(), newBufPtr);
				newBuf->unlock();
				vstream.setStream(posSrc, newBuf);
			}
			if (normBuf != NULL && posBuf.get() != normBuf.get())
			{
				DVertexBufferPtr newBuf = DRenderResourceManager::getSingleton().
					createVetexBuffer(normBuf->getVertexSize(), normBuf->getVertexCount(), HBU_DynamicWriteOnly, true);
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


	const DString& DSkyComponent::getName()
	{
		return mName;
	}

	Duel::DMaterialInstancePtr DSkyComponent::getMaterialInstance()
	{
		return mMtlInst;
	}

	void DSkyComponent::setCameraPosition( DVector3 pos )
	{
		mCamPos = pos;
	}


	Duel::DReal DSkyComponent::getSquaredViewDepthToCamera( const DCamera* cam )
	{
		return DMath::REAL_POS_INFINITY;
	}

	void DSkyComponent::getWorldTransform( DMatrix4& outMat )
	{
		outMat = DMatrix4::IDENTITY;
		DVector3 camRelativeMove = mCamPos/mParent->getSkyCompacity();
		outMat.setTranslate(mCamPos-camRelativeMove);
	}


	DRenderTechnique* DSkyComponent::getRenderTechnique( uint32 stage, DCamera* cam, LightIterator li )
	{
		if (mMtlInst != NULL)
		{
			return mMtlInst->getRenderTechnique(stage, cam, li);
		}
		return NULL;
	}

	void DSkyComponent::updateCustomGpuParameter( DShaderObject* so )
	{
		if (mMtlInst != NULL)
		{
			mMtlInst->updateGpuParameter(so);
		}
	}

	DSkyDome::DSkyDome() : 
		mSkyCompacity(10000.0f)
	{

	}

	void DSkyDome::setSkyCompacity( DReal comp )
	{
		mSkyCompacity = comp;
	}

	Duel::DReal DSkyDome::getSkyCompacity()
	{
		return mSkyCompacity;
	}

	void DSkyDome::loadFromMesh( DResourcePtr mesh )
	{
		SkyComponentList::iterator i, iend = mCompList.end();
		for (i = mCompList.begin(); i != iend; ++i)
		{
			delete (*i);
		}
		mCompList.clear();
		DMesh* meshPtr = mesh->getAs<DMesh>(false);
		if (meshPtr != NULL && meshPtr->isLoaded())
		{
			mMeshTarget = mesh;
			DMesh::SubMeshIterator sbi = meshPtr->getSubMeshIterator();
			while (sbi.hasMoreElements())
			{
				DSubMeshPtr sb = sbi.getNext();
				DSkyComponent* newComp = new DSkyComponent(this, sb);
				mCompList.push_back(newComp);
			}
		}
	}

	void DSkyDome::applyToRenderQueue( DRenderQueue* destQueue, DCamera* cam )
	{
		if (destQueue != NULL)
		{
			for(uint32 i = 0; i < mCompList.size(); ++i)
			{
				mCompList[i]->setCameraPosition(cam->getEyePosition());
				destQueue->addRenderable(RG_Sky, mCompList[i]);
			}
		}
	}

	DSkyComponent* DSkyDome::getSkyComponent( const DString& name )
	{
		SkyComponentList::iterator i, iend = mCompList.end();
		for (i = mCompList.begin(); i != iend; ++i)
		{
			if ((*i)->getName() == name)
			{
				return (*i);
			}
		}
		return NULL;
	}

}