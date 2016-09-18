#include <iostream>

namespace ll
{

template <class type>
Node<type>::Node(): next(NULL)
{;}

template <class type>
Node<type>::Node(const type& val): val(val),next(NULL)
{;}

template <class type>
Node<type>::Node(const Node<type>& node)
{
	*this = node;
}

template <class type>
Node<type>::~Node()
{;}

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
{;}

template<class type>
List<type>::List(const List<type>& list)
{
	*this = list;
}

template<class type>
List<type>::~List()
{
	Node<type>* aux;
	Node<type>* np;

	np = this->head.next;
	while(np != NULL)	
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

	for(np = &this->head; np->next != NULL; np = np->next);
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
	for(Node<type>* np = this->head.next; np != NULL; np = np->next)
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
	std::cout << "[";
	for(List<type>::iterator it = this->begin(); it != this->end(); ++it)
	{
		std::cout << (*it);
		if(it.next() != NULL)
			std::cout << ", ";	
	}
	std::cout << "]" << std::endl;
}

}
