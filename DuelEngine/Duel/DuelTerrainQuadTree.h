//  [12/14/2013 OMEGA] created

#ifndef _DUELTERRAINQUADTREE_H_
#define _DUELTERRAINQUADTREE_H_

#include "DuelCommon.h"
#include "DuelTerrainPage.h"
#include "DuelIndexData.h"
#include "DuelRenderable.h"
#include "DuelMaterial.h"

namespace Duel
{
	// region specify an area on the terrain page, since we use left-hand coordinate,
	// we use x-z plane as our horizon, so we need an struct to describe the area.
	struct DTerrainRegion
	{
	public:
		DTerrainRegion() : xMin(0), xMax(0), zMin(0), zMax(0) {}
		DTerrainRegion(int16 _xMin, int16 _xMax, int16 _zMin, int16 _zMax) : xMin(_xMin), xMax(_xMax), zMin(_zMin), zMax(_zMax) {}
		bool		isValid() const { return (xMin <= xMax && zMin <= zMax); }
		int16		getXExtend() const { return xMax - xMin; }
		int16		getZExtend() const { return zMax - zMin; }
		bool		contains(const DTerrainRegion& rh) const { return (xMin <= rh.xMin && xMax >= rh.xMax && zMin <= rh.zMin && zMax >= rh.zMax); }
		bool		intersects(const DTerrainRegion& rh) const { return (xMin <= rh.xMax || xMax >= rh.xMin || zMin <= rh.zMax || zMax >= rh.zMin); }
		bool		isInside(int16 x, int16 z) const { return (x >= xMin && x <= xMax && z >= zMin && z <= zMax); }
		int16		xMin;
		int16		xMax;
		int16		zMin;
		int16		zMax;
	};

	// single render tile of one quadtree node.
	class DUEL_API DTerrainRenderNode : public DRenderable
	{
		DUEL_DECLARE_RTTI(DTerrainRenderNode)
	public:
		DTerrainRenderNode(DTerrainQuadTreeNode* parentNode);
		// override : DRenderable-----------------
		virtual DRenderLayout*	getRenderLayout();
		virtual DRenderTechnique*	getRenderTechnique(uint32 stage);
		virtual void	updateCustomGpuParameter(DShaderObject* so);
		virtual	void	getWorldTransform(DMatrix4& outMat);
		
		void		setMorphWeight(DReal weight);
	protected:
		DTerrainQuadTreeNode*	mParentNode;
		// renderable's required data.
		DRenderLayoutPtr		mRenderLayout;

		DReal					mMorphWeight;
		DMaterialInstancePtr	mMaterial;
	};

	class DUEL_API DTerrainQuadTreeNode : public DObject
	{
		DUEL_DECLARE_RTTI(DTerrainQuadTreeNode)
	public:
		// region is the space where this node should manage, batchSize is the
		// base on how this node should be divided, when the region's width/height is
		// smaller than the batchSize, this quadtree node will not be divided.
		DTerrainQuadTreeNode(DTerrainPage* parentPage, DTerrainQuadTreeNode* parentNode, DTerrainRegion region, uint16 batchSize);
		~DTerrainQuadTreeNode();

		DTerrainPage*	getParentPage();

		DReal			getMaxGeometryError() const;

		// this method will recursively update children nodes' bounding box.
		void			updateBoundingBox();
		DAxisAlignedBox	getBoundingBox() const; 


		// called when the terrain page re generate a new vertex data.
		void			generateIndexData();
		DIndexBufferPtr	getIndexBuffer();
		bool			isLeaf() const;

		void			applyToRenderQueue(const DCamera* cam, DReal K, DRenderQueue* queue, uint32 groupID);

		// reset the geometry error to 0.0f, only the node totally within given region
		// will be clear, this method will recurse to the children.
		void			clearGeometryError(const DTerrainRegion& region);
		// call this value to change geometry error for this node.
		void			modifyGeometryError(int16 x, int16 z, DReal gerror);
		// finalize the geometry error, make it validate(i.e, same depth nodes have same value,
		// deeper node has smaller geometry error)
		void			finalizeGeometryError(const DTerrainRegion& region);


	protected:
		DTerrainQuadTreeNode*	mParentNode;
		DTerrainQuadTreeNode*	mChildrenNode[4];
		DTerrainPage*	mParentPage;

		DTerrainRenderNode*		mRenderNode;

		DTerrainRegion	mRegion;
		uint16			mBatchSize;

		// the index data, used in drawing parent's page.
		DIndexBufferPtr	mIndexBuffer;

		// geometry error is the largest difference between full detail terrain and 
		// the terrain in this quadtree level. 
		DReal			mGeometryError;
		DAxisAlignedBox	mBoundingBox;

	};
}

#endif