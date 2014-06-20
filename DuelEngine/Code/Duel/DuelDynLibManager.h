//  [12/4/2012 OMEGA] created

#ifndef _DUELDYNLIBMANAGER_H_
#define _DUELDYNLIBMANAGER_H_

#include "DuelCommon.h"
#include "DuelDynLib.h"

namespace Duel
{

	// manager class that manage DynLibs, for loading/unloading
	class DUEL_API DDynLibManager : public DObject, public DSingleton<DDynLibManager>
	{
	DUEL_DECLARE_RTTI(DDynLibManager)
	public:
		// this class should be initialized in Core, as other Singletons.
		DDynLibManager();
		virtual ~DDynLibManager();
		// load specified library, the extension can be omitted.
		DDynLib*		load(const DString& fileName);
		// unload specified library
		void		unload(DDynLib* lib);
	protected:
		typedef	std::map<DString, DDynLib*>	DynLibMap;
		DynLibMap	mDynLibMap;
	};
}

#endif