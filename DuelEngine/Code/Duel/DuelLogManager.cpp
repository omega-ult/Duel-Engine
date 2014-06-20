//  [7/15/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelLogManager.h"



namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(DLogManager, DObject);

	DLogManager::DLogManager( const DString& dir, /*= "\\log" */ DLog::LogLevel granularity /*= LL_DEBUG*/ )
	{
		setLogDirectory(dir);
		mGranularity = granularity;
		createLog("DLog.log",true);
	}

	void DLogManager::setLogDirectory( const DString& dir )
	{
#ifdef DUEL_PLATFORM_WINDOWS_PHONE_8

		DWString Wdir;
		DStringTool::convert(Wdir, dir);
		Platform::String^ localfolder = Windows::Storage::ApplicationData::Current->LocalFolder->Path;
		Platform::String^ localDir = ref new Platform::String(Wdir.c_str(), Wdir.length());
		Platform::String^ destDir = Platform::String::Concat(localfolder, localDir);
		if(_waccess(destDir->Begin(),2) == 0)
		{
			mLogDirectory = DString(destDir->Begin(),destDir->End());
		}
		else 
		{
			// the target directory is not exit
			if(CreateDirectory(destDir->Begin(),NULL))
			{
				mLogDirectory = DString(destDir->Begin(),destDir->End());
			}
			else
			{
				DUEL_EXCEPT(Duel::DException::ET_InvalidParameters,
					"Specified directory can't be created",
					"Specified directory can't be created due to unknown reason(the specified path include invalid character(s)? or the directory is read-only?)",
					"Duel::DLogManager::setLogDirectory"
					)
			}
		}
#else
		// because we use wchar, so we have to deal with wide char
		// Every wide char will prefix with a 'W'.
		wchar_t WcurPath[MAX_PATH];
		GetCurrentDirectory(MAX_PATH,WcurPath);
		DWString WdestPath = WcurPath;
		DWString Wdir = DStringTool::convert(dir);
		WdestPath = WdestPath + Wdir;
		DString destPath = DStringTool::convert(WdestPath);
		// _access use '2' as second parameter will check if we can write to the directory
		if(_waccess(WdestPath.c_str(),2) == 0)
		{
			mLogDirectory = destPath;
		}
		else 
		{
			// the target directory is not exit
			if(CreateDirectory(WdestPath.c_str(),NULL))
			{
				mLogDirectory = destPath;
			}
			else
			{
				DUEL_EXCEPT(Duel::DException::ET_InvalidParameters,
					"Specified directory can't be created",
					"Specified directory can't be created due to unknown reason(the specified path include invalid character(s)? or the directory is read-only?)",
					"Duel::DLogManager::setLogDirectory"
					)
			}
		}


#endif // DEBUG
	}



	DLogManager::~DLogManager()
	{
		LogMap::iterator i;
		for(i = mLogs.begin(); i != mLogs.end(); ++i)
		{
			delete i->second;
		}
		mLogs.clear();
	}

	const DString& DLogManager::getLogDirectory()
	{
		return mLogDirectory;
	}

	DLog* DLogManager::createLog( const DString& name, bool asCurLog )
	{
		LogMap::iterator i;
		i = mLogs.find(name);
		DLog *l = NULL;
		if (i == mLogs.end())
		{
			l = new DLog(mLogDirectory + "\\" +  name, true);
			mLogs.insert(LogMap::value_type(name,l));
		}
		else
		{
			l = i->second;
		}

		if(asCurLog)
		{
			mCurLog = l;
		}
		return	l;
	}

	void DLogManager::setCurLog( const DString& name )
	{
		LogMap::iterator i;
		i = mLogs.find(name);
		if(i != mLogs.end())
		{
			mCurLog = i->second;
		}

	}

	void DLogManager::logMessage( const DString& message, DLog::LogLevel level /*= LL_DEBUG*/ )
	{
		DUEL_LOCK_AUTO_MUTEX
		if(mGranularity <= level)
		{
			mCurLog->logMessage(message,level);
		}
		
	}

	void DLogManager::logMessage( const DString& name, const DString& message, DLog::LogLevel level /*= LL_DEBUG*/ )
	{
		DUEL_LOCK_AUTO_MUTEX
		LogMap::iterator i;
		i = mLogs.find(name);
		if(i != mLogs.end())
		{
			if(mGranularity <= level)
			{
				i->second->logMessage(message,level);
				return;
			}
		}
		else
		{
			DLog* l = createLog(name, false);
			l->logMessage(message,level);
		}
	}

	void DLogManager::disableLog( const DString& name )
	{
		LogMap::iterator i;
		i = mLogs.find(name);
		if(i != mLogs.end())
		{
			i->second->setEnable(false);
		}

	}

	void DLogManager::enableLog( const DString& name )
	{
		LogMap::iterator i;
		i = mLogs.find(name);
		if(i != mLogs.end())
		{
			i->second->setEnable(true);
		}
	}


}