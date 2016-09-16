#include "graph.h"
#include <iostream>

using namespace std;

template<class type>
Vertex<type>::Vertex()
{
	cout << "Vertex ctor" << endl;	
	this->adj_list = new Vertex<type>[1];
}

template<class type>
Vertex<type>::~Vertex()
{
	cout << "Vertex dtor" << endl;	
}

void test()
{
	Vertex<int> vi;

	cout << "main" << endl;
}

int main()
{
	test();

	return 0;
}
