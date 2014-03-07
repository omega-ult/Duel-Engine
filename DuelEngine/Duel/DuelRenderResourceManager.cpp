//  [10/30/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelHardwareBuffer.h"
#include "DuelRenderResourceFactory.h"
#include "DuelRenderResourceManager.h"
#include "DuelRenderWindow.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DRenderResourceManager, DObject);

	DRenderResourceManager::DRenderResourceManager( DRenderResourceFactory* factory ) :
		mFactory(factory)
	{
		
	}

	DVertexBufferPtr DRenderResourceManager::createVetexBuffer( size_t vertexSize, size_t verticesCount,
			HardwareBufferUsage usage, bool useShadow, VertexBufferType type/* = VBT_GEOMETRY*/ )
	{
		return mFactory->createVetexBuffer(vertexSize, verticesCount, usage, useShadow, type);
	}

	DIndexBufferPtr DRenderResourceManager::createIndexBuffer( IndexType itype, size_t indexCount, HardwareBufferUsage usage, bool useShadow )
	{
		return mFactory->createIndexBuffer(itype, indexCount, usage, useShadow);
	}

	Duel::DRenderLayoutPtr DRenderResourceManager::createRenderLayout()
	{
		return mFactory->createRenderLayout();
	}

	Duel::DRasterizerStateObjectPtr DRenderResourceManager::createRasterizerStateObject( const DRasterizerState& state )
	{
		return mFactory->createRasterizerStateObject(state);
	}

	Duel::DDepthStencilStateObjectPtr DRenderResourceManager::createDepthStencilStateObject( const DDepthStencilState& state )
	{
		return mFactory->createDepthStencilStateObject(state);
	}

	Duel::DBlendStateObjectPtr DRenderResourceManager::createBlendStateObject( const DBlendState& state )
	{
		return mFactory->createBlendStateObject(state);
	}

	Duel::DShaderObjectPtr DRenderResourceManager::createShaderObject( DRenderPass* pass )
	{
		return mFactory->createShaderObject(pass);
	}

	DRenderWindow* DRenderResourceManager::createRenderWindow( const DString& name, const RenderWindowSetting& WndSetting, uint32 targetWnd )
	{
		RenderWindowSet::iterator i, iend = mRenderWindowSet.end();
		for (i = mRenderWindowSet.begin(); i != iend; ++i)
		{
			if ((*i)->getName() == name)
			{
				return *i;
			}
		}
		DRenderWindow* ret = mFactory->createRenderWindow(name);
		ret->create(name, WndSetting, targetWnd);
		mRenderWindowSet.insert(ret);
		return ret;
	}

	void DRenderResourceManager::destroyRenderWindow( DRenderWindow* wind )
	{
		RenderWindowSet::iterator i = mRenderWindowSet.find(wind);
		if (i != mRenderWindowSet.end())
		{
			mFactory->destroyRenderWindow(wind);
			mRenderWindowSet.erase(i);
		}
	}

	DFrameBuffer* DRenderResourceManager::createFrameBuffer( uint32 w, uint32 h, uint32 colorBits )
	{
		return mFactory->createFrameBuffer(w, h, colorBits);
	}

	void DRenderResourceManager::destroyFrameBuffer( DFrameBuffer* buf )
	{
		mFactory->destroyFrameBuffer(buf);
	}

	DRawBufferPtr DRenderResourceManager::createRawBuffer( DHardwareBuffer* src )
	{
		return DRawBufferPtr(new DRawBuffer(src));
	}

	DRawBufferPtr DRenderResourceManager::createRawBuffer( size_t size )
	{
		return DRawBufferPtr(new DRawBuffer(size));
	}

}