//  [7/15/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelException.h"
#include "DuelString.h"


namespace Duel
{
	
	DException::DException(int type, const DString& description, const DString& source) :
		mTypeCode(type),
		mDescription(description),
		mSource(source),
		mLine(0),
		mTypeName(DString("NOT_DEFINED")),
		mFile(DStringTool::BLANK),
		mFullDesc(description)
	{

	}


	DException::DException( int type, const DString& description, const DString& fullDesc, const DString& source, const char* name, const char* file, long line ) :
		mTypeCode(type),
		mDescription(description),
		mFullDesc(fullDesc),
		mSource(source),
		mTypeName(name),
		mFile(file),
		mLine(line)
	{
		
	}

}