/* Copyright 2016 Erik Perillo */

#ifndef __LL_HEADER__
#define __LL_HEADER__

#include <algorithm>

namespace ll
{

#define FAIL -1
#define SUCC 1

//class representing linked list node.
template <class type>
class Node
{
	//List can manipulate Node members
	template<typename> friend class List;

	public:
	//ctors
	Node();
	Node(const type& val);
	Node(const Node<type>& node);
	//dtor
	~Node();

	//getters
	type get_val();
	Node<type>* get_next();
	//setters
	void set_val(const type& val);
	
	private:
	//value of node.
	type val;
	//next element.
	Node<type>* next;
};

//class for linked list.
template <class type>
class List 
{
	public:
	//iterator base class overloading for list.
	//iterates over values of list, abstracting away nodes.
	class iterator: public virtual std::iterator<
		std::input_iterator_tag, 
		Node<type>, 
		std::ptrdiff_t, 
		const Node<type>*, 
		Node<type>&>
	{
		Node<type>* itr;
		public:
		explicit iterator(Node<type>* itr): itr(itr){}
		iterator(): itr(nullptr){}
		iterator& operator++(){itr = itr->next; return *this;}
		//Node<type>* operator+(int i){Node<type>* itr2 = this->itr;
		//	for(int j=0; j<i; j++) itr2 = itr2->next; return itr2;}
		iterator operator++(int){iterator ret(*this); ++(*this); return ret;}
		bool operator==(const iterator& iter) const {return itr == iter.itr;}
		bool operator!=(const iterator& iter) const {return itr != iter.itr;}
		Node<type>*& next() const {return itr->next;}
		type& operator*() const {return itr->val;}
		type& operator->() const {return itr->val;}
	};
	//iterator methods
	iterator begin() {return iterator(this->head.next);}
	iterator end() {return iterator(nullptr);}

	//ctors
	List();
	List(const List<type>& list);
	//dtor
	~List();

	//getters
	//gets head of list.
	Node<type> get_head();
	//gets size of list.
	int get_size();
	
	//returns true if empty
	bool is_empty();
	//puts value at the beginning of list.
	void put(const type& val);
	//puts value at end of list.
	void append(const type& val);
	//deletes first occurrence of node with value val.
	//returns a positive number on success.
	int del(const type& val);
	//prints linked list.
	void print();

	private:
	int size;
	Node<type> head;
	
	//puts node at the beginning of list.
	void _put(Node<type>* node);
	//puts node at the end of list.
	void _append(Node<type>* node);
};

}

#endif
