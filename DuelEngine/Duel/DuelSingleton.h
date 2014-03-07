//  [7/15/2012 OMEGA] created

#ifndef _DUELSINGLETON_H_
#define _DUELSINGLETON_H_

#include <assert.h>


namespace Duel {

	template <class T>
	class DSingleton
	{
		// forbidden operation.
	private:
		DSingleton(const DSingleton<T> &);
		DSingleton& operator=(const DSingleton<T> &);
		
		static T* & msSingleton()
		{
			static T* msSingleton_ =0 ;
			return msSingleton_;
		}



	public:
		// father class will do for sub-class
		DSingleton(void)
		{
			assert(!(msSingleton()));
			msSingleton() = static_cast<T*>(this);
		}
		// this will be call after sub-class's destruction 
		virtual ~DSingleton(void)
		{
			assert((msSingleton()));
			msSingleton() = 0;
		}
		// use _yourClass_::getSingleton() to get a reference of the class.
		static T& getSingleton(void)
		{
			assert((msSingleton()));
			return (*msSingleton());
		}
		// use _yourClass_::getSingletonPtr() to get a pointer to the class.
		static T* getSingletonPtr(void)
		{
			return (msSingleton());
		}
	};

}



#endif