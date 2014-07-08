//  [4/12/2013 OMEGA] created

#ifndef _DUELOCTREEPLUGIN_H_
#define _DUELOCTREEPLUGIN_H_

#include "DuelOctreeCommon.h"
#include "DuelPlugin.h"

namespace Duel
{

	class DUELOCTREE_API OctreePlugin : public DPlugin
	{
	public:
		OctreePlugin();

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
		OctreeSceneManagerFactory* mFactory;
	};

}

#endif