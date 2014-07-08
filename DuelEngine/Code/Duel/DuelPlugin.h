//  [12/4/2012 OMEGA] created

#ifndef _DUELPLUGIN_H_
#define _DUELPLUGIN_H_

#include "DuelCommon.h"

namespace Duel
{

	// common interface for a plugin, which support dynamic load/unload
	class DUEL_API DPlugin : public DObject
	{
	DUEL_DECLARE_RTTI(DPlugin)
	public:
		DPlugin() {}
		virtual ~DPlugin() {}

		// get the unique name of this plugin.
		virtual	const DString&	getName() const = 0;

		// install this plugin into sytem.
		virtual	void			install() = 0;

		// initialize this plugin, call this after install();
		virtual	void			initialize(const DString& config) = 0;

		// do clearup work before uninstall this plugin, call this:
		virtual	void			shutdown() = 0;
		
		// uninstall this plugin, it must peform cleanup work which has not
		// been already performed in shutdown(), this method will make this 
		// plugin unavailable.
		virtual	void			uninstall() = 0;
	};

}

#endif