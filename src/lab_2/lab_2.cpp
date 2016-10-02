/* Copyright 2016 Erik Perillo */

#include <list>
#include <vector>
#include <iostream>
#include <string>

//edge colors enumeration
enum
{
	GREEN=0,
	YELLOW,
	RED
};

//foward-declaration
template<class type>
class Graph;

/*
Class representing graph vertices.
Each vertex has an adjacency list of ids. 
Those ids are the indexes of the vertices to which they connect in graph.
*/
template <class type>
class Vertex
{
	//the graph may manipulate private members
	friend class Graph<type>;

	public:
	//constructors
	Vertex()
	{;}
	Vertex(const type& val): val(val)
	{;}
	//destructor
	~Vertex()
	{;}

	//two vertices are equal iff their 'val' attribute is equal
	bool operator==(const Vertex<type>& vertex) const
	{
		return this->val == vertex.get_val();
	}
	bool operator!=(const Vertex<type>& vertex) const
	{
		return !this->operator==(vertex);
	}

	//Returns index of vertex vtx_id in ajdacency list. 
	//If vtx_id is not present, returns a negative number.
	int index(int vtx_id) const
	{
		for(unsigned i=0; i<this->adj_list.size(); i++)
			if(this->adj_list[i] == vtx_id)
				return (int)i;

		return -1;
	}

	//True if vertex index vtx_id is positive.
	bool connects(int vtx_id) const
	{
		return this->index(vtx_id) >= 0;
	}

	//Appends vertex index to adjacency list.
	void add(int vtx_id)
	{
		this->adj_list.push_back(vtx_id);
	}

	//Prints vertex value.
	void print(bool newline=true) const
	{
		std::cout << "V(" << this->val << ")";
		if(newline)	
			std::cout << std::endl;		
	}

	//Getter for vertex value.
	type get_val() const
	{
		return this->val;
	}

	//Returns number of vertices to which vertex connects.
	int n_connections() const
	{
		return this->adj_list.size();
	}

	//Returns i'th vertex id on adjacency list.
	int operator[](int i) const
	{
		return this->adj_list[i];
	}
	int operator[](int i) 
	{
		return this->adj_list[i];
	}

	private:
	//value vertex holds
	type val;
	//adjacency list
	std::vector<int> adj_list;
};

/*
Class representing an edge, ie, a pair of vertices.
Connection goes from u to v, that is, first to second vertices passed.
*/
template <class type>
class Edge
{
	public:
	//constructors
	Edge()
	{;}
	Edge(const Vertex<type>& u, const Vertex<type>& v, int color): 
		u(u), v(v), color(color)
	{;}
	//destructor
	~Edge()
	{;}

	//two edges are equal iff their respective vertices are equal
	bool operator==(const Edge<type>& edge) const
	{
		return this->u == edge.get_u() && this->v == edge.get_v();
	}
	bool operator!=(const Edge<type>& edge) const
	{
		return !this->operator==(edge);
	}

	//getters for vertices
	Vertex<type> get_u() const
	{
		return this->u;
	}
	Vertex<type> get_v() const
	{
		return this->v;
	}
	int getColor() const
	{
		return this->color;
	}
	
	private:
	//first connection vertex
	Vertex<type> u;
	//second connection vertex
	Vertex<type> v;
	//color of vertex
	int color;
	
};

//forward declaration
template <class type>
void fill_graph(Graph<type>& graph);

/*
Class representing a graph using adjacency lists.
Each vertex has an id which is the position it is in graph's list.
Each vertex adjacency list id refer to the graph ids.
*/
template <class type>
class Graph
{
	public:
	//constructors
	Graph()
	{;}
	Graph(int num_vertices, int num_edges): num_vertices(num_vertices),
		num_edges(num_edges)
	{;}
	//destructor
	~Graph()
	{;}

	//helper function to build graph
	friend void fill_graph<type>(Graph<type>& graph);

	//Returns index of vertex in graph. 
	//If it doesn't exist, returns a negative number.
	int index(const Vertex<type>& vertex) const
	{
		for(unsigned i=0; i<this->vertices.size(); i++)
			if(this->vertices[i] == vertex)
				return (int)i;

		return -1;
	}

	//True if graph has vertex equal to specified.
	bool has_vertex(const Vertex<type>& vertex) const
	{
		return this->index(vertex) >= 0;
	}

	//Returns number of vertices in graph.
	int n_vertices() const
	{
		return this->vertices.size();
	}
	//Returns number of edges in graph.
	int n_edges() const
	{
		return this->num_edges;
	}

	//Returns ith vertex in graph.
	Vertex<type> operator[](int i) const
	{
		return this->vertices[i];
	}
	Vertex<type>& operator[](int i) 
	{
		return this->vertices[i];
	}

	//Gets the color of edge (u, v).
	int edge_color(int u_idx, int v_idx) const
	{
		if(u_idx > this->n_vertices() || v_idx > this->n_vertices()) 
			return -1;

		return this->edges_colors[u_idx][v_idx];
	}
	
	//Prints all vertices with their adjacencies.
	void print() const
	{
		for(unsigned i=0; i<this->vertices.size(); i++)
		{
			std::cout << "[" << i << "] ";
			this->vertices[i].print(false);

			if(this->vertices[i].adj_list.size() > 0)
			{
				unsigned j;
				std::cout << " -> ";
				for(j=0; j<this->vertices[i].adj_list.size()-1; j++)
				{
					this->vertices[this->vertices[i].adj_list[j]].print(false);	
					std::cout << " -> ";
				}
				this->vertices[this->vertices[i].adj_list[j]].print(false);	
			}
			
			std::cout << std::endl;	
		}
	}

	private:
	//list of vertices
	int num_vertices;
	int num_edges;
	std::vector<Vertex<type> > vertices;
	std::vector<std::vector<int > > edges_colors;

	protected:
	//functions used to build graph
	int add_vertex(Vertex<type>& vertex)
	{
		int id = this->index(vertex);

		if(id >= 0)
			return id;

		this->vertices.push_back(vertex);
		this->edges_colors.push_back(std::vector<int>(this->num_vertices, -1));

		return this->vertices.size() - 1;
	}
	void add_edge(const Edge<type>& edge)
	{
		int u_id, v_id;
		Vertex<type> u = edge.get_u();
		Vertex<type> v = edge.get_v();
	
		u_id = this->add_vertex(u);
		v_id = this->add_vertex(v);

		this->vertices[u_id].add(v_id);
		this->edges_colors[u_id][v_id] = edge.getColor();
	}
};

//Reads from stdin num_edges times, each time getting one edge and 
//adding it to graph.
template <class type>
void fill_graph(Graph<type>& graph)
{
	type u_tp, v_tp;
	int color;

	for(int i=0; i<graph.n_edges(); i++)
	{
		std::cin >> u_tp;
		std::cin >> v_tp;	
		std::cin >> color;	
		Vertex<type> u(u_tp);
		Vertex<type> v(v_tp);

		graph.add_edge(Edge<type>(u, v, color));
	}
}

//Struct to store results from a call to dfs.
struct DFSResults
{
	DFSResults(const std::vector<int>& start_times,  
		const std::vector<int>& end_times,	const std::vector<int>& parents,
		const std::list<int>& top_sort):
		start_times(start_times), end_times(end_times), 
		parents(parents)
	{
		std::copy(top_sort.begin(), top_sort.end(), 
			std::back_inserter(this->top_sort));
	}
	DFSResults()
	{;}
	~DFSResults()
	{;}

	//start times of vertices resulting from dfs.
	//the ith element has the time for the vertex with index i in graph.
	std::vector<int> start_times;
	//end times of vertices resulting from dfs
	//the ith element has the time for the vertex with index i in graph.
	std::vector<int> end_times;	
	//parents list.
	//the ith element has the id of the parent of the vertex with index i.
	std::vector<int> parents;	
	//topological sort. the ith element has the id of the vertex in graph 
	//in ith position in topological sort.
	std::vector<int> top_sort;
};

//dfs helper function.
template <class type>
void _dfs(const Graph<type>& graph, int root_idx, int& time,
	std::vector<int>& start_times, std::vector<int>& end_times, 
	std::vector<int>& parents, std::vector<bool>& visiteds,
	std::list<int>& top_sort)
{
	visiteds[root_idx] = true;
	start_times[root_idx] = time++;

	for(int i=0; i<graph[root_idx].n_connections(); i++)
		if(!visiteds[graph[root_idx][i]])
		{
			parents[graph[root_idx][i]] = root_idx;
			_dfs(graph, graph[root_idx][i], time, start_times, end_times,
				parents, visiteds, top_sort);
		}

	end_times[root_idx] = time++;
	top_sort.push_front(root_idx);
}

//Computes dfs on graph.
//gets start/end times, parents(pi) list and a topological sort for graph.
template <class type>
DFSResults dfs(const Graph<type>& graph)
{
	std::vector<int> start_times(graph.n_vertices(), -1);
	std::vector<int> end_times(graph.n_vertices(), -1);	
	std::vector<int> parents(graph.n_vertices(), -1);	
	std::vector<bool> visiteds(graph.n_vertices(), false);
	std::list<int> top_sort;
	int time=0;

	for(int i=0; i<graph.n_vertices(); i++)
		if(!visiteds[i])
			_dfs(graph, i, time, start_times, end_times, parents, visiteds,
				top_sort);

	return DFSResults(start_times, end_times, parents, top_sort);
}

//get_paths auxiliar function.
template <class type>
void _get_paths(const Graph<type>& graph, const DFSResults& dfs_res, int idx,
	std::vector<int>& g_paths, std::vector<int>& y_paths, 
	std::vector<int>& r_paths, int end_idx)
{
	int u_idx;
	int v_idx;

	u_idx = dfs_res.top_sort[idx];

	for(int i=0; i<graph[u_idx].n_connections(); i++)
	{
		v_idx = graph[u_idx][i];

		switch(graph.edge_color(u_idx, v_idx))
		{
			case GREEN:
				g_paths[u_idx] += (v_idx == end_idx)?1:\
					(g_paths[v_idx] + y_paths[v_idx] + r_paths[v_idx]);
				break;
			case YELLOW:
				y_paths[u_idx] += (v_idx == end_idx)?1:\
					(y_paths[v_idx] + g_paths[v_idx]);
				break;
			case RED:
				r_paths[u_idx] += (v_idx == end_idx)?1:g_paths[v_idx];
				break;
		}	
	}
}

//Computes number of possible paths following the colors rule 
//from start vertex to end vertex for each vertex in graph. 
template <class type>
std::vector<int> get_paths(const Graph<type> graph, const DFSResults& dfs_res,
	int start_idx, int end_idx)
{
	//the ith element has the number of paths from the vertex of index
	//i in graph to the end vertex
	std::vector<int> g_paths(dfs_res.top_sort.size(), 0);
	std::vector<int> y_paths(dfs_res.top_sort.size(), 0);
	std::vector<int> r_paths(dfs_res.top_sort.size(), 0);
	std::vector<int> paths(dfs_res.top_sort.size(), 0);

	for(int i=(int)dfs_res.top_sort.size()-1; i>=0; i--)
		_get_paths(graph, dfs_res, i, g_paths, y_paths, r_paths, end_idx);

	for(unsigned i=0; i<paths.size(); i++)
		paths[i] = g_paths[i] + y_paths[i] + r_paths[i];

	return paths;
}

//Gets number of possible paths from start vertex to end vertex
//following the colors rule.
template <class type>
int num_paths(const Graph<type> graph, const Vertex<type>& start_vtx,
	const Vertex<type>& end_vtx)
{
	int start_vtx_idx, end_vtx_idx;
	std::vector<int> paths;
	DFSResults dfs_res;

	if(start_vtx == end_vtx)
		return 1;

	dfs_res = dfs(graph);

	start_vtx_idx = graph.index(start_vtx);
	end_vtx_idx = graph.index(end_vtx);
	if(start_vtx_idx < 0)
		return 0;

	paths = get_paths(graph, dfs_res, start_vtx_idx, end_vtx_idx);

	return paths[start_vtx_idx];
}

using namespace std;

int main()
{
	int num_vtxs, num_edges, start_vtx, end_vtx;
	int ret;
	DFSResults dfs_res;

	//getting initial graph parameters
	cin >> num_vtxs;
	cin >> num_edges;
	cin >> start_vtx;
	cin >> end_vtx;

	//building graph
	Graph<int> graph(num_vtxs, num_edges);
	fill_graph<int>(graph);

	//getting number of paths
	ret = num_paths(graph, Vertex<int>(start_vtx), Vertex<int>(end_vtx));

	cout << ret << endl;

	return 0;
}
