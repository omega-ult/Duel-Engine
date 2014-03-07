//  [6/22/2013 OMEGA] created

#ifndef _DUELHELPERITEMFACTORY_H_
#define _DUELHELPERITEMFACTORY_H_

#include "DuelCommon.h"
#include "DuelMovableManager.h"
#include "DuelHelperPlaneGrid.h"

namespace Duel
{
// 
// 	class DUEL_API DHPlaneGridFactory : public DMovableFactory
// 	{
// 	DUEL_DECLARE_RTTI(DHPlaneGridFactory)
// 	public:
// 		DHPlaneGridFactory() : DMovableFactory("PlaneGrid") {}
// 		~DHPlaneGridFactory() {}
// 
// 		DMovable* createMovable( const DString& name )
// 		{
// 			return new DHPlaneGrid(this, name, 100.0f);
// 		}
// 
// 		void destroyMovable( DMovable* obj )
// 		{
// 			if (obj->getCreator() == this)
// 			{
// 				delete obj;
// 			}
// 		}
// 	};
	/*
	class ItemFactory : public DMovableFactory
	{
		DUEL_DECLARE_RTTI(ItemFactory)
	public:
		ItemFactory() : DMovableFactory("Item") {}
		~ItemFactory() {}

		DMovable* createMovable( const DString& name )
		{
			return new Item(this, name);
		}

		void destroyMovable( DMovable* obj )
		{
			if (obj->getCreator() == this)
			{
				delete obj;
			}
		}

	private:

	};
	*/
}

#endif