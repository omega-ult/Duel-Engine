//  [6/22/2013 OMEGA] created

#ifndef _DUELSCENE_H_
#define _DUELSCENE_H_

#include "DuelCommon.h"
#include "DuelAutoGpuParameter.h"

namespace Duel
{

	// generally a game scene, which can be displayed, 
	class DUEL_API DAScene : public DAutoGpuParameterDelegate
	{
	public:
		DAScene();
		~DAScene();

		virtual const DMatrix4& getViewMatrix() 
		{
			throw std::exception("The method or operation is not implemented.");
		}

		virtual const DMatrix4& getProjectionMatrix() 
		{
			throw std::exception("The method or operation is not implemented.");
		}

	protected:
		DCamera*	mObserverCamera;

	};


}

#endif