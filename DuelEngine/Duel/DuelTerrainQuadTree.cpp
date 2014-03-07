//  [12/27/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelTerrainPage.h"
#include "DuelTerrainQuadTree.h"
#include "DuelRenderResourceManager.h"
#include "DuelRenderable.h"
#include "DuelRenderQueue.h"
#include "DuelMaterialManager.h"
#include "DuelMaterial.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DTerrainQuadTreeNode, DObject);
	DUEL_IMPLEMENT_RTTI_1(DTerrainRenderNode, DRenderable);

	 


	DTerrainQuadTreeNode::DTerrainQuadTreeNode( DTerrainPage* parentPage, DTerrainQuadTreeNode* parentNode, DTerrainRegion region, uint16 batchSize ) :
		mParentPage(parentPage),
		mParentNode(parentNode),
		mRegion(region),
		mBatchSize(batchSize),
		mGeometryError(0.0f),
		mRenderNode(NULL)
	{
		/*
			+----+----+
			| 3  |  4 |
			+----+----+
			| 1  |  2 |
			+----+----+
		*/
		if (region.isValid() && region.getXExtend() > mBatchSize-1 && region.getZExtend() > mBatchSize-1 && mBatchSize > 1)
		{
			int16 halfSize = (mRegion.getXExtend()) / 2;
			mChildrenNode[0] = new DTerrainQuadTreeNode(mParentPage, this,
				DTerrainRegion(mRegion.xMin, mRegion.xMin+halfSize, mRegion.zMin, mRegion.zMin+halfSize), mBatchSize);
			mChildrenNode[1] = new DTerrainQuadTreeNode(mParentPage, this,
				DTerrainRegion(mRegion.xMin+halfSize, mRegion.xMax, mRegion.zMin, mRegion.zMin+halfSize), mBatchSize);
			mChildrenNode[2] = new DTerrainQuadTreeNode(mParentPage, this,
				DTerrainRegion(mRegion.xMin, mRegion.xMin+halfSize, mRegion.zMin+halfSize, mRegion.zMax), mBatchSize);
			mChildrenNode[3] = new DTerrainQuadTreeNode(mParentPage, this,
				DTerrainRegion(mRegion.xMin+halfSize, mRegion.xMax, mRegion.zMin+halfSize, mRegion.zMax), mBatchSize);
		}
		else
		{
			mChildrenNode[0] = NULL;
			mChildrenNode[1] = NULL;
			mChildrenNode[2] = NULL;
			mChildrenNode[3] = NULL;
		}
	}

	DTerrainQuadTreeNode::~DTerrainQuadTreeNode()
	{
		if (!isLeaf())
		{
			delete mChildrenNode[0];
			delete mChildrenNode[1];
			delete mChildrenNode[2];
			delete mChildrenNode[3];
		}
	}

	Duel::DReal DTerrainQuadTreeNode::getMaxGeometryError() const
	{
		return 0.0f;
	}

	Duel::DAxisAlignedBox DTerrainQuadTreeNode::getBoundingBox() const
	{
		return mBoundingBox;
	}
	
	bool DTerrainQuadTreeNode::isLeaf() const
	{
		// only test the first child.
		return mChildrenNode[0] == NULL;
	}

	void DTerrainQuadTreeNode::clearGeometryError( const DTerrainRegion& region )
	{
		if (region.intersects(mRegion))
		{
			// relevant to this node (overlaps)

			// if the rect covers the whole node, reset the max height
			// this means that if you recalculate the deltas progressively, end up keeping
			// a max height that's no longer the case (ie more conservative lod), 
			// but that's the price for not recaculating the whole node. If a 
			// complete recalculation is required, just dirty the entire node. (or terrain)

			// Note we use the 'calc' field here to avoid interfering with any
			// ongoing LOD calculations (this can be in the background)
			if (region.contains(mRegion))
			{
				mGeometryError = 0.0f;
			}
			if (!isLeaf())
			{
				mChildrenNode[0]->clearGeometryError(region);
				mChildrenNode[1]->clearGeometryError(region);
				mChildrenNode[2]->clearGeometryError(region);
				mChildrenNode[3]->clearGeometryError(region);
			}

		}
	}

	void DTerrainQuadTreeNode::modifyGeometryError( int16 x, int16 z, DReal gerror )
	{
		if(mRegion.isInside(x, z))
		{
			if (!isLeaf())
			{
				mChildrenNode[0]->modifyGeometryError(x,z, mGeometryError);
				mChildrenNode[1]->modifyGeometryError(x,z, mGeometryError);
				mChildrenNode[2]->modifyGeometryError(x,z, mGeometryError);
				mChildrenNode[3]->modifyGeometryError(x,z, mGeometryError);				
				mGeometryError = DMath::Max<DReal>(mGeometryError, DMath::Abs(gerror));
			}
		}
	}

	void DTerrainQuadTreeNode::finalizeGeometryError( const DTerrainRegion& region )
	{
		if (region.intersects(mRegion))
		{
			// relevant to this node (overlaps)

			// each non-leaf node should know which of its children transitions
			// to the lower LOD level last, because this is the one which controls
			// when the parent takes over
			if (!isLeaf())
			{
				mChildrenNode[0]->finalizeGeometryError(region);
				mChildrenNode[1]->finalizeGeometryError(region);
				mChildrenNode[2]->finalizeGeometryError(region);
				mChildrenNode[3]->finalizeGeometryError(region);

				DReal maxChildGError = -1.0f;
				for (uint32 i = 0; i < 4; ++i)
				{
					maxChildGError = DMath::Max<DReal>(maxChildGError, mChildrenNode[i]->mGeometryError);
				}
				// make sure the parent's geometry error is bigger than children.
				if (mGeometryError < maxChildGError)
				{
					mGeometryError = 1.05f * maxChildGError;
				}
			}
		}
	}

	void DTerrainQuadTreeNode::generateIndexData()
	{
		
		/* For even / odd tri strip rows, triangles are this shape:
		even, leftward     odd, rightward
		3---2             3---2
		| / |             | \ |
		0---1             0---1
		1, 2, 0, 3        0, 3, 1, 2
		*/
		
		// fist allocate a new vertex buffer.
		DRenderResourceManager* hm = DRenderResourceManager::getSingletonPtr();

		// calculate the index count.
		// first, is the main surface.
		uint32 vtxSurfaceCount = 2 * mBatchSize * (mBatchSize - 1);
		// skirts vertex count.
		uint32 vtxSkirtCount = 2 * mBatchSize * 4;

		mIndexBuffer = hm->createIndexBuffer(IT_32Bit, vtxSurfaceCount+vtxSkirtCount, HBU_Static, false);
		uint32*	bufPtr = (uint32*)mIndexBuffer->lock(HBL_Discard);

		// vertex count along per row.
		uint32 vtxPitch = mParentPage->getPageSize();
		// indicate how many vertices we should skip while generating this lod level.
		uint32 vtxSkip = (mRegion.xMax - mRegion.xMin) / (mBatchSize - 1);
		// offset vertex count of this region.
		uint32 vtxXOffset = (mRegion.xMin - (-(int32)mParentPage->getPageSize()/2));
		uint32 vtxZOffset = (mRegion.zMin - (-(int32)mParentPage->getPageSize()/2));
		uint32 vtxRowOffset = vtxZOffset * vtxPitch;
		uint32 regionExt = (mRegion.xMax - mRegion.xMin);

		// construct main surface index.
		for (uint16 row = 0; row < mBatchSize - 1; ++row)
		{
			for (uint16 col = 0; col < mBatchSize; ++col)
			{
				// even row, (0-based)
				if (row % 2 == 0)
				{
					uint32 vid1 = vtxPitch * row * vtxSkip + vtxXOffset + regionExt - col * vtxSkip;
					uint32 vid2 = vid1 + vtxPitch * vtxSkip;
					*bufPtr = vtxRowOffset + vid1;/*0-based*/ bufPtr++;
					*bufPtr = vtxRowOffset + vid2;/*0-based*/ bufPtr++;
				}
				// odd row, (0-based)
				else
				{
					uint32 vid1 = vtxPitch * row * vtxSkip + col * vtxSkip;
					uint32 vid2 = vid1 + vtxPitch * vtxSkip;
					*bufPtr = vtxRowOffset + vid1;/*0-based*/ bufPtr++;
					*bufPtr = vtxRowOffset + vid2;/*0-based*/ bufPtr++;
				}
			}
		}

		// construct skirts 
		uint32 vtxRowSkirtStart = mParentPage->getPageSize() * mParentPage->getPageSize();
		uint32 skirtRowColCount = (mParentPage->getPageSize() - 1) / (mParentPage->getBatchSize() - 1) + 1;
		uint32 skirtPitch = mParentPage->getPageSize()*2; // single strip skirt.
		uint32 vtxColSkirtStart = vtxRowSkirtStart + skirtRowColCount*skirtPitch;
		// since the skirts are continuous, and store as: start->surfacePnt->bottomPnt->sufacePnt->....
		// so it's simple to construct them.
		// row skirts first, from bottom to top.
		uint32 botSkirtStart = vtxRowSkirtStart + (mRegion.zMin - (-(int32)mParentPage->getPageSize()/2)) / (mBatchSize-1) * skirtPitch;
		uint32 topSkirtStart = botSkirtStart + (mRegion.getZExtend()/(mBatchSize-1)) * skirtPitch;
		
		// col skirts, from left to right.
		uint32 lftSkirtStart = vtxColSkirtStart + (mRegion.xMin - (-(int32)mParentPage->getPageSize()/2)) / (mBatchSize-1) * skirtPitch;
		uint32 rhtSkirtStart = lftSkirtStart + (mRegion.getXExtend()/(mBatchSize-1)) * skirtPitch;

		// construct skirt index. since our skirts follow the surface triangle strip,
		// we must construct the indices continuously, there are two types of surface's
		// triangle strip ending : odd and even, meaning, our skirt can start from top-left or
		// top-right, anyway, our skirts are constructed clockwise.
		// we construct skirts as:
		/*
            0       
            +-------+1
            |       |
            |       |  
            |       |
           3+-------+
                    2
		*/
		// as mentioned above, we could start construction from 0 or 1, decided by (mBatchSize % 2)
		for (int32 s = 0; s < 4; ++s)
		{
			switch ((s + (mBatchSize % 2)) % 4)
			{
			case 0:	// from top-left, rightward
				{
					for (int16 col = 0; col < mBatchSize; ++col)
					{
						uint32 sid1 = topSkirtStart + col * (vtxSkip * 2);
						uint32 sid2 = sid1 + 1;
						*bufPtr = sid1; bufPtr++;
						*bufPtr = sid2; bufPtr++;
					}
				}
				break;
			case 1: // from top-right, downward
				{
					for (int16 row = mBatchSize-1; row >= 0; --row)
					{
						uint32 sid1 = rhtSkirtStart + row * (vtxSkip * 2);
						uint32 sid2 = sid1 + 1;
						*bufPtr = sid1; bufPtr++;
						*bufPtr = sid2; bufPtr++;
					}
				}
				break;
			case 2: // from bottom-right, leftward
				{
					for (int16 col = mBatchSize-1; col >= 0; --col)
					{
						uint32 sid1 = botSkirtStart + col * (vtxSkip * 2);
						uint32 sid2 = sid1 + 1;
						*bufPtr = sid1; bufPtr++;
						*bufPtr = sid2; bufPtr++;
					}
				}
				break;
			case 3: // from bottom-left, upward
				{
					for (int16 row = 0; row < mBatchSize; ++row)
					{
						uint32 sid1 = lftSkirtStart + row * (vtxSkip * 2);
						uint32 sid2 = sid1 + 1;
						*bufPtr = sid1; bufPtr++;
						*bufPtr = sid2; bufPtr++;
					}
				}
				break;
			default:
				break;
			}
		}


		mIndexBuffer->unlock();

		// regenerate renderable object.
		if (mRenderNode)
		{
			delete mRenderNode;
		}
		mRenderNode = new DTerrainRenderNode(this);

		if (!isLeaf())
		{
			mChildrenNode[0]->generateIndexData();
			mChildrenNode[1]->generateIndexData();
			mChildrenNode[2]->generateIndexData();
			mChildrenNode[3]->generateIndexData();
		}
	}

	void DTerrainQuadTreeNode::updateBoundingBox()
	{
		int32 vtxSkip = (mRegion.xMax - mRegion.xMin) / (mBatchSize - 1);
		DReal maxH = FLT_MIN;
		DReal minH = FLT_MAX;
		for (int32 z = mRegion.zMin; z <= mRegion.zMax; z += vtxSkip)
		{
			for (int32 x = mRegion.xMin; x <= mRegion.xMax; x+=vtxSkip)
			{
				DReal height = mParentPage->getPoint(x,z).y;
				maxH = DMath::Max<DReal>(maxH, height);
				minH = DMath::Min<DReal>(minH, height);
			}
		}
		mBoundingBox.setExtent((DReal)mRegion.xMin,minH, (DReal)mRegion.zMin,
							(DReal)mRegion.xMax, maxH, (DReal)mRegion.zMax);
		if (!isLeaf())
		{
			mChildrenNode[0]->updateBoundingBox();
			mChildrenNode[1]->updateBoundingBox();
			mChildrenNode[2]->updateBoundingBox();
			mChildrenNode[3]->updateBoundingBox();
		}
	}

	DTerrainPage* DTerrainQuadTreeNode::getParentPage()
	{
		return mParentPage;
	}

	Duel::DIndexBufferPtr DTerrainQuadTreeNode::getIndexBuffer()
	{
		return mIndexBuffer;
	}

	void DTerrainQuadTreeNode::applyToRenderQueue( const DCamera* cam, DReal K, DRenderQueue* queue, uint32 groupID )
	{
		DVector3 camPos = cam->getEyePosition();
		DVector3 boxPos = mBoundingBox.getClosestPointTo(camPos);
		DReal dist = boxPos.getDistanceTo(camPos);
		if (DMath::realEqual(dist, 0.0f))
		{
			dist = 0.001f;
		}
		if (mParentPage->getGeometryErrorTolerance() >= (mGeometryError/dist)*K)
		{
			if (mRenderNode)
			{
				queue->addRenderale(groupID, mRenderNode);
			}
		}
		else
		{
			if (!isLeaf())
			{
				mChildrenNode[0]->applyToRenderQueue(cam, K, queue, groupID);
				mChildrenNode[1]->applyToRenderQueue(cam, K, queue, groupID);
				mChildrenNode[2]->applyToRenderQueue(cam, K, queue, groupID);
				mChildrenNode[3]->applyToRenderQueue(cam, K, queue, groupID);
			}
			else 
			{
				if (mRenderNode)
				{
					queue->addRenderale(groupID, mRenderNode);
				}
			}
		}

	}


	DTerrainRenderNode::DTerrainRenderNode( DTerrainQuadTreeNode* parentNode ) :
		mParentNode(parentNode),
		mMorphWeight(0.0f)
	{
		DRenderResourceManager* hm = DRenderResourceManager::getSingletonPtr();
		mRenderLayout = hm->createRenderLayout();
		mRenderLayout->setTopologyType(PT_TriangleStrip);
		mRenderLayout->setVertexData(DVertexData(mParentNode->getParentPage()->getVertexStreams(),
			mParentNode->getParentPage()->getVertexDeclaration()));
		mRenderLayout->setIndexData(DIndexData(mParentNode->getIndexBuffer()));
	}

	DRenderLayout* DTerrainRenderNode::getRenderLayout()
	{
		return mRenderLayout.get();
	}

	DRenderTechnique* DTerrainRenderNode::getRenderTechnique( uint32 stage )
	{
		if (mMaterial != NULL)
		{
			return mMaterial->getRenderTechnique(stage);
		}
		return NULL;

	}

	void DTerrainRenderNode::updateCustomGpuParameter( DShaderObject* so )
	{
		if (mMaterial != NULL)
		{
			mMaterial->updateGpuParameter(so);
		}
	}

	void DTerrainRenderNode::getWorldTransform( DMatrix4& outMat )
	{

	}

	void DTerrainRenderNode::setMorphWeight( DReal weight )
	{
		mMorphWeight = weight;
		// clamp.
		mMorphWeight = DMath::Max<DReal>(mMorphWeight, 0.0f);
		mMorphWeight = DMath::Min<DReal>(mMorphWeight, 1.0f);
	}


}
