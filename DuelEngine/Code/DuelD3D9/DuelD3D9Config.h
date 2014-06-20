//  [12/24/2012 OMEGA] created

#ifndef _DUELD3D9CONFIG_H_
#define _DUELD3D9CONFIG_H_

namespace Duel
{

#ifdef	DUELD3D9_EXPORT
#define	DUELD3D9_API	__declspec(dllexport)
#else
#define	DUELD3D9_API	__declspec(dllimport)
#endif

#ifdef _DEBUG
#define  D3D_DEBUG_INFO
#endif // DEBUG


}

#endif