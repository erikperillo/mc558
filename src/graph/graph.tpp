#include <iostream>

template<class type>
Vertex<type>::Vertex()
{;}

template<class type>
Vertex<type>::Vertex(const type& val): val(val)
{;}

template<class type>
Vertex<type>::Vertex(const Vertex<type>& vertex)
{
	*this = vertex;
}

template<class type>
Vertex<type>::~Vertex()
{;}

template<class type>
bool Vertex<type>::operator==(const Vertex<type>& vertex) const
{
	return this->val == vertex.val;
}

template<class type>
Edge<type>::Edge()
{;}

template<class type>
Edge<type>::Edge(const Vertex<type>& u, const Vertex<type>& v):
	u(u), v(v)
{;}

template<class type>
Edge<type>::Edge(const Edge<type>& edge)
{
	*this = edge;
}

template<class type>
Edge<type>::~Edge()
{;}

template<class type>
bool Edge<type>::operator==(const Edge<type>& edge) const
{
	return this->u == edge.u && this->v == edge.v;
}

template<class type>
Graph<type>::Graph(const Graph<type>& graph)
{
	*this = graph;
}

template<class type>
Graph<type>::Graph(const std::vector<Edge<type>> edges)
{
	for(auto const& edge: edges)
	{
		Edge<type> u = edge.u;
		Edge<type> v = edge.v;
	}
}

