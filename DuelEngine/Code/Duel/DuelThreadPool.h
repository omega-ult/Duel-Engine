//  [7/12/2013 OMEGA] created

#ifndef _DUELTHREADPOOL_H_
#define _DUELTHREADPOOL_H_

#include "DuelCommon.h"
#include "DuelThread.h"

namespace Duel
{

	class DUEL_API DThreadPool : public DObject
	{
	DUEL_DECLARE_RTTI(DThreadPool)
	public:
		DThreadPool(uint8 threadCount = 4);
		virtual	~DThreadPool();

		size_t		getThreadCount() const;
		
		void		start();
		// //wait until all threads is terminated;  
		void		join();
		// wait until this thread has no tasks pending.
		void		waitForIdle();
		// set the mask to termminiate 
		void		terminate();
		// return the current load of specified thread index.
		// load 这里的意思是 负荷.
		uint32		load(uint8	threadIndex);
		// append a task to do  
		void		append(DFunction<void(void)> func);
	protected:
		// vector contains all the threads. 
		typedef std::vector<SharedPtr<DThread>>	WorkThreads;
		WorkThreads		mThreads;
	};
}

#endif