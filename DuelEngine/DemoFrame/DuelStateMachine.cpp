//  [3/15/2013 OMEGA] created

#include "pch.h"
#include "DuelStateMachine.h"
#include "DuelApplication.h"


namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DAStateBase, DObject);
	DUEL_IMPLEMENT_RTTI_1(DANullState, DAStateBase);

	DANullState DAStateManager::msNullState;

	DAStateManager::DAStateManager( DApplication* app ) : 
		mParentApp(app), 
		mCurState(NULL),
		mNextState(NULL)
	{
		registerState(&msNullState);
		requestStateChange(NULL,"Null");
	}



	DAStateManager::~DAStateManager()
	{
		if (mCurState)
		{
			mCurState->release();
		}
	}

	void DAStateManager::registerState( DAStateBase* state )
	{
		StateMap::iterator i = mStateMap.find(state->getName());
		if (i == mStateMap.end())
		{
			mStateMap.insert(std::make_pair(state->getName(), state));
		}
	}

	void DAStateManager::unregisterState( DAStateBase* state )
	{
		if (state == mCurState || state == mNextState)
		{
			return;
		}
		StateMap::iterator i = mStateMap.find(state->getName());
		if (i != mStateMap.end())
		{
			mStateMap.erase(i);
		}
	}
}