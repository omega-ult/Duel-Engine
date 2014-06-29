//  [3/15/2013 OMEGA] created

#ifndef _DUELSTATEMACHINE_H_
#define	_DUELSTATEMACHINE_H_

#include "pch.h"
#include "Duel.h"

// 状态机基类, 所有游戏状态从这里继承.这里同时包含一个状态机管理器. 使用单例实现?
namespace Duel
{
	class DAStateBase;
	class DAStateBase : public DObject
	{
	DUEL_DECLARE_RTTI(DAStateBase)
	public:
		DAStateBase(const DString& stateName) : mName(stateName) {}
		~DAStateBase() {}

		const DString&	getName() { return mName; }

		// 初始化用的函数. 絕對!! 不要在這個函數裏面申請狀態轉換!!
		virtual	void	initialize() {}
		// 转出到其他状态之前的函数. 絕對!! 不要在這個函數裏面申請狀態轉換!!
		virtual	void	release() {}

		// 具体的参数根据需要来取得. 這裡的參數是已經商量好的那個格式.
		virtual	void	parseInput() {}
		// 这个函数里需要把所有需要绘制的东西全都放到renderqueue里面并且主动调用每个组的sort()函数.
		virtual	void	update() {}
		// 这个函数里面使用renderqueue的render函数进行绘制.
		virtual	void	render() {}
		// 这个函数使绘制完毕的结果展示出来.
		virtual	void	present() {}

		// 状态转换的时候需要被调用的函数: 返回true表示接受转换, stateManager会在下一帧切换状态.
		virtual	bool	stateChangingFrom(DAStateBase* preState) { return true; }

	protected:
		// 用来唯一标示这个状态的名字, 之所以使用string来代替enum是出于扩展性的考虑.
		DString	mName;

	};

	class DAStateBase;
	class DAGameDirector;
	class DApplication;
	class DANullState;
	class DAStateManager
	{
	public:
		DAStateManager(DApplication* app);
		~DAStateManager();

		//////////////////////////////////////////////////////////////////////////
		// signals block
		DSignal<void(DAStateManager* sm)>
			signalTransactionStart;
		DSignal<void(DAStateManager* sm, DAStateBase* before, DAStateBase* after)>
			signalStateChanged;
		DSignal<void(DAStateManager* sm)>
			signalTransactionFinish;
		//////////////////////////////////////////////////////////////////////////

		// 不要传空值. 注册/注销一个状态, 使其能够正常工作.
		void		registerState(DAStateBase* state);
		// 注销一个state 但是不会删除它.注意. 而且不能刪除當前狀態或者下一個狀態.
		void		unregisterState(DAStateBase* state);
		void		shutdown()
		{
			if (mCurState != NULL)
			{
				mCurState->release();
				mCurState = NULL;
				mNextState = NULL;
			}
			
		}

		// 在一个事务开始前执行的函数, 一般调用eventListener就可以了.
		void		transactionStart()
		{
			// 重置一下.
			if (mCurState != NULL) // 處理首次加入狀態的特殊情況.
			{
				mNextState = mCurState;
			}
			else
			{
				if (mNextState != NULL)
				{
					mNextState->initialize();
				}
				mCurState = mNextState;
			}
			signalTransactionStart(this);
		}

		// 需要转换状态的时候使用这个函数. 参数分别是发出申请的状态的指针和需要转入的状态名称.
		void		requestStateChange(DAStateBase* requestingState, const Duel::DString& targetState)
		{
			// 取得相应的state 如果不存在, 转换失败.
			StateMap::iterator i = mStateMap.find(targetState);
			if (i != mStateMap.end())
			{
				if(i->second->stateChangingFrom(requestingState))
				{
					signalStateChanged(this, requestingState, i->second);
					mNextState = i->second;
				}
			}
		}

		void		parseInput()
		{ 
			if(mCurState != NULL) 
			{
				mCurState->parseInput();
			}
		}
		void		update()
		{
			if(mCurState != NULL) 
			{
				mCurState->update(); 
			}
		}
		void		render()
		{
			if(mCurState != NULL) 
			{
				mCurState->render(); 
			}
		}

		void		present()
		{
			if(mCurState != NULL) 
			{
				mCurState->present(); 
			}
		}

		// 
		DApplication*	getParentApplication() { return mParentApp; }

		DAStateBase*	getCurrentState() { return mCurState; }
		DAStateBase*	getNextState() { return mNextState; }

		// 在一个事务完成后调用的函数, 用来处理状态切换以及其他杂项的函数.
		void		transactionFinish()
		{
			signalTransactionFinish(this);
			if (mNextState != mCurState)
			{
				// 初始化一些资源. 和释放当前状态的资源.
				if (mCurState != NULL)
				{
					mCurState->release();
				}
				if (mNextState != NULL)
				{
					mNextState->initialize();
				}
			}
			mCurState = mNextState;
		}

	protected:

		// 保存所有已经注册的状态.
		typedef	std::map<DString, DAStateBase*>	StateMap;
		StateMap			mStateMap;
		// 当前状态.
		DAStateBase*		mCurState;
		// 下一状态.
		DAStateBase*		mNextState;

		DApplication*		mParentApp;
		static DANullState	msNullState;
	};

	// 做個示例: 什?都不幹的一個狀態.
	class DANullState : public DAStateBase
	{
	DUEL_DECLARE_RTTI(DANullState)
	public:
		DANullState() : DAStateBase("Null") {}
		~DANullState() {}

	private:

	};
}

#endif