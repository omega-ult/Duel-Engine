//  [12/4/2012 OMEGA]

#include "DuelCommon.h"
#include "DuelDynLib.h"
#include "DuelException.h"
#include "DuelLogManager.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DDynLib, DObject);

	DDynLib::DDynLib( const DString& name ) :
		mName(name),
		mHandle(NULL)
	{

	}

	DDynLib::~DDynLib()
	{
#ifdef DUEL_PLATFORM_WINDOWS
		FreeLibrary(mHandle);
#endif
		// TODO: add other platform support
	}

	void DDynLib::load()
	{
		// Log library load
		DLogManager::getSingleton().logMessage("Loading library " + mName);

		DString name = mName;
		// TODO: add other platform support
#ifdef DUEL_PLATFORM_WINDOWS
		// Although LoadLibraryEx will add .dll itself when you only specify the library name,
		// if you include a relative path then it does not. So, add it to be sure.
		if (name.substr(name.length() - 4, 4) != ".dll")
			name += ".dll";
#endif
#ifdef UNICODE
		DWString	finalName = DStringTool::convert(name);
		
#else	
		DString	finalName = name;
#endif
		mHandle = (DYNLIB_HANDLE)DYNLIB_LOAD(finalName.c_str());

		if (!mHandle)
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InternalError,
				"unable to load dynamic library" + mName + 
				"with error: " + dynLibError(),
				"Duel::DDynLib::load");
		}
	}

	void DDynLib::unload()
	{
		// Log library unload
		DLogManager::getSingleton().logMessage("Unloading library " + mName);
		if (DYNLIB_UNLOAD(mHandle))
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InternalError,
				"unable to unload dynamic library" + mName + 
				"with error: " + dynLibError(),
				"Duel::DDynLib::unload");
		}
	}

	void* DDynLib::getSymbol( const DString& name ) const 
	{
		return (void*)DYNLIB_GETSYM(mHandle, name.c_str());
	}

	Duel::DString DDynLib::dynLibError()
	{
#ifdef DUEL_PLATFORM_WINDOWS
#ifdef DUEL_PLATFORM_WINDOWS_PHONE_8
		LPVOID lpMsgBuf; 
		FormatMessage( 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS, 
			NULL, 
			GetLastError(), 
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
			(LPTSTR) &lpMsgBuf, 
			0, 
			NULL 
			); 
		DString ret = (char*)lpMsgBuf;
		return ret;

#else
		LPVOID lpMsgBuf; 
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS, 
			NULL, 
			GetLastError(), 
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
			(LPTSTR) &lpMsgBuf, 
			0, 
			NULL 
			); 
		DString ret = (char*)lpMsgBuf;
		// Free the buffer.
		LocalFree( lpMsgBuf );
		return ret;

#endif // DUEL_PLATFORM_WINDOWS_PHONE_8

#endif
		//TODO: add other platform support
	}

}