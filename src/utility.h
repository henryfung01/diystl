#ifndef _UTILITY_
#define _UTILITY_

#include "config.h"

DIYSTL_NAMESPACE_BEGIN 

template<bool B,class T=void>
struct enable_if 
{ 
};

template<class T>
struct enable_if<true, T>
{
	typedef T type;
};

template<class T>
struct is_pointer
{
	static const bool value = false;
};

template<class T>
struct is_pointer<T*>
{
	static const bool value = true;
};


template<class T>
struct is_pointer<const T*>
{
	static const bool value = true;
};

template<class T>
struct remove_reference
{
	typedef T value_type;
};


template<class T>
struct remove_reference<T&>
{
	typedef T value_type;
};

template<class T>
struct remove_reference<T&&>
{
	typedef T value_type;
};

template<class T>
struct is_lvalue_reference
{
	static const bool value = false;
};

template<class T>
struct is_lvalue_reference<T&>
{
	static const bool value = true;
};

template<class T>
T&& forward(typename remove_reference<T>::value_type& val)
{
	return static_cast<T&&>(val);
}

template<class T>
T&& forward(typename remove_reference<T>::value_type&& val)
{
	static_assert(!is_lvalue_reference__<_Tp>::value,"can not forward an rvalue as an lvalue");
	return static_cast<T&&>(val);
}


template<class T>
typename remove_reference<T>::value_type&& move(T&& t)
{
	typedef remove_reference<T>::value_type value_type;
	return static_cast<value_type&&>(t);
}



DIYSTL_NAMESPACE_END

#endif