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
	Node<type>* aux;
	Node<type>* np;

	np = this->head.next;
	while(np != nullptr)	
	{
		aux = np;
		np = np->next;	
		delete aux;
	}
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
void List<type>::_put(Node<type>* node)
{
	Node<type>* last_next;
	
	last_next = this->head.next;
	this->head.next = node;
	node->next = last_next;	
	this->size++;
}

template<class type>
void List<type>::_append(Node<type>* node)
{
	Node<type>* np;

	for(np = &this->head; np->next != nullptr; np = np->next);
	np->next = node;
	this->size++;
}

template<class type>
void List<type>::put(const type& val)
{
	Node<type>* node = new Node<type>(val);
	this->_put(node);
}

template<class type>
void List<type>::append(const type& val)
{
	Node<type>* node = new Node<type>(val);
	this->_append(node);
}

template<class type>
bool List<type>::is_empty()
{
	return this->get_size() == 0;
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
			delete np;
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
	List<float> list;

	return 0;
}
