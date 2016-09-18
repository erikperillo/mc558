#include "ll.h"
#include <iostream>

using namespace std;
using namespace ll;

int main()
{
	List<int> l;

	cout << "oi" << endl;
	l.append(10);
	l.put(-1);
	l.put(4);

	l.print();
}
