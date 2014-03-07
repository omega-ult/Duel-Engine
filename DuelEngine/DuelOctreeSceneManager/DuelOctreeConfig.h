//  [1/18/2013 OMEGA] created

#ifndef _DUELOCTREECONFIG_H_
#define _DUELOCTREECONFIG_H_

//#define DUEL_PLATFORM_WINDOWS_PHONE_8
// 直接在这里写项目设置了.导出- -

namespace Duel
{
#ifdef	DUELOCTREE_EXPORT
#define	DUELOCTREE_API	__declspec(dllexport)
#else
#define	DUELOCTREE_API	__declspec(dllimport)
#endif

}


#endif
