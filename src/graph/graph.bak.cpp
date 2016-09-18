#include <vector>
#include <iostream>
#include <map>
#include <string>

enum
{
	WHITE,
	GRAY,
	BLACK
};

template <class type>
class Vertex
{
	public:
	//ctors
	Vertex(): id(-1)
	{;}
	Vertex(const type& val): val(val), id(-1)
	{;}
	//dtor
	~Vertex()
	{;}

	bool operator==(const Vertex<type>& vertex) const
	{
		return this->val == vertex.get_val();
	}

	int index(const Vertex<type>& vertex) const
	{
		for(unsigned i=0; i<this->adj_list.size(); i++)
			if(vertex == this->adj_list[i])
				return (int)i;

		return -1;
	}
	
	bool connects(const Vertex<type>& vertex) const
	{
		return this->index(vertex) >= 0;
	}

	void add(const Vertex<type>& vertex) 
	{
		this->adj_list.push_back(vertex);
	}

	void del(const Vertex<type>& vertex)
	{
		int id = this->index(vertex);
		if(id >= 0)
			this->adj_list.erase(this->adj_list.begin() + id);
	}

	void print(bool newline=true) const
	{
		std::cout << "V(" << this->val << ")";

		if(newline)	
			std::cout << std::endl;		
	}

	void print_adj_list(bool newline=true) const
	{
		unsigned i;

		if(this->adj_list.size() > 0)
		{
			for(i=0; i<this->adj_list.size() - 1; i++)
			{
				this->adj_list[i].print(false);
				std::cout << " -> ";
			}

			this->adj_list[i].print(false);
		}
	
		if(newline)	
			std::cout << std::endl;		
	}

	void print_with_adj_list(bool newline=true) const
	{
		this->print(false);
		if(this->adj_list.size() > 0)
			std::cout << " -> ";
		this->print_adj_list(false);

		if(newline)
			std::cout << std::endl;
	}

	type get_val() const
	{
		return this->val;
	}

	int n_connections() const
	{
		return this->adj_list.size();
	}

	Vertex<type> operator[](int i) const
	{
		return this->adj_list[i];
	}

	Vertex<type>& operator[](int i) 
	{
		return this->adj_list[i];
	}

	void set_id(int id)
	{
		this->id = id;
	}

	int get_id()
	{
		return this->id;
	}

	private:
	type val;
	int id;
	std::vector<Vertex<type> > adj_list;
};

template <class type>
class Edge
{
	public:
	Edge()
	{;}
	Edge(const Vertex<type>& u, const Vertex<type>& v): u(u), v(v)
	{;}
	~Edge()
	{;}

	bool operator==(const Edge<type>& edge) const
	{
		return this->u == edge.get_u() && this->v == edge.get_v();
	}

	Vertex<type> get_u() const
	{
		return this->u;
	}
	Vertex<type> get_v() const
	{
		return this->v;
	}
	
	private:
	Vertex<type> u;
	Vertex<type> v;
};

template <class type>
class Graph
{
	public:
	Graph()
	{;}
	Graph(const std::vector<Edge<type> > edges)
	{
		for(int i=0; i<edges.size(); i++)
			this->add_edge(edges[i]);
	}

	~Graph()
	{;}

	int index(const Vertex<type>& vertex) const
	{
		for(unsigned i=0; i<this->vertices.size(); i++)
			if(vertex == this->vertices[i])
				return (int)i;

		return -1;
	}

	void add_vertex(Vertex<type>& vertex)
	{
		vertex.set_id(this->n_vertices());
		this->vertices.push_back(vertex);
	}

	void add_edge(const Edge<type> edge)
	{
		int id_u, id_v;
		Vertex<type> u = edge.get_u();
		Vertex<type> v = edge.get_v();
	
		id_v = this->index(v);
		if(id_v < 0)
			this->add_vertex(v);
		id_v = this->n_vertices() - 1;

		id_u = this->index(u);
		if(id < 0)
		{
			this->add_vertex(u);
			id_u = this->n_vertices() - 1;
		}
		this->vertices[id_u].add(this->vertices[id_v]);
	}

	bool has_vertex(const Vertex<type>& vertex) const
	{
		return this->index(vertex) >= 0;
	}

	bool has_edge(const Edge<type>& edge) const
	{
		Vertex<type> u = edge.get_u();
		Vertex<type> v = edge.get_v();
		int id;
			
		id = this->index(u);	
		return (id >= 0) && this->vertices[id].connects(v);
	}

	int n_vertices() const
	{
		return this->vertices.size();
	}
	
	int n_egdes() const
	{
		int total = 0;
	
		for(int i=0; i<this->vertices.size(); i++)
			total += this->vertices[i].n_connections();

		return total;
	}

	void del_vertex(const Vertex<type>& vertex)
	{
		int id = this->index(vertex);

		if(id < 0)
			return;

		this->vertices.erase(this->vertices.begin() + id);
	}

	void del_edge(const Edge<type>& edge)
	{
		Vertex<type> u = edge.get_u();
		Vertex<type> v = edge.get_v();
		int id = this->index(u);

		if(id < 0)
			return;

		this->vertices[id].del(v);
	}

	Vertex<type> operator[](int i) const
	{
		return this->vertices[i];
	}

	Vertex<type>& operator[](int i) 
	{
		return this->vertices[i];
	}
	
	void print(bool newline=true) const
	{
		for(unsigned i=0; i<this->vertices.size(); i++)
		{
			std::cout << "[" << i << "] ";
			this->vertices[i].print_with_adj_list(true);
		}
		
		if(newline)
			std::cout << std::endl;
	}

	private:
	std::vector<Vertex<type> > vertices;
};

using namespace std;

void fill_graph(Graph<string>& graph, int size)
{
	string u_str, v_str;

	for(int i=0; i<size; i++)
	{
		cin >> u_str;
		cin >> v_str;	
		Vertex<string> u(u_str);
		Vertex<string> v(v_str);

		graph.add_edge(Edge<string>(u, v));
		graph.add_edge(Edge<string>(v, u));
	}
}

template <class type>
void _dfs_visit(Graph<type>& graph, int idx,
	vector<int>& pi, vector<int>& colors)
{
	Vertex<type>& u = graph[idx];
	int conn_idx;

	colors[idx] = GRAY;

	cout << "[idx=" << idx << "] on vertex "; u.print();
	cout << "on adj list..." << endl;

	for(int i=0; i<u.n_connections(); i++)
	{
		conn_idx = u[i].get_id();
		cout << "adj list vtx n. " << i << " = "; 
		u[i].print(false);
		cout << " | conn_idx = " << conn_idx << endl;

		if(colors[conn_idx] == WHITE)
		{
			pi[conn_idx] = idx;
			_dfs_visit(graph, conn_idx, pi, colors);
		}
	}

	cout << "[idx=" << idx << "] END of vertex "; u.print();
	colors[idx] = BLACK;
}

template <class type>
std::vector<int> dfs_visit(Graph<type>& graph, int index)
{
	std::vector<int> pi(graph.n_vertices(), -1);
	std::vector<int> colors(graph.n_vertices(), WHITE);

	_dfs_visit<type>(graph, index, pi, colors);

	return pi;
}

bool contains(const Graph<string>& a, const Graph<string>& b)
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

bool task_2(const Graph<string>& a, const Graph<string>& b)
{
	//the ith position of this vector is true if and only if
	//a's ith vertex has been checked to be in b
	vector<bool> checked(a.n_vertices(), false);

	return true;
}

int main()
{
	//the blurred blueprint
	Graph<string> a;
	//the old blueprint
	Graph<string> b;
	int a_num, b_num;

	cin >> b_num;
	fill_graph(b, b_num);
	cin >> a_num;
	fill_graph(a, a_num);

	cout << "b:" << endl;
	b.print();
	cout << "a:" << endl;
	a.print();

	if(!contains(a, b))
	{
		cout << "NAO" << endl;
		return 0;
	}
	vector<int> pi = dfs_visit(a, 5);

	for(unsigned i=0; i<pi.size(); i++)
		cout << "pi[" << i << "] = " << pi[i] << endl;

	return 0;
}
