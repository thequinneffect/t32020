#include <cstdio>
#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;

#define MAXN 200000 + 5
typedef pair<int, int> pii;

vector<int> graph[MAXN];
multimap<int, pii> edge_mins;
vector<pair<pii, int>> output_order;
map<pii, int> mst; // the MST found by kruskalls, with the cost of this edge
set<int> subset[MAXN];
map<pii, int> max_path_weight;

int root[MAXN];
int sz[MAXN];

int get_rep(int i) {
    if (root[i] == i) return i;
    return root[i] = get_rep(root[i]);
}

int merge(int x, int y) {
    x = get_rep(x);
    y = get_rep(y);
    if (x == y) return 0;
    if (sz[x] < sz[y]) root[x] = y;
    else if (sz[x] > sz[y]) root[y] = x;
    else {
        sz[y]++;
        root[x] = y;
    }
    return root[x];
}

int main() {

    int n, m;
    cin >> n >> m;

    for (int i=0; i < m; i++) {
        int f1, f2, cost;
        cin >> f1 >> f2 >> cost;
        edge_mins.insert({cost, {f1, f2}});
        output_order.push_back({{f1, f2}, cost});
        graph[f1].push_back(f2);
        graph[f2].push_back(f1);
    }   

    // init DSU and subsets
    for (int i=1; i <= n; i++) {
        root[i] = i;
        sz[i] = 1;
        subset[i].insert(i);
    }

    int total_cost = 0, edges_done = 0, mst_rep = -1;
    for (multimap<int, pii>::iterator it = edge_mins.begin(); it != edge_mins.end(); it++) {

        if (get_rep(it->second.first) == get_rep(it->second.second)) continue; // can't merge within same set

        // add max path weights for all pairs a,b from the different subsets
        int n1 = it->second.first, n2 = it->second.second, edge_weight = it->first;
        int s, l;
        set<int> ssub, lsub;
        if (sz[get_rep(n1)] < sz[get_rep(n2)]) {
            // n1 is part of the smaller subset
            ssub = subset[get_rep(n1)];
            lsub = subset[get_rep(n2)];
            s = n1;
            l = n2;
        } else {
            // n2 is part of the smaller subset
            ssub = subset[get_rep(n2)];
            lsub = subset[get_rep(n1)];
            s = n2;
            l = n1;
        }
        // iterate over the nodes of the smaller subset, see if their neighbours are part of the larger subset
        for (auto node : ssub) {
            for (auto neighbour : graph[node]) {
                if (lsub.find(neighbour) != lsub.end()) {
                    // neighbour is in larger subset, so this is maximum edge among path of these two nodes
                    int small, big;
                    (node < neighbour)? max_path_weight[{node, neighbour}] = edge_weight : max_path_weight[{neighbour, node}] = edge_weight;
                }
            }
        }

        // now perform the merge
        mst_rep = merge(it->second.first, it->second.second);

        // update the subsets
        if (mst_rep == s) {
            for (auto n : lsub) subset[s].insert(n);
        } else {
            for (auto n : ssub) subset[l].insert(n);
        }

        edges_done++;
        total_cost += it->first;
        mst[it->second] = it->first;
        if (edges_done == n-1) break;
    }

    // MST is now constructed, do queries
    for (auto e : output_order) {
        // check if the edge is already part of the mst
        if (mst.find(e.first) != mst.end()) printf("%d\n", total_cost);
        else {
            // need to remove cost of maximum edge and add cost of new edge
            int s, l;
            if (e.first.first < e.first.second) {
                s = e.first.first; l = e.first.second;
            } else {
                l = e.first.first; s = e.first.second;
            }
            printf("%d\n", total_cost + e.second - max_path_weight[{s, l}]);
        }
    }
}

/* 

    - start by doing kruskals algorithm to get the actual MST

    - then for each of the 200,000 queries (edges), check if the current edge is in the actual MST
    - if it is then print the cost of the MST
    - if it isn't, then we need to swap out 1 of the edges with the new edge. The edge we swap out must
    be the maximal cost edge that we can remove whilst still having a spanning tree when we add the 
    new edge in. This is equivalent to the maximum cost edge along the path from v to u (where v and u
    are the endpoints of the new edge to be added). 

    How to find the maximum cost edge on path(u, v)? When we do Kruskals, the most recently added
    edge is the largest cost edge added so far (because we sort in increasing order of edge weight).
    This means that when we merge two subsets in the DSU, the edge they are being merged on is
    the largest edge used so far, and hence it is the largest edge on the path between any two nodes
    that are each in a different one of these subsets (i.e. node1 is from subset x, node2 from subset y).

*/

