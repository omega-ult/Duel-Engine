//  [6/7/2014 OMEGA] created

#include "DuelD3D9Common.h"
#include "DuelD3D9IndexBuffer.h"
#include "DuelD3D9RenderResourceFactory.h"
#include "DuelD3D9Include.h"
#include "DuelD3D9Translator.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(D3D9IndexBuffer, DIndexBuffer);

	
	D3D9IndexBuffer::D3D9IndexBuffer( D3D9RenderResourceFactory* fact, IndexType type, size_t indexCount, HardwareBufferUsage usage, bool useShadow ) :
		DIndexBuffer(type, indexCount, usage, useShadow),
		mIndxBuf(NULL),
		mShadowBuf(NULL)
	{
		if (mbUseShadow)
		{
			mShadowBuf = new char[mByteSize];
			memset(mShadowBuf,0,mByteSize);
		}
		IDirect3DDevice9* dev = fact->getMainDevice();
		D3DFORMAT indxFmt;
		if (type == IT_16Bit)
		{
			indxFmt = D3DFMT_INDEX16;
		}
		else
		{
			indxFmt = D3DFMT_INDEX32;
		}
		dev->CreateIndexBuffer(mByteSize, 0, indxFmt, D3DPOOL_MANAGED,
			&mIndxBuf, NULL);
	}

	D3D9IndexBuffer::~D3D9IndexBuffer()
	{
		ReleaseCOM(mIndxBuf);
		if (mbUseShadow)
		{
			delete[] mShadowBuf;
		}
	}

	void D3D9IndexBuffer::readData( size_t offset, size_t length, void* pDest )
	{
		void* pSrc = lock(offset, length, HBL_ReadOnly);
		memcpy(pDest, pSrc, length);
		this->unlock();	
	}

	void D3D9IndexBuffer::writeData( size_t offset, size_t length, const void* pSource, bool discardWholeBuffer /*= false */ )
	{
		HardwareBufferLock hblock = discardWholeBuffer? HBL_Discard : HBL_Normal;
		void* pDest = lock(offset, length, hblock);
		memcpy(pDest, pSource, length);
		this->unlock();	
	}

	IDirect3DIndexBuffer9* D3D9IndexBuffer::getD3DIndexBuffer()
	{
		return mIndxBuf;
	}

	void* D3D9IndexBuffer::lockImpl( size_t offset, size_t size, HardwareBufferLock lockType )
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
		mIndxBuf->Lock(offset, size, &retPtr, D3D9Translator::getD3DLock(mLockType, mUsage));
		return retPtr;
	}

	void D3D9IndexBuffer::unlockImpl()
	{
		if (mLockType == HBL_ReadOnly || mLockType == HBL_NoOverwrite)
		{
			if (!mbUseShadow)
			{
				mIndxBuf->Unlock();
			}
		}
		else
		{
			if (mbUseShadow)
			{
				void* pBuf = 0;
				mIndxBuf->Lock(0, mByteSize, &pBuf, D3DLOCK_DISCARD);
				memcpy(pBuf, mShadowBuf, mByteSize);
			}
			mIndxBuf->Unlock();
		}
	}

}