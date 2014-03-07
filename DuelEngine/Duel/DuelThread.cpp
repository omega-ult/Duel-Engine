//  [7/9/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelAtomic.h"
#include "DuelMutex.h"
#include "DuelThread.h"

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(DThread, DObject);

	DThread::DThread() : 
		mbFinished(false),
		mWorkThread(NULL)
	{

	}

	DThread::~DThread()
	{
		if (mWorkThread != NULL)
		{
			delete mWorkThread;
		}
	}

	void DThread::start()
	{
		mbFinished.set(false);
	}

	void DThread::join()
	{
		terminate();  
		if (mWorkThread != NULL)  
			mWorkThread->join(); 
	}

	void DThread::terminate()
	{
		mbFinished = true;
		mTaskCond.notify_one();
	}

	Duel::uint32 DThread::load()
	{
		uint32 sz = 0;  
		mTaskMutex.lock();  
		sz = mTaskList.size();  
		mTaskMutex.unlock();  
		return sz; 
	}

	void DThread::waitForIdle()
	{
		while(load())  
			DUEL_THREAD_SLEEP(200);
	}

	Duel::uint32 DThread::append( DFunction<void (void)> func )
	{
		if (mWorkThread == NULL)  
			mWorkThread = new WorkThread(boost::bind(&DThread::run,this));  
		size_t sz = 0;  
		mTaskMutex.lock();  
		mTaskList.push_back(func);  
		sz = mTaskList.size();  
		//if there were no tasks before, we should notidy the thread to do next job.  
		if (sz==1)  
			mTaskCond.notify_one();  
		mTaskMutex.unlock();  
		return sz;  
	}

	void DThread::run()
	{
		// loop wait  
		while (mbFinished.get() != true)   
		{  
			bool bHasTasks = false;  
			TaskList::iterator i;
			mTaskMutex.lock();  
			if (mTaskList.empty() == false)  
			{  
				bHasTasks = true;  
				i = mTaskList.begin();
			}  
			mTaskMutex.unlock();  
			//doing task  
			if (bHasTasks)  
			{  
				(*i)();
				mTaskMutex.lock(); 
				mTaskList.erase(i);
				mTaskMutex.unlock();  
			}  
			if (!load())  
			{  
				DWriteLock m_cond_locker(mCondMutex);  
				boost::system_time const timeout = boost::get_system_time() + boost::posix_time::milliseconds(5000);  
				if (m_cond_locker.mutex())  
					mTaskCond.timed_wait(m_cond_locker,timeout);
			}  
		}  
	}

	bool DThread::isRunning() const
	{
		return !mbFinished.get();
	}

	bool DThread::isFinished() const
	{
		return mbFinished.get();
	}

}