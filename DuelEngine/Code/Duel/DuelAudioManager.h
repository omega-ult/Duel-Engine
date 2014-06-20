//  [5/24/2013 OMEGA] created

#ifndef _DUELAUDIOMANAGER_H_
#define _DUELAUDIOMANAGER_H_

#include "DuelCommon.h"
#include "DuelResourceManager.h"

namespace Duel
{

	class DAudioManager : public DResourceManager
	{
	public:
		DAudioManager();
		~DAudioManager();

		virtual DResource* createImpl( DResourceDescription* createParam );

	private:

	};

}


#endif