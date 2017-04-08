#ifndef _DIYSTL_ALGORITHM_
#define _DIYSTL_ALGORITHM_

template<class iterator,class T>
iterator find(iterator first,iterator end,const T& value)
{
	while(first!=end)
	{
		if(*first!=value)
			first++;
		else
			return first;
	}
}


template<class InputIterator,class ForwardIterator> //assert src and dest no overlap
ForwardIterator copy(InputIterator first,InputIterator last,ForwardIterator des )
{
	
}

template<class Iterator>
Iterator copy_backward(Iterator first,Iterator last,Iterator des)
{
	return copy_backward_aux(first,last,dest, value_type_pointer<Iterator>());
}

template<class Iterator,class T>
 Iterator copy_backward_aux(Iterator first,Iterator last,Iterator des,T*)
{
	return type_traits<T>::is_POD_type();
}

template<class Iterator,class T>
 Iterator __copy_backward_aux(Iterator first,Iterator last,Iterator des,__true_type)
{
	
}

#endif