/* Copyright 2016 Erik Perillo */

#include <vector>
#include <iostream>
#include <list>

using namespace std;

//default edge cost if no edge is found
#define DEF_EDGE_COST 0

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
    int edge_cost(int u_id, int v_id) const
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

    private:
    //list of nodes
    vector<list<pair<int, int> > > nodes;
};

int main()
{
    Graph g(10);

    g.add_edge(1, 2, 1);
    g.add_edge(1, 4, 3);
    g.add_edge(2, 1, 0);
    g.add_edge(1, 3, 23);
    g.del_edge(1, 2);

    adj_list_it_t it;
    for(it=g.adj_list_begin(1); it!=g.adj_list_end(1); ++it)
    {
        cout << (*it).first << ", " << (*it).second << endl;
    }
    cout << g.has_edge(1, 2) << endl;
    cout << g.has_edge(2, 2) << endl;
    cout << g.has_edge(2, 1) << endl;
    cout << g.has_edge(4, 6) << endl;

    return 0;
}
