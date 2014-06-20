//  [8/21/2012 OMEGA] created


#include "DuelCommon.h"
#include "DuelDataStream.h"
#include "DuelMemoryStream.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DMemoryStream, DDataStream);

	DMemoryStream::DMemoryStream( DDataStream* src, DataAccess da/* = DA_WRITE*/, bool autoFree/* = true*/) :
		DDataStream(da),
		mData(NULL),
		mSize(0),
		mPos(NULL),
		mEnd(NULL)
	{
		if (src != NULL)
		{
			if (src->isValid())
			{
				mName = src->getName();
				size_t size = src->getSize();
				mSize = size;
				mbAutoFree = autoFree;
				mData = mPos = new char[size];
				mEnd  = mData + size;
				src->read(mData, size);
				mbValid = true;
			}
		}
	}

	DMemoryStream::DMemoryStream( void* pMem, size_t size, DataAccess da, bool autoFree /*= true*/ ) :
		DDataStream(da),
		mSize(size),
		mbAutoFree(autoFree)
	{
		mData = mPos = static_cast<char*>(pMem);
		mEnd  = mData + size;
		mbValid = true;
	}

	DMemoryStream::DMemoryStream( const DString& name, void* pMem, size_t size, DataAccess da, bool autoFree /*= true*/ ) :
		DDataStream(name, da),
		mSize(size),
		mbAutoFree(autoFree)
	{
		mData = mPos = static_cast<char*>(pMem);
		mEnd  = mData + size;
		mbValid = true;
	}

	DMemoryStream::DMemoryStream( size_t size, DataAccess da, bool autoFree /*= true*/ ) :
		DDataStream(da),
		mSize(size),
		mbAutoFree(autoFree)
	{
		mData = mPos = new char[size];
		mEnd  = mData + size;
		mbValid = true;
	}

	DMemoryStream::DMemoryStream( const DString& name, size_t size, DataAccess da, bool autoFree /*= true*/ ) :
		DDataStream(name, da),
		mSize(size),
		mbAutoFree(autoFree)
	{
		mData = mPos = new char[size];
		mEnd  = mData + size;
		mbValid = true;
	}

	DMemoryStream::~DMemoryStream()
	{
		close();
	}

	size_t DMemoryStream::read( void* buf, size_t length )
	{
		if (mbValid)
		{
			size_t	l = length;
			// Read over end of memory?
			if(mPos + l > mEnd)
			{
				l = mEnd - mPos;
			}
			if (l == 0)
			{
				return 0;
			}

			assert(l <= length);

			memcpy(buf, mPos, l);
			mPos += l;
			return l;
		}
		return 0;
	}

	size_t DMemoryStream::write( const void* buf, size_t length )
	{
		if (mbValid)
		{
			size_t	written = 0;
			if(isWritable())
			{
				written = length;
				// we only allow writing within the extents of allocated memory
				// check for buffer overrun & disallow
				if(mPos + written > mEnd)
				{
					written = mEnd - mPos;
				}
				if(written == 0)
				{
					return 0;
				}

				memcpy(mPos, buf, written);
				mPos += written;
			}
			return written;
		}
		return 0;
	}

	size_t DMemoryStream::readLine( char* buf, size_t maxLength, const DString& delim /*= "\n"*/ )
	{
		if(delim.empty() || !mbValid)
		{
			*buf = NULL;
			return 0;
		}
		// Deal with both Unix & Windows LFs
		bool trimCR = false;
		if(delim.find_first_of('\n') != DString::npos)
		{
			trimCR = true;
		}

		size_t pos = 0;

		// Make sure pos can never go past the end of the data 
		while(pos < maxLength && mPos < mEnd)
		{
			if(delim.find(*mPos) != DString::npos)
			{
				// Trim off trailing CR if this was a CR/LF entry
				if( trimCR && pos && buf[pos - 1] == '\r')
				{
					// terminate 1 character early
					--pos;
				}

				// Found terminator, skip and break out
				++mPos;
				break;
			}
			buf[pos++] = *mPos++;
		}

		// terminate
		buf[pos] = '\0';

		return pos;
	}

	void DMemoryStream::skip( long length )
	{
		if (mbValid)
		{
			// make sure we will never cross boundary
			size_t newpos = (size_t)((mPos - mData) + length);
			mPos = (mData + newpos <= mEnd) ? mData+newpos : mEnd;
		}
	}

	void DMemoryStream::seek( size_t pos )
	{
		if (mbValid)
		{
			// make sure we will never cross boundary
			mPos = (mData + pos <= mEnd) ? mData+pos : mEnd;
		}
	}

	size_t DMemoryStream::tell()
	{
		if (mbValid)
		{
			return mPos - mData;
		}
		return 0;
	}

	bool DMemoryStream::eof() const
	{
		if (mbValid)
		{
			return mPos >= mEnd;
		}
		return true;
	}

	size_t DMemoryStream::getSize() const
	{
		if (mbValid)
		{
			return mSize;
		}
		return 0;
	}

	void DMemoryStream::close()
	{
		if(mbAutoFree && mData)
		{
			delete[] mData;
			mData = NULL;
		}
		mbValid = false;
	}

	char* DMemoryStream::getChunkPtr()
	{
		if (mbValid)
		{
			return mData;
		}
		return NULL;
	}

	const char* DMemoryStream::getConstChunkPtr()
	{
		if (mbValid)
		{
			return mData;
		}
		return NULL;
	}

}