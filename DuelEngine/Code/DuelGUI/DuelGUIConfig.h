//  [9/28/2013 OMEGA] created

#ifndef _DUELGUICONFIG_H_
#define _DUELGUICONFIG_H_


// platform defines
#if defined(_WIN32) || defined(_WIN32_) || defined(WIN32)
#define DUELGUI_PLATFORM_WINDOWS
#if defined(_WIN64)
#define DUELGUI_PLATFORM_WIN64
#else
#define DUELGUI_PLATFORM_WIN32
#endif
#endif 


#ifdef	DUELGUI_EXPORT
#define	DUELGUI_API	__declspec(dllexport)
#else
#define	DUELGUI_API	__declspec(dllimport)
#endif

#endif