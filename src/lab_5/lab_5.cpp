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

    //Removes edge (u, v) to graph.
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

    void print() const
    {
        adj_list_it_t it;

        for(int i=0; i<this->n_nodes(); i++)
        {
            cout << "(" << i << ")";
            for(it=this->adj_list_begin(i); it!=this->adj_list_end(i); ++it)
                cout << " -> " << (*it).first << "[" << (*it).second << "]";
            cout << endl;
        }
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

struct BFSResults
{
    BFSResults(vector<int> pi, vector<int> dists):
        pi(pi), dists(dists)
    {;}

    //predecessors tree
    vector<int> pi;
    //distances vector
    vector<int> dists;
};

BFSResults bfs(const Graph& graph, int src_id)
{
    queue<int> q;
    vector<bool> visited(graph.n_nodes(), false);
    BFSResults res(
        vector<int>(graph.n_nodes(), NONE), vector<int>(graph.n_nodes(), NONE));

    res.dists[src_id] = 0;
    res.pi[src_id] = NONE;
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

            if(visited[v_id])
                continue;

            res.dists[v_id] = res.dists[u_id] + 1;
            res.pi[v_id] = u_id;
            visited[v_id] = true;

            q.push(v_id);
        }
    }

    return res;
}

Graph residual_net(Graph& flow, const Graph& caps)
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

int residual_capacity(const Graph& res_net,
    vector<int> pi, int src_id, int dst_id)
{
    int v_id = dst_id;
    int res_cap = res_net.get_edge_cost(pi[dst_id], dst_id);

    //getting residual capacity
    for(v_id=pi[v_id]; v_id!=src_id; v_id=pi[v_id])
    {
        int cap = res_net.get_edge_cost(pi[v_id], v_id);
        if(cap < res_cap)
            res_cap = cap;
    }

    return res_cap;
}

//assumes there is such path.
void augment_flow(Graph& flow, const Graph& res_net,
        const vector<int> pi, int src_id, int dst_id)
{
    int res_cap;

    //getting residual capacity
    res_cap = residual_capacity(res_net, pi, src_id, dst_id);

    for(int v_id=dst_id; v_id!=src_id; v_id=pi[v_id])
    {
        int u_id = pi[v_id];
        int curr_flow;
        bool rev = false;

        if(flow.has_edge(v_id, u_id))
        {
            swap(&u_id, &v_id);
            rev = true;
        }

        curr_flow = flow.get_edge_cost(u_id, v_id);
        flow.del_edge(u_id, v_id);
        flow.add_edge(u_id, v_id, curr_flow + (rev?(-res_cap):res_cap));
    }
}

template<class tp>
void print_vec(const vector<tp> vec, const string& prefix="")
{
    for(unsigned i=0; i<vec.size(); i++)
        cout << prefix << "[" << i << "] = " << vec[i] << endl;
}

void edmonds_karp(Graph& flow, const Graph& capacities, int src_id, int dst_id)
{
    while(true)
    {
        Graph res_net = residual_net(flow, capacities);
        BFSResults bfs_res = bfs(res_net, src_id);

        if(bfs_res.pi[dst_id] == NONE)
            break;

        augment_flow(flow, res_net, bfs_res.pi, src_id, dst_id);
    }
}

int flow_val(const Graph& flow, int src_id)
{
    int val = 0;

    for(int i=0; i<flow.n_nodes(); i++)
        val -= flow.get_edge_cost(i, src_id);
    for(int i=0; i<flow.n_nodes(); i++)
        val += flow.get_edge_cost(src_id, i);

    return val;
}

int main()
{
    Graph flow(6);
    Graph cap(6);

    cap.add_edge(0, 1, 16);
    cap.add_edge(0, 2, 13);
    cap.add_edge(1, 3, 12);
    cap.add_edge(2, 1, 4);
    cap.add_edge(2, 4, 14);
    cap.add_edge(3, 2, 9);
    cap.add_edge(3, 5, 20);
    cap.add_edge(4, 3, 7);
    cap.add_edge(4, 5, 4);

    cout << "caps:" << endl;
    cap.print();
    cout << "flow:" << endl;
    flow.print();

    cout << "running max flow..." << endl;
    edmonds_karp(flow, cap, 0, 5);

    cout << "flow:" << endl;
    flow.print();
    cout << "val: " << flow_val(flow, 0) << endl;

    return 0;
}
