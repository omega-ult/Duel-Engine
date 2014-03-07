//  [6/23/2013 OMEGA] created

#ifndef _DUELAPPLICATION_H_
#define _DUELAPPLICATION_H_

#include "DuelCommon.h"

namespace Duel
{

	class DUEL_API DAApplication : public DSingleton<DAApplication>
	{
	public:
		class DUEL_API EventListener
		{
		public:
			//////////////////////////////////////////

		};
		DAApplication();
		~DAApplication();


	};

}


#endif