//  [5/23/2013 OMEGA] created

#ifndef _DUELMATERIALMANAGER_H_
#define _DUELMATERIALMANAGER_H_

#include "DuelCommon.h"

namespace Duel
{

	class DUEL_API DMaterialManager : public DObject, public DSingleton<DMaterialManager>
	{
	DUEL_DECLARE_RTTI(DMaterialManager)
	public:
		DMaterialManager();
		~DMaterialManager();

		typedef	std::map<DString, DMaterialPtr>	MaterialMap;
		typedef MapIterator<MaterialMap>		MaterialIterator;
		MaterialIterator	getMaterialIterator() { return MaterialIterator(mMatMap); }
		// register a new material use unique name.
		void registerMaterial(DMaterialPtr mat);
		// unregister a material, this method will not affect created associated material instance.
		void				unregisterMaterial(const DString& name);
		// create a material instance, may return NULL if no specified material resource registered.
		DMaterialInstancePtr	createMaterialInstance(const DString& name);
		// use an existing instance to create another one, this function will copy the
		// parameters of specified instance to the resulting one.
		DMaterialInstancePtr	copyMaterialInstance(DMaterialInstancePtr inst);

	protected:
		MaterialMap	mMatMap;
	};

}

#endif