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

template<class type>
class Graph;

template <class type>
class Vertex
{
	friend class Graph<type>;

	public:
	//ctors
	Vertex()//: id(-1)
	{;}
	Vertex(const type& val): val(val)//, id(-1)
	{;}
	//dtor
	~Vertex()
	{;}

	bool operator==(const Vertex<type>& vertex) const
	{
		return this->val == vertex.get_val();
	}

	bool operator!=(const Vertex<type>& vertex) const
	{
		return !this->operator==(vertex);
	}

	int index(int vtx_id) const
	{
		for(unsigned i=0; i<this->adj_list.size(); i++)
			if(this->adj_list[i] == vtx_id)
				return (int)i;

		return -1;
	}

	bool connects(int vtx_id) const
	{
		return this->index(vtx_id) >= 0;
	}

	void add(int vtx_id)
	{
		this->adj_list.push_back(vtx_id);
	}

	void print(bool newline=true) const
	{
		std::cout << "V(" << this->val << ")";
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

	int operator[](int i) const
	{
		return this->adj_list[i];
	}

	int operator[](int i) 
	{
		return this->adj_list[i];
	}

	private:
	type val;
	std::vector<int> adj_list;
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

	bool operator!=(const Edge<type>& edge) const
	{
		return !this->operator==(edge);
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
void fill_graph(Graph<type>& graph, int num_edges);

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

	friend void fill_graph<type>(Graph<type>& graph, int num_edges);

	int index(const Vertex<type>& vertex) const
	{
		for(unsigned i=0; i<this->vertices.size(); i++)
			if(this->vertices[i] == vertex)
				return (int)i;

		return -1;
	}

	bool has_vertex(const Vertex<type>& vertex) const
	{
		return this->index(vertex) >= 0;
	}

	bool has_edge(const Edge<type>& edge) const
	{
		Vertex<type> u = edge.get_u();
		Vertex<type> v = edge.get_v();
		int u_id = this->index(u);
		int v_id = this->index(v);
		
		if(u_id < 0)
			return false;

		return this->vertices[u_id].connects(v_id);
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

	Vertex<type> operator[](int i) const
	{
		return this->vertices[i];
	}

	Vertex<type>& operator[](int i) 
	{
		return this->vertices[i];
	}
	
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
	std::vector<Vertex<type> > vertices;

	protected:
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

using namespace std;

template <class type>
void fill_graph(Graph<type>& graph, int num_edges)
{
	type u_tp, v_tp;

	for(int i=0; i<num_edges; i++)
	{
		cin >> u_tp;
		cin >> v_tp;	
		Vertex<type> u(u_tp);
		Vertex<type> v(v_tp);

		graph.add_edge(Edge<type>(u, v));
		graph.add_edge(Edge<type>(v, u));
	}
}

template <class type>
void _dfs_visit(const Graph<type>& graph, int idx,
	vector<int>& pi, vector<int>& colors)
{
	Vertex<type> u = graph[idx];
	int conn_idx;

	colors[idx] = GRAY;

	//cout << "[idx=" << idx << "] on vertex "; u.print();
	//cout << "on adj list..." << endl;

	for(int i=0; i<u.n_connections(); i++)
	{
		conn_idx = u[i];
		//cout << "adj list vtx n. " << i << " = "; 
		//graph[conn_idx].print(false);
		//cout << " | conn_idx = " << conn_idx << endl;

		if(colors[conn_idx] == WHITE)
		{
			pi[conn_idx] = idx;
			_dfs_visit(graph, conn_idx, pi, colors);
		}
	}

	//cout << "[idx=" << idx << "] END of vertex "; u.print();
	colors[idx] = BLACK;
}

template <class type>
std::vector<int> dfs_visit(const Graph<type>& graph, int index)
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

	vector<int> b_a_index_map(b.n_vertices(), -1);

	int a_u_idx, a_v_idx;

	for(int i=0; i<b.n_vertices(); i++)
	{
		for(int j=0; j<b[i].n_connections(); j++)
		{
			//cout << "on edge ("; 
			//	b[i].print(false); cout << ", "; b[b[i][j]].print(false);
			//	cout << ")" << endl;

			if(b_a_index_map[i] < 0)
				b_a_index_map[i] = a.index(b[i]);
			a_u_idx = b_a_index_map[i];

			if(b_a_index_map[b[i][j]] < 0)
				b_a_index_map[b[i][j]] = a.index(b[b[i][j]]);
			a_v_idx = b_a_index_map[b[i][j]];

			vector<int> pi = dfs_visit(a, a_u_idx);

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

	/*cout << "b:" << endl;
	b.print();
	cout << "a:" << endl;
	a.print();*/

	//cout << "-----" << endl;

	if(!contains(a, b))
	{
		cout << "NAO" << endl;//"(contains)" << endl;
		return 0;
	}
	else if(!task_2(a, b))
	{
		cout << "NAO" << endl;//"(task_2)" << endl;
	}
	else
		cout << "SIM" << endl;

	return 0;
}
