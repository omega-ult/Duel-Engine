//  [9/16/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelDataStream.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DDataStream, DObject);

	DDataStream::DDataStream( DataAccess da /*= DA_READ*/ ) :
		mAccess(da), 
		mName(DStringTool::BLANK),
		mbValid(false)
	{

	}


	DDataStream::DDataStream(DString name, DataAccess da /*= DA_READ*/ ) :
		mName(name), 
		mAccess(da),
		mbValid(false)
	{

	}


	DDataStream::~DDataStream(void)
	{
	}


	size_t DDataStream::getSize() const
	{
		return 0;
	}

	DString DDataStream::getLine( bool trim /*= true*/ )
	{
		char tmpBuf[DUEL_TEMP_BUFFER_SIZE];
		DString	retString;
		size_t	readCount;
		// Keep looping while not hitting delimiter
		while((readCount = read(tmpBuf, DUEL_TEMP_BUFFER_SIZE-1)) != 0)
		{
			// Terminate string
			tmpBuf[readCount] = '\0';
			
			char* p = strchr(tmpBuf, '\n');
			if (p != NULL)
			{
				// Reposition backwards
				skip((long)(p + 1 - tmpBuf - readCount));
				*p = '\0';
			}
			retString += tmpBuf;
			if ( p != NULL)
			{
				// Trim off trailing CR if this was a CR/LF entry
				if (retString.length() && retString[retString.length()-1] == '\r')
				{
					retString.erase(retString.length()-1, 1);
				}

				// Found terminator, break out
				break;
			}
		}
		if (trim)
		{
			DStringTool::trim(retString);
		}
		return retString;
	}

}
