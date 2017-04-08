#ifndef _DIY_VECTOR_
#define _DIY_VECTOR_

#include "memory.h"
#include "utility.h"
#include "stl_uninitialized.h"

#include<stdio.h>

DIYSTL_NAMESPACE_BEGIN

//多冗余代码，简化

template<class T,class Allocator>
class vector_base                 //responsible for allocate and deallocate memory
{
public:
	typedef Allocator allocator_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	const allocator_type& get_allocator() const { return _allocator; }

	allocator_type& get_allocator()  { return _allocator; }

	void set_allocator(const allocator_type& allocator) { _allocator = allocator; }

	vector_base() :_begin(nullptr), _end(nullptr), _end_of_storage(nullptr){}//_allocator自动调用默认构造函数。

	vector_base(const allocator_type& allocator) :_begin(nullptr), _end(nullptr), _end_of_storage(nullptr),
		_allocator(allocator)
	{
	}

	vector_base(size_type n)
	{
		_end = _begin = allocate(n);
	}

	vector_base(size_type n, const allocator_type& allocator) :_allocator(allocator)
	{
		_end = _begin = allocate(n);
	}

	~vector_base()
	{
		deallocate(_begin);
	}

protected:
	T* allocate(size_type n)
	{
		T* ret= _allocator.allocate(n);
		_end_of_storage = ret + n;
		return ret;
	}

	void deallocate(T* p)
	{
		_allocator.deallocate(p);
		_end_of_storage = nullptr;
	}

	allocator_type _allocator;
	T* _begin;
	T* _end;
	T* _end_of_storage;
};

template< class T,class Allocator=allocator<T> >
class vector:public vector_base<T,Allocator>
{
	typedef vector_base<T,Allocator> base_type;
public:
	typedef T value_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T* iterator;
	typedef const T* const_iterator;
	typedef reverse_iterator<const_iterator> const_reverse_iterator;
	typedef reverse_iterator<iterator> reverse_iterator;

	vector():base_type(){}
	explicit vector(const allocator_type& alloc):base_type(alloc){ }
	explicit vector(size_type n);
	explicit vector(size_type n, const value_type& val);
	explicit vector(size_type n,const allocator_type& alloc);
	explicit vector(size_type n,const value_type& val,const allocator_type& );
	
	template<class InputIterator>
	vector(InputIterator  first, typename enable_if<is_pointer<InputIterator>::value, InputIterator >::type last);
	
	
	template<class InputIterator>
	vector(InputIterator  first, typename enable_if< is_pointer<InputIterator>::value, InputIterator >::type last, const allocator_type& alloc);

	//	vector(initializer_list<value_type> il,const allocator_type& alloc=allocator_type());

	vector(const vector& x);
	vector(const vector& x,const allocator_type& alloc);
	vector(vector&& x);
	vector(vector&& x,const allocator_type& alloc);

	// operator=()

	//void assign(initializer_list il);
	void assign(size_type n,const value_type& val);
	template<class InputIterator>
	void assign(InputIterator first,InputIterator last);

	reference operator[](size_type n) {return *(_begin+n);}
	const_reference operator[](size_type n) const {return *(_begin+n);}

	value_type* data() DIYSTL_NOEXCEPT
	{
		return &(*_begin);
	}
	const value_type* data() const DIYSTL_NOEXCEPT
	{
		return &(*_begin);
	}

	reference at(size_type n)
	{
		if (n >=size()) printf("argument n of function vector::at out of range"); //throw 
		assert(n < size());
		return (*this)[n];
	}

	const_reference at(size_type n) const
	{
		// if(n=>size()) throw();
		return (*this)[n];
	}
	
	void pop_back() { if(_begin!=_end) diystl::destroy(--_end); }
	
	reference front() {return *_begin;}
	reference back() {return *(_end-1);}
	const_reference front() const {return *_begin;}
	const_reference back() const {return *(_end-1);}

	iterator begin() DIYSTL_NOEXCEPT
	{ return _begin; }

	const_iterator begin() const DIYSTL_NOEXCEPT
	{ return _begin; }

	iterator end() DIYSTL_NOEXCEPT
	{ return _end; }

	const_iterator end() const DIYSTL_NOEXCEPT
	{ return _end; }

	reverse_iterator rbegin() DIYSTL_NOEXCEPT
	{ return reverse_iterator(_end); }

	const_reverse_iterator rbegin() const DIYSTL_NOEXCEPT
	{ return const_reverse_iterator(_end); }

	reverse_iterator rend() DIYSTL_NOEXCEPT
	{ return reverse_iterator(_begin); }

	const_reverse_iterator rend() const DIYSTL_NOEXCEPT
	{ return const_reverse_iterator(_begin); }

	const_iterator cbegin() const DIYSTL_NOEXCEPT
	{ return _begin; }
	
	const_iterator cend() const DIYSTL_NOEXCEPT
	{ return _end; }

	const_reverse_iterator crbegin() const DIYSTL_NOEXCEPT
	{ return const_reverse_iterator(_end); }

	const_reverse_iterator crend() const DIYSTL_NOEXCEPT
	{ return const_reverse_iterator(_begin); }

	size_type capacity()
	{
		return size_type(_end_of_storage - _begin);
	}

	size_type capacity() const DIYSTL_NOEXCEPT 
	{ return size_type(_end_of_storage - _begin); }

	bool empty()
	{ return _begin==_end; }

	//size_type max_size();

	void swap(vector& x);
	void reserve(size_type n);

	void shrink_to_fit();

	void clear();
	void push_back(const value_type& val);
	void push_back(value_type&& val);

	iterator insert(const_iterator position,const value_type& val);
	iterator insert(const_iterator position,value_type&& val);
	iterator insert(const_iterator position,size_type n,const value_type& val);
//	iterator insert(const_iterator position,uninitialized_list<value_type> il);
	template<class InputIterator>
	iterator insert(const_iterator position,InputIterator first,InputIterator last);

	 size_type size() const {return _end - _begin;}
	 size_type size()  { return _end - _begin; }

	 vector<T, Allocator>& operator=(const vector& x);
	 vector<T, Allocator>& operator=(vector&& x);

	 template<class... Args>
	 void emplace_back(Args&&...);
	 template<class... Args>
	 iterator emplace(const_iterator ,Args&&...);

	 iterator erase(const_iterator);
	 iterator erase(const_iterator first, const_iterator last);

	 void resize(size_type n);
	 void resize(size_type n, const value_type& val);

	 ~vector();

protected:
	void init_with_n_same_elements(size_type n, const value_type& val);

	void reallocate_and_copy_original(size_type);
	void reallocate_and_copy_original(size_type old_cap, iterator insert_beg, size_type insert_n);

	size_type get_new_capacity(size_type curCap)
	{
		if(curCap<=0) return 1;
	/*	return (max_size()/2<curCap) ? max_size():2*curCap;*/
		return 2 * curCap;
	}

	iterator end_of_storage() {return _end_of_storage;}
	iterator inc_end(){ return ++ _end;}
	iterator dec_end(){ return -- _end;}
	void set_begin(const iterator& beg_){ _begin = beg_; }
	void set_end(const iterator& end_) { _end = end_; }

	void __swap(iterator& i1, iterator& i2)
	{
		auto temp = i2;
		i2 = i1;
		i1 = temp;
	}

	void destroy(pointer first, pointer last);

};

 template<class T,class Allocator>
 void vector<T, Allocator>::destroy(pointer first, pointer last)
 {
	 for (; first != last; first++)
		 diystl::destroy(first);
 }

//template<class T,class Allocator>
//vector<T,Allocator>::vector(const allocator_type& alloc):base_type(alloc)
//{
//}
//

template<class T, class Allocator>
void vector<T, Allocator>::init_with_n_same_elements(size_type n,const value_type& val)
{
	try
	{
		set_end( uninitialized_fill_n(begin(), n, val) );
	}
	catch (...){ deallocate(begin()); }
}

template<class T, class Allocator>
vector<T, Allocator>::vector(size_type n) :base_type(n) //add try catch 
{
	init_with_n_same_elements(n, value_type());
}

template<class T,class Allocator>
vector<T,Allocator>::vector(size_type n,const allocator_type& alloc):base_type(n,alloc)
{
	init_with_n_same_elements(n, value_type());
}

template<class T, class Allocator>
vector<T, Allocator>::vector(size_type n, const value_type& val) :base_type(n)
{
	init_with_n_same_elements(n, val);
}
	
template<class T,class Allocator>
vector<T,Allocator>::vector(size_type n,const value_type& val,const allocator_type& alloc):base_type(n,alloc)
{
	init_with_n_same_elements(n, val);
}

template<class T, class Allocator>
template<class InputIterator>
vector<T, Allocator>::vector(InputIterator  first, typename enable_if< is_pointer<InputIterator>::value,InputIterator >::type last)
:base_type(size_type(last - first))
{
	try
	{
		set_end( uninitialized_copy(first, last, begin()) );
	}
	catch (...) { deallocate(begin()); }
}


template<class T,class Allocator>
template<class InputIterator>
vector<T, Allocator>::vector(InputIterator  first, typename enable_if< is_pointer<InputIterator>::value, InputIterator >::type last, const allocator_type& alloc)
:base_type(size_type(last - first), alloc)
{
	try
	{
		set_end( uninitialized_copy(first, last, begin()) );
	}
	catch (...) { deallocate(begin()); }
}
	
//	//vector(initializer_list<value_type> il,const allocator_type& alloc=allocator_type());
//
template<class T,class Allocator>
vector<T, Allocator>::vector(const vector& x) 
:vector(x.begin(), x.end())
{
}

template<class T,class Allocator>
vector<T, Allocator>::vector(const vector& x, const allocator_type& alloc) 
:vector(x.begin(), x.end(),alloc)
{
}

template<class T,class Allocator>
vector<T,Allocator>::vector(vector&& x)
:base_type(x.size())
{
	printf("move constructor\n");
	set_begin(x.begin());
	set_end(x.end());
	x.set_begin(nullptr);
	x.set_end(nullptr);
}

template<class T,class Allocator>
vector<T,Allocator>::vector(vector&& x,const allocator_type& alloc)
:base_type(x.size(),alloc)
{
	printf("move constructor alloc\n");
	set_begin(x.begin());
	set_end(x.end());
	x.set_begin(nullptr);
	x.set_end(nullptr);
}

template<class T, class Allocator>
vector<T, Allocator>& vector<T, Allocator>::operator=(const vector& x)
{
		vector(x).swap(*this);
		return *this;
}

template<class T, class Allocator>
vector<T, Allocator>& vector<T, Allocator>::operator=(vector&& x)
{
	swap(x);
	return *this;
}

//template<class T, class Allocator>
//vector& vector<T, Allocator>::operator= (initializer_list<value_type> il);

template<class T, class Allocator>
template<class InputIterator>
void vector<T, Allocator>::assign(InputIterator first, InputIterator last)
{
	vector(first, last).swap(*this);
}

template<class T, class Allocator>
void vector<T, Allocator>::assign(size_type n, const value_type& val)
{
	vector(n, val).swap(*this);
}

//template<class T, class Allocator>
//void vector<T, Allocator>::assign(initializer_list<value_type> il)
//{
//}

template<class T, class Allocator>
void vector<T, Allocator>::shrink_to_fit()
{
	vector(*this).swap(*this);
}

//Whether the container allocators are also swapped is not defined, unless in the case the appropriate allocator traits indicate explicitly that they shall propagate.
//The bool specialization of vector provides an additional overload for this function(see vector<bool>::swap).
template<class T, class Allocator>
void vector<T, Allocator>::swap(vector& x)
{
	__swap(_begin, x._begin);
	__swap(_end, x._end);
	__swap(_end_of_storage, x._end_of_storage);
}

template<class T, class Allocator>
void vector<T, Allocator>::reallocate_and_copy_original(size_type n) //push_back etc should increase _end after call this function
{
	auto new_begin = allocate(get_new_capacity(n));
	auto new_end = new_begin;
	bool fail = false;
	try
	{
		 new_end=uninitialized_copy(begin(), end(), new_begin);
	}
	catch (...)
	{
		fail = true;
		destroy(new_begin, new_end);
		deallocate(new_begin);
	}
	if (fail) return;
	set_begin(new_begin); set_end(new_end);
	auto old_begin = begin(); auto old_end = end();
	destroy(old_begin, old_end);
}

template<class T, class Allocator>
void vector<T, Allocator>::reallocate_and_copy_original(size_type old_cap,iterator insert_beg,size_type insert_n)//insert etc needn't increase _end after call this function
{
	auto new_begin = allocate(get_new_capacity(old_cap));
	auto new_end1 = new_begin, new_end2 = new_begin;
	bool fail = false;
	bool half_finish = false;
	try
	{
		new_end1 = uninitialized_copy(begin(), insert_beg, new_begin);
		half_finish = true;
		new_end2 = uninitialized_copy(insert_beg, end(), new_end1 + insert_n);
	}
	catch (...)
	{
		fail = true;
		if(!half_finish) destroy(new_begin, new_end1);
		else 
		{
			destroy(new_begin,new_end1); 
			destroy(new_end1+insert_n,new_end2); 
		}
		deallocate(new_begin);
	}
	if (fail) return;
	set_begin(new_begin); set_end(new_end2);
	auto old_begin = begin(); auto old_end = end();
	destroy(old_begin, old_end);
}

template<class T,class Allocator>
void vector<T,Allocator>::push_back(const T& val)
{
	if(end()!=end_of_storage())
	{
		diystl::construct(end(),val);
	}
	else
	{
		auto old_size = size();
		reallocate_and_copy_original(size() + 1);
		diystl::construct(end(),val);
	}
	inc_end();
}

template<class T,class Allocator>
void vector<T,Allocator>::push_back( T&& val)
{
	if (end() != end_of_storage())
	{
		diystl::construct(end(), diystl::move(val));
	}
	else
	{
		auto old_size = size();
		reallocate_and_copy_original(size() + 1);
		diystl::construct(end(),diystl::move(val));
	}
	inc_end();
}

template<class T, class Allocator>
typename vector<T, Allocator>::iterator
vector<T, Allocator>::erase(const_iterator position)
{
	auto pos = _begin + (position - _begin);
	diystl::destroy(position);
	move_forward(pos+1,_end,pos);//des_begin
	_end--;
	return _begin+(position+1-_begin);
}

template<class T, class Allocator>
typename vector<T, Allocator>::iterator
vector<T, Allocator>::erase(const_iterator first, const_iterator last)
{
	destroy(const_cast<iterator>(first), const_cast<iterator>(last));
	move_forward(const_cast<iterator>(last), _end, const_cast<iterator>(first));
	_end-=last-first;
	return const_cast<iterator>(last);
}

template<class T,class Allocator>
typename vector<T, Allocator>::iterator
vector<T,Allocator>::insert(const_iterator position,const value_type& val)
{
	auto offset = position - begin();
	auto pos = begin() + (position - begin());
	if(_end!=_end_of_storage)
	{
		if(position!=end()) 
			 move_backward(pos,_end,_end+1);
		diystl::construct(pos,val);
		inc_end();
	}
	else 
	{
		reallocate_and_copy_original(size()+1,pos,1); 
		diystl::construct(begin() + offset, val);
	}
	return begin() + offset;
}


template<class T, class Allocator>
typename vector<T, Allocator>::iterator
vector<T, Allocator>::insert(const_iterator position, value_type&& val)
{
	auto offset = position - begin();
	auto pos = begin() + (position - begin());
	if (_end != _end_of_storage)
	{
		if (position != end())
			move_backward(pos, _end, _end + 1);
		diystl::construct(pos, diystl::move(val));
		inc_end();
	}
	else
	{
		reallocate_and_copy_original(size() + 1, pos, 1);
		diystl::construct(begin() + offset, diystl::move(val));
	}
	return begin() + offset;
}


 //template<class T,class Allocator>
 //typename vector<T, Allocator>::iterator 
 //vector<T,Allocator>::insert(const_iterator position,size_type n,const value_type& val)
 //{
 //}

 //template<class T,class Allocator>
 //template<class InputIterator>
 //typename vector<T, Allocator>::iterator
 //vector<T, Allocator>::insert(const_iterator position, InputIterator first, InputIterator last)
 //{

 //}

 //template<class T, class Allocator>
 //iterator vector<T, Allocator>::insert(const_iterator position, uninitialized_list<value_type> il)
 //{

 //}

template<class T, class Allocator>
template<class... Args>
typename vector<T, Allocator>::iterator
vector<T, Allocator>::emplace(const_iterator position, Args&&... args)
{
	auto offset = position - begin();
	auto pos = begin() + (position - begin());
	if (_end != _end_of_storage)
	{
		if (position != end())
			move_backward(pos, _end, _end + 1);
		diystl::construct(pos,diystl::forward<Args>(args)...);
		inc_end();
	}
	else
	{
		reallocate_and_copy_original(size() + 1, pos, 1);
		diystl::construct(begin() + offset, diystl::forward<Args>(args)...);
	}
	return begin() + offset;
}

 template<class T,class Allocator>
 template<class... Args>
 void vector<T,Allocator>::emplace_back(Args&&...args)
 {
	if (end() != end_of_storage())
	{
		diystl::construct(end(), diystl::forward<Args>(args)...);
	}
	else
	{
		auto old_size = size();
		reallocate_and_copy_original(size() + 1);
		diystl::construct(end(), diystl::forward<Args>(args)...);
	}
	inc_end();
 }

 template<class T,class Allocator>
 void vector<T,Allocator>::resize(size_type n)
 {
 	if(n==size()) return;
 	if(n<size()) 
 	{
		destroy(begin() + n, end());
 	}
 	else
 	{
		auto old_size = size();
 		reallocate_and_copy_original(n);
 		uninitialized_fill_n(begin()+old_size,n-old_size,value_type());
 	}
	set_end(begin() + n);
 }

 template<class T,class Allocator>
 void  vector<T, Allocator>::resize(size_type n, const value_type& val)
 {
	 if (n == size()) return;
	 if (n<size())
	 {
		 destroy(begin() + n, end());
	 }
	 else
	 {
		 auto old_size = size();
		 reallocate_and_copy_original(n);
		 uninitialized_fill_n(begin() + old_size, n - old_size, val);
	 }
	 set_end(begin() + n);
 }

template<class T, class Allocator>
void vector<T, Allocator>::reserve(size_type n)
{
	if (n <= capacity()) return;
	reallocate_and_copy_original(n);
}

 template<class T,class Allocator>
 void vector<T,Allocator>::clear()
 {
	 vector().swap(*this);
 }

 template<class T,class Allocator>
 vector<T,Allocator>::~vector()
 {
	  destroy(begin(), end());
	 _begin = _end = _end_of_storage = iterator();
 }

 template<class T,class Alloc>
 bool operator==(vector<T, Alloc> v1, vector<T, Alloc> v2)
 {
	 if (v1.size() != v2.size()) return false;
	 for (auto i = 0; i < v1.size(); i++)
		 if (v1[i] != v2[i])
			return false;
	return true;
 }

 template<class T,class Alloc>
 bool operator!=(vector<T, Alloc> v1, vector<T, Alloc> v2)
 {
	 return !(v1 == v2);
 }

 /*template<class T, class Alloc>
 bool operator<(vector<T, Alloc> v1, vector<T, Alloc> v2)
 {

 }

 template<class T, class Alloc>
 bool operator>(vector<T, Alloc> v1, vector<T, Alloc> v2)
 {

 }

 template<class T,class Alloc>
 bool operator<=(vector<T, Alloc> v1, vector<T, Alloc> v2)
 {

 }

 template<class T,class Alloc>
 bool operator>=(vector<T, Alloc> v1, vector<T, Alloc> v2)
 {

 }
*/
 template<class T,class Alloc>
 void swap(vector<T, Alloc>& lhs, vector<T, Alloc>& rhs)
 {
	 lhs.swap(rhs);
 }

DIYSTL_NAMESPACE_END

#endif