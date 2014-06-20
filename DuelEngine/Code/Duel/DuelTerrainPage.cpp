//  [12/27/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelTerrainPage.h"
#include "DuelTerrainQuadTree.h"
#include "DuelImage2D.h"
#include "DuelRenderResourceManager.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DTerrainPage, DObject);


	DTerrainPage::DTerrainPage() :
		mQuadTreeRoot(NULL),
		mbEnable(true),
		mHeightMap(NULL),
		mMorphDeltaMap(NULL),
		mNormalMap(NULL),
		mPageScale(0.0f),
		mPageSize(0xffff),
		mBatchSize(0xffff),
		mGeometryErrorTolerance(1.0f)
	{
		mWorkerThread.start();
		mVertexBufferDirty.set(false);
	}

	DTerrainPage::~DTerrainPage()
	{
		// wait the thread complete its tasks.
		mWorkerThread.terminate();
		mWorkerThread.join();
		if (mQuadTreeRoot)
		{
			delete mQuadTreeRoot;
		}
		if (mHeightMap)
		{
			delete mHeightMap;
		}
		if (mMorphDeltaMap)
		{
			delete mMorphDeltaMap;
		}
		if (mNormalMap)
		{
			delete[] mNormalMap;
		}
	}

	void DTerrainPage::setEnable( bool flag )
	{
		mbEnable = flag;
	}

	bool DTerrainPage::isEnabled() const
	{
		return mbEnable;
	}

	DAxisAlignedBox DTerrainPage::getBoundingBox() const
	{
		if (mQuadTreeRoot)
		{
			return mQuadTreeRoot->getBoundingBox();
		}
		return DAxisAlignedBox::AAB_NULL;
	}

	uint16	validatePageSize(uint16 pageSize)
	{
		uint16 co = 1;
		uint16 tmpSize = pageSize >> 1;
		while (tmpSize)
		{
			tmpSize >>= 1;
			co <<= 1;
		}
		return co + 1;
	}

	uint16	validateBatchSize(uint16 batchSize)
	{
		uint16 co = 1;
		uint16 tmpSize = batchSize >> 1;
		while (tmpSize)
		{
			tmpSize >>= 1;
			co <<= 1;
		}
		return co + 1;
	}

	void DTerrainPage::generate( DTerrainPageData* data )
	{
		if (!data)
		{
			return;
		}
		// validate input data
		if (data->batchSize > data->pageSize)
		{
			return;
		}

		bool bRegenerateVB = false;
		// we need the size to be valid.
		uint16 finalPageSize = validatePageSize(data->pageSize);
		uint16 finalBatchSize = validateBatchSize(data->batchSize);
		DReal  finalPageScale = data->pageScale;
		// if the new vertex settings are different to current one, we
		// have to re-generate vertext buffer.
		if (mPageScale != finalPageScale || mPageSize != finalPageSize || mBatchSize != finalBatchSize)
		{
			bRegenerateVB = true;
		}
		mPageSize = finalPageSize;
		mBatchSize = finalBatchSize;
		mPageScale = finalPageScale;

		// that's a must.
		assert(mBatchSize >= 2);

		// reset our quadtree
		if (mQuadTreeRoot)
		{
			delete mQuadTreeRoot;
		}
		DTerrainRegion rootRegion;
		int16 halfPageSize = (mPageSize - 1)/2;
		rootRegion.xMin = -halfPageSize;
		rootRegion.zMin = -halfPageSize;
		rootRegion.xMax = halfPageSize;
		rootRegion.zMax = halfPageSize;
		mQuadTreeRoot = new DTerrainQuadTreeNode(this, NULL, rootRegion, mBatchSize);

		// if the parameter is a null image, use an empty one.
		bool useEmptyImage = false;
		DImage2D* scaledHeightMap = NULL;
		scaledHeightMap = new DImage2D();
		float* emptyData = new float[mPageSize*mPageSize];
		memset(emptyData, 0, sizeof(float) * mPageSize*mPageSize);
		DImageDesc imgDesc;
		imgDesc.width = mPageSize;
		imgDesc.height = mPageSize;
		imgDesc.depth = 1;
		imgDesc.mipMapCount = 1;
		imgDesc.arraySize = 1;
		imgDesc.pixelFormat = PF_R32_Float;
		scaledHeightMap->loadFromData((char*)emptyData, imgDesc);
		scaledHeightMap->setAccess(IL_Write);
		scaledHeightMap->setWorkRect(ImageRect(0,0,mPageSize,mPageSize));
		delete emptyData;
		if (data->heightMap != NULL)
		{
			data->heightMap->scale(*scaledHeightMap);
		}

		// copy data to the height map.
		DUEL_LOCK_MUTEX(mDataMutex);

		if (mHeightMap != NULL)
		{
			delete mHeightMap;
		}
		mHeightMap = new DReal[mPageSize*mPageSize];
		if (mMorphDeltaMap != NULL)
		{
			delete mMorphDeltaMap;
		}
		mMorphDeltaMap = new DReal[mPageSize*mPageSize];
		memset(mMorphDeltaMap, 0, sizeof(DReal) * mPageSize * mPageSize);
		if (mNormalMap != NULL)
		{
			delete[] mNormalMap;
		}
		mNormalMap = new DVector3[mPageSize*mPageSize];
		memset(mNormalMap, 0, sizeof(DVector3) *mPageSize * mPageSize);

		float* imgSrc = (float*)scaledHeightMap->getChunkPtr();
		DReal* heightMapDst = mHeightMap;
		size_t destRowPitch = scaledHeightMap->getRowPitch();

		for (uint32 i = 0; i < mPageSize; ++i)
		{
			for (uint32 j = 0; j < mPageSize; ++j)
			{
				mHeightMap[i * mPageSize + j] = *(imgSrc + mPageSize * i + j);
			}
		}
		delete scaledHeightMap;

		if (bRegenerateVB)
		{
			generateVertexData();
			// now we have vertex data, we need to re-create index data for quadtree nodes.
			mQuadTreeRoot->generateIndexData();
		}

		requestHeightMapUpdate(rootRegion, false);
	}

	void DTerrainPage::generateVertexData()
	{
		// 重新做一个vertexDeclaration吧.
		DRenderResourceManager* hm = DRenderResourceManager::getSingletonPtr();
		mVDecl.clear();

		size_t offset = 0;
		// position element.
		mVDecl.addElement(0, offset, VET_Float3, VES_Position);
		offset += DVertexElement::getElementTypeSize(VET_Float3);
		// morph delta element.
		mVDecl.addElement(0, offset, VET_Float1, VES_TexCoord, 0);	
		offset += DVertexElement::getElementTypeSize(VET_Float1);
		// tex coord 
		mVDecl.addElement(0, offset, VET_Float2, VES_TexCoord, 1);

		size_t vertexSize = mVDecl.getVertexSize(0);

		// base vertices 
		uint32 vertexCount = mPageSize * mPageSize;

		// skirts vertices.
		uint32 skirtRowColCount = (mPageSize - 1) / (mBatchSize - 1) + 1;
		// each column/row hase same vertex count to the 2x page size,
		// notice that our skirts vertex use origin base vertex's copy.
		vertexCount += skirtRowColCount * (mPageSize * 2)/*single col/row*/ * 2;/*colSkirt and rowSkirt*/

		// ok we create vertex buffer now.
		DVertexBufferPtr vb = hm->createVetexBuffer(vertexSize, vertexCount, HBU_Static, false);
		size_t vbByteSize = vertexSize * vertexCount;
		// initialize buffer
		char* buf = (char*)vb->lock(HBL_Discard);
		memset(buf, 0, vbByteSize); 
		vb->unlock();
		mVStream.setStream(0, vb);
	}

	void DTerrainPage::requestHeightMapUpdate(DTerrainRegion region, bool background )
	{
		if (background)
		{
			mWorkerThread.append(DBind(&DTerrainPage::updateHeightData, this, region));
		}
		else
		{
			updateHeightData(region);
		}
	}

	void DTerrainPage::updateHeightData(DTerrainRegion region)
	{
		DUEL_LOCK_MUTEX(mDataMutex);

		if (!region.isValid())
		{
			return;
		}

		// first update bounding box 
		mQuadTreeRoot->updateBoundingBox();

		// calculate geometry error and delta values.
		mQuadTreeRoot->clearGeometryError(region);

		int32 halfPageSize = (mPageSize-1)/2;
		// instead of using depth to control our looping, we use vertex skipping.
		int32 vtxStep = (mPageSize-1)/(mBatchSize-1);
		while (vtxStep != 1)
		{
			// need to widen the dirty rectangle since change will affect surrounding
			// vertices at lower LOD
			DTerrainRegion lodRegion;
			lodRegion.xMin = DMath::Max<int32>(-halfPageSize, region.xMin - vtxStep);
			lodRegion.xMax = DMath::Min<int32>(halfPageSize, region.xMax + vtxStep);
			lodRegion.zMin = DMath::Max<int32>(-halfPageSize, region.zMin - vtxStep);
			lodRegion.zMax = DMath::Min<int32>(halfPageSize, region.zMax + vtxStep);
			
			// now round the rectangle at this level so that it starts & ends on 
			// the step boundaries
			lodRegion.xMin -= lodRegion.xMin % vtxStep;
			lodRegion.zMin -= lodRegion.zMin % vtxStep;
			if (lodRegion.xMax % vtxStep)
			{
				lodRegion.xMax += vtxStep - (lodRegion.xMax % vtxStep);
			}
			if (lodRegion.zMax % vtxStep)
			{
				lodRegion.zMax += vtxStep - (lodRegion.zMax % vtxStep);
			}
			for (int32 z = lodRegion.zMin; z < lodRegion.zMax; z+=vtxStep)
			{
				for (int32 x = lodRegion.xMin; x < lodRegion.xMax; x+=vtxStep)
				{
					// Form planes relating to the lower detail tris to be produced
					// For even tri strip rows, they are this shape:
					// 2---3
					// | / |
					// 0---1
					// For odd tri strip rows, they are this shape:
					// 2---3
					// | \ |
					// 0---1

					DVector3 pt0 = getPoint(x, z);
					DVector3 pt1 = getPoint(x+vtxStep, z);
					DVector3 pt2 = getPoint(x, z+vtxStep);
					DVector3 pt3 = getPoint(x+vtxStep, z+vtxStep);

					DPlane p0, p1;
					bool evenRow = true;
					if ((z + halfPageSize)/vtxStep % 2 == 0) // even.
					{
						p0.reset(pt0, pt2, pt3);
						p1.reset(pt0, pt3, pt1);
					}
					else
					{
						p0.reset(pt1, pt0, pt2);
						p1.reset(pt1, pt2, pt3);
						bool backwardTri = false;
					}

					// include the bottommost row of vertices if this is the last row
					int32 zBound = (z + vtxStep == lodRegion.zMax) ? vtxStep : vtxStep - 1;
					for (int32 interpZ = 0; interpZ <= zBound; ++interpZ)
					{
						// include the rightmost col of vertices if this is the last col
						int32 xBound = (x + vtxStep == lodRegion.xMax) ? vtxStep : vtxStep - 1;
						for (int32 interpX = 0; interpX <= xBound; ++interpX)
						{
							int32 fullDetailZ = z + interpZ;
							int32 fullDetailX = x + interpX;
							if (fullDetailZ % vtxStep == 0 &&
								fullDetailX % vtxStep == 0)
							{
								// Skip, this one is a vertex at this level
								continue;
							}

							// Determine which tri we're on 
							DReal zpct = (DReal)interpZ / (DReal)vtxStep;
							DReal xpct = (DReal)interpX / (DReal)vtxStep;
							DPlane targetPlane;
							if ((xpct > zpct && evenRow) ||
								(xpct > (1-zpct) && !evenRow))
							{
								targetPlane = p1;
							}
							else
							{
								targetPlane = p0;
							}


							//interpolated height
							DVector3 actualPos = getPoint(fullDetailX, fullDetailZ);


							DReal interpHeight = (-targetPlane.normal.x*actualPos.x
												-targetPlane.normal.z*actualPos.z
												-targetPlane.d)/targetPlane.normal.y;

							DReal actualHeight = actualPos.y;
							DReal delta = interpHeight - actualHeight;
							// max(delta) is the worst case scenario at this LOD
							// compared to the original heightmap
							
							// tell the quadtree about this 
							mQuadTreeRoot->modifyGeometryError(fullDetailX, fullDetailZ, delta);
							
							// If this vertex is being removed at this LOD, 
							// then save the height difference since that's the move
							// it will need to make. Vertices to be removed at this LOD
							// are halfway between the steps, but exclude those that
							// would have been eliminated at earlier levels
							int32 halfStep = vtxStep / 2;
							int32 absFullDetailX = fullDetailX + halfPageSize;
							int32 absFullDetailZ = fullDetailZ + halfPageSize;
							if (((absFullDetailX % vtxStep) == halfStep && (absFullDetailZ % halfStep) == 0) ||
								((absFullDetailZ % vtxStep) == halfStep && (absFullDetailX % halfStep) == 0))
							{
								// Save height difference 
								mMorphDeltaMap[absFullDetailX + absFullDetailZ * mPageSize] = delta;
							}
							
						}
					}
				}
			}

			vtxStep >>= 1;
		}

		mQuadTreeRoot->finalizeGeometryError(region);


		// 下一个是.... normal?

		mVertexBufferDirty.set(true);
	}

	void DTerrainPage::update()
	{
		if (mVertexBufferDirty.get() == true)
		{
			updateVertexData();
			mVertexBufferDirty.set(false);
		}
	}

	void DTerrainPage::updateVertexData()
	{
		DVertexBufferPtr vb = mVStream.getStream(0);
		if (vb == NULL)
		{
			return;
		}
		// skirts vertices.
		uint32 skirtRowColCount = (mPageSize - 1) / (mBatchSize - 1) + 1;
		// each skirt stride's vertex count.
		uint32 skirtPitch = 2 * mPageSize;
		char* buf = (char*)vb->lock(HBL_Discard);
		float* mainSufacePtr = (float*)buf;
		float* rowSkirtPtr = (float*)(buf + (vb->getVertexSize()*mPageSize*mPageSize));
		float* colSkirtPtr = (float*)(((char*)rowSkirtPtr) + skirtRowColCount * skirtPitch * vb->getVertexSize());
		// first we need to write position/delta/texcoord data on main surface.
		// at the same time, we test whether this point is on the skirts, if it is,
		// write it to the skirts buffer area.
		/*
			vertex buffer:
			main surface               skirts
			|---------------------------|-row----|-col----|
			
		*/ 

		int16 halfPageSize = (mPageSize-1)/2;
		DReal texcorrdScale = 1.0f/(DReal)mPageSize;
		float* skirtPtr = NULL;
		uint32 skirtOffset = 0;
		uint32 floatVertexCount = vb->getVertexSize()/sizeof(float);
		for (int32 x = -halfPageSize; x <= halfPageSize; ++x)
		{
			for (int32 z = -halfPageSize; z <= halfPageSize; ++z)
			{
				DVector3 pos = getPoint(x, z);
				DReal	delta = getDelta(x, z);
				DVector2 tex((DReal)(x + halfPageSize) * texcorrdScale, (DReal)(z + halfPageSize) * texcorrdScale);
				*mainSufacePtr = pos.x; mainSufacePtr++;
				*mainSufacePtr = pos.y; mainSufacePtr++;
				*mainSufacePtr = pos.z; mainSufacePtr++;
				*mainSufacePtr = delta; mainSufacePtr++;
				*mainSufacePtr = tex.x; mainSufacePtr++;
				*mainSufacePtr = tex.y; mainSufacePtr++;
				// vertex on skirt test.
				// row skirt.
				if (z % (mBatchSize-1) == 0)
				{
					// get the pointer skirtOffset.
					skirtOffset = skirtPitch * ((x+halfPageSize) / (mBatchSize-1));
					skirtOffset += (uint32)(z+halfPageSize) * 2;
					skirtOffset *= floatVertexCount;
					skirtPtr = rowSkirtPtr + skirtOffset;
					// first is the surface point copy.
					*skirtPtr = pos.x; skirtPtr++;
					*skirtPtr = pos.y; skirtPtr++;
					*skirtPtr = pos.z; skirtPtr++;
					*skirtPtr = delta; skirtPtr++;
					*skirtPtr = tex.x; skirtPtr++;
					*skirtPtr = tex.y; skirtPtr++;
					// then is the bottom point;
					*skirtPtr = pos.x; skirtPtr++;
					*skirtPtr = 0.0f; skirtPtr++;
					*skirtPtr = pos.z; skirtPtr++;
					*skirtPtr = 0.0f; skirtPtr++;
					*skirtPtr = tex.x; skirtPtr++;	// use same texcoord.
					*skirtPtr = tex.y;
				}
				// col skirt.
				if (x % (mBatchSize-1) == 0)
				{
					// get the pointer skirtOffset.
					skirtOffset = skirtPitch * ((z+halfPageSize) / (mBatchSize-1));
					skirtOffset += (uint32)(x+halfPageSize) * 2;
					skirtOffset *= floatVertexCount;
					skirtPtr = colSkirtPtr + skirtOffset;
					// first is the surface point copy.
					*skirtPtr = pos.x; skirtPtr++;
					*skirtPtr = pos.y; skirtPtr++;
					*skirtPtr = pos.z; skirtPtr++;
					*skirtPtr = delta; skirtPtr++;
					*skirtPtr = tex.x; skirtPtr++;
					*skirtPtr = tex.y; skirtPtr++;
					// then is the bottom point;
					*skirtPtr = pos.x; skirtPtr++;
					*skirtPtr = 0.0f; skirtPtr++;
					*skirtPtr = pos.z; skirtPtr++;
					*skirtPtr = 0.0f; skirtPtr++;
					*skirtPtr = tex.x; skirtPtr++;	// use same texcoord.
					*skirtPtr = tex.y;
				}
			}
		}
		vb->unlock();
	}

	Duel::DVector3 DTerrainPage::getPoint( int16 xIdx, int16 zIdx )
	{
		int32 offset = (mPageSize - 1)/2;
		if (xIdx >= -offset && xIdx <= offset &&
			zIdx >= -offset && zIdx <= offset)
		{		
			return DVector3((DReal)(xIdx * mPageScale), 
				mHeightMap[(xIdx + offset) + (zIdx + offset)*mPageSize], (DReal)(zIdx * mPageScale));
		}
		return DVector3::ZERO;
	}

	Duel::DReal DTerrainPage::getDelta( int16 xIdx, int16 zIdx )
	{
		int32 offset = (mPageSize - 1)/2;
		if (xIdx >= -offset && xIdx <= offset &&
			zIdx >= -offset && zIdx <= offset)
		{
			return mMorphDeltaMap[(xIdx + offset) + (zIdx + offset)*mPageSize];
		}
		return 0.0f;
	}

	Duel::uint32 DTerrainPage::getPageSize() const
	{
		return mPageSize;
	}

	Duel::uint32 DTerrainPage::getBatchSize() const
	{
		return mBatchSize;
	}

	Duel::DReal DTerrainPage::getPageScale() const
	{
		return mPageScale;
	}

	void DTerrainPage::applyToRenderQueue( DRenderQueue* queue, const DCamera* cam, DViewport vp )
	{
		DReal K;
		K = ((DReal)vp.getWidth()/(2 * DMath::Tan(cam->getFOV()/2)));
		 
	}

	void DTerrainPage::setGeometryErrorTolerance( DReal val )
	{
		mGeometryErrorTolerance = val;
	}

	DReal DTerrainPage::getGeometryErrorTolerance() const
	{
		return mGeometryErrorTolerance;
	}

	DVertexDeclaration DTerrainPage::getVertexDeclaration()
	{
		return mVDecl;
	}

	DVertexStreams DTerrainPage::getVertexStreams()
	{
		return mVStream;
	}




}