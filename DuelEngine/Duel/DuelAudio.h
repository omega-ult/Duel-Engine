//  [5/24/2013 OMEGA] created

#ifndef _DUELAUDIO_H_
#define _DUELAUDIO_H_

#include "DuelCommon.h"
#include "DuelResource.h"

namespace Duel
{

	class DAudio : public DResource
	{
	public:
		DAudio(DResourceManager* parent, const DString& name, const DString& groupName);
		~DAudio();

		virtual void loadImpl() 
		{
			throw std::exception("The method or operation is not implemented.");
		}

		virtual void unloadImpl() 
		{
			throw std::exception("The method or operation is not implemented.");
		}

		virtual size_t calculateSize() 
		{
			throw std::exception("The method or operation is not implemented.");
		}

	private:

	};
}

#endif