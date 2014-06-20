//  [10/11/2012 OMEGA] created

#ifndef	_DUELATOMIC_H_
#define _DUELATOMIC_H_

#include "DuelConfig.h"

namespace Duel
{

#ifdef DUEL_PLATFORM_WINDOWS
#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#endif
#if !defined(NOMINMAX) && defined(_MSC_VER)
#	define NOMINMAX // required to stop windows.h messing up std::min
#endif
#include <windows.h>
#include <intrin.h>

#define LOCKED_COMPARE_EXCHANGE16	_InterlockedCompareExchange16
#define LOCKED_COMPARE_EXCHANGE32	_InterlockedCompareExchange
#define LOCKED_COMPARE_EXCHANGE64	_InterlockedCompareExchange64

#define LOCKED_INCREMENT16			InterlockedIncrement16
#define LOCKED_INCREMENT32			InterlockedIncrement
#define LOCKED_INCREMENT64			InterlockedIncrement64

#define LOCKED_DECREMENT16			InterlockedDecrement16
#define LOCKED_DECREMENT32			InterlockedDecrement
#define LOCKED_DECREMENT64			InterlockedDecrement64

#endif

	template<class T> 
	class DUEL_API DAtomicScalar
	{
	public:

		DAtomicScalar (const T &initial)
			: mField(initial)
		{   }

		DAtomicScalar (const DAtomicScalar<T> &cousin)
			: mField(cousin.mField)
		{   }

		DAtomicScalar () 
		{   }

		void operator= (const DAtomicScalar<T> &cousin)
		{
			mField = cousin.mField;
		}

		T get (void) const
		{
			return mField;
		}

		void set (const T &v)
		{
			mField = v;
		}   

		bool cas (const T &old, const T &nu)
		{
			if (sizeof(T)==2) {
				return LOCKED_COMPARE_EXCHANGE16((SHORT*)&mField, static_cast<SHORT>(nu), static_cast<SHORT>(old)) == static_cast<SHORT>(old);
			} 
			else if (sizeof(T)==4) 
			{
				return LOCKED_COMPARE_EXCHANGE32((LONG*)&mField, static_cast<LONG>(nu), static_cast<LONG>(old)) == static_cast<LONG>(old);
			} 
			else if (sizeof(T)==8) {
				return LOCKED_COMPARE_EXCHANGE64((LONGLONG*)&mField, static_cast<LONGLONG>(nu), static_cast<LONGLONG>(old)) == static_cast<LONGLONG>(old);
			} 
			// not a supported size, action failed
			return false;
		}

		T operator++ (void)
		{
			if (sizeof(T)==2) {
				return LOCKED_INCREMENT16((SHORT*)&mField);
			} else if (sizeof(T)==4) {
				return LOCKED_INCREMENT32((LONG*)&mField);
			} else if (sizeof(T)==8) {
				return LOCKED_INCREMENT64((LONGLONG*)&mField);
			}
			// not a supported size, action failed
			return mField;
		}

		T operator-- (void)
		{
			if (sizeof(T)==2) {
				return LOCKED_DECREMENT16((SHORT*)&mField);
			} else if (sizeof(T)==4) {
				return LOCKED_DECREMENT32((LONG*)&mField);
			} else if (sizeof(T)==8) {
				return LOCKED_DECREMENT64((LONGLONG*)&mField);
			}
			// not a supported size, action failed
			return mField;
		}

		T operator++ (int)
		{
			if (sizeof(T)==2) {
				return LOCKED_INCREMENT16((SHORT*)&mField)-1;
			} else if (sizeof(T)==4) {
				return LOCKED_INCREMENT32((LONG*)&mField)-1;
			} else if (sizeof(T)==8) {
				return LOCKED_INCREMENT64((LONGLONG*)&mField)-1;
			}
			// not a supported size, action failed
			return mField;
		}

		T operator-- (int)
		{
			if (sizeof(T)==2) {
				return LOCKED_DECREMENT16((SHORT*)&mField)+1;
			} else if (sizeof(T)==4) {
				return LOCKED_DECREMENT32((LONG*)&mField)+1;
			} else if (sizeof(T)==8) {
				return LOCKED_DECREMENT64((LONGLONG*)&mField)+1;
			}
			// not a supported size, action failed
			return mField;
		}

		volatile T mField;

	};

}

#endif