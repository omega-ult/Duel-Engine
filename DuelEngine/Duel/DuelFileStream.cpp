//  [8/20/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelFileStream.h"
#include "DuelLogmanager.h"

// stl type check warning
#pragma warning(push)
#pragma warning(disable : 4715)

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(DFileStream, DDataStream)

	DFileStream::DFileStream( const DString& file, DataAccess access /*= DA_READ*/ ) :
		DDataStream(file, access)
	{
		mName = file;
		if (access == DA_Read)
		{
			mFStream.open(file, std::ios_base::binary | std::ios_base::in);
		}
		else
		{
			mFStream.open(file, std::ios_base::binary | std::ios_base::in | std::ios_base::out);
		}

		if (mFStream.fail())
		{
			mbValid = false;
			DLogManager::getSingleton().logMessage("Open file failed : " + file);
		}
		else
		{
			mbValid = true;
			mFStream.seekg(0, std::ios_base::end);
			mSize = (size_t)mFStream.tellg();
			mFStream.seekg(0, std::ios_base::beg);
		}
	}

	DFileStream::~DFileStream()
	{
		close();
	}

	size_t DFileStream::read( void* buf, size_t length )
	{
		if (mbValid)
		{
			mFStream.read(static_cast<char*>(buf), static_cast<std::streamsize>(length));
			return static_cast<size_t>(mFStream.gcount());
		}
		return 0;
	}

	size_t DFileStream::write( const void* buf, size_t length )
	{
		if(mAccess == DA_Read)
		{
			return 0;
		}
		else if(mAccess == DA_Write)
		{
			if (mbValid)
			{
				mFStream.write(static_cast<const char*>(buf), static_cast<std::streamsize>(length));
				return length;
			}
		}
		return 0;
	}

	size_t DFileStream::readLine( char* buf, size_t maxLength, const DString& delim /*= "\n"*/ )
	{
		if(delim.empty() || !mbValid)
		{
			*buf = NULL;
			return 0;
		}
		if(delim.size() > 1)
		{
			DLogManager::getSingleton().logMessage(
				"WARNING: DFileStream::readLine - delimeter too long, using only first delimeter");
		}
		// Deal with both Unix & Windows LFs
		bool trimCR = false;
		if( delim.at(0) == '\n')
		{
			trimCR = true;
		}
		size_t ret;
		// maxLength + 1 since count excludes terminator in getline

		// three options
		// 1) we had an eof before we read a whole line
		// 2) we ran out of buffer space
		// 3) we read a whole line - in this case the delim character is taken from the stream but not written in the buffer so the read data is of length ret-1 and thus ends at index ret-2
		// in all cases the buffer will be null terminated for us


		mFStream.getline(buf, static_cast<std::streamsize>(maxLength+1), delim.at(0) );
		ret = static_cast<size_t>(mFStream.gcount());
		if(mFStream.eof())
		{
			// no problem
		}
		else if(mFStream.fail())
		{
			// Did we fail because of maxLength hit? No - no terminating character
			// in included in the count in this case
			if (ret == maxLength)
			{
				// clear failbit for next time 
				mFStream.clear();
			}
			else
			{
				DUEL_EXCEPT(Duel::DException::ET_InternalError, 
					"Streaming error occurred",
					"buffer length is too small",
					"Duel::FileStreamDataStream::readLine");
			}
		}
		else 
		{
			// we need to adjust ret because we want to use it as a
			// pointer to the terminating null character and it is
			// currently the length of the data read from the stream
			// i.e. 1 more than the length of the data in the buffer and
			// hence 1 more than the _index_ of the NULL character
			--ret;
		}

		// trim off CR if we found CR/LF
		if (trimCR && buf[ret-1] == '\r')
		{
			--ret;
			buf[ret] = '\0';
		}
		return ret;

	}

	void DFileStream::skip( long length )
	{


#if defined(STLPORT)
		// Workaround for STLport issues: After reached eof of file stream,
		// it's seems the stream was putted in intermediate state, and will be
		// fail if try to repositioning relative to current position.
		// Note: tellg() fail in this case too.

		if(mCurStream->eof())
		{
			mCurStream->clear();
			// Use seek relative to either begin or end to bring the stream
			// back to normal state.
			mCurStream->seekg(0, std::ios::end);
		}

		
#endif 	
		if (mbValid)
		{
			mFStream.clear(); //Clear fail status in case eof was set
			mFStream.seekg(static_cast<std::ifstream::pos_type>(length), std::ios::cur);
		}

	}

	void DFileStream::seek( size_t pos )
	{
		if (mbValid)
		{
			mFStream.clear(); //Clear fail status in case eof was set
			mFStream.seekg(static_cast<std::ifstream::pos_type>(pos), std::ios::beg);
		}

	}

	size_t DFileStream::tell() 
	{
		if (!mbValid)
		{
			return 0;
		}
		mFStream.clear(); //Clear fail status in case eof was set
		return (size_t)mFStream.tellg();

	}

	bool DFileStream::eof() const
	{
		if (mbValid)
		{
			return mFStream.eof();
		}
		return true;
	}

	size_t DFileStream::getSize() const
	{
		return mSize;
	}

	void DFileStream::close()
	{
		mFStream.close();
		mbValid = false;
	}

}

#pragma warning(pop)