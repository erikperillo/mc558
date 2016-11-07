/* Copyright 2016 Erik Perillo */
/* PS: I don't think Mr. Feynman would need my help at all! */

#include <vector>
#include <iostream>
#include <list>
#include <utility>
#include <limits>

//symbol for 'no parent'
#define NONE (-1)
//symbol for infinite
#define INF (std::numeric_limits<int>::max() >> 2)

//heap macros
#define heap_parent(i) ((i+1)/2-1)
#define heap_left(i) (2*(i+1)-1)
#define heap_right(i) (2*(i+1))

/*
Class representing a graph via adjacency lists.
Edges may be of any signed numeric type.
*/
template <class num_type>
class Graph
{
    public:
    //constructors
    Graph()
    {;}
    Graph(int n_vertices):
        vertices(std::vector<std::vector<std::pair<int, num_type> > >
            (n_vertices, std::vector<std::pair<int, num_type> >()))
    {;}
    //destructor
    ~Graph()
    {;}

    //Returns number of vertices in graph.
    int n_vertices() const
    {
        return (int)this->vertices.size();
    }

    //Returns number of connections of vertex u.
    int n_connections(int u_id) const
    {
        return (int)this->vertices[u_id].size();
    }

    //Adds edge (u, v) to graph.
    void add_edge(int u_id, int v_id, num_type cost)
    {
        this->vertices[u_id].push_back(std::make_pair(v_id, cost));
    }

    //Returns true iff edge (u, v) exists in graph.
    bool has_edge(int u_id, int v_id) const
    {
        for(unsigned i=0; i<this->vertices[u_id].size(); i++)
            if(this->vertices[u_id][i].first == v_id)
                return true;

        return false;
    }

    //Gets cost of edge (u, v).
    num_type get_edge_cost(int u_id, int v_id) const
    {
        //return this->vertices[u_id][v_id].second;
        for(unsigned i=0; i<this->vertices[u_id].size(); i++)
            if(this->vertices[u_id][i].first == v_id)
                return this->vertices[u_id][i].second;

        return 0;
    }

    //Gets cost of edge (u, v).
    int get_connection(int u_id, int j) const
    {
        return this->vertices[u_id][j].first;
    }

    private:
    //list of vertices
    std::vector<std::vector<std::pair<int, num_type> > > vertices;
};

//Swaps values of x and y.
void swap(int* x, int* y)
{
    int aux = *x;
    *x = *y;
    *y = aux;
}

/*
Class representing minimum priority queue via heap.
*/
class MinHeapByKey
{
    public:
    //constructor
    MinHeapByKey(const std::vector<int>& vals, std::vector<int>& keys):
        vals(vals), keys(keys), positions(std::vector<int>(keys.size(), 0))
    {
        this->build_heap();
        for(unsigned i=0; i<vals.size(); i++)
            this->positions[vals[i]] = i;
    }
    //destructor
    ~MinHeapByKey()
    {;}

    //Gets key from value.
    int get_key(int val) const
    {
        return this->keys[val];
    }

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
    void update_key(int val, int key_val)
    {
        this->keys[val] = key_val;
        int id = this->positions[val];
        while(id > 0 && this->compare(id, heap_parent(id)))
        {
            this->swap(id, heap_parent(id));
            id = heap_parent(id);
        }
    }

    private:
    //values
    std::vector<int> vals;
    //keys of values
    std::vector<int> keys;
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
        //return cost_greater(this->keys[this->vals[id_2]],
         //       this->keys[this->vals[id_1]]);
        return this->keys[this->vals[id_1]] < this->keys[this->vals[id_2]];
    }

    //Swaps val[id_2] with val[id_1], updating its positions.
    void swap(int id_1, int id_2)
    {
        ::swap(&this->positions[this->vals[id_1]],
            &this->positions[this->vals[id_2]]);
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

//Auxiliar routine for method dijkstra.
void relax(const Graph<int>& graph, int u_id, int v_id,
        std::vector<int>& pi, std::vector<int>& dists,
        MinHeapByKey& queue)
{
    int dist;

    dist = queue.get_key(u_id) + graph.get_edge_cost(u_id, v_id);
    if((dists[v_id] > dist))
    {
        pi[v_id] = u_id;
        dists[v_id] = dist;
        queue.update_key(v_id, dist);
    }
}

//Struct for dijkstra algorithm return values.
struct DijkstraResults
{
    DijkstraResults()
    {;}
    DijkstraResults(int root, std::vector<int> pi, std::vector<int> dists):
        root(root),
        pi(pi),
        dists(dists)
    {;}
    ~DijkstraResults()
    {;}

    //root node algorithm was run
    int root;
    //parents vector
    std::vector<int> pi;
    //dists vector
    std::vector<int> dists;
};

//Gets shortest paths of graph, starting from vertex root.
//Returns a struct with the root, pi (the parents list) and distances.
DijkstraResults dijkstra(const Graph<int>& graph, int src_id)
{
    //the ith element has the father of the ith vertex in path from src_id
    //to vertex i.
    std::vector<int> pi(graph.n_vertices(), NONE);
    //the ith element has the current smallest distance from vertex src_id
    //to vertex i.
    std::vector<int> dists(graph.n_vertices(), INF);
    //setting root to have cost zero
    dists[src_id] = 0;
    //priority queue
    MinHeapByKey queue(seq(0, graph.n_vertices()-1), dists);

    while(!queue.empty())
    {
        int u_id = queue.pop();

        for(int i=0; i<graph.n_connections(u_id); i++)
        {
            int v_id = graph.get_connection(u_id, i);
            relax(graph, u_id, v_id, pi, dists, queue);
        }
    }

    return DijkstraResults(src_id, pi, dists);
}

//Auxiliar for mark.
void _mark(Graph<int> graph, int u_id, std::vector<bool>& reachable)
{
    reachable[u_id] = true;

    for(int i=0; i<graph.n_connections(u_id); i++)
    {
        int v_id = graph.get_connection(u_id, i);

        if(!reachable[v_id])
            _mark(graph, v_id, reachable);
    }
}

//Marks every node in graph that are acessible via the root.
std::vector<bool> mark(const Graph<int>& graph, int root_id)
{
    //the ith' element is true iff it is reachable from root
    std::vector<bool> reachable(graph.n_vertices(), false);
    reachable[root_id] = true;

    for(int i=0; i<graph.n_connections(root_id); i++)
    {
        int v_id = graph.get_connection(root_id, i);

        if(!reachable[v_id])
            _mark(graph, v_id, reachable);
    }

    return reachable;
}

//Returns transposed graph.
Graph<int> transpose(const Graph<int>& graph)
{
    Graph<int> t_graph(graph.n_vertices());
    int u_id, v_id, cost;

    for(int i=0; i<graph.n_vertices(); i++)
    {
        u_id = i;

        for(int j=0; j<graph.n_connections(u_id); j++)
        {
            v_id = graph.get_connection(u_id, j);
            cost = graph.get_edge_cost(u_id, v_id);

            t_graph.add_edge(v_id, u_id, cost);
        }
    }

    return t_graph;
}

//max_edge_w_limited_path auxiliar.
int _max_edge_w_limited_path(const Graph<int>& t_graph,
    int path_w_limit,
    const std::vector<bool>& reachable,
    const std::vector<int>& graph_dists, const std::vector<int>& t_graph_dists)
{
    int max_edge_w = -1;

    for(int i=0; i<t_graph.n_vertices(); i++)
    {
        int u_id = i;

        if(!reachable[u_id])
            continue;

        for(int j=0; j<t_graph.n_connections(u_id); j++)
        {
            int v_id = t_graph.get_connection(u_id, j);
            int edge_w = t_graph.get_edge_cost(u_id, v_id);
            int path_w = graph_dists[v_id] + edge_w + t_graph_dists[u_id];

            if(edge_w > max_edge_w && path_w <= path_w_limit)
                max_edge_w = edge_w;
        }
    }

    return max_edge_w;
}

//Returns heaviest edge in a path from src to dst vertices not exceding
//limit weight.
int max_edge_w_limited_path(const Graph<int>& graph,
    int src_id, int dst_id, int path_w_limit)
{
    //transposed graph
    Graph<int> t_graph = transpose(graph);
    //reachable nodes
    std::vector<bool> reachable = mark(graph, src_id);
    //shortest paths of graph and transposed graph
    DijkstraResults res = dijkstra(graph, src_id);
    DijkstraResults t_res = dijkstra(t_graph, dst_id);

    return _max_edge_w_limited_path(t_graph,
        path_w_limit,
        reachable,
        res.dists, t_res.dists);
}

//Reads values from stdin and builds graph.
Graph<int> fill_graph(int n_vertices, int n_edges)
{
    Graph<int> graph(n_vertices);
    int u_id, v_id, cost;

    for(int i=0; i<n_edges; i++)
    {
        std::cin >> u_id;
        std::cin >> v_id;
        std::cin >> cost;
        graph.add_edge(u_id, v_id, cost);
    }

    return graph;
}

using namespace std;

int main()
{
    int n_test_cases;
    int n_vertices, n_edges, src_id, dst_id, points_limit;
    int best;

    cin >> n_test_cases;
    for(int i=0; i<n_test_cases; i++)
    {
        cin >> n_vertices;
        cin >> n_edges;
        cin >> src_id;
        cin >> dst_id;
        cin >> points_limit;

        Graph<int> graph = fill_graph(n_vertices, n_edges);

        best = max_edge_w_limited_path(graph, src_id, dst_id, points_limit);
        cout << best << endl;
    }

    return 0;
}
