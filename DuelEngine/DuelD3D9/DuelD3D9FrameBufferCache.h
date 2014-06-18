//  [6/18/2014 OMEGA] created

#ifndef _DUELD3D9FRAMEBUFFERCACHE_H_
#define _DUELD3D9FRAMEBUFFERCACHE_H_

#include "DuelCore.h"
#include "DuelRenderSystem.h"
#include "DuelFrameBuffer.h"

namespace Duel
{
	// this class do simple work: attath cached frame buffer to the
	// render system when destruction happened.
	struct D3D9FrameBufferCache
	{
		D3D9FrameBufferCache(DRenderSystem* sys, DFrameBuffer* fb)
		{
			renderSystem = sys;
			cacheFrameBuffer = fb;
		}
		~D3D9FrameBufferCache()
		{
			renderSystem->bindFrameBuffer(cacheFrameBuffer);
		}
		DRenderSystem*	renderSystem;
		DFrameBuffer*	cacheFrameBuffer;
	};

}


#endif