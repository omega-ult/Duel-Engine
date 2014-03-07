//  [8/20/2012 OMEGA] created

#ifndef _DUELDATASTREAM_H_
#define _DUELDATASTREAM_H_

#include "DuelString.h"

namespace Duel
{
	// base stream class usd to define interface for I/O
	// subclass must implement on specified environment
	// such as read/write to a zip or elsewhere.

	enum DataAccess
	{
		DA_Read,
		DA_Write
	};

	// this abstract class will not provide thread safe, all read/write/any operation else 
	// should be provided by client code.
	class DUEL_API DDataStream : public DObject
	{
	DUEL_DECLARE_RTTI(DDataStream)
	public:
		DDataStream(DataAccess da = DA_Read);
		DDataStream(DString name, DataAccess da = DA_Read);
		virtual ~DDataStream(void);

		DataAccess		getAccess() { return mAccess; }
		const DString&	getName(){ return mName; }

		virtual	bool	isReadable() { return (mAccess == DA_Read); }
		virtual bool	isWritable() { return (mAccess == DA_Write); }

		/** Read the requisite number of bytes from the stream, 
			stopping at the end of the file.
		@param buf Reference to a buffer pointer
		@param length Number of bytes to read
		@returns The number of bytes read
		*/
		virtual size_t	read(void* buf, size_t length) = 0;

		/** Write the requisite number of bytes from the stream (only applicable to 
			streams that are not read-only)
		@param buf Pointer to a buffer containing the bytes to write
		@param length Number of bytes to write
		@returns The number of bytes written
		*/
		virtual size_t	write(const void* buf, size_t length)
		{
			// default to not supported
			return 0;
		}

		/** Get a single line from the stream.
		@remarks
			The delimiter character is not included in the data
			returned, and it is skipped over so the next read will occur
			after it. The buffer contents will include a
			terminating character.
        @note
            If you used this function, you <b>must</b> open the stream in <b>binary mode</b>,
            otherwise, it'll produce unexpected results.
		@param buf Reference to a buffer pointer
		@param maxLength The maximum length of data to be read, excluding the terminating character
		@param delim The delimiter to stop at
		@returns The number of bytes read, excluding the terminating character
		*/
		virtual size_t	readLine(char* buf, size_t maxLength, const DString& delim = "\n") = 0;

		/** Returns a DString containing the next line of data, optionally 
		    trimmed for whitespace. 
	    @remarks
		    This is a convenience method for text streams only, allowing you to 
		    retrieve a DString object containing the next line of data. The data
		    is read up to the next newline character and the result trimmed if
		    required.
        @note
            If you used this function, you <b>must</b> open the stream in <b>binary mode</b>,
            otherwise, it'll produce unexpected results.
	    @param 
		    trimAfter If true, the line is trimmed for whitespace (as in 
		    DString.trim(true,true))
	    */
		virtual	DString	getLine(bool trim = true);

		// move forward specified length of bytes.
		virtual	void	skip(long length) = 0;

		// repositions the read point to a specified byte.
		virtual	void	seek(size_t pos) = 0;

		// get the current byte offset from beginning.
		virtual	size_t	tell() = 0;

		// query if the stream reaches the end.
		virtual	bool	eof() const = 0;

		// return the size of the stream.
		virtual size_t	getSize() const;

		// close the stream, make further operation invalid.
		virtual	void	close() = 0;

		// return false if this stream is available for reading/writing.
		virtual	bool	isValid() const { return mbValid; }

		// normally 128.
#define DUEL_TEMP_BUFFER_SIZE	128
	protected:
		// name is used to indicate the name of the stream.
		DString	mName;
		// define which type of the stream is.
		DataAccess	mAccess;

		bool	mbValid;
	};
}

#endif


