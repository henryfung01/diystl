#include "vector.h"
#include<stdio.h>
using namespace diystl;

template<class T>
void print1DVec(const vector<T>& v,const char* name)
{
	printf("\n");
	printf("%s: \n",name);
	for (size_t i = 0; i<v.size(); i++)
	{
		printf("%d, ", v[i]);
	}
	printf("\n");
}

template<class T>
void print2DVec(const vector<vector<T>>& v, const char* name)
{
	printf("\n");
	printf("%s \n", name);
	for (size_t i = 0; i < v.size(); i++)
	{
		for (size_t j = 0; j < v[0].size(); j++)
		{
			printf("%d, ", v[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

class T1
{
public:
	T1(){ printf("default constructor\n"); }
	explicit T1(int a) :_a(a){ printf("constructor2\n"); }
	T1(const T1& t) :_a(t._a) { printf("copy constructor\n"); }
	T1(T1&& t){ _a = t._a; t._a = 0; printf("move constructor\n"); }
	T1& operator=(const T1& t) { _a = t._a;  printf("move assign\n"); return *this; }
	T1& operator=(T1&& t) { _a = t._a; t._a = 0; printf("move assign\n"); return *this; }
	
private:
	int _a;
};

void test_vector()
{
	 // Create a vector containing integers
   // diystl::vector<int> v = {7, 5, 16, 8};


	allocator<int> alloc;
	int a[] = { 34, 34, 2, 3, 6 };
	vector<int> v0(3);
	printf("v0 capacity before shrink: %u\n", v0.capacity());

	vector<int> v00(alloc);
	diystl::vector<int> v1(4);   //ok now
	diystl::vector<int> v11(4,alloc);
	diystl::vector<int> v2(4,9);
	vector<int> v22(4, 9,alloc);

	diystl::vector<int> v3(&a[0], a + sizeof(a) / sizeof(int));
	printf("v3 capacity before shrink: %u\n", v3.capacity());
	diystl::vector<int> v33(&a[0], a + sizeof(a) / sizeof(int),alloc);
	diystl::vector<int> v4(v3);
	diystl::vector<int> v44(v3,alloc);
	vector<int> v5(vector<int>(5,3)); //should call move constructor
	vector< vector<int> > v6(2, vector<int>(4, 5));



	print1DVec(v0, "v0");
	print1DVec(v1, "v1");
	print1DVec(v2, "v2");
	print1DVec(v3, "v3");
	print1DVec(v4, "v4");
	print1DVec(v5, "v5");
	print2DVec(v6, "v6");

	print1DVec(v00, "v00");
	print1DVec(v11, "v11");
	print1DVec(v22, "v22");
	print1DVec(v33, "v33");
	print1DVec(v44, "v44");
 
    // Add two more integers to vector
   /* v.insert(v.begin(),25);
    v.insert(v.begin(),13);
 */
   
	//if(diystl::find(v.begin(),v.end(),3)!=v.end())
	//	//diystl::cout<<"found"<<diystl::endl;
	//	printf("found\n");
	int k = 7;
	v3.push_back(5);
	printf("v3 capacity before shrink: %u\n", v3.capacity());
	v3.push_back(k);
	printf("v3 capacity before shrink: %u\n", v3.capacity());
	v3.push_back(k);
	printf("v3 capacity before shrink: %u\n", v3.capacity());
	v3.push_back(5);
	printf("v3 capacity before shrink: %u\n", v3.capacity());
	print1DVec(v3, "v3");
	v3.insert(v3.begin() , k);
	print1DVec(v3, "v3");
	v3.insert(v3.begin() + 2, 7);
	print1DVec(v3, "v3");
	v3.insert(v3.end(), k);
	print1DVec(v3, "v3");
	print1DVec(v3, "v3 before erase");
	v3.erase(v3.begin());
	print1DVec(v3, "v3 after erase1");
	v3.erase(v3.begin(),v3.end());
	print1DVec(v3, "v3 after erase2");
	
	v3.resize(3);
	print1DVec(v3, "v3");
	v3.resize(5);
	print1DVec(v3, "v3");
	v3.resize(6,1);
	print1DVec(v3, "v3");

	vector<int> vv3;
	vv3.assign(v3.begin(), v3.end());
	print1DVec(vv3, "vv3");
	vv3.assign(4, 3);
	print1DVec(vv3, "vv3");
	vv3.back() = 4;
	print1DVec(vv3, "vv3");
	printf("vv3[1] %d\n", vv3.data()[1]);
	vv3.data()[1] = 90;
	printf("vv3[1] %d\n", vv3.data()[1]);
	vv3.push_back(77);
	print1DVec(vv3, "vv3");
	printf("vv3 capacity before shrink: %u\n", vv3.capacity());
	vv3.shrink_to_fit();
	printf("vv3 capacity after shrink: %u\n", vv3.capacity());


	vector<T1> vt;
	printf("vt push_back\n");
	vt.push_back(T1());
	printf("vt emplace_back\n");
	//emplace_back与push_back的差异
	vt.emplace_back(3);
	//vt.push_back(3); //error. push_back参数不能为vector内元素的构造函数的参数;emplace_back可以.
	printf("vt emplace\n");
	vt.emplace(vt.end(), 6);
	printf("vt insert\n");
	vt.insert(vt.end(),T1(4));
;
}

int main()
{
	test_vector();
}