#include "ll.h"
#include <iostream>

using namespace std;


namespace ll
{

template <class type>
Node<type>::Node(): next(nullptr)
{
	cout << "node ctor" << endl;	
}

template <class type>
Node<type>::Node(const type& val): val(val),next(nullptr)
{
	cout << "node ctor2" << endl;	
}

template <class type>
Node<type>::Node(const Node<type>& node)
{
	cout << "node ctor3" << endl;	
	*this = node;
}

template <class type>
Node<type>::~Node()
{
	cout << "node dtor" << endl;	
}

template <class type>
type Node<type>::get_val()
{
	return this->val;
}

template <class type>
void Node<type>::set_val(const type& val)
{
	this->val = val;
}

template <class type>
Node<type>* Node<type>::get_next()
{
	return this->next;
}

template<class type>
List<type>::List(): size(0)
{
	cout << "list ctor" << endl;
}

template<class type>
List<type>::List(const List<type>& list)
{
	cout << "list ctor2" << endl;
	*this = list;
}

template<class type>
List<type>::~List()
{
	cout << "list dtor" << endl;
}

template<class type>
int List<type>::get_size()
{
	return this->size;
}

template<class type>
Node<type> List<type>::get_head()
{
	return this->head;
}

template<class type>
void List<type>::put(Node<type>& node)
{
	Node<type>* last_next;
	
	last_next = this->head.next;
	this->head.next = &node;
	node.next = last_next;	
	this->size++;
}

template<class type>
void List<type>::append(Node<type>& node)
{
	Node<type>* np;

	for(np = &this->head; np->next != nullptr; np = np->next);
	np->next = &node;
	this->size++;
}

template<class type>
int List<type>::del(const Node<type>& node)
{
	Node<type>* last;
	
	last = &this->head;
	for(Node<type>* np = this->head.next; np != nullptr; np = np->next)
	{
		if(np == &node)	
		{
			last->next = np->next;
			this->size--;
			return SUCC;
		}
		last = np;
	}
	
	return FAIL;
}

template<class type>
int List<type>::del(const type& val)
{
	Node<type>* last;
	
	last = &this->head;
	for(Node<type>* np = this->head.next; np != nullptr; np = np->next)
	{
		if(np->val == val)	
		{
			last->next = np->next;
			this->size--;
			return SUCC;
		}
		last = np;
	}
	
	return FAIL;
}

template<class type>
void List<type>::print()
{
	cout << "[";
	for(auto it = this->begin(); it != this->end(); ++it)
	{
		cout << *it;
		if(it.next() != nullptr)
			cout << ", ";	
	}
	cout << "]" << endl;
}

}

using namespace ll;

int main()
{
	Node<int> node2(3);
	Node<int> node4 = Node<int>(5);
	Node<int> node6(33);
	Node<int> node7(13);
	List<int> list;

	list.append(node4);
	list.put(node6);
	list.put(node2);
	node2.set_val(-13);
	list.print();
	cout << list.del(node6) << endl;
	cout << list.del(node7) << endl;
	for(auto it = list.begin(); it != list.end(); ++it)
		cout << "it: " << (*it) << endl;
	cout << list.del(-13) << endl;
	for(auto it = list.begin(); it != list.end(); ++it)
		cout << "it: " << (*it) << endl;
	cout << list.del(3333) << endl;
	list.del(5);
	list.del(5);
	list.del(node4);
	list.print();

	for(auto it = list.begin(); it != list.end(); ++it)
		cout << (*it) << endl;

	return 0;
}
