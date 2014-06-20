//  [4/2/2013 OMEGA] Viewport会渐渐移除, 这个类不适应当前的架构需求. 新的类会命名为DViewport, 为了
//	达到多视口的效果, DViewport将适用于多个RenderTarget, 并且由RenderWorkShop统一管理.
//  [7/16/2012 OMEGA] created

#ifndef _DUELVIEWPORT_H_
#define _DUELVIEWPORT_H_

#include "DuelCommon.h"
#include "DuelCamera.h"


namespace Duel
{
	// the left, top, width, height range from 0 to the width of current render target.
	class DUEL_API DViewport : public DObject
	{
	DUEL_DECLARE_RTTI(DViewport)
	public:
		DViewport();
		DViewport(uint32 left, uint32 top, uint32 width, uint32 height);

		void				reset(uint32 left, uint32 top, uint32 width, uint32 height);
		uint32				getLeft() const { return mLeft; }
		uint32				getTop() const { return mTop; }
		uint32				getWidth() const { return mWidth; }
		uint32				getHeight() const { return mHeight; }

	protected:
		uint32				mLeft;
		uint32				mTop;
		uint32				mWidth;
		uint32				mHeight;
	};



}


#endif