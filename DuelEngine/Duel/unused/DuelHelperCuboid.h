//  [6/23/2013 OMEGA] created

#ifndef _DUELHELPERCUBOID_H_
#define _DUELHELPERCUBOID_H_

#include "DuelCommon.h"
#include "DuelRenderable.h"
#include "DuelMovable.h"

namespace Duel
{
// 
// 	class DHCuboidRend : public DRenderable
// 	{
// 	DUEL_DECLARE_RTTI(DHCuboidRend)
// 	public:
// 		DHCuboidRend(DHCuboid* parent, DReal width, DReal height, DReal depth );
// 		// reset the height/width/depth of the cuboid.
// 		void					reset(DReal width = 1.0f, DReal height = 1.0f, DReal depth = 1.0f);
// 		void					setColor(const DColor& color);
// 		// 不做defer shading, 总是返回空.
// 		DRenderTechnique*		getDeferRenderTechinique(DeferRenderStage stage) { return NULL; }
// 		// override : DRenderable-------------------------
// 		bool					isDeferRenderEnabled() const { return false; }
// 		// override : DRenderable-------------------------
// 		DRenderLayout*			getRenderLayout();
// 		// override : DRenderable-------------------------
// 		DRenderTechnique*		getForwardRenderTechnique();
// 		// override : DRenderable-------------------------
// 		virtual DReal			getSquaredViewDepthToCamera(const DCamera* cam);
// 		// override : DRenderable-------------------------
// 		void					updateCustomGpuParameter();
// 		// override : DRenderable-------------------------
// 		void					getWorldTransform(DMatrix4* outMat);
// 		// override : DRenderable-------------------------
// 		void					postRender();
// 	protected:
// 		DHCuboid*				mParent;
// 		DReal					mLength;
// 		DReal					mUnit;
// 
// 		const DCamera*			mCacheCamera;
// 		DReal					mCacheDepth;
// 
// 		// 實際存儲的頂點信息, 會從SubMesh資源裏面拷貝出來.
// 		DVertexStreams			mVStreams;
// 		DVertexDeclarationPtr	mVDeclare;
// 		DIndexBufferPtr			mIndices;
// 		DRenderLayout			mRenderLayout;
// 		DRenderEffectPtr		mRenderEffect;
// 		DColor					mColor;
// 
// 	};
// 
// 	class DHCuboid : public DMovable
// 	{
// 	DUEL_DECLARE_RTTI(DHCuboid)
// 	public:
// 		DHCuboid(DMovableFactory* creator, const DString& name,
// 			DReal width = 1.0f, DReal height = 1.0f, DReal depth = 1.0f);
// 		~DHCuboid();
// 		// reset the height/width/depth of the cuboid.
// 		void			reset(DReal width = 1.0f, DReal height = 1.0f, DReal depth = 1.0f);
// 		void			setColor(const DColor& color);
// 		// override:DMovable---------------------------------
// 		void			applyToRenderQueue(DRenderQueue* destQueue);
// 
// 	protected:
// 		DHCuboidRend*	mRendComp;
// 	};
}

#endif