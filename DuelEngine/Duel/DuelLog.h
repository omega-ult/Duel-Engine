//  [7/15/2012 OMEGA] created

#ifndef _DUELLOG_H_
#define _DUELLOG_H_


#include "DuelCommon.h"

namespace Duel
{

	// log level is the priority of log, increases from debug to fatal, 
	// only logs that has higher priority than the one specified in LogManager
	// will be logged, 
	class DUEL_API DLog : public DObject
	{
	DUEL_DECLARE_RTTI(DLog)
	public:
		DUEL_AUTO_MUTEX
		enum LogLevel
		{
			LL_Debug,
			LL_Info,
			LL_Warn,
			LL_Error,
			LL_Fatal,
			LL_Off
		};
		
		DLog(const DString& path, bool enable);
		virtual ~DLog();

		const DString& getName() { return mName; }
		bool	isEnabled() { return mEnable; }
		bool	isTimeStempEnabled() { return mUseTimeStamp; }

		void	setEnable(bool flag) { mEnable = flag; }
		void	setTimeStampEnable(bool flag) { mUseTimeStamp = flag; }

		void	logMessage(const DString& message, LogLevel level = LL_Debug);

	protected:
		std::ofstream	mfLog;
		bool			mUseTimeStamp;
		bool			mEnable;
		DString			mPath;
		DString			mName;
		
	};


}

#endif