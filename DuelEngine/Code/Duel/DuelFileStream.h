//  [8/20/2012 OMEGA] created


#ifndef _DUELFILESTREAM_H_
#define _DUELFILESTREAM_H_

#include "DuelCommon.h"
#include "DuelDataStream.h"


namespace Duel
{
	// provide basic functionality for I/O, simply a wrap of
	// std::basic_istream

	class DUEL_API DFileStream : public DDataStream
	{
	DUEL_DECLARE_RTTI(DFileStream)
	public:
		// use full-path name to open this stream.
		DFileStream(const DString& file, DataAccess access = DA_Read);

		virtual ~DFileStream();

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
		size_t		tell();
		// override: DDataStream----------------
		bool		eof() const;
		// override: DDataStream----------------
		size_t		getSize() const;
		// override: DDataStream----------------
		void		close();

	protected:
		// read-write
		std::fstream	mFStream;
		// file size in byte, never bigger than 4GB.
		uint32		mSize;

	};
}

#endif