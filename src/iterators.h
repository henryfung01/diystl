#ifndef _ITERATORS_
#define _ITERATORS_

#include "type_traits.h"

DIYSTL_NAMESPACE_BEGIN

struct input_iterator_tag{};
struct output_iterator_tag{};
struct forward_iterator_tag :input_iterator_tag{};
struct bidirectional_iterator_tag :forward_iterator_tag{};
struct random_access_iterator_tag :bidirectional_iterator_tag{};


template<class _Category, class _Tp, class _Distance = ptrdiff_t,
class _Pointer = _Tp*, class _Reference = _Tp&>
struct __iterator
{
	typedef _Tp        value_type;
	typedef _Distance  difference_type;
	typedef _Pointer   pointer;
	typedef _Reference reference;
	typedef _Category  iterator_category;
};

template <class _Tp, class _Distance> struct input_iterator {
	typedef input_iterator_tag iterator_category;
	typedef _Tp                value_type;
	typedef _Distance          difference_type;
	typedef _Tp*               pointer;
	typedef _Tp&               reference;
};

struct output_iterator {
	typedef output_iterator_tag iterator_category;
	typedef void                value_type;
	typedef void                difference_type;
	typedef void                pointer;
	typedef void                reference;
};

template <class _Tp, class _Distance> struct forward_iterator {
	typedef forward_iterator_tag iterator_category;
	typedef _Tp                  value_type;
	typedef _Distance            difference_type;
	typedef _Tp*                 pointer;
	typedef _Tp&                 reference;
};


template <class _Tp, class _Distance> struct bidirectional_iterator {
	typedef bidirectional_iterator_tag iterator_category;
	typedef _Tp                        value_type;
	typedef _Distance                  difference_type;
	typedef _Tp*                       pointer;
	typedef _Tp&                       reference;
};

template <class _Tp, class _Distance> struct random_access_iterator {
	typedef random_access_iterator_tag iterator_category;
	typedef _Tp                        value_type;
	typedef _Distance                  difference_type;
	typedef _Tp*                       pointer;
	typedef _Tp&                       reference;
};


template<class Iterator>
struct iterator_traits
{
	typedef typename Iterator::iterator_category iterator_category;
	typedef typename Iterator::value_type value_type;
	typedef typename Iterator::pointer pointer;
	typedef typename Iterator::reference reference;
	typedef typename Iterator::difference_type difference_type;

	typedef __false_type is_pointer_type;
	typedef __false_type is_const_pointer_type;
};


template<class T>
struct iterator_traits<T*>
{
	typedef random_access_iterator_tag iterator_category;
	typedef T value_type;
	typedef T* pointer;
	typedef T& reference;
	typedef ptrdiff_t difference_type;

	typedef __true_type is_pointer_type;
	typedef __false_type is_const_pointer_type;
};

template<class T>
struct iterator_traits<const T*>
{
	typedef random_access_iterator_tag iterator_category;
	typedef T value_type;
	typedef T* pointer;
	typedef T& reference;
	typedef ptrdiff_t difference_type;

	typedef __false_type is_pointer_type;
	typedef __true_type is_const_pointer_type;
};

template<class Iter>
inline typename iterator_traits<Iter>::iterator_category
iterator_cateory()
{
	typedef typename iterator_traits<Iter>::iterator_category catergory;
	return catergory();
}

template<class Iter>
inline typename iterator_traits<Iter>::value_type*
value_type_pointer()
{
	return static_cast<typename iterator_traits<Iter>::value_type*>(0);
}

template<class Iter>
inline typename iterator_traits<Iter>::difference_type*
distance_type_pointer()
{
	return static_cast<typename iterator_traits<Iter>::difference_type*>(0);
}


template<class Iter>
class reverse_iterator :
	  public __iterator<typename iterator_traits<Iter>::iterator_category,
					  typename iterator_traits<Iter>::value_type,
					  typename iterator_traits<Iter>::difference_type,
					  typename iterator_traits<Iter>::pointer, 
					  typename iterator_traits<Iter>::reference >
{
public:
	typedef Iter iterator_type;

	reverse_iterator() :_base(){ }
	explicit reverse_iterator(iterator_type it) :_base(it){ }
	reverse_iterator(const reverse_iterator& rhs) :_base(rhs._base){ }
	template<class __Iter>
	reverse_iterator(const reverse_iterator<__Iter>& revi) : _base(revi.base())
	{ }
	iterator_type base() const { return _base; }
	value_type& operator*() const 
	{ 
		iterator_type temp(base());
		return *--temp;
	}
	reverse_iterator& operator++()
	{
		_base--;
		return *this;
	}
	reverse_iterator operator++(int)
	{
		reverse_iterator temp(*this);
		++(*this);
		return temp;
	}
	reverse_iterator& operator--()
	{
		_base++;
		return *this;
	}
	reverse_iterator operator--(int)
	{
		reverse_iterator temp(*this);
		--(*this);
		return temp;
	}
	reverse_iterator operator+(difference_type n) const 
	{ return reverse_iterator(_base-n);}
	reverse_iterator& operator+=(difference_type n)
	{ _base -= n; return *this;}
	reverse_iterator operator-(difference_type n) const
	{ return reverse_iterator(_base + n); }
	reverse_iterator& operator-=(difference_type n)
	{ _base += n; return *this;}
	reference operator[](difference_type n)
	{ return *(_base - n - 1); }
protected:
	Iter _base;
};


template <typename Iter1, typename Iter2>
inline bool operator==(const reverse_iterator<Iter1>& a, const reverse_iterator<Iter2>& b)
{
	return a.base() == b.base();
}

template <typename Iter1, typename Iter2>
inline bool operator<(const reverse_iterator<Iter1>& a, const reverse_iterator<Iter2>& b)
{
	return a.base() > b.base();
}

template <typename Iter1, typename Iter2>
inline bool operator!=(const reverse_iterator<Iter1>& a, const reverse_iterator<Iter2>& b)
{
	return a.base() != b.base();
}


template <typename Iter1, typename Iter2>
inline bool operator>(const reverse_iterator<Iter1>& a, const reverse_iterator<Iter2>& b)
{
	return a.base() < b.base();
}

template <typename Iter1, typename Iter2>
inline bool operator<=(const reverse_iterator<Iter1>& a, const reverse_iterator<Iter2>& b)
{
	return a.base() >= b.base();
}

template <typename Iter1, typename Iter2>
inline bool operator>=(const reverse_iterator<Iter1>& a, const reverse_iterator<Iter2>& b)
{
	return a.base() <= b.base();
}

template <typename Iter1, typename Iter2>
inline typename reverse_iterator<Iter1>::difference_type
operator-(const reverse_iterator<Iter1>& a, const reverse_iterator<Iter2>& b)
{
	return b.base() - a.base();
}

template <typename Iterator>
inline reverse_iterator<Iterator>
operator+(typename reverse_iterator<Iterator>::difference_type n, const reverse_iterator<Iterator>& a)
{
	return reverse_iterator<Iterator>(a.base() - n);
}

DIYSTL_NAMESPACE_END

#endif
