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

#define abs(x) ((x < 0)?(-x):x)

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

int residual_capacity(const Graph& res_net,
    const vector<int>& pi, int src_id, int dst_id)
{
    int v_id = dst_id;
    int res_cap = res_net.get_edge_cost(pi[v_id], v_id);

    //cout << "res_cap: src=" << src_id << ", dst=" << dst_id << "\n";

    //getting residual capacity
    for(v_id=pi[v_id]; v_id!=src_id; v_id=pi[v_id])
    {
        int cap = res_net.get_edge_cost(pi[v_id], v_id);
        if(cap < res_cap)
            res_cap = cap;

     //   cout << "\tv_id=" << v_id << ", pi[v_id]=" << pi[v_id] <<
      //      ", cap=" << cap << ", res_cap=" << res_cap << endl;
    }

    return res_cap;
}

template<class tp>
void print_vec(const vector<tp> vec, const string& prefix="")
{
    for(unsigned i=0; i<vec.size(); i++)
        cout << prefix << "[" << i << "] = " << vec[i] << endl;
}


//assumes there is such path.
void augment_flow(Graph& flow, const Graph& res_net,
        const vector<int>& pi, int src_id, int dst_id)
{
    int res_cap;

    //getting residual capacity
    res_cap = residual_capacity(res_net, pi, src_id, dst_id);
    //cout << "RESCAP:" << res_cap << endl;

    for(int v_id=dst_id; v_id!=src_id; v_id=pi[v_id])
    {
        int u_id = pi[v_id];
        int new_flow;
        bool rev = false;

        //cout << "u_id=" << u_id << ", v_id=" << v_id << endl;

        if(flow.has_edge(v_id, u_id))
        {
            //swap(&u_id, &v_id);
            rev = true;
            //cout << "\tREV. u_id=" << u_id << ", v_id=" << v_id << endl;
        }

        //new_flow = flow.get_edge_cost(u_id, v_id) + (rev?(-res_cap):res_cap);
        if(rev)
            new_flow = flow.get_edge_cost(v_id, u_id) - res_cap;
        else
            new_flow = flow.get_edge_cost(u_id, v_id) + res_cap;

        flow.del_edge(rev?v_id:u_id, rev?u_id:v_id);

        //cout << "\trev=" << rev << ", new_flow=" << new_flow << endl;
        if(new_flow == -1)
        {
            cout << "\twow...\n";
            cout << "\tEYB)SS\n", print_vec(pi, "pi");
        }
        if(new_flow > 0)
            flow.add_edge(rev?v_id:u_id, rev?u_id:v_id, new_flow);
    }
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
    int iter=0;
    while(true)
    {
        Graph res_net = residual_net(flow, capacities);
        BFSResults bfs_res = bfs(res_net, src_id);

        //cout << "ITER " << iter << ":\n";
        //cout << "caps:\n";
        //capacities.print();
        //cout << "flow:\n";
        //flow.print();
        //cout << "res_new:\n";
        //res_net.print();
        //if(iter == 4)
            //cout << "PI:\n", print_vec(bfs_res.pi);
        iter++;

        if(bfs_res.pi[dst_id] == NONE)
            break;

        //if(iter == 4) cout << "AUGMENTING FLOW...\n";
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

int mat_sum(const vector<vector<bool> >& mat)
{
    int summ = 0;

    for(int i=0; i<(int)mat.size(); i++)
        for(int j=0; j<(int)mat[i].size(); j++)
            summ += (int)mat[i][j];

    return summ;
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

    //if(!(mat[i1][j1] && mat[i2][j2]))
     //   return false;

    bool around_1 = around(mat, i1, j1, ignore_1);
    bool around_2 = around(mat, i2, j2, mirror(ignore_1));

    return !(around_1 && around_2);
}

#define out_of_bounds(mat, i, j) ((i < 0) || (i >= (int)mat.size()) ||\
    (j < 0) || (j >= (int)mat[i].size()))

Graph reduce(const vector<vector<bool> >& mat)
{
    int n_true=0;
    vector<vector<int> > map = map_mat(mat, &n_true);
    int g_size = 2*(n_true + 1);
    Graph graph(g_size);
    int trues = 0;

    //building graph
    for(int i=0; i<n_true; i++)
        graph.add_edge(0, i+1, 1);

    for(int i=0; i<(int)mat.size(); i++)
        for(int j=0; j<(int)mat[i].size(); j++)
        {
            int cap = 0;

            if(!mat[i][j])
                continue;

            for(int i_shift=-1; i_shift<2; i_shift++)
                for(int j_shift=-1; j_shift<2; j_shift++)
                {
                    int i2 = i + i_shift;
                    int j2 = j + j_shift;

                    if(abs(i_shift) == abs(j_shift))
                        continue;
                    if(out_of_bounds(mat, i2, j2))
                        continue;

                    if(mat[i2][j2])
                    {
                        //cout << "anal (" << i << ", " << j << "), (" <<
                         //   i2 << ", " << j2 << "): ";
                        //cout << 1+map[i][j] << ", " << 1+n_true+map[i2][j2] << "\n";
                        graph.add_edge(1+map[i][j], 1+n_true+map[i2][j2], 1);
                        cap++;
                    }
                }

            graph.add_edge(1+n_true+trues, graph.n_nodes()-1, 1);
            trues++;
        }

    return graph;
}

int sol_to_sol(Graph& flow, const Graph& cap)
{
    int n_trues;
    int sol=0;
    int reps=0;
    int sol2=0;
    vector<int> alphas;

    n_trues = (flow.n_nodes() - 2)/2;

    for(int u_id=1; u_id<1+n_trues; u_id++)
    {
        /*adj_list_it_t it;
        for(it=flow.adj_list_begin(u_id); it!=flow.adj_list_end(u_id); ++it)
        {
            int v_id = (*it).first;
            sol++;
            if(flow.has_edge(v_id-n_trues, u_id+n_trues))
            {
                sol--;
                reps++;
            }
        }*/
        for(int v_id=1+n_trues; v_id<1+2*n_trues; v_id++)
        {
            if(flow.has_edge(u_id, v_id))
                sol++;
            if(flow.has_edge(v_id-n_trues, u_id+n_trues))
            {
                sol--;
                reps++;
            }
        }
    }

    return n_trues - (sol + reps/2);
}

void solve(bool debug)
{
    int lines, cols;

    cin >> lines;
    cin >> cols;

    //cout << "reading mat..." << endl;
    vector<vector<bool> > mat = read_mat(lines, cols);

    //cout << "reducing..." << endl;
    Graph caps = reduce(mat);
    //cout << "making flow..." << endl;
    Graph flow(caps.n_nodes());

    if(debug)
    {
        cout << "caps:" << endl;
        caps.print();
        cout << "FLOW BF:" << endl;
        flow.print();
    }

    edmonds_karp(flow, caps, 0, flow.n_nodes()-1);

    if(debug)
    {
        cout << "FLOW AFTER:" << endl;
        flow.print();
        cout << "mat:" << endl;
        print_mat(mat);
        cout << "n_trues: " << mat_sum(mat) << endl;
        cout << "num: " << flow_val(flow, 0) << endl;
    }

    cout << sol_to_sol(flow, caps) << endl;

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
        {
            //cout << i << ": ";
            solve(false);
            //if(i==75)
             //   break;
        }

    return 0;
}
