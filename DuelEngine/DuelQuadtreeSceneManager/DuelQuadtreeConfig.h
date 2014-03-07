//  [3/7/2013 OMEGA] created

#ifndef _DUELQUADTREECONFIG_H_
#define _DUELQUADTREECONFIG_H_

namespace Duel
{
#ifdef	DUELQUADTREE_EXPORT
#define	DUELQUADTREE_API	__declspec(dllexport)
#else
#define	DUELQUADTREE_API	__declspec(dllimport)
#endif

}


#endif // !_DUELQUADTREECONFIG_H_
