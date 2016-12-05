/* Copyright 2016 Erik Perillo */

#include <vector>
#include <iostream>
#include <list>
#include <queue>

using namespace std;

//default edge cost if no edge is found
#define DEF_EDGE_COST 0
//default int value for none
#define NONE (-1)
//character for alpha sector
#define ALPHA_CHR '#'

//type for adjacency list iterator
typedef list<pair<int, int> >::const_iterator adj_list_it_t;

/*
Class representing a graph via adjacency lists.
*/
class Graph
{
    public:
    //constructors
    Graph()
    {;}
    Graph(int n_nodes):
        nodes(vector<list<pair<int, int> > >(n_nodes))
    {;}
    //destructor
    ~Graph()
    {;}

    //Returns number of nodes in graph.
    int n_nodes() const
    {
        return (int)this->nodes.size();
    }

    //Adds edge (u, v) to graph.
    void add_edge(int u_id, int v_id, int val)
    {
        this->nodes[u_id].push_back(make_pair(v_id, val));
    }

    //Removes edge (u, v) from graph.
    void del_edge(int u_id, int v_id)
    {
        list<pair<int, int> >::iterator it;
        for(it=this->nodes[u_id].begin(); it!=this->nodes[u_id].end(); ++it)
            if((*it).first == v_id)
            {
                this->nodes[u_id].erase(it);
                break;
            }
    }

    //Returns true iff edge (u, v) exists in graph.
    bool has_edge(int u_id, int v_id) const
    {
        adj_list_it_t it;
        for(it=this->nodes[u_id].begin(); it!=this->nodes[u_id].end(); ++it)
            if((*it).first == v_id)
                return true;

        return false;
    }

    //Gets cost of edge (u, v).
    int get_edge_cost(int u_id, int v_id) const
    {
        adj_list_it_t it;
        for(it=this->nodes[u_id].begin(); it!=this->nodes[u_id].end(); ++it)
            if((*it).first == v_id)
                return (*it).second;

        return DEF_EDGE_COST;
    }

    //Gets iterator to beginning of adjacency list of node u.
    adj_list_it_t adj_list_begin(int u_id) const
    {
        return this->nodes[u_id].begin();
    }

    //Gets iterator to end of adjacency list of node u.
    adj_list_it_t adj_list_end(int u_id) const
    {
        return this->nodes[u_id].end();
    }

    //Gets size of adjacency list of node u.
    int adj_list_size(int u_id) const
    {
        return this->nodes[u_id].size();
    }

    private:
    //list of nodes
    vector<list<pair<int, int> > > nodes;
};

void swap(int* x, int* y)
{
    int aux = *x;
    *x = *y;
    *y = aux;
}

//Breadth-first search customized to get a path from src to dst.
vector<int> bfs(const Graph& graph, int src_id, int dst_id)
{
    queue<int> q;
    vector<bool> visited(graph.n_nodes(), false);
    vector<int> pi(graph.n_nodes(), NONE);

    visited[src_id] = true;
    q.push(src_id);

    while(!q.empty())
    {
        int u_id = q.front();
        q.pop();

        adj_list_it_t it;
        for(it=graph.adj_list_begin(u_id); it!=graph.adj_list_end(u_id); ++it)
        {
            int v_id = (*it).first;

            if(!visited[v_id])
            {
                pi[v_id] = u_id;
                visited[v_id] = true;
                q.push(v_id);
                if(v_id == dst_id)
                    return pi;
            }
        }
    }

    return pi;
}

//Gets residual net from flow.
Graph residual_net(const Graph& flow, const Graph& caps)
{
    Graph res_net = Graph(flow.n_nodes());

    for(int u_id=0; u_id<caps.n_nodes(); u_id++)
    {
        adj_list_it_t it;
        for(it=caps.adj_list_begin(u_id); it!=caps.adj_list_end(u_id); ++it)
        {
            int v_id = (*it).first;
            int cap = (*it).second;
            int flux = flow.get_edge_cost(u_id, v_id);

            if(cap - flux > 0)
                res_net.add_edge(u_id, v_id, cap - flux);
            if(flux > 0)
                res_net.add_edge(v_id, u_id, flux);
        }
    }

    return res_net;
}

//Gets residual capacity of path from src to dst given by pi.
int residual_capacity(const Graph& res_net,
    const vector<int>& pi, int src_id, int dst_id)
{
    int v_id = dst_id;
    int res_cap = res_net.get_edge_cost(pi[v_id], v_id);

    for(v_id=pi[v_id]; v_id!=src_id; v_id=pi[v_id])
    {
        int cap = res_net.get_edge_cost(pi[v_id], v_id);
        if(cap < res_cap)
            res_cap = cap;
    }

    return res_cap;
}

//Augments flow. Assumes there is a path from src to dst in pi.
void augment_flow(Graph& flow, Graph& res_net,
        const vector<int>& pi, int src_id, int dst_id)
{
    int res_cap = residual_capacity(res_net, pi, src_id, dst_id);

    for(int v_id=dst_id; v_id!=src_id; v_id=pi[v_id])
    {
        int u_id = pi[v_id];
        int new_flow;
        bool rev = flow.has_edge(v_id, u_id);

        //updating residual net
        int old_cap_1 = res_net.get_edge_cost(u_id, v_id);
        int old_cap_2 = res_net.get_edge_cost(v_id, u_id);
        res_net.del_edge(u_id, v_id);
        res_net.del_edge(v_id, u_id);
        if(old_cap_1 - res_cap > 0)
            res_net.add_edge(u_id, v_id, old_cap_1 - res_cap);
        if(old_cap_2 + res_cap > 0)
            res_net.add_edge(v_id, u_id, old_cap_2 + res_cap);

        if(rev)
            new_flow = flow.get_edge_cost(v_id, u_id) - res_cap;
        else
            new_flow = flow.get_edge_cost(u_id, v_id) + res_cap;

        flow.del_edge(rev?v_id:u_id, rev?u_id:v_id);
        if(new_flow > 0)
            flow.add_edge(rev?v_id:u_id, rev?u_id:v_id, new_flow);
    }
}

//Gets maximum flow using bfs (a.k.a. edmonds karp).
void edmonds_karp(Graph& flow, const Graph& capacities, int src_id, int dst_id)
{
    Graph res_net = residual_net(flow, capacities);

    while(true)
    {
        vector<int> pi = bfs(res_net, src_id, dst_id);

        if(pi[dst_id] == NONE)
            break;

        augment_flow(flow, res_net, pi, src_id, dst_id);
    }
}

//Gets flow value on node src.
int flow_val(const Graph& flow, int src_id)
{
    int val = 0;

    for(int i=0; i<flow.n_nodes(); i++)
        val -= flow.get_edge_cost(i, src_id);
    for(int i=0; i<flow.n_nodes(); i++)
        val += flow.get_edge_cost(src_id, i);

    return val;
}

//Reads char matrix from stdin in boolean matrix format.
vector<vector<bool> > read_mat(int h, int w)
{
    vector<vector<bool> > mat(h, vector<bool>(w, false));
    char chr;

    for(int i=0; i<h; i++)
        for(int j=0; j<w; j++)
        {
            cin >> chr;

            if(chr == ALPHA_CHR)
                mat[i][j] = true;
        }

    return mat;
}

//Maps positive elements into nodes indexes. Also, gets number of positives.
vector<vector<int> > map_mat(const vector<vector<bool> >& mat, int* size)
{
    vector<vector<int> > map(mat.size(), vector<int>(mat[0].size(), NONE));

    for(int i=0; i<(int)mat.size(); i++)
        for(int j=0; j<(int)mat[i].size(); j++)
            if(mat[i][j])
                map[i][j] = (*size)++;

    return map;
}

inline int abs(int x)
{
    return (x > 0)?x:(-x);
}

inline bool in_bounds(const vector<vector<bool> >& mat, int i, int j)
{
    if((i < 0) || (i >= (int)mat.size()))
        return false;
    return (j >= 0) && (j < (int)mat[i].size());
}

//Reduces problem of fitting 2x1 rectangles in an arbitrary area
//into the problem of maximum matching using maximum flow.
Graph reduce(const vector<vector<bool> >& mat)
{
    //number of squares on region
    int n_ones = 0;
    //the element i, j maps to the k'th node in graph that represents it.
    vector<vector<int> > map = map_mat(mat, &n_ones);
    //matching graph
    Graph bipartition(n_ones + 2);
    //destiny flow node
    int drain_id = bipartition.n_nodes() - 1;
    //boolean variables to ensure right side for each node in bipartition
    bool new_line_left = false;
    bool left = false;
    vector<bool> src_to_node(bipartition.n_nodes(), false);
    vector<bool> node_to_drain(bipartition.n_nodes(), false);

    //building graph
    for(int i=0; i<(int)mat.size(); i++)
    {
        new_line_left = !new_line_left;
        left = new_line_left;

        for(int j=0; j<(int)mat[i].size(); j++, left=!left)
        {
            if(!mat[i][j])
                continue;

            for(int i_shift=0; i_shift<2; i_shift++)
                for(int j_shift=0; j_shift<2; j_shift++)
                {
                    int i2 = i + i_shift;
                    int j2 = j + j_shift;

                    if(abs(i_shift) == abs(j_shift) || !in_bounds(mat, i2, j2))
                        continue;

                    if(mat[i2][j2])
                    {
                        int u_id = 1 + map[i][j];
                        int v_id = 1 + map[i2][j2];

                        if(!left)
                            swap(&u_id, &v_id);

                        //if(!bipartition.has_edge(0, u_id))
                        if(!src_to_node[u_id])
                        {
                            bipartition.add_edge(0, u_id, 1);
                            src_to_node[u_id] = true;
                        }
                        if(!node_to_drain[v_id])
                        {
                            bipartition.add_edge(v_id, drain_id, 1);
                            node_to_drain[v_id] = true;
                        }
                        bipartition.add_edge(u_id, v_id, 1);
                    }
                }
        }
    }

    return bipartition;
}

//Transforms solution from maximum matching into solution to fitting rectangles.
int solution_transform(Graph& flow, const Graph& cap)
{
    return (flow.n_nodes()-2) - flow_val(flow, 0);
}

//Solves rectangles fitting problem.
void solve()
{
    int lines, cols, sol;

    cin >> lines;
    cin >> cols;
    vector<vector<bool> > mat = read_mat(lines, cols);

    //reducing problem
    Graph caps = reduce(mat);
    Graph flow(caps.n_nodes());

    //solving
    edmonds_karp(flow, caps, 0, flow.n_nodes()-1);

    //applying transformation
    sol = solution_transform(flow, caps);
    cout << sol << endl;
}

int main()
{
    int n_cases;

    cin >> n_cases;
    for(int i=0; i<n_cases; i++)
        solve();

    return 0;
}
