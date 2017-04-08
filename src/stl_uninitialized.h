#ifndef _DIYSTL_UNINITIALIZED_
#define _DIYSTL_UNINITIALIZED_

#include "memory.h"
#include "iterators.h"
#include "utility.h"

DIYSTL_NAMESPACE_BEGIN

//construct和destroy应该写在allocator_traits和allocator（可以不提供）中

template<class T,class... Args>    
void construct(T* p, Args&&... args)
{
	new(p) T(forward<Args>(args)...); 
}

template<class T>
void destroy(T* p)
{
	p->~T();
}

//uninitialized_fill和uninitialized_fill_n可以针对单字节数据类型进行特例化（通过memset优化）

template<class ForwardIter,class T>
void uninitialized_fill(ForwardIter first, ForwardIter last, const T& val)
{
	auto oldFirst = first;
	try
	{
		for (; first != last; first++)
			construct(&(*first), val);
	}
	catch (...)
	{
		for (; oldFirst != first; oldFirst++)
			destruct(&(*oldFirst));
		throw;
	}
}

template<class ForwardIter, class size, class T>
ForwardIter uninitialized_fill_n(ForwardIter first, size n, const T& val)
{
	auto oldFirst = first; auto ret = first;
	try
	{
		for (; n>0; first++,n--)
			construct(&(*first), val);
		ret = first;
	}
	catch (...)
	{
		for (; oldFirst != first; oldFirst++)
			destroy(&(*oldFirst));
		throw;
	}
	return ret;
}

//used for cases where src and des do not overlap;use move_range instead if src and dest overlap
template<class InputIter,class ForwardIter>
ForwardIter uninitialized_copy(InputIter first, InputIter last, ForwardIter result)
{ 
	return uninitialized_copy_aux(first, last, result, value_type_pointer<ForwardIter>());
}

template<class InputIter, class ForwardIter, class T>
ForwardIter uninitialized_copy_aux(InputIter first, InputIter last, ForwardIter result, T*)
{
	return __uninitialized_copy_aux(first,last,result,type_traits<T>::is_POD_type());	
}

template<class T,class U>
U* __uninitialized_copy_aux(T* first,T* last,U* dest,__true_type )
{//is POD type
	memcpy(dest, first, (last - first)*sizeof(T));
	return dest+(last-first);
}

template<class InputIter, class ForwardIter>
ForwardIter __uninitialized_copy_aux(InputIter first, InputIter last, ForwardIter result, __false_type)
{
	auto ori = result; auto ret = result;
	try
	{
		for(;first!=last;first++,result++)
		{
			construct(&(*result),*first); //如果知道源对象马上要销毁（比如vector reallocate时），应该改成construct(&(*result),move(*first));
		}
		ret = result;
	}
	catch(...)
	{
		for(;ori!=result;ori++)
			destroy(&(*ori));
		throw;
	}
	return ret;
}


//vector.erase move_forward
//vector.insert move_backward

template<class BiIterator>
BiIterator move_backward(BiIterator first,BiIterator last,BiIterator des_end)
{
	return move_backward_aux(first, last, des_end, value_type_pointer<BiIterator>());
	
}

template<class BiIterator,class T>
BiIterator move_backward_aux(BiIterator first, BiIterator last, BiIterator des_end, T*)
{
	return __move_backward_aux(first, last, des_end, type_traits<T>::is_POD_type());
}

template<class BiIterator>
BiIterator __move_backward_aux(BiIterator first, BiIterator last, BiIterator des_end, __false_type)
{                                            
	while(last!=first)
	{
		*--des_end = diystl::move(*--last);
	}
	return des_end; //return beginning of destination
}

template<class T>
T* __move_backward_aux(T* first, T* last, T* des_end, __true_type)
{
	auto des_beg = des_end - (last - first);
	memmove(des_beg, first, (last - first)*sizeof(T));
	return des_beg;
}

template<class BiIterator>
BiIterator move_forward(BiIterator first, BiIterator last, BiIterator des_beg)
{
	return move_forward_aux(first, last, des_beg, value_type_pointer<BiIterator>());
}

template<class BiIterator, class T>
BiIterator move_forward_aux(BiIterator first, BiIterator last, BiIterator des_beg, T*)
{
	return __move_forward_aux(first, last, des_beg, type_traits<T>::is_POD_type());
}

template<class BiIterator>
BiIterator __move_forward_aux(BiIterator first, BiIterator last, BiIterator des_beg, __false_type)
{
	while (last != first)
	{
		*des_beg++ = diystl::move(*first++);
	}
	return des_beg; //return end of destination
}

template<class T>
T* __move_forward_aux(T* first, T* last, T* des_beg, __true_type)
{
	memmove(des_beg, first, (last - first)*sizeof(T));
	return des_beg;
}

DIYSTL_NAMESPACE_END

#endif




