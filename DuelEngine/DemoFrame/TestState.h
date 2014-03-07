//  [6/28/2013 OMEGA] created

#ifndef _TESTSTATE_H_
#define _TESTSTATE_H_

#include "pch.h"
#include "Duel.h"
#include "DuelStateMachine.h"

namespace DemoKit
{
	
	class TestState : public Duel::DAStateBase
	{
	DUEL_DECLARE_RTTI(TestState)
	public:
		TestState(); // "TestState"
		~TestState();
		// override:DAStateBase-------------------------
		virtual void	initialize();
		// override:DAStateBase-------------------------
		virtual void	release();
		// override:DAStateBase-------------------------
		virtual void	parseInput();
		// override:DAStateBase-------------------------
		virtual void	update();
		// override:DAStateBase-------------------------
		virtual void	render();
		// override:DAStateBase-------------------------
		virtual void	present();
		// override:DAStateBase-------------------------
		virtual bool	stateChangingFrom( Duel::DAStateBase* preState );

	private:
		// 时间参数.
		Duel::uint64	mStartTime;
		Duel::uint64	mCurTime;
		Duel::DTimer*	mTimer;

		Duel::DRenderWindow*	mRenderWindow;
		Duel::DGUIManager*		mGManager;

		Duel::DSceneInstance*	mSceneInstance;

		Duel::DEntity*			mTestEntity;
		//yoooo
		Duel::DGSPictureBox*		mTestBox;
		Duel::DGSPictureBox*		mSubTestBox;
	};
}

#endif
