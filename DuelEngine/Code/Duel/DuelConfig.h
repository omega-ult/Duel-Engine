//  [7/15/2012 OMEGA] created


// 现在是编译成dll库, 使用dllexport定义: //  [3/6/2013 OMEGA]


// build defines
#if defined(_DEBUG) || defined(DEBUG)
	#define DUEL_DEBUG 
#endif

// platform defines
#if defined(_WIN32) || defined(_WIN32_) || defined(WIN32)
	#define DUEL_PLATFORM_WINDOWS
	#if defined(_WIN64)
		#define DUEL_PLATFORM_WIN64
	#else
		#define DUEL_PLATFORM_WIN32
	#endif
#endif 



// compiler defines
#if defined(_MSC_VER)
	#define DUEL_COMPILER_MSVC
#endif

// cpu defines
#if defined(DUEL_COMPILER_MSVC)
	#if defined(_M_IA64)
		#define DUEL_CPU_X64
	#elif defined(_M_IX86)
		#define DUEL_CPU_X86
	#endif
#endif

// endian defines
#if defined(DUEL_CPU_X86) || defined(DUEL_CPU_X64) || defined(DUEL_PLATFORM_WINDOWS)
	#define DUEL_LITTLE_ENDIAN
#else
	#define DUEL_BIG_ENDIAN
#endif




#ifdef	DUEL_EXPORT
#define	DUEL_API	__declspec(dllexport)
#else	
#define DUEL_API	__declspec(dllimport)
#endif // DUEL_EXPORT

#ifndef DUEL_DEPRECATED
#define DUEL_DEPRECATED	__declspec(deprecated)
#endif


// singleton's private construction warning
#pragma warning(disable : 4661)
// dll export warning
#pragma warning(disable : 4251)
// boost signals2 warning
#pragma warning(disable : 4996)
