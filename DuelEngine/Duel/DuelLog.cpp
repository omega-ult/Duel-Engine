//  [7/15/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelLog.h"

// 'localtime' compile warning
#pragma warning(push)
#pragma warning(disable : 4996)

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DLog, DObject);

	DLog::DLog(const DString& path, bool enable) : 
			mUseTimeStamp(true),
			mEnable(enable)
	{
		
		DStringTool::splitFileName(path,mName,mPath);
		
		if(mEnable)
		{
			mfLog.open(mPath + mName,std::ios::out | std::ios::app);
			// add a new time stamp
			/*std::time_t t = std::time(NULL);
			mfLog << std::endl << "File touched at: " <<
			std::put_time(std::localtime(&t), "%c %Z")
			<< std::endl;*/

		}
	}
	DLog::~DLog()
	{
		if(mfLog.is_open())
		{
			mfLog.close();
		}
	}

	void DLog::logMessage( const DString& message, LogLevel level /*= LL_DEBUG*/ )
	{
		DUEL_LOCK_AUTO_MUTEX
		if(mEnable)
		{
			if(mUseTimeStamp)
			{
				std::time_t t = std::time(NULL);
				mfLog << std::put_time(std::localtime(&t), "%c  ");
			}
			else
			{
				// use blank to match pattern
				mfLog << "                 ";
			}

			// out put log level
			switch(level)
			{
			case LL_Debug:
				mfLog << "DEBUG    ";
				break;
			case LL_Info:
				mfLog << "INFO     ";
				break;
			case LL_Warn:
				mfLog << "WARNING  ";
				break;
			case LL_Error:
				mfLog << "ERROR    ";
				break;
			case LL_Fatal:
				mfLog << "FATAL    ";
				break;
			case LL_Off:
				mfLog << "         ";
				break;
			}

			mfLog << message << std::endl;

			mfLog.flush();
		}
	}

}

#pragma warning(pop)
