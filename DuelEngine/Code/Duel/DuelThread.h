//  [9/16/2012 OMEGA] created

#ifndef _DUELTHREAD_H_
#define _DUELTHREAD_H_

#include "DuelCommon.h"
#include "DuelRTTI.h"
#include "DuelObject.h"
#include "DuelAtomic.h"
#include "DuelMutex.h"
#include "DuelCondition.h"
///////////////////////////////////
// boost include
#ifdef DUEL_PLATFORM_WINDOWS_PHONE_8
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#else
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/locks.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/function.hpp>
#endif // DUAL_PLATFORM_WINDOWS_PHONE_8


namespace Duel
{
	//////////////////////////////////////////////////////////////////////////
	// thread wrap
#ifdef DUEL_PLATFORM_WINDOWS_PHONE_8
	typedef	std::recursive_mutex	Mutex;
	typedef std::mutex				RWMutex;

	// a simple class used for scoped_lock
	class DUEL_API _scope_lock
	{
	public:
		_scope_lock(Mutex& m) : mp(m)
		{
			mp.lock();
		}
		~_scope_lock()
		{
			mp.unlock();
		}
	private:
		Mutex&	mp;	
	};

#define DUEL_AUTO_MUTEX_NAME	AutoMutex

#define DUEL_AUTO_MUTEX			mutable	Mutex		DUEL_AUTO_MUTEX_NAME;
#define	DUEL_LOCK_AUTO_MUTEX	_scope_lock	DuelAutoMutexLock(DUEL_AUTO_MUTEX_NAME);
#define DUEL_LOCK_MUTEX(name)	_scope_lock	DuelMutexLock(name);


#define DUEL_THREAD_SYNC(sync)			std::condition_variable	sync;
#define DUEL_THREAD_WAIT(sync,lock)		sync.wait(lock);	
#define DUEL_THREAD_NOTIFY_ONE(sync)	sync.notify_one();
#define DUEL_THREAD_NOTIFY_ALL(sync)	sync.notify_all();

#define DUEL_RW_MUTEX(name)				mutable	RWMutex	name;
#define DUEL_LOCK_RW_MUTEX_READ(name)	std::unique_lock<RWMutex> DuelRWMutex(name);
#define DUEL_LOCK_RW_MUTEX_WRITE(name)	std::unique_lock<RWMutex> DuelRWMutex(name);

	// TODO: 搞清楚再弄
	// Thread-local pointer
	// #define _THREAD_POINTER(T, var) boost::thread_specific_ptr<T> var
	// #define _THREAD_POINTER_INIT(var) var(&deletePtr)
	// #define _THREAD_POINTER_VAR(T, var) boost::thread_specific_ptr<T> var (&deletePtr<T>)
	// #define _THREAD_POINTER_SET(var, expr) var.reset(expr)
	// #define _THREAD_POINTER_GET(var) var
	// #define _THREAD_POINTER_DELETE(var) var.reset(0)
	// Thread objects and related functions
	// #define _THREAD_TYPE boost::thread
	// #define _THREAD_CREATE(name, worker) boost::thread* name = _NEW_T(boost::thread, MEMCATEGORY_GENERAL)(worker);
	// #define _THREAD_DESTROY(name) _DELETE_T(name, thread, MEMCATEGORY_GENERAL)
	// #define _THREAD_HARDWARE_CONCURRENCY boost::thread::hardware_concurrency()
	// #define _THREAD_CURRENT_ID boost::this_thread::get_id()
	// #define _THREAD_WORKER_INHERIT

#define DUEL_THREAD_SLEEP(ms)			std::this_thread::sleep_for(ms);

#else

	typedef	boost::recursive_mutex	Mutex;
	// read-write mutex
	typedef	boost::shared_mutex		RWMutex;
	
#define DUEL_AUTO_MUTEX_NAME	AutoMutex

#define DUEL_AUTO_MUTEX			mutable	Mutex		DUEL_AUTO_MUTEX_NAME;
#define	DUEL_LOCK_AUTO_MUTEX	Mutex::scoped_lock	DuelAutoMutexLock(DUEL_AUTO_MUTEX_NAME);
#define DUEL_LOCK_MUTEX(name)	Mutex::scoped_lock	DuelMutexLock(name);

// 
#define DUEL_THREAD_SYNC(sync)			boost::condition	sync;
#define DUEL_THREAD_WAIT(sync,lock)		sync.wait(lock);	
#define DUEL_THREAD_NOTIFY_ONE(sync)	sync.notify_one();
#define DUEL_THREAD_NOTIFY_ALL(sync)	sync.notify_all();
// 
// #define DUEL_RW_MUTEX(name)				mutable	RWMutex	name;
// #define DUEL_LOCK_RW_MUTEX_READ(name)	boost::shared_lock<RWMutex> DuelRWMutex(name);
// #define DUEL_LOCK_RW_MUTEX_WRITE(name)	boost::unique_lock<RWMutex> DuelRWMutex(name);

	// TODO: 搞清楚再弄
	// Thread-local pointer
	// #define _THREAD_POINTER(T, var) boost::thread_specific_ptr<T> var
	// #define _THREAD_POINTER_INIT(var) var(&deletePtr)
	// #define _THREAD_POINTER_VAR(T, var) boost::thread_specific_ptr<T> var (&deletePtr<T>)
	// #define _THREAD_POINTER_SET(var, expr) var.reset(expr)
	// #define _THREAD_POINTER_GET(var) var
	// #define _THREAD_POINTER_DELETE(var) var.reset(0)
	// Thread objects and related functions
	// #define _THREAD_TYPE boost::thread
	// #define _THREAD_CREATE(name, worker) boost::thread* name = _NEW_T(boost::thread, MEMCATEGORY_GENERAL)(worker);
	// #define _THREAD_DESTROY(name) _DELETE_T(name, thread, MEMCATEGORY_GENERAL)
	// #define _THREAD_HARDWARE_CONCURRENCY boost::thread::hardware_concurrency()
	// #define _THREAD_CURRENT_ID boost::this_thread::get_id()
	// #define _THREAD_WORKER_INHERIT

#define DUEL_THREAD_SLEEP(ms)			boost::this_thread::sleep(boost::posix_time::millisec(ms));

#endif // DUAL_PLATFORM_WINDOWS_PHONE_8

	//  [7/9/2013 OMEGA] 正式版本.

	// for non-c++0x compiler. use boost instead.
#define DFunction		boost::function		
typedef	boost::thread	WorkThread;

	class DUEL_API DThread : public DObject
	{
	DUEL_DECLARE_RTTI(DThread)
	public:
		DThread();
		virtual ~DThread();

		void			start();

		bool			isRunning() const;	
		bool			isFinished() const;
		// wait until this thread is terminated;  
		void			join();
		// set the mask to termminiate  
		void			terminate();
		// wait until this thread has no tasks pending.  
		void			waitForIdle();
		//return the current load of this thread
		// load 这里的意思是 负荷.
		uint32			load();
		// append a task to do
		uint32			append(DFunction<void (void)> func);

		// signals-----------------------------

	protected:
		void			run();

		// atomic bool var to mark the thread the next loop will be terminated.
		DAtomicScalar<bool>		mbFinished;
		// the Task List contains function objects  
		typedef	std::list<DFunction<void (void)>>	TaskList;
		TaskList				mTaskList;
		// the mutex with which we protect task list
		DMutex					mTaskMutex;			
		// inside the thread, a task queue will be maintained.
		WorkThread*				mWorkThread;
		// condition mutex used by m_cond_locker   
		DSharedMutex			mCondMutex;
		// condition var with which we notify the thread for incoming tasks
		DCondition				mTaskCond;
	private:
		// non-copyable
		DThread&	operator = (const DThread& rh) { return *this; }
	};
	
}

#endif
