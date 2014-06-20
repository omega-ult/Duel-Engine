//  [8/21/2012 OMEGA] created

#ifndef _DUELMEMORYSTREAM_H_
#define _DUELMEMORYSTREAM_H_

#include "DuelCommon.h"
#include "DuelDataStream.h"

namespace Duel
{
	// a memory stream is a wrap of a chunk memory, it provides
	// functions similiar to a standard stream, make sure to free
	// memory after close, or enable auto free if you want the stream
	// to handle it, WARNING: if use DMemoryStream(size_t size, DataAccess da, bool autoFree(false))
	// or DMemoryStream(const DString& name, size_t size, DataAccess da, bool autoFree(false))
	// and for get to release it may cause memory leak!
	class DUEL_API DMemoryStream : public DDataStream
	{
	DUEL_DECLARE_RTTI(DMemoryStream)
	public:
		// initialize the memory stream with specified data.
		DMemoryStream(DDataStream* src, DataAccess da = DA_Write, bool autoFree = true);
		/** Wrap an existing memory chunk in a stream.
		@param pMem Pointer to the existing memory
		@param size The size of the memory chunk in bytes.
		@param da Whether to make the stream on this memory read-only once created
		@param autoFree Indicate whether to free memory when close
		*/
		DMemoryStream(void* pMem, size_t size, DataAccess da, bool autoFree = true);

		/** Wrap an existing memory chunk in a stream.
		@param name The name given to the stream
		@param pMem Pointer to the existing memory
		@param size The size of the memory chunk in bytes.
		@param da Whether to make the stream on this memory read-only once created
		@param autoFree Indicate whether to free memory when close
		*/
		DMemoryStream(const DString& name, void* pMem, size_t size, DataAccess da, bool autoFree = true);

		/** Create a stream with a brand new empty memory chunk.
		@param size The size of the memory chunk to create in bytes
		@param da Whether to make the stream on this memory read-only once
		@param autoFree Indicate whether to free memory when close created
		*/
		DMemoryStream(size_t size, DataAccess da, bool autoFree = true);

		/** Create a stream with a brand new empty memory chunk.
		@param name The name given to the stream
		@param size The size of the memory chunk to create in bytes
		@param da Whether to make the stream on this memory read-only once created
		@param autoFree Indicate whether to free memory when close
		*/
		DMemoryStream(const DString& name, size_t size, DataAccess da, bool autoFree = true);

		virtual ~DMemoryStream();

		// get the memory chunk
		char*		getChunkPtr();
		const char*	getConstChunkPtr();

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

		// make the stream auto free
		void		enableAutoFree() { mbAutoFree = true;}

		bool		isAutoFree() { return mbAutoFree;}

		// if you don't wan auto free
		void		disableAutoFree() { mbAutoFree = false;}

	protected:
		// data pointer
		char*	mData;
		// pointer to the current position of the data
		char*	mPos;
		// pointer to the end of the data
		char*	mEnd;
		// size of the data, in byte, never bigger than 4GB
		uint32	mSize;
		// need we free memory when close?
		bool	mbAutoFree;
	};

}

#endif