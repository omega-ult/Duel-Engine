//  [7/15/2012 OMEGA] created

#ifndef _DUELLOGMANAGER_H_
#define _DUELLOGMANAGER_H_

#include "DuelSingleton.h"
#include "DuelLog.h"
#include "DuelException.h"

namespace Duel
{

	
	
	class DUEL_API DLogManager : public DObject, public DSingleton<DLogManager>
	{
	DUEL_DECLARE_RTTI(DLogManager)
	public:
		DUEL_AUTO_MUTEX
		// set the directory for log files, using relative path
		// param: dir
		//		use "\\xxx\\xxx" pattern, don't use any invalid character(s)
		//		caution: no "\\.." supported
		// param: granularity
		//		the lowest level of a message that can be logged.
		DLogManager( const DString& dir = "\\log", DLog::LogLevel granularity = DLog::LL_Debug);
		virtual ~DLogManager();
// 		static DLogManager&	getSingleton(void);
// 		static DLogManager*	getSingletonPtr(void);

		// set the directory for log files, using relative path
		// param: dir
		//		use "\\xxx\\xxx" pattern, don't use any invalid character(s)
		//		caution: no "\\.." turn back style supported
		void			setLogDirectory(const DString& dir);
		const DString&	getLogDirectory();

		DLog*			createLog(const DString& name, bool asCurLog);
		void			setCurLog(const DString& name);
		
		// if no log name specified, then write to current log
		void			logMessage(const DString& message, DLog::LogLevel level = DLog::LL_Debug);
		void			logMessage(const DString& name, const DString& message, DLog::LogLevel level = DLog::LL_Debug);

		// make a log working or suppressed.
		void			disableLog(const DString& name);
		void			enableLog(const DString& name);



	private:
		// granularity means lowest level of log that can be logged.
		DLog::LogLevel	mGranularity;
		DString		mLogDirectory;
		typedef HashMap<DString,DLog*> LogMap;
		LogMap		mLogs;
		DLog*		mCurLog;

	};

}



#endif