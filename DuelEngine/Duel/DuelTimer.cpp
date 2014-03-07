//  [7/15/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelTimer.h"
#include "DuelException.h"



namespace Duel {

	DUEL_IMPLEMENT_RTTI_1(DTimer, DObject);
#ifdef DUEL_PLATFORM_WINDOWS
	DTimer::DTimer() : mTimerMask( 0 )
	{
		reset();
	}
	DTimer::~DTimer()
	{
	}
	//////////////////////////////////////////////////////////
	void DTimer::reset()
	{
#ifdef DUEL_PLATFORM_WINDOWS_PHONE_8
		if (!QueryPerformanceFrequency(&mFrequency))
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InternalError,
				"Can not query cpu clock",
				"Duel::DTimer::reset")
		}

		if (!QueryPerformanceCounter(&mStartTime))
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InternalError,
				"Can not query cpu clock",
				"Duel::DTimer::reset")
		}

#else
		// Get the current process core mask
		DWORD_PTR	procMask;
		DWORD_PTR	sysMask;
		GetProcessAffinityMask(GetCurrentProcess(), &procMask, &sysMask);

		// if procMask is 0, consider there is only one core available
		// but using 0 will cause an infinite loop below
		if( procMask == 0 )
			procMask = 1;

		// find the lowest core that this process uses
		if( mTimerMask == 0 )
		{
			mTimerMask = 1;
			while( ( mTimerMask & procMask ) == 0 )
			{
				mTimerMask <<= 1;
			}
		}

			HANDLE thread = GetCurrentThread();

			// Set affinity to the first core
			DWORD_PTR oldMask = SetThreadAffinityMask(thread, mTimerMask);

			// Get the constant frequency
			QueryPerformanceFrequency(&mFrequency);

			// Query the timer
			QueryPerformanceCounter(&mStartTime);
			mStartTick = GetTickCount();

			// Reset affinity
			SetThreadAffinityMask(thread, oldMask);

			mLastTime = 0;
			mZeroClock = clock();
#endif
	}
	//////////////////////////////////////////////////////////
	uint64 DTimer::getMilliseconds()
	{
#ifdef	DUEL_PLATFORM_WINDOWS_PHONE_8
		LARGE_INTEGER curTime;
		if (!QueryPerformanceCounter(&curTime))
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InternalError,
				"Can not query cpu clock",
				"Duel::DTimer::getMilliseconds")
		}
		LONGLONG newTime = curTime.QuadPart - mStartTime.QuadPart;

		// Record last time for adjust
		mLastTime = curTime;
		
		// Re-calculate milliseconds
		unsigned long newTicks = (unsigned long) (1000 * newTime / mFrequency.QuadPart);

		return newTicks;
#else
		LARGE_INTEGER curTime;

		HANDLE thread = GetCurrentThread();

		// Set affinity to the first core
		DWORD_PTR oldMask = SetThreadAffinityMask(thread, mTimerMask);

		// Query the timer
		QueryPerformanceCounter(&curTime);

		// Reset affinity
		SetThreadAffinityMask(thread, oldMask);

		LONGLONG newTime = curTime.QuadPart - mStartTime.QuadPart;
    
		// scale by 1000 for milliseconds
		unsigned long newTicks = (unsigned long) (1000 * newTime / mFrequency.QuadPart);

		// detect and compensate for performance counter leaps
		// (surprisingly common, see Microsoft KB: Q274323)
		unsigned long check = GetTickCount() - mStartTick;
		signed long msecOff = (signed long)(newTicks - check);
		if (msecOff < -100 || msecOff > 100)
		{
			// We must keep the timer running forward :)
			LONGLONG adjust = (std::min)(msecOff * mFrequency.QuadPart / 1000, newTime - mLastTime);
			mStartTime.QuadPart += adjust;
			newTime -= adjust;

			// Re-calculate milliseconds
			newTicks = (unsigned long) (1000 * newTime / mFrequency.QuadPart);
		}

		// Record last time for adjust
		mLastTime = newTime;

		return newTicks;
#endif
	}
	//////////////////////////////////////////////////////////
	uint64 DTimer::getMicroseconds()
	{
#ifdef DUEL_PLATFORM_WINDOWS_PHONE_8
		LARGE_INTEGER curTime;
		if (!QueryPerformanceCounter(&curTime))
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InternalError,
				"Can not query cpu clock",
				"Duel::DTimer::getMicroseconds")
		}
		LONGLONG newTime = curTime.QuadPart - mStartTime.QuadPart;

		// Record last time for adjust
		mLastTime = curTime;

		// scale by 1000000 for microseconds
		unsigned long newMicro = (unsigned long) (1000000 * newTime / mFrequency.QuadPart);

		return newMicro;

#else

		LARGE_INTEGER curTime;

		HANDLE thread = GetCurrentThread();

		// Set affinity to the first core
		DWORD_PTR oldMask = SetThreadAffinityMask(thread, mTimerMask);

		// Query the timer
		QueryPerformanceCounter(&curTime);

		// Reset affinity
		SetThreadAffinityMask(thread, oldMask);

		LONGLONG newTime = curTime.QuadPart - mStartTime.QuadPart;
    
		// get milliseconds to check against GetTickCount
		unsigned long newTicks = (unsigned long) (1000 * newTime / mFrequency.QuadPart);
    
		// detect and compensate for performance counter leaps
		// (surprisingly common, see Microsoft KB: Q274323)
		unsigned long check = GetTickCount() - mStartTick;
		signed long msecOff = (signed long)(newTicks - check);
		if (msecOff < -100 || msecOff > 100)
		{
			// We must keep the timer running forward :)
			LONGLONG adjust = (std::min)(msecOff * mFrequency.QuadPart / 1000, newTime - mLastTime);
			mStartTime.QuadPart += adjust;
			newTime -= adjust;
		}

		// Record last time for adjust
		mLastTime = newTime;

		// scale by 1000000 for microseconds
		unsigned long newMicro = (unsigned long) (1000000 * newTime / mFrequency.QuadPart);

		return newMicro;

#endif // DUAL_PLATFORM_WINDOWS_PHONE_8
	}
	//////////////////////////////////////////////////////////
	uint64 DTimer::getMillisecondsCPU()
	{
		clock_t newClock = clock();
		return (unsigned long)( (DReal)( newClock - mZeroClock ) / ( (DReal)CLOCKS_PER_SEC / 1000.0 ) ) ;
	}
	//////////////////////////////////////////////////////////
	uint64 DTimer::getMicrosecondsCPU()
	{
		clock_t newClock = clock();
		return (unsigned long)( (DReal)( newClock - mZeroClock ) / ( (DReal)CLOCKS_PER_SEC / 1000000.0 ) ) ;
	}

#endif
}