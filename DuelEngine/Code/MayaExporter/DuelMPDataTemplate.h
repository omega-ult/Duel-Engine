// created [10/24/2013 OMEGA]

#ifndef _DUELMPDATATEMPLATE_H_
#define _DUELMPDATATEMPLATE_H_


#include <vector>
#include <string>
#include <hash_map>
#include <map>
#include <iostream>
#include <set>
#include <stdarg.h>

using std::string;

template<typename Type, unsigned int Size>
class DataTemplate
{
public:
	DataTemplate()
	{
		this->set(NULL);
	}
	DataTemplate(Type const * ptr)
	{
		this->set(ptr);
	}

	DataTemplate(size_t num, ...)
	{
		va_list argptr;
		va_start(argptr, num);
		this->set(num, argptr);
	}

	Type& operator[](int i)
	{
		return value_[i];
	}
	Type const & operator[](int i) const
	{
		return value_[i];
	}

	DataTemplate& operator=(DataTemplate const & rhs)
	{
		for (size_t i = 0; i < Size; ++ i)
		{
			value_[i] = rhs.value_[i];
		}
		return *this;
	}

	bool operator==(DataTemplate const & rhs) const
	{
		bool is_same = true;
		for (size_t i = 0; i < Size; ++ i)
		{
			if (value_[i] != rhs.value_[i])
			{
				is_same = false;
			}
		}
		return is_same;
	}

	void set(Type const * ptr)
	{
		if (!ptr)
		{
			for (size_t i = 0; i < Size; ++ i)
			{
				value_[i] = static_cast<Type>(0);
			}
		}
		else
		{
			for (size_t i = 0; i < Size; ++ i)
			{
				value_[i] = (*ptr + i);
			}
		}
	}

	void set(size_t num, va_list argptr)
	{
		for (size_t i = 0; i < num; ++ i)
		{
			value_[i] = va_arg(argptr, Type);
		}
		va_end(argptr);
	}

private:
	Type value_[Size];
};

#endif