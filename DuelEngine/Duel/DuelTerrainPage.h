//  [12/14/2013 OMEGA] created

#ifndef _DUELTERRAINPAGE_H_
#define _DUELTERRAINPAGE_H_

#include "DuelCommon.h"
#include "DuelTerrain.h"
#include "DuelVertexData.h"
#include "DuelTerrainQuadTree.h"


namespace Duel
{

	struct DTerrainPageData
	{
	public:
		// size of the page, notice that this value only indicate the vertex count along 
		// an axis.
		uint16		pageSize;
		// batch size is the size of each quad tree node.
		uint16		batchSize;
		// distance between two vertices along one side.
		DReal		pageScale;
		// if specified height map is not the size we prefered, the DTerrainPage will scale it.
		// if specified map is a NULL ptr, then the terrain will use a zero-height.
		DImage2D*	heightMap;
	};

	// 一个terrainpage就是一个完整的chunk lod地形链
	class DUEL_API DTerrainPage : public DObject
	{
		DUEL_DECLARE_RTTI(DTerrainPage)
	public:
		//DTerrainPage(DTerrain* parent, TerrainPageIndex pageIndex);
		DTerrainPage();
		~DTerrainPage();

		/*TerrainPageIndex	getPageIndex() const;*/
			
		// enable/disable this page to render
		void		setEnable(bool flag);
		bool		isEnabled() const;
		
		// use specified data to re-generate terrain
		void		generate(DTerrainPageData* data);
		// the bounding box always use local coordinate, i.e origin(0,0,0) as its center.
		DAxisAlignedBox		getBoundingBox() const;

		void		applyToRenderQueue(DRenderQueue* queue, const DCamera* cam, DViewport vp);

		// call this method every frame to update contents.
		void		update();

		// call this function to update the height map, parameter background indicate
		// whether this work should be done in background thread.
		void		requestHeightMapUpdate(DTerrainRegion region, bool background);

		DTerrainQuadTreeNode*	getQuadTreeRoot();

		uint32		getPageSize() const;
		uint32		getBatchSize() const;
		DReal		getPageScale() const;
		void		setGeometryErrorTolerance(DReal val);
		DReal		getGeometryErrorTolerance() const;

		// get specified point at given x and z index, the point's y value is the height,
		// x, z may be negative since we use page's center(row pagesize/2, col pagesize/2) as its origin.
		DVector3	getPoint(int16 xIdx, int16 zIdx);

		DReal		getDelta(int16 xIdx, int16 zIdx);

		DVertexDeclaration	getVertexDeclaration();
		DVertexStreams		getVertexStreams();


	protected:
		// methods that works in background thread.
		void		updateHeightData(DTerrainRegion region);
		// this method generate an empty vertex buffer only.
		void		generateVertexData();
		// this method will use current height data and delta data
		// to update currrent vertex data.
		void		updateVertexData();
		DThread		mWorkerThread;
		
		bool		mbEnable;
		DVertexDeclaration		mVDecl;
		DVertexStreams			mVStream;

		// the quadtree root.. ah...
		DTerrainQuadTreeNode*	mQuadTreeRoot;

		//TerrainPageIndex		mPageIndex;

		// indicating the vertex size of this terrain, must be 2^n+1
		uint16		mPageSize;
		// quad tree division factor.
		uint16		mBatchSize;

		// tolerance gError.
		DReal		mGeometryErrorTolerance;

		// indicate the unit scale of thie page. 
		DReal		mPageScale;
		// DReal[mPageSize * mPageSize], the height map, should I explain it?
		DReal*		mHeightMap;
		// morph delta map, the size is identical to the height map.
		DReal*		mMorphDeltaMap;
		// normal map, identical to each point.
		DVector3*	mNormalMap;

		// used to synchronize with background thread.
		DMutex		mDataMutex;
		// indicate that we should rewrite vertex buffer.
		DAtomicScalar<bool>		mVertexBufferDirty;
	};
}

#endif