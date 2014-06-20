//  [8/20/2013 OMEGA] created

#ifndef _DUELSIGNAL_H_
#define _DUELSIGNAL_H_

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/signals2.hpp>

namespace Duel
{

	// use DSignal<returnType(param1,param2)> to declare a signal.
	// use sig.connect(DBind(&className::memberFunc, obj)) to bind a member function,
	// for more information about boost signal. just google.
	// for using signal, _SCL_SECURE_NO_WARNINGS will supress the warning C4996.
#define _SCL_SECURE_NO_WARNINGS
#define D_SCL_SECURE_NO_WARNINGS
	// about error : C2298 :http://stackoverflow.com/questions/4929537/how-can-i-use-boostbind-to-bind-a-class-member-function
#define BOOST_MEM_FN_ENABLE_STDCALL
#define	DSignal				boost::signals2::signal
#define DSignalConnection	boost::signals2::connection
#define DBind				boost::bind
	// used in retrieve signal results. 
	// use std::vector<T> result = signal() to retrieve execution results
	template <typename T> 
	struct __declspec(dllexport) ResultCombiner   
	{ 
		typedef std::vector<T> result_type; 

		template <typename InputIterator> 
		result_type operator()(InputIterator first, InputIterator last) const 
		{ 
			result_type	ret;
			while (first != last)
			{
				ret.push_back(*first);
				++first;
			}
			return ret;
		} 

	}; 

}

#endif