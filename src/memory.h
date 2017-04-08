#ifndef _DIYSTL_MEMORY_
#define _DIYSTL_MEMORY_
#include "config.h"
#include<new>
#include<cassert>

//Header <memory> provides a specialization of allocator for the void type,why?
//因为sizeof(void)非法
//default allocator

//In the standard default allocator, the block of storage is allocated using ::operator new one or more times, and throws bad_alloc if it cannot allocate the total amount of storage requested.

DIYSTL_NAMESPACE_BEGIN

template<class T>
class allocator     
{
public:
	typedef T value_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t  difference_type;

	template<class U>
	struct rebind { typedef allocator<U> other;};

	allocator(){}
	allocator(const allocator&){}

	template<class U>
	allocator(const allocator<U>&){}

	~allocator(){}

	pointer address(reference t)  {return &t;}
	const_pointer address(const_reference t) const {return &t;}

	 pointer allocate(size_type n,const void* unused=nullptr)
	{
		 
		 if (n > max_size())
		 {  //throw 
			 printf("warning:the first argument n is too big,should assign a smaller number.\n");
		 }
		 assert(n <= max_size());
		pointer p=(pointer)::operator new(n*sizeof(T)); //分配内存失败时调用set_new_handler设置的处理函数
		return p;
	}

	 void deallocate(pointer p,size_type n=0)
	{
		::operator delete(p); //这样有问题？再改。只删除n个T的内存，但可能new时的内存超过这个
	}

	 size_type max_size() const //malloc和::operator new的参数类型都为size_t
	{
		static size_type n= (~size_type(0))/sizeof(T);
		return n;
	}

	template<class... Args>
	 void construct(pointer p,Args&&... args)//args为universal reference
	{
		new((void*)p) T(forward<Args>(args)...); //forward用于保持传递到construct的参数传递到placement new时的值特性（左值或右值）。
	}

	 void destroy(pointer p)
	{
		p->~T();
	}
};




template<>
class allocator<void>
{
	typedef void value_type;
	typedef void* pointer;
	typedef const void* const_pointer;
	typedef size_t size_type;
	typedef ptrdiff_t  difference_type;
	template<class U> struct rebind{ typedef allocator<U> other;};
};


// template <class T>
// class min_allocator //来自libc++
// {
// public:
//     typedef T value_type;
//     typedef min_pointer<T> pointer;

//     min_allocator() = default;
//     template <class U>
//     min_allocator(min_allocator<U>) {}

//     pointer allocate(std::ptrdiff_t n)
//     {
//         return pointer(static_cast<T*>(::operator new(n*sizeof(T))));
//     }

//     void deallocate(pointer p, std::ptrdiff_t)
//     {
//         return ::operator delete(p.ptr_);
//     }

//     friend bool operator==(min_allocator, min_allocator) {return true;}
//     friend bool operator!=(min_allocator x, min_allocator y) {return !(x == y);}
// };



DIYSTL_NAMESPACE_END

#endif