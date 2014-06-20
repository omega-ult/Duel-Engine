//  [8/8/2012 OMEGA] created

#ifndef _DUELRENDERLAYOUT_H_
#define _DUELRENDERLAYOUT_H_

#include "DuelCommon.h"
#include "DuelIndexData.h"
#include "DuelPixelFormat.h"
#include "DuelVertexData.h"
#include "DuelRenderResourceFactory.h"

namespace Duel
{

	enum PrimitiveTopology
	{
		PT_PointList,
		PT_LineList,
		PT_LineStrip,
		PT_TriangleList,
		PT_TriangleStrip,
		PT_TriangleFan
	};


	// renderlayout, used for binding the vertex/index in the rendering pipeline
	class DUEL_API DRenderLayout : public DObject
	{
	DUEL_DECLARE_RTTI(DRenderLayout)
	public:
		DRenderLayout(DRenderResourceFactory* creator);
		virtual ~DRenderLayout();


		PrimitiveTopology	getTopologyType() { return mPrimitiveTopology; }
		void				setTopologyType( PrimitiveTopology pt ) { mPrimitiveTopology = pt; }

		// use a new vertex buffer to replace an existed one
		void				setVertexData(DVertexData data) { mVertexData = data; }
		const DVertexData&	getVertexData() const { return mVertexData; }

		// set index stream.
		void				setIndexData(DIndexData ib) { mIndexData = ib; }
		const DIndexData&	getIndexData() const { return mIndexData; }

		// seal the data and try to create hardware-related resource so
		// that the render system can quickly bind it.
		virtual	void		seal() {}

	protected:

		DRenderResourceFactory*	mCreator;

		PrimitiveTopology	mPrimitiveTopology;

		DVertexData			mVertexData;

		DIndexData			mIndexData;
	};

}
#endif