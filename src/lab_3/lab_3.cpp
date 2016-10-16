/* Copyright 2016 Erik Perillo */

#include <vector>
#include <iostream>
#include <string>
#include <queue>
#include <cmath>

#define NONE (-1)
#define INF (-1)

#define cost_greater(a, b) ((b == INF)?false:((a == INF)?true:(a > b)))

using namespace std;
/*
Class representing graph vertices.
Each vertex has an adjacency list of ids.
Those ids are the indexes of the vertices to which they connect in graph.
*/
template <class type>
class Vertex
{
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
        return this->val != vertex.get_val();
    }

	/*//Returns i'th vertex id on adjacency list.
	int operator[](int i) const
	{
		return this->conns_vertices[i];
	}
	int operator[](int i)
	{
		return this->conns_vertices[i];
	}*/

    //Returns ith vertex in graph.
    int conn_vtx(int i) const
    {
        return this->conns_vertices[i];
    }

    //Returns ith cost in graph.
    double conn_cost(int i) const
    {
        return this->conns_costs[i];
    }

    //Appends vertex index to adjacency list.
    void add(int vtx_id, double cost)
    {
        this->conns_vertices.push_back(vtx_id);
        this->conns_costs.push_back(cost);
    }

	//If vtx_id is not present, returns a negative number.
	int index(int vtx_id) const
	{
		for(unsigned i=0; i<this->conns_vertices.size(); i++)
			if(this->conns_vertices[i] == vtx_id)
				return (int)i;

		return -1;
	}

	//True if vertex index vtx_id is positive.
	bool connects(int vtx_id) const
	{
		return this->index(vtx_id) >= 0;
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
    int n_conns() const
    {
        return this->conns_vertices.size();
    }

    private:
    //value vertex holds
    type val;
    //adjacency list
    std::vector<int> conns_vertices;
    //connections weights.
    std::vector<double> conns_costs;
};

/*
Struct representing an edge, ie, a pair of vertices.
Connection goes from u to v, that is, first to second vertices passed.
*/
template <class type>
struct Edge
{
    Edge(const Vertex<type>& u, const Vertex<type>& v, double cost):
        u(u), v(v), cost(cost)
    {;}
    ~Edge()
    {;}

    Vertex<type> u;
    Vertex<type> v;
    double cost;
};

/*
Class representing a graph using adjacency lists.
Each vertex has an id which is the position it is in graph's list.
Each vertex adjacency list id refer to the graph ids.
The graph is not dynamic, ie. it doesn't change after construction time.
*/
template <class type>
class Graph
{
    public:
    //constructors
    Graph()
    {;}
    Graph(const std::vector<Edge<type> >& edges)
    {
        //adding all vertices
        for(unsigned i=0; i<edges.size(); i++)
        {
            this->add_vertex(edges[i].u);
            this->add_vertex(edges[i].v);
        }
        //making edges
        for(unsigned i=0; i<edges.size(); i++)
            this->add_edge(edges[i]);
    }
    //destructor
    ~Graph()
    {;}

    //Returns index of vertex in graph.
    //If it doesn't exist, returns a negative number.
    int index(const Vertex<type>& vtx) const
    {
        for(int i=0; i<this->n_vertices(); i++)
            if(this->vertices[i] == vtx)
                return i;

        return -1;
    }

    //True if graph has vertex equal to specified.
    bool has_vertex(const Vertex<type>& vtx) const
    {
        return this->index(vtx) >= 0;
    }

    //Returns number of vertices in graph.
    int n_vertices() const
    {
        return (int)this->vertices.size();
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

    //Prints all vertices with their adjacencies.
    void print() //const
    {
        for(int i=0; i<this->n_vertices(); i++)
        {
            std::cout << "[" << i << "] ";
            this->vertices[i].print(false);

            for(int j=0; j<this->vertices[i].n_conns(); j++)
            {
                std::cout << " -> ";
                int v_idx = this->vertices[i].conn_vtx(j);
                this->vertices[v_idx].print(false);
            }

            std::cout << std::endl;
        }
    }

    private:
    //list of vertices
    std::vector<Vertex<type> > vertices;

    //functions used to build graph. they are only called at construction time.
    //Adds vertex to graph if its not still in it and returns its index
    void add_vertex(const Vertex<type>& vtx)
    {
        if(!this->has_vertex(vtx))
            this->vertices.push_back(vtx);
    }
    //Adds edge to graph. assumes both vertices are already in graph.
    void add_edge(const Edge<type>& edge)
    {
        int u_id, v_id;

        u_id = this->index(edge.u);
        v_id = this->index(edge.v);

        this->vertices[u_id].add(v_id, edge.cost);
    }
};

//Squares argument passed.
double sqr(double x)
{
    return x*x;
}

//Returns euclidean distance between two points.
double distance(double x1, double y1, double x2, double y2)
{
    return sqrt(sqr(x1 - x2) + sqr(y1 - y2));
}

class MSTWeightComp
{
    public:
    MSTWeightComp(std::vector<double>& min_costs): min_costs(min_costs)
    {;}
    ~MSTWeightComp()
    {;}

    //true if key with index u_id is greater than key from index v_id.
    bool operator()(int u_id, int v_id)
    {
        return cost_greater(this->min_costs[u_id], this->min_costs[v_id]);
    }

    private:
    std::vector<double>& min_costs;
};

std::vector<int> in_queue_seq(const std::vector<bool>& in_queue)
{
    std::vector<int> vec;

    for(unsigned i=0; i<in_queue.size(); i++)
        if(in_queue[i])
            vec.push_back((int)i);

    return vec;
}

template <class type>
std::vector<int> min_spanning_tree(const Graph<type>& graph, 
        const Vertex<type>& vtx)
{
    //the ith element has the father of the ith vertex in graph in the mst
    //(minimum spanning tree).
    std::vector<int> pi(graph.n_vertices(), NONE);
    //the ith element has the current smallest distance from ith vertex in
    //graph to the mst.
    std::vector<double> min_costs(graph.n_vertices(), INF);
    //setting root to have cost zero
    min_costs[graph.index(vtx)] = 0.0;
    //the ith element tells if the ith vertex in graph is in the priority queue.
    std::vector<bool> in_queue(graph.n_vertices(), true);
    //comparer function for min_heap
    MSTWeightComp comparer(min_costs);

    for(int i=0; i<graph.n_vertices(); i++)
    {
        //simply a vector of indexes of graph
        std::vector<int> indexes = in_queue_seq(in_queue);
        std::priority_queue<int, std::vector<int>, MSTWeightComp>
            queue(comparer, indexes);

        int u_id = queue.top();
        queue.pop();
        in_queue[u_id] = false;

        for(int j=0; j<graph[u_id].n_conns(); j++)
        {
            int v_id = graph[u_id].conn_vtx(j);
            double cost = graph[u_id].conn_cost(j);

            if(in_queue[v_id] && cost_greater(min_costs[v_id], cost))
            {
                pi[v_id] = u_id;
                min_costs[v_id] = cost;
            }
        }

        /*while(!queue.empty())
        {
            std::cout << queue.top() << ", " << min_costs[queue.top()] << "\n";
            queue.pop();
        }*/
    }

    return pi;
}

//Reads from stdin coordinates values and builds graph.
Graph<int> fill_graph(int n_lines)
{
    std::vector<Vertex<int> > vertices;
    std::vector<double> xs;
    std::vector<double> ys;
    std::vector<Edge<int> > edges;
    double x, y;

    for(int i=0; i<n_lines; i++)
    {
        std::cin >> x;
        std::cin >> y;
        xs.push_back(x);
        ys.push_back(y);
        vertices.push_back(Vertex<int>(i));
    }

    for(int i=0; i<n_lines; i++)
        for(int j=0; j<n_lines; j++)
            if(i != j)
            {
                double dist = distance(xs[i], ys[i], xs[j], ys[j]);
                edges.push_back(Edge<int>(vertices[i], vertices[j], dist));
            }

    return Graph<int>(edges);
}

template <class type>
void get_dists(const Graph<type> graph, const std::vector<int>& pi,
        double fiber_thresh, double* normal_cable, double* fiber_cable)
{
    *normal_cable = 0.0;
    *fiber_cable = 0.0;

    for(int i=0; i<(int)pi.size(); i++)
    {
        if(pi[i] == NONE)
            continue;

        int v_id = pi[i];
        double cost = graph[v_id].conn_cost(graph[v_id].index(i));

        if(cost > fiber_thresh)
            (*fiber_cable) += cost;
        else
            (*normal_cable) += cost;
    }
}

double round(double x)
{
    return floor(x + 0.5);
}

void problem()
{
    int n_points;
    double fiber_thresh;
    double fiber_cost, normal_cost;
    std::vector<int> pi;

    std::cin >> n_points;
    std::cin >> fiber_thresh;

    Graph<int> graph = fill_graph(n_points);
    //graph.print();
    if(n_points < 2)
    {
        fiber_cost = 0.0;
        normal_cost = 0.0;
    }
    else
    {
        pi = min_spanning_tree(graph, graph[0]);
        get_dists(graph, pi, fiber_thresh, &normal_cost, &fiber_cost);
    }

    cout << round(normal_cost) << " " << round(fiber_cost) << endl;
    //cout << "normal = " << round(normal_cost/2)
     //   << ", fiber = " << round(fiber_cost/2) << endl;
}


int main()
{
    int n_test_cases;

    cin >> n_test_cases;
    for(int i=0; i<n_test_cases; i++)
    {
        //cout << "case " << i << endl;
        problem();
    }

    return 0;
}
