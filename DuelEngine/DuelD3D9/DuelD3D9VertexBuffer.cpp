//  [6/7/2014 OMEGA] created

#include "DuelD3D9Common.h"
#include "DuelD3D9VertexBuffer.h"
#include "DuelD3D9RenderResourceFactory.h"
#include "DuelD3D9Include.h"
#include "DuelD3D9Translator.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(D3D9VertexBuffer, DVertexBuffer);

	D3D9VertexBuffer::D3D9VertexBuffer(D3D9RenderResourceFactory* fact, size_t vertexSize, size_t verticesCount, HardwareBufferUsage usage, bool useShadow, VertexBufferType type /*= VBT_Geometry*/ ) :
		DVertexBuffer(vertexSize, verticesCount, usage, useShadow),
		mVertBuf(NULL),
		mShadowBuf(NULL)
	{
		if (mbUseShadow)
		{
			mShadowBuf = new char[mByteSize];
			memset(mShadowBuf,0,mByteSize);
		}
		IDirect3DDevice9* dev = fact->getMainDevice();
		dev->CreateVertexBuffer(mByteSize, D3D9Translator::getD3DUsage(mUsage), NULL, D3DPOOL_MANAGED,
			&mVertBuf, NULL);
	}


	D3D9VertexBuffer::~D3D9VertexBuffer()
	{
		ReleaseCOM(mVertBuf);
		if (mbUseShadow)
		{
			delete[] mShadowBuf;
		}
	}

	void D3D9VertexBuffer::readData( size_t offset, size_t length, void* pDest )
	{
		void* pSrc = lock(offset, length, HBL_ReadOnly);
		memcpy(pDest, pSrc, length);
		this->unlock();	
	}

	void D3D9VertexBuffer::writeData( size_t offset, size_t length, const void* pSource, bool discardWholeBuffer /*= false */ )
	{
		HardwareBufferLock hblock = discardWholeBuffer? HBL_Discard : HBL_Normal;
		void* pDest = lock(offset, length, hblock);
		memcpy(pDest, pSource, length);
		this->unlock();	
	}

	IDirect3DVertexBuffer9* D3D9VertexBuffer::getVertexBuffer()
	{
		return mVertBuf;
	}

	void* D3D9VertexBuffer::lockImpl( size_t offset, size_t size, HardwareBufferLock lockType )
	{
		if (mbIsLocked)
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InternalError,
				"Invalid attempt to lock an vertex buffer that has already been locked",
				"Duel::GLVertexBuffer::lockImpl");
		}
		mLockType = lockType;

		if (mbUseShadow)
		{
			return mShadowBuf + offset;
		}
		void* retPtr = 0;
		mVertBuf->Lock(offset, size, &retPtr, D3D9Translator::getD3DLock(mLockType, mUsage));
		return retPtr;
	}

	void D3D9VertexBuffer::unlockImpl()
	{
		if (mLockType == HBL_ReadOnly || mLockType == HBL_NoOverwrite)
		{
			if (!mbUseShadow)
			{
				mVertBuf->Unlock();
			}
		}
		else
		{
			if (mbUseShadow)
			{
				void* pBuf = 0;
				mVertBuf->Lock(0, mByteSize, &pBuf, D3DLOCK_DISCARD);
				memcpy(pBuf, mShadowBuf, mByteSize);
			}
			mVertBuf->Unlock();
		}
	}

}