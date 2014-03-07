//  [6/22/2013 OMEGA] created

#ifndef _DUELHELPERPLANEGRID_H_
#define _DUELHELPERPLANEGRID_H_


#include "DuelCommon.h"
#include "DuelRenderable.h"
#include "DuelMovable.h"

namespace Duel
{
// 	class DHPlaneGrid;
// 	class DHGrid : public DRenderable
// 	{
// 	DUEL_DECLARE_RTTI(DHGrid)
// 	public:
// 		DHGrid(DHPlaneGrid* parent, DReal longtitude, DReal unit);
// 		// 重置区域.
// 		void					reset(DReal longtitude, DReal unit);
// 		void					setColor(const DColor& color);
// 		// override : DRenderable-------------------------
// 		bool					isDeferRenderEnabled() const { return false; }
// 		// override : DRenderable-------------------------
// 		DRenderLayout*			getRenderLayout();
// 		// override : DRenderable-------------------------
// 		DRenderTechnique*		getForwardRenderTechnique();
// 		// override : DRenderable-------------------------
// 		void					updateCustomGpuParameter();
// 		// override : DRenderable-------------------------
// 		void					getWorldTransform(DMatrix4& outMat);
// 	protected:
// 		DHPlaneGrid*			mParent;
// 		DReal					mLength;
// 		DReal					mUnit;
// 
// 
// 		// 實際存儲的頂點信息, 會從SubMesh資源裏面拷貝出來.
// 		DVertexStreams			mVStreams;
// 		DVertexDeclaration		mVDeclare;
// 		DIndexBufferPtr			mIndices;
// 		DRenderLayoutPtr		mRenderLayout;
// 		DRenderEffectPtr		mRenderEffect;
// 		DColor					mColor;
// 	};
// 
// 	// a grid plane base on xz-plane
// 	class DUEL_API DHPlaneGrid : public DMovable
// 	{
// 	DUEL_DECLARE_RTTI(DHPlaneGrid)
// 	public:
// 		DHPlaneGrid(DMovableFactory* creator, const DString& name,
// 			DReal longtitude, DReal unit = 10.0f);
// 		~DHPlaneGrid();
// 		// reset the height/width of the grid.
// 		void			reset(DReal longtitude, DReal unit);
// 		void			setColor(const DColor& color);
// 		// override:DMovable---------------------------------
// 		void			applyToRenderQueue(DRenderQueue* destQueue);
// 
// 	protected:
// 		DHGrid*			mGrid;
// 	};


}

#endif