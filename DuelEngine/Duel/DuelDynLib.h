//  [12/4/2012 OMEGA] created

#ifndef _DUELDYNLIB_H_
#define _DUELDYNLIB_H_

#include "DuelCommon.h"

#ifdef	DUEL_PLATFORM_WINDOWS
#ifdef DUEL_PLATFORM_WINDOWS_PHONE_8

#define	DYNLIB_HANDLE		hInstance
#define DYNLIB_LOAD(a)		LoadPackagedLibrary(a, NULL)
#define DYNLIB_GETSYM(a, b)	GetProcAddress(a, b)
#define DYNLIB_UNLOAD(a)	!FreeLibrary(a)

struct	HINSTANCE__;
typedef	struct HINSTANCE__*	hInstance;

#else
#define	DYNLIB_HANDLE		hInstance
#define DYNLIB_LOAD(a)		LoadLibraryEx(a, NULL, LOAD_WITH_ALTERED_SEARCH_PATH)
#define DYNLIB_GETSYM(a, b)	GetProcAddress(a, b)
#define DYNLIB_UNLOAD(a)	!FreeLibrary(a)

struct	HINSTANCE__;
typedef	struct HINSTANCE__*	hInstance;
#endif

#endif // DUEL_PLATFORM_WINDOWS_PHONE_8

// TODO: add other platform support

namespace Duel
{

	class DUEL_API DDynLib : public DObject
	{
	DUEL_DECLARE_RTTI(DDynLib)
	public:
		// do not call this method directly, used by DynLibManager
		DDynLib(const DString& name);
		virtual ~DDynLib();
		// get the name of the DDynLib
		const DString&	getName() const { return mName; }
		// load the DDynLib, should be called by DynLibManager.
		void			load();
		// unload the DDynLib, should be called by DynLibManager.
		void			unload();
		// get the address of the given symbol from the library
		void*			getSymbol(const DString& name) const ;
	protected:
		// name of this DDynLib
		DString			mName;
		// get the last error
		DString			dynLibError();
		// the handle of library, platform-specified.
		DYNLIB_HANDLE	mHandle;
	};
}

#endif