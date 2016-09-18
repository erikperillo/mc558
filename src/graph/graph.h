#ifndef __GRAPH_HEADER__
#define __GRAPH_HEADER__

#include "ll.h"
#include <vector>
#include <map>

template <class type>
class Vertex
{
	public:
	//ctors
	Vertex();
	Vertex(const Vertex<type>& vertex);
	Vertex(const type& val);
	//dtor
	~Vertex();

	bool operator==(const Vertex<type>& vertex) const;

	private:
	type val;
};

template <class type>
class Edge
{
	public:
	Edge();
	Edge(const Edge<type>& edge);
	Edge(const Vertex<type>& u, const Vertex<type>& v);
	~Edge();

	bool operator==(const Edge<type>& edge) const;

	private:
	Vertex<type> u;
	Vertex<type> v;
};

template <class type>
class Graph
{
	public:
	Graph();
	Graph(const Graph<type>& graph);
	Graph(const std::vector<Edge<type>> edges);
	~Graph();

	private:
	std::vector<
		std::map<
			Vertex<type>, 
			List<Vertex<type>>
			>
		> vertexes;
};

#include "graph.tpp"

#endif
