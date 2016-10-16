/* Copyright 2016 Erik Perillo */

#include <vector>
#include <iostream>
#include <string>
#include <cmath>
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

    //Returns ith vertex in graph.
    Vertex<type>* conn_vtx(int i) const
    {
        return this->conns_vertices[i];
    }
    //Returns ith cost in graph.
    double conn_cost(int i) const
    {
        return this->conns_costs[i];
    }

    //Appends vertex index to adjacency list.
    void add(Vertex<type>* vtx, double cost)
    {
        this->conns_vertices.push_back(vtx);
        this->conns_costs.push_back(cost);
    }

    //Returns true iff vertex vtx is in conns.
    bool connects(const Vertex<type>& vtx) const
    {
        for(unsigned i=0; i<this->conns.size(); i++)
            if(*(this->conns_vertices[i]) == vtx)
                return true;

        return false;
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
    std::vector<Vertex<type>*> conns_vertices;
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
                (this->vertices[i].conn_vtx(j))->print(false);
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

        this->vertices[u_id].add(&this->vertices[v_id], edge.cost);
    }
};

//Squares argument passed.
double sqr(double x)
{
    return x*x;
}

//Returns euclidean distance between two points.
double dist(double x1, double y1, double x2, double y2)
{
    return sqrt(sqr(x1 - x2) + sqr(y1 - y2));
}

using namespace std;

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
                edges.push_back(Edge<int>(vertices[i], vertices[j],
                    dist(xs[i], ys[i], xs[j], ys[j])));

    return Graph<int>(edges);
}

void problem()
{
    int n_points;
    double fiber_thresh;

    std::cin >> n_points;
    std::cin >> fiber_thresh;

    Graph<int> graph = fill_graph(n_points);
    graph.print();
}


int main()
{
    int n_test_cases;

    cin >> n_test_cases;
    for(int i=0; i<n_test_cases; i++)
    {
        cout << "case " << i << endl;
        problem();
    }

    return 0;
}
