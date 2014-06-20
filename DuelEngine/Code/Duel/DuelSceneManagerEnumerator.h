//  [1/7/2013 OMEGA] created

#ifndef _DUELSCENEMANAGERENUMERATOR_H_
#define _DUELSCENEMANAGERENUMERATOR_H_

#include "DuelCommon.h"
#include "DuelSceneManager.h"

namespace Duel
{

	class DUEL_API DSceneManagerFactory : public DObject
	{
	DUEL_DECLARE_RTTI(DSceneManagerFactory)
	public:
		DSceneManagerFactory(const DString& typeName) : mTypeName(typeName) {}
		// unique type name should be given by each factory.
		virtual const DString&	getTypeName() { return mTypeName; }

		// create an instance with spcified name.
		virtual	DSceneManager*	createSceneManager(const DString& name) = 0;
		// destroy a scenemanager.
		virtual	void			destroySceneManager(DSceneManager* sm) = 0;

	protected:
		// the name indicate the type of the DSceneManagerFactory
		DString		mTypeName;
	};

	class DUEL_API DSceneManagerEnumerator : public DObject,  public DSingleton<DSceneManagerEnumerator>
	{
	DUEL_DECLARE_RTTI(DSceneManagerEnumerator)
	public:
		// register a factory for creating scenemanager
		void			registerSceneManagerFactory(DSceneManagerFactory* factory);
		// unregister a factory for creating scenemanager
		void			unregisterSceneManagerFactory(DSceneManagerFactory* factory);
		// create a scenemanager
		DSceneManager*	createSceneManager(const DString& typeName, const DString& name);
		// destroy a scenemanager
		void			destroySceneManager(DSceneManager* sm);

		typedef	std::map<DString, DSceneManagerFactory*>	SceneManagerFactoryMap;
		typedef	MapIterator<SceneManagerFactoryMap>		SceneManagerFactoryIterator;
		SceneManagerFactoryIterator	getSceneManagerFactoryIterator() { return SceneManagerFactoryIterator(mFactoryMap.begin(), mFactoryMap.end()); }

	protected:
		SceneManagerFactoryMap	mFactoryMap;
	};
}

#endif