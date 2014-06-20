//  [3/12/2013 OMEGA] created

#ifndef _DUELFILESTREAMWP8_H_
#define _DUELFILESTREAMWP8_H_

#include "DuelCommon.h"
#include "DuelDataStream.h"
#include "DuelMemoryStream.h"

namespace Duel
{
#ifdef DUEL_PLATFORM_WINDOWS_PHONE_8

	class DUEL_API FileStreamWP8 : public DDataStream
	{
	DUEL_DECLARE_RTTI(FileStreamWP8)
	public:
		// create a read-only stream from an relative file name.
		// i.e "Assets\\sss.xml"
		FileStreamWP8(const DString& fileName, DataAccess da = DA_READ);

		~FileStreamWP8();

		// override: DDataStream----------------
		size_t		read(void* buf, size_t length);
		// override: DDataStream----------------
		size_t		write(const void* buf, size_t length);
		// override: DDataStream----------------
		size_t		readLine(char* buf, size_t maxLength, const DString& delim = "\n");
		// override: DDataStream----------------
		void		skip(long length);
		// override: DDataStream----------------
		void		seek(size_t pos);
		// override: DDataStream----------------
		size_t		tell() const;
		// override: DDataStream----------------
		bool		eof() const;
		// override: DDataStream----------------
		size_t		getSize() const;
		// override: DDataStream----------------
		void		close();

	protected:
		DString		mFileName;
		HANDLE		mFileHandle;
		// -0- 投机取巧的用这个代替吧, 全读出来再弄.
		DMemoryStream*	mContentStream;

	};


#endif // DUEL_PLATFORM_WINDOWS_PHONE_8
}

#endif