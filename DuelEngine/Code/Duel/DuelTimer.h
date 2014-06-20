//  [7/15/2012 OMEGA] created

#ifndef _DUELTIMER_H_
#define _DUELTIMER_H_


#include "DuelCommon.h"

namespace Duel
{
#ifdef DUEL_PLATFORM_WINDOWS

	class DUEL_API DTimer : public DObject
	{
	DUEL_DECLARE_RTTI(DTimer)
	public:
		DTimer();			// must be called on the same thread that calls getMilliseconds.
		virtual ~DTimer();

		// reset DTimer
		void		reset();
		
		// using system call to get milliseconds since reset
		uint64		getMilliseconds();
		// using system call to get microseconds since reset
		uint64		getMicroseconds();
		// using system call to get milliseconds since reset, only cpu time measured
		uint64		getMillisecondsCPU();
		// using system call to get microseconds since reset, only cpu time measured
		uint64		getMicrosecondsCPU();


	private:	
		clock_t		mZeroClock;

        DWORD		mStartTick;
#ifdef DUEL_PLATFORM_WINDOWS_PHONE_8
		LARGE_INTEGER mLastTime;
#else
		LONGLONG	mLastTime;
#endif

        LARGE_INTEGER mStartTime;
        LARGE_INTEGER mFrequency;

		DWORD_PTR	mTimerMask;
	};
#endif
}



#endif