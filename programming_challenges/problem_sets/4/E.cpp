#include <cstdio>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

#define MAXN 200000 + 5
typedef pair<long long, long long> pii;

multimap<long long, pii> edge_mins;
vector<pair<pii, long long>> output_order;
map<pii, long long> mst; // the MST found by kruskalls, with the cost of this edge
set<long long> subset[MAXN];
map<pii, long long> max_path_weight;
vector<long long> neighbours[MAXN];

long long root[MAXN];
long long sz[MAXN];

long long get_rep(long long i) {
    if (root[i] == i) return i;
    return root[i] = get_rep(root[i]);
}

long long merge(long long x, long long y) {
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

    long long n, m;
    cin >> n >> m;

    for (long long i=0; i < m; i++) {
        long long f1, f2, cost;
        cin >> f1 >> f2 >> cost;
        edge_mins.insert({cost, {f1, f2}});
        output_order.push_back({{f1, f2}, cost});
        neighbours[f1].push_back(f2);
        neighbours[f2].push_back(f1);
    }   

    // init DSU and subsets
    for (long long i=1; i <= n; i++) {
        root[i] = i;
        sz[i] = 1;
        subset[i].insert(i);
    }

    long long total_cost = 0, edges_done = 0, mst_rep = -1;
    for (multimap<long long, pii>::iterator it = edge_mins.begin(); it != edge_mins.end(); it++) {

        if (get_rep(it->second.first) == get_rep(it->second.second)) continue; // can't merge within same set
        //printf("adding edge %lld -> %lld of cost %lld to the MST\n", it->second.first, it->second.second, it->first);

        // add max path weights for all pairs a,b from the different subsets
        long long n1 = it->second.first, n2 = it->second.second, edge_weight = it->first;
        long long s, l;
        if (sz[get_rep(n1)] < sz[get_rep(n2)]) { s = n1; l = n2; } 
        else { s = n2; l = n1; }
        set<long long> &ssub = subset[get_rep(s)], &lsub = subset[get_rep(l)];;
        //printf("smaller subset contains:\n");
        //for (auto n : ssub) printf("node %lld\n", n);
        //printf("larger subset contains:\n");
        //for (auto n : lsub) printf("node %lld\n", n);
        // iterate over the nodes of the smaller subset, see if their neighbours are part of the larger subset
        for (auto n1 : ssub) {
            for (auto n2 : neighbours[n1]) {
                if (lsub.find(n2) != lsub.end()) {
                    //if (max_path_weight[{n1, n2}] != 0) continue;
                    //printf("max weight path (%lld)->(%lld) = %lld added.\n", n1, n2, edge_weight);
                    max_path_weight[{n1, n2}] = edge_weight;
                    max_path_weight[{n2, n1}] = edge_weight;
                }
            }
            
        }

        // now perform the merge
        mst_rep = merge(it->second.first, it->second.second);

        // update the subsets
        if (mst_rep == s) {
            //printf("combined set has representative %lld\n", get_rep(s));
            for (auto n : lsub) {
                //printf("-adding %lld to combined set\n", n);
                ssub.insert(n);
            }
            //printf("combined subset contains:\n");
            //for (auto n : subset[get_rep(s)]) printf("node %lld\n", n);
        } else {
            //printf("combined set has representative %lld\n", get_rep(l));
            for (auto n : ssub) {
                //printf("-adding %lld to combined set\n", n);
                lsub.insert(n);
            }
            //printf("combined subset contains:\n");
            //for (auto n : subset[get_rep(l)]) printf("node %lld\n", n);
        }

        edges_done++;
        total_cost += edge_weight;
        mst[it->second] = it->first;
        max_path_weight[{n1, n2}] = edge_weight;
        max_path_weight[{n2, n1}] = edge_weight;
        if (edges_done == n-1) break;
    }

    // MST is now constructed, do queries
    for (auto e : output_order) {
        // check if the edge is already part of the mst
        if (mst.find(e.first) != mst.end()) printf("%lld\n", total_cost);
        else {
            // need to remove cost of maximum edge and add cost of new edge
            //printf("total cost: %lld, adding new edge: %lld, removing old max path edge(%lld, %lld): %lld\n", total_cost, e.second, e.first.first, e.first.second, max_path_weight[{e.first.first, e.first.second}]);
            printf("%lld\n", total_cost - max_path_weight[{e.first.first, e.first.second}] + e.second);
        }
    }
}
