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

enum
{
    UPPER=0,
    LOWER,
    LEFT,
    RIGHT
};

#define mirror(dir) ((dir==UPPER)?LOWER:((dir==LEFT)?RIGHT:(\
    (dir==LOWER)?UPPER:LEFT)))

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

    void add_node(int n_nodes=1)
    {
        for(int i=0; i<n_nodes; i++)
            this->nodes.push_back(list<pair<int, int> >());
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

    int adj_list_size(int u_id) const
    {
        return this->nodes[u_id].size();
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
            if(i > 100)
                break;
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

template<class tp>
void print_mat(const vector<vector<tp> > mat)
{
    for(unsigned i=0; i<mat.size(); i++)
    {
        for(unsigned j=0; j<mat[i].size(); j++)
            cout << mat[i][j] << " ";
        cout << endl;
    }
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

#define upper_true(mat, i, j) (i > 0 && mat[i-1][j])
#define lower_true(mat, i, j) (i < (int)mat.size()-1 && mat[i+1][j])
#define left_true(mat, i, j) (j > 0 && mat[i][j-1])
#define right_true(mat, i, j) (j < (int)mat[i].size()-1 && mat[i][j+1])

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

vector<vector<int> > map_mat(const vector<vector<bool> >& mat, int* size)
{
    vector<vector<int> > map(mat.size(), vector<int>(mat[0].size(), NONE));

    for(int i=0; i<(int)mat.size(); i++)
        for(int j=0; j<(int)mat[i].size(); j++)
            if(mat[i][j])
                map[i][j] = (*size)++;

    return map;
}

bool around(const vector<vector<bool> >& mat, int i, int j, int ignore=NONE)
{
    if(ignore != UPPER && upper_true(mat, i, j))
        return true;
    if(ignore != LOWER && lower_true(mat, i, j))
        return true;
    if(ignore != LEFT && left_true(mat, i, j))
        return true;
    if(ignore != RIGHT && right_true(mat, i, j))
        return true;

    return false;
}


bool kernel(const vector<vector<bool> >& mat, int i1, int j1, int i2, int j2)
{
    int ignore_1 = (j2 > j1)?RIGHT:((j2 < j1)?LEFT:((i2 < i1)?UPPER:LOWER));

    if(!(mat[i1][j1] && mat[i2][j2]))
        return false;

    bool around_1 = around(mat, i1, j1, ignore_1);
    bool around_2 = around(mat, i2, j2, mirror(ignore_1));

    return !(around_1 && around_2);
}

Graph reduce(const vector<vector<bool> >& mat)
{
    int n_true=0;
    vector<vector<int> > map = map_mat(mat, &n_true);
    Graph graph(n_true + 2);
    int dst_id = n_true + 1;

    //building graph
    cout << "start:" << endl;
    graph.print();

    for(int i=0; i<n_true; i++)
        graph.add_edge(0, i+1, 1000);

    cout << "fst transform:" << endl;
    graph.print();

    for(int i=0; i<(int)mat.size(); i++)
        for(int j=0; j<(int)mat[i].size(); j++)
        {
            if(!mat[i][j])
                continue;

            cout << "on mat[" << i << "][" << j << "] = " << mat[i][j] << endl;
            if(left_true(mat, i, j))
            {
                if(!(graph.adj_list_size(1 + map[i][j]) > 0 ||
                    graph.adj_list_size(1 + map[i][j-1]) > 0))
                {
                    cout << "\tLEFT" << endl;
                    graph.add_node();
                    graph.add_edge(1 + map[i][j], dst_id, 1);
                    graph.add_edge(1 + map[i][j-1], dst_id, 1);
                    dst_id++;
                }
            }
            if(right_true(mat, i, j))
            {
                if(!(graph.adj_list_size(1 + map[i][j]) > 0 ||
                    graph.adj_list_size(1 + map[i][j+1]) > 0))
                {
                    cout << "\tRIGHT" << endl;
                    graph.add_node();
                    graph.add_edge(1 + map[i][j], dst_id, 1);
                    graph.add_edge(1 + map[i][j+1], dst_id, 1);
                    dst_id++;
                }
            }
            if(upper_true(mat, i, j))
            {
                if(!(graph.adj_list_size(1 + map[i][j]) > 0 ||
                    graph.adj_list_size(1 + map[i-1][j]) > 0))
                {
                    cout << "\tUP" << endl;
                    graph.add_node();
                    graph.add_edge(1 + map[i][j], dst_id, 1);
                    graph.add_edge(1 + map[i-1][j], dst_id, 1);
                    dst_id++;
                }
            }
            if(lower_true(mat, i, j))
            {
                if(!(graph.adj_list_size(1 + map[i][j]) > 0 ||
                    graph.adj_list_size(1 + map[i+1][j]) > 0))
                {
                    cout << "\tDOWN" << endl;
                    graph.add_node();
                    graph.add_edge(1 + map[i][j], dst_id, 1);
                    graph.add_edge(1 + map[i+1][j], dst_id, 1);
                    dst_id++;
                }
            }
        }

    for(int i=n_true+1; i<graph.n_nodes()-1; i++)
        graph.add_edge(i, graph.n_nodes()-1, 1);

    cout << "snd transform:" << endl;
    graph.print();


    cout << "final transform:" << endl;
    graph.print();

    return graph;
}

int sol_to_sol(const Graph& flow, const Graph& cap)
{
    int root_alone=0, sol=0;
    vector<int> alphas;

    for(int v_id=0; v_id<flow.n_nodes(); v_id++)
        if(cap.has_edge(0, v_id))
            alphas.push_back(v_id);

    for(int i=0; i<(int)alphas.size(); i++)
        if(flow.adj_list_size(alphas[i]) != cap.adj_list_size(alphas[i]))
            sol++;

    return sol;
}

void solve()
{
    int lines, cols;

    cin >> lines;
    cin >> cols;

    cout << "reading mat..." << endl;
    vector<vector<bool> > mat = read_mat(lines, cols);
    cout << "mat:" << endl;

    cout << "reducing..." << endl;
    Graph caps = reduce(mat);
    cout << "making flow..." << endl;
    Graph flow(caps.n_nodes());

    cout << "caps:" << endl;
    caps.print();

    cout << "FLOW BF:" << endl;
    flow.print();

    edmonds_karp(flow, caps, 0, flow.n_nodes()-1);

    cout << "FLOW AFTER:" << endl;
    flow.print();

    print_mat(mat);
    cout << "num: " << flow_val(flow, 0) << endl;

    cout << "sol: " << sol_to_sol(flow, caps) << endl;

}

int main()
{
    /*
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
    */
    int n_cases;

    cin >> n_cases;
    for(int i=0; i<n_cases; i++)
    //for(int i=0; i<1; i++)
        solve();

    return 0;
}
