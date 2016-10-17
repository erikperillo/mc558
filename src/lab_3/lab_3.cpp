/* Copyright 2016 Erik Perillo */

#include <vector>
#include <iostream>
#include <string>
#include <cmath>

//symbol for 'no parent'
#define NONE (-1)
//infinite graph weight symbol
#define INF (-1)

//true if edge graph weight a is bigger than b
#define cost_greater(a, b) ((b == INF)?false:((a == INF)?true:(a > b)))

//rounds float/double to nearest integer
#define round(x) ((int)floor(x + 0.5))

//square of x
#define sqr(x) ((x)*(x))
//euclidean distance between two points
#define distance(x1, y1, x2, y2) (sqrt(sqr(x1 - x2) + sqr(y1 - y2)))

//heap macros
#define heap_parent(i) ((i+1)/2-1)
#define heap_left(i) (2*(i+1)-1)
#define heap_right(i) (2*(i+1))

/*
Class representing a fully-connected graph.
*/
class FullyConnectedGraph
{
    public:
    //constructors
    FullyConnectedGraph()
    {;}
    FullyConnectedGraph(int num_vertices):
        vertices(std::vector<std::vector<double> >
            (num_vertices, std::vector<double>(num_vertices, 0.0)))
    {;}
    //destructor
    ~FullyConnectedGraph()
    {;}

    //Returns number of vertices in graph.
    int n_vertices() const
    {
        return (int)this->vertices.size();
    }

    //Adds edge to graph. Assumes both vertices are already in graph.
    void set_edge_cost(int u_id, int v_id, double cost)
    {
        this->vertices[u_id][v_id] = cost;
    }
    double get_edge_cost(int u_id, int v_id) const
    {
        return this->vertices[u_id][v_id];
    }

    private:
    //list of vertices
    std::vector<std::vector<double> > vertices;
};

//Swaps values of x and y.
void swap(int* x, int* y)
{
    int aux = *x;
    *x = *y;
    *y = aux;
}

/*
Class representing min-heap, to be sorted by keys.
*/
class MinHeapByKey
{
    public:
    //constructor
    MinHeapByKey(const std::vector<int>& vals, std::vector<double>& keys):
        vals(vals), keys(keys), positions(std::vector<int>(keys.size(), 0))
    {
        this->build_heap();
        for(unsigned i=0; i<vals.size(); i++)
            this->positions[vals[i]] = i;
    }
    //destructor
    ~MinHeapByKey()
    {;}

    //True if heap is empty.
    bool empty()
    {
        return this->vals_size() == 0;
    }

    //Returns minimum element, removing it from heap and updating it.
    int pop()
    {
        int ret;

        ret = this->vals[0];
        this->swap(0, this->vals_size()-1);
        this->vals.pop_back();
        this->heapify(0);

        return ret;
    }

    //Updates pair key, val and updates heap.
    void update_key(int val, double key_val)
    {
        this->keys[val] = key_val;
        int id = this->positions[val];
        while(id >= 0 && this->compare(id, heap_parent(id)))
        {
            this->swap(id, heap_parent(id));
            id = heap_parent(id);
        }
    }

    private:
    //values
    std::vector<int> vals;
    //keys of values
    std::vector<double> keys;
    //positions of values in vals vector
    std::vector<int> positions;

    //Number of elements in heap.
    int vals_size()
    {
        return (int)this->vals.size();
    }

    //True if key[val[id_1]] < key[val[id_2]].
    bool compare(int id_1, int id_2)
    {
        return cost_greater(this->keys[this->vals[id_2]],
                this->keys[this->vals[id_1]]);
    }

    //Swaps val[id_2] with val[id_1], updating its positions.
    void swap(int id_1, int id_2)
    {
        this->positions[this->vals[id_1]] = id_2;
        this->positions[this->vals[id_2]] = id_1;
        ::swap(&this->vals[id_1], &this->vals[id_2]);
    }

    //'floats down' value[id] in heap in order to put it at the right place.
    void heapify(int id)
    {
        int left_id = heap_left(id);
        int right_id = heap_right(id);
        int min_id = id;

        if(left_id < this->vals_size() && this->compare(left_id, min_id))
            min_id = left_id;
        if(right_id < this->vals_size() && this->compare(right_id, min_id))
            min_id = right_id;

        if(min_id != id)
        {
            this->swap(min_id, id);
            this->heapify(min_id);
        }
    }

    //Makes heap.
    void build_heap()
    {
        for(int i=this->vals_size()/2; i>=0; i--)
            this->heapify(i);
    }
};

//Returns a list of vertex indexes that are still in priority queue.
std::vector<int> seq(int start, int end)
{
    std::vector<int> vec;

    for(int i=start; i<=end; i++)
        vec.push_back(i);

    return vec;
}

//Gets minimum spanning tree of graph, rooted in vertex vtx.
//Returns pi, the parents list.
std::vector<int> min_spanning_tree(const FullyConnectedGraph& graph, int vtx_id)
{
    //the ith element has the father of the ith vertex in graph in the mst
    //(minimum spanning tree).
    std::vector<int> pi(graph.n_vertices(), NONE);
    //the ith element has the current smallest distance from ith vertex
    //to vertexes in the mst.
    std::vector<double> min_costs(graph.n_vertices(), INF);
    //setting root to have cost zero
    min_costs[vtx_id] = 0.0;
    //the ith element tells if the ith vertex in graph is in the priority queue.
    std::vector<bool> in_queue(graph.n_vertices(), true);
    //simply a vector of indexes of vertexes.
    std::vector<int> indexes = seq(0, graph.n_vertices()-1);
    //priority queue
    MinHeapByKey queue(indexes, min_costs);

    while(!queue.empty())
    {
        //getting element with minimum weight to mst.
        int u_id = queue.pop();
        in_queue[u_id] = false;

        //updating key values
        for(int j=0; j<graph.n_vertices(); j++)
        {
            int v_id = j;
            double cost = graph.get_edge_cost(u_id, v_id);

            if(in_queue[v_id] && cost_greater(min_costs[v_id], cost))
            {
                pi[v_id] = u_id;
                min_costs[v_id] = cost;
                queue.update_key(v_id, cost);
            }
        }
    }

    return pi;
}

//Reads from stdin coordinates values and builds graph.
FullyConnectedGraph fill_graph(int n_lines)
{
    FullyConnectedGraph graph(n_lines);
    std::vector<double> xs;
    std::vector<double> ys;
    double x, y;

    //reading coordinates
    for(int i=0; i<n_lines; i++)
    {
        std::cin >> x;
        std::cin >> y;
        xs.push_back(x);
        ys.push_back(y);
    }

    //making edges with weights as distances
    for(int i=0; i<n_lines; i++)
        for(int j=0; j<n_lines; j++)
        {
            double dist = distance(xs[i], ys[i], xs[j], ys[j]);
            graph.set_edge_cost(i, j, dist);
        }

    return graph;
}

//Gets quantity of normal and fiber cable to be used from minimum spanning tree.
void get_dists(const FullyConnectedGraph& graph, const std::vector<int>& pi,
        double fiber_thresh, double* normal_cable, double* fiber_cable)
{
    *normal_cable = 0.0;
    *fiber_cable = 0.0;

    for(int i=0; i<(int)pi.size(); i++)
    {
        if(pi[i] == NONE)
            continue;

        double cost = graph.get_edge_cost(pi[i], i);

        if(cost > fiber_thresh)
            (*fiber_cable) += cost;
        else
            (*normal_cable) += cost;
    }
}

//Solves the problem of getting a full connection of minimum cost.
void min_cost_full_connection()
{
    int n_points;
    double fiber_thresh;
    double fiber_cost, normal_cost;
    std::vector<int> pi;

    std::cin >> n_points;
    std::cin >> fiber_thresh;

    //building graph from coordinates
    FullyConnectedGraph graph = fill_graph(n_points);

    //getting costs for connection
    if(n_points < 2)
    {
        fiber_cost = 0.0;
        normal_cost = 0.0;
    }
    else
    {
        pi = min_spanning_tree(graph, 0);
        get_dists(graph, pi, fiber_thresh, &normal_cost, &fiber_cost);
    }

    std::cout << round(normal_cost) << " " << round(fiber_cost) << std::endl;
}

using namespace std;

int main()
{
    int n_test_cases;

    cin >> n_test_cases;
    for(int i=0; i<n_test_cases; i++)
        min_cost_full_connection();

    return 0;
}
