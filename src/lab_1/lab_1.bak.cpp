/* Copyright 2016 Erik Perillo */

#include <vector>
#include <iostream>
#include <string>

#define POSITIVE "SIM"
#define NEGATIVE "NAO"

//colors for depth-first-search (dfs)
/*enum
{
	WHITE,
	GRAY,
	BLACK
};*/

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
	Edge(const Vertex<type>& u, const Vertex<type>& v): u(u), v(v)
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
	
	private:
	//first connection vertex
	Vertex<type> u;
	//second connection vertex
	Vertex<type> v;
};

//forward declaration
template <class type>
void fill_graph(Graph<type>& graph, int num_edges);

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
	Graph(const std::vector<Edge<type> > edges)
	{
		for(int i=0; i<edges.size(); i++)
			this->add_edge(edges[i]);
	}
	//destructor
	~Graph()
	{;}

	//helper function to build graph
	friend void fill_graph<type>(Graph<type>& graph, int num_edges);

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

	/*//True if graph has edge specified.
	bool has_edge(const Edge<type>& edge) const
	{
		Vertex<type> u = edge.get_u();
		Vertex<type> v = edge.get_v();
		int u_id = this->index(u);
		int v_id = this->index(v);
		
		if(u_id < 0)
			return false;

		return this->vertices[u_id].connects(v_id);
	}*/

	//Returns number of vertices in graph.
	int n_vertices() const
	{
		return this->vertices.size();
	}
	
	/*//Returns number of edges in graph.	
	int n_egdes() const
	{
		int total = 0;
	
		for(int i=0; i<this->vertices.size(); i++)
			total += this->vertices[i].n_connections();

		return total;
	}*/

	//Returns ith vertex in graph.
	Vertex<type> operator[](int i) const
	{
		return this->vertices[i];
	}
	Vertex<type>& operator[](int i) 
	{
		return this->vertices[i];
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
	std::vector<Vertex<type> > vertices;

	protected:
	//functions used to build graph
	int add_vertex(Vertex<type>& vertex)
	{
		int id = this->index(vertex);

		if(id >= 0)
			return id;

		this->vertices.push_back(vertex);

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
	}
};


//Reads from stdin num_edges times, each time getting two vertices and 
//adding them to graph.
template <class type>
void fill_graph(Graph<type>& graph, int num_edges)
{
	type u_tp, v_tp;

	for(int i=0; i<num_edges; i++)
	{
		std::cin >> u_tp;
		std::cin >> v_tp;	
		Vertex<type> u(u_tp);
		Vertex<type> v(v_tp);

		graph.add_edge(Edge<type>(u, v));
		graph.add_edge(Edge<type>(v, u));
	}
}

//DFS-VISIT iterative function.
//Fills pi with the parents indexes of graph.
template <class type>
void _dfs_visit(const Graph<type>& graph, int idx,
	std::vector<int>& pi, std::vector<bool>& visited)
{
	Vertex<type> u = graph[idx];
	int conn_idx;

	//colors[idx] = GRAY;
	visited[idx] = true;

	//cout << "[idx=" << idx << "] on vertex "; u.print();
	//cout << "on adj list..." << endl;

	for(int i=0; i<u.n_connections(); i++)
	{
		conn_idx = u[i];
		//cout << "adj list vtx n. " << i << " = "; 
		//graph[conn_idx].print(false);
		//cout << " | conn_idx = " << conn_idx << endl;

		//if(colors[conn_idx] == WHITE)
		if(!visited[conn_idx])
		{
			pi[conn_idx] = idx;
			_dfs_visit(graph, conn_idx, pi, visited);
		}
	}

	//cout << "[idx=" << idx << "] END of vertex "; u.print();
	//colors[idx] = BLACK;
}

//Performs DFS-VISIT using as root index-th vertex of graph.
//Returns vector pi, which is the parents vector.
template <class type>
std::vector<int> dfs_visit(const Graph<type>& graph, int index)
{
	std::vector<int> pi(graph.n_vertices(), -1);
	//std::vector<int> colors(graph.n_vertices(), WHITE);
	std::vector<bool> visited(graph.n_vertices(), false);

	_dfs_visit<type>(graph, index, pi, visited);

	return pi;
}

//Returns true if graph b's vertices are all in graph a and false otherwise.
template <class type>
bool contains(const Graph<type>& a, const Graph<type>& b)
{
	int a_num_vtxs = a.n_vertices();
	int b_num_vtxs = b.n_vertices();

	if(b_num_vtxs > a_num_vtxs)
		return false;
	
	for(int i=0; i<b_num_vtxs; i++)
		if(!a.has_vertex(b[i]))
			return false;

	return true;
}

//For every edge (u, v) in b, gets a path (if any) from u to v in a.
//Returns true iff there is a path P and for every w in P - {u,v} w is not in b.
template <class type>
bool _new_vertices_in_old_connections(const Graph<type>& a, const Graph<type>& b)
{
	//the ith position of this vector is true if and only if
	//a's ith vertex has been checked to be in b
	std::vector<bool> checked(a.n_vertices(), false);

	//mapping from b indexes to a indexes. the vector value at the ith position
	//-- if not negative -- represents the index of the ith vertex of b in a.
	std::vector<int> b_a_index_map(b.n_vertices(), -1);

	for(int i=0; i<b.n_vertices(); i++)
	{
		for(int j=0; j<b[i].n_connections(); j++)
		{
			//cout << "on edge ("; 
			//	b[i].print(false); cout << ", "; b[b[i][j]].print(false);
			//	cout << ")" << endl;

			if(b_a_index_map[i] < 0)
				b_a_index_map[i] = a.index(b[i]);
			int a_u_idx = b_a_index_map[i];

			if(b_a_index_map[b[i][j]] < 0)
				b_a_index_map[b[i][j]] = a.index(b[b[i][j]]);
			int a_v_idx = b_a_index_map[b[i][j]];

			std::vector<int> pi = dfs_visit(a, a_u_idx);

			//for(unsigned k=0; k<pi.size(); k++)
			//	cout << "pi[" << k << "] = " << pi[k] << endl;

			int vtx_id = pi[a_v_idx];

			if(vtx_id < 0)
				return false;

			/*int x = vtx_id;
			cout << "path:" << endl;
			a[a_v_idx].print(false);
			while(x >= 0)
			{
				cout << " -> "; a[x].print(false);
				x = pi[x];
			}
			cout << endl;*/
		
			while(a[vtx_id] != b[i])
			{
				//cout << "vtx_id = " << vtx_id << endl;
				//cout << "a[vtx_id] = "; a[vtx_id].print(false); cout << endl;

				if(!checked[vtx_id])
				{
					if(b.has_vertex(a[vtx_id]))
						return false;

					checked[vtx_id] = true;
				}

				vtx_id = pi[vtx_id];
			}
		}
	}

	return true;
}

//For every edge (u, v) in b, gets a path (if any) from u to v in a.
//Returns true iff there is a path P and for every w in P - {u,v} w is not in b.
template <class type>
bool new_vertices_in_old_connections(const Graph<type>& a, const Graph<type>& b)
{
	//the ith position of this vector is true if and only if
	//a's ith vertex has been checked to be in b
	std::vector<int> a_vtxs_in_b(a.n_vertices(), -1);

	//mapping from b indexes to a indexes. the vector value at the ith position
	//-- if not negative -- represents the index of the ith vertex of b in a.
	std::vector<int> b_a_index_map(b.n_vertices(), -1);

	for(int i=0; i<b.n_vertices(); i++)
		for(int j=0; j<b[i].n_connections(); j++)
		{
			if(b_a_index_map[i] < 0)
				b_a_index_map[i] = a.index(b[i]);
			int a_u_idx = b_a_index_map[i];

			if(b_a_index_map[b[i][j]] < 0)
				b_a_index_map[b[i][j]] = a.index(b[b[i][j]]);
			int a_v_idx = b_a_index_map[b[i][j]];

			/*std::cout << "on vertex (";
				a[a_u_idx].print(false);
				std::cout << ", ";
				a[a_v_idx].print(false);
				std::cout << ") " << std:: endl;*/

			/*if(a_u_idx == 2 && a_v_idx == 1)
			{
				std::cout << "checked_vec = " << std::endl;
				for(unsigned k=0; k<checked.size(); k++)
				{
					std::cout << "graph[" << k << "] = "; a[k].print(false);
					std::cout << " checked[" << k << "] = " << checked[k] 
					<< std::endl;
				}
			}*/
			if(!new_vtxs_in_old_conn(a, b, a_u_idx, a_v_idx, a_vtxs_in_b))
			//std::vector<bool> checked2(a.n_vertices(), false);
			//bool x = _task_2(a, b, a_u_idx, a_v_idx, checked2);
			//std::cout << "x = " << x << std::endl;
			//if(!x)
				return false;
		}

	return true;
}

template <class type>
bool new_vtxs_in_old_conn(const Graph<type>& a, const Graph<type>& b, 
	int u_id, int v_id, std::vector<int>& a_vt_in_b)
{
	std::vector<bool> visited(a.n_vertices(), false);

	visited[u_id] = true;

	for(int i=0; i<a[u_id].n_connections(); i++)
		if(!visited[a[u_id][i]] && 
			_new_vtxs_in_old_conn(a, b, a[u_id][i], v_id, visited, a_vt_in_b))
			return true;

	return false;
}

template <class type>
bool _new_vtxs_in_old_conn(const Graph<type>& a, const Graph<type>& b,
	int u_id, int v_id, std::vector<bool>& visited, std::vector<int>& a_vt_in_b)
{
	visited[u_id] = true;

	if(u_id == v_id)
		return true;

	if(a_vt_in_b[u_id] < 0)
		a_vt_in_b[u_id] = (int)b.has_vertex(a[u_id]);

	if(a_vt_in_b[u_id] == (int)true)
		return false;

	for(int i=0; i<a[u_id].n_connections(); i++)
		if(!visited[a[u_id][i]] && 
			_new_vtxs_in_old_conn(a, b, a[u_id][i], v_id, visited, a_vt_in_b))
			return true;

	return false;
}
using namespace std;

int main()
{
	//the blurred blueprint
	Graph<string> a;
	//the old blueprint
	Graph<string> b;
	//number of edges
	int a_num, b_num;

	//reading input and making graphs
	cin >> b_num;
	fill_graph(b, b_num);
	cin >> a_num;
	fill_graph(a, a_num);

	/*cout << "b:" << endl;
	b.print();
	cout << "a:" << endl;
	a.print();*/

	//cout << "-----" << endl;

	//first condition: b vertices must all exist in a
	if(!contains(a, b))
		cout << NEGATIVE << endl;
		//cout << NEGATIVE << " (task 1)" << endl;//"(contains)" << endl;
	//second condition: paths in a from u to v for (u, v) in b must
	//have inner vertices only in a
	else if(!new_vertices_in_old_connections(a, b))
		cout << NEGATIVE << endl;
		//cout << NEGATIVE << " (task 2)" << endl;//"(contains)" << endl;
	else
		cout << POSITIVE << endl;

	return 0;
}
