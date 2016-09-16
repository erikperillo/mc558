#ifndef __GRAPH_HEADER__
#define __GRAPH_HEADER__

template <class type>
class Vertex
{
	public:
	Vertex();
	~Vertex();

	private:
	type val;
	Vertex<type> *adj_list;
};

#endif
