//  [3/12/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelException.h"
#include "DuelMemoryStream.h"
#include "DuelFileStreamWP8.h"

#ifdef DUEL_PLATFORM_WINDOWS_PHONE_8


using namespace Microsoft::WRL;
using namespace Windows::Storage;
using namespace Windows::Storage::FileProperties;
using namespace Windows::Storage::Streams;
using namespace Windows::ApplicationModel;

namespace Duel
{

	DUEL_IMPLEMENT_RTTI(FileStreamWP8, DDataStream);


	FileStreamWP8::FileStreamWP8( const DString& fileName, DataAccess da /* = DA_READ*/) :
		DDataStream(da),
		mFileName(fileName),
		mContentStream(NULL)
	{
		Windows::Storage::StorageFolder^ systemLocation = Package::Current->InstalledLocation;
		Platform::String^ systemLocationPath = Platform::String::Concat(systemLocation->Path, "\\");
		
		DWString	tmp;
		DStringTool::Convert(tmp, fileName);
		Platform::String^ fn = ref new Platform::String(tmp.c_str(), tmp.length());
// 		
// 		std::wifstream s;
// 		s.open(tmp.c_str(), std::ios_base::binary);
// 		if (s.is_open())
// 		{
// 			uint32 h = 0;
// 		}
		
		DWORD	openFlag = da == DA_READ ? GENERIC_READ : GENERIC_WRITE | GENERIC_READ;
		fn = Platform::String::Concat(systemLocationPath, fn);
		mFileHandle = CreateFile2(
			fn->Data(),
			openFlag,
			da == DA_READ? FILE_SHARE_READ : NULL,
			OPEN_ALWAYS,
			NULL
			);
		if (mFileHandle == INVALID_HANDLE_VALUE)
		{
			DUEL_EXCEPT_BRIEF(DException::ET_ITEM_NOT_FOUND,
				"Failed to open file : " + fileName,
				"Duel::FileStreamWP8")
		}

		FILE_STANDARD_INFO fileInfo;
		ZeroMemory(&fileInfo, sizeof(FILE_STANDARD_INFO));

		if (!GetFileInformationByHandleEx(
			mFileHandle,
			FileStandardInfo,
			&fileInfo,
			sizeof(fileInfo)))
		{
			DUEL_EXCEPT_BRIEF(DException::ET_INTERNAL_ERROR,
				"Can not get file info : " + mFileName,
				"Duel::FileStreamWP8")
		}
		if (fileInfo.EndOfFile.HighPart != 0)
		{
			DUEL_EXCEPT_BRIEF(DException::ET_INTERNAL_ERROR,
				"Out of memory : " + mFileName,
				"Duel::FileStreamWP8")
		}
		size_t contentSize = fileInfo.EndOfFile.LowPart;
		mContentStream = new DMemoryStream(fileName, contentSize, da);

		if (!ReadFile(
			mFileHandle,
			mContentStream->getChunkPtr(),
			contentSize,
			NULL,
			NULL))
		{
			DUEL_EXCEPT_BRIEF(DException::ET_INTERNAL_ERROR,
				"Can not read data from : " + mFileName,
				"Duel::FileStreamWP8")
		}
	}

	FileStreamWP8::~FileStreamWP8()
	{
		close();
	}

	size_t FileStreamWP8::read( void* buf, size_t length )
	{
		return mContentStream->read(buf, length);
	}

	size_t FileStreamWP8::write( const void* buf, size_t length )
	{
		return mContentStream->write(buf, length);
	}

	size_t FileStreamWP8::readLine( char* buf, size_t maxLength, const DString& delim /*= "\n"*/ )
	{
		return mContentStream->readLine(buf, maxLength, delim);
	}

	void FileStreamWP8::skip( long length )
	{
		mContentStream->skip(length);
	}

	void FileStreamWP8::seek( size_t pos )
	{
		mContentStream->seek(pos);
	}

	size_t FileStreamWP8::tell() const
	{
		return mContentStream->tell();
	}

	bool FileStreamWP8::eof() const
	{
		return mContentStream->eof();
	}

	size_t FileStreamWP8::getSize() const
	{
		return mContentStream->getSize();
	}

	void FileStreamWP8::close()
	{
		if(mContentStream != NULL)
		{
			DWORD numBytesWritten;
			if (mAccess == DA_WRITE)
			{
				if (!WriteFile(
					mFileHandle,
					mContentStream->getChunkPtr(),
					mContentStream->getSize(),
					&numBytesWritten,
					NULL
					) || numBytesWritten != mContentStream->getSize())
				{
					DUEL_EXCEPT_BRIEF(DException::ET_INTERNAL_ERROR,
						"Can not write data to : " + mFileName,
						"Duel::FileStreamWP8::close")
				}
			}
			delete mContentStream;
		}
		CloseHandle(mFileHandle);
	}

}

#endif // DUEL_PLATFORM_WINDOWS_PHONE_8
