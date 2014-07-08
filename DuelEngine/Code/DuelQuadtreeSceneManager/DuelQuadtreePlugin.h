//  [3/10/2013 OMEGA] craeted

#ifndef _DUELQUADTREEPLUGIN_H_
#define _DUELQUADTREEPLUGIN_H_


#include "DuelQuadtreeCommon.h"
#include "DuelPlugin.h"

namespace Duel
{

	class DUELQUADTREE_API QuadtreePlugin : public DPlugin
	{
	public:
		QuadtreePlugin();
		
		// override : DPlugin-----------------------------
		const DString&	getName() const;
		// override : DPlugin-----------------------------
		void			install();
		// override : DPlugin-----------------------------
		void			initialize( const DString& config );
		// override : DPlugin-----------------------------
		void			shutdown();
		// override : DPlugin-----------------------------
		void			uninstall();

	protected:
		QuadtreeSceneManagerFactory* mFactory;
	};

}

#endif // !_DUELQUADTREEPLUGIN_H_

