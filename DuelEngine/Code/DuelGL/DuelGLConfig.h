//  [4/7/2013 OMEGA] created

#ifndef _DUELGLCONFIG_H_
#define _DUELGLCONFIG_H_

#pragma warning(disable : 4355) // “this”: 用于基成员初始值设定项列表, 这个是个vs的缺陷

#ifdef	DUELGL_EXPORT
#define	DUELGL_API	__declspec(dllexport)
#else
#define	DUELGL_API	__declspec(dllimport)
#endif
#endif