lass template
<initializer_list>
std::initializer_list
template<class T> class initializer_list;
Initializer list
This type is used to access the values in a C++ initialization list, which is a list of elements of type const T.

Objects of this type are automatically constructed by the compiler from initialization list declarations, which is a list of comma-separated elements enclosed in braces:
 
auto il = { 10, 20, 30 };  // the type of il is an initializer_list 


Notice though that this template class is not implicitly defined and the header <initializer_list> shall be included to access it, even if the type is used implicitly.

initializer_list objects are automatically constructed as if an array of elements of type T was allocated, with each of the elements in the list being copy-initialized to its corresponding element in the array, using any necessary non-narrowing implicit conversions.

The initializer_list object refers to the elements of this array without containing them: copying an initializer_list object produces another object referring to the same underlying elements, not to new copies of them (reference semantics).

The lifetime of this temporary array is the same as the initializer_list object.

Constructors taking only one argument of this type are a special kind of constructor, called initializer-list constructor. Initializer-list constructors take precedence over other constructors when the initializer-list constructor syntax is used:
1
2
3
4
5
6
7
8
struct myclass {
  myclass (int,int);
  myclass (initializer_list<int>);
  /* definitions ... */
};

myclass foo {10,20};  // calls initializer_list ctor
myclass bar (10,20);  // calls first constructor 




Template parameters
T
Type of the elements.
Aliased as member type initializer_list::value_type.

Member types
member type	definition
value_type	The template parameter (T)
reference	const T&
const_reference	const T&
size_type	size_t
iterator	const T*
const_iterator	const T*

Member functions
(constructor)
Construct empty initializer_list (public member function )
size
Return size of list (public member function )
begin
Return iterator to beginning (public member function )
end
Return iterator to end (public member function )

Non-member function overloads
begin (initializer_list)
Return iterator to beginning (function template )
end (initializer_list)
Return iterator to end (function template )