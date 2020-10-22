#include <cstdio>
#include <iostream>
#include <map>
#include <set>

using namespace std;

#define MAXN 2005
#define INF 1000000000 + 1
typedef pair<int, int> pii;

multimap<int, pii> edge_mins; 
set<int> nodes_done;

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
        //if (f2 < f1) swap(f1, f2);
        edge_mins.insert({cost, {f1, f2}});
    }

    // init DSU
    for (int i=1; i <= n; i++) {
        root[i] = i;
        sz[i] = 1;
    }

    int max_cost = 0, edges_done = 0;
    for (multimap<int, pii>::iterator it = edge_mins.begin(); it != edge_mins.end(); it++) {
        if (merge(it->second.first, it->second.second)) {
            edges_done++;
            max_cost = it->first;
        }
        if (edges_done == n-1) break;

    }
    printf("%d\n", max_cost);
}


/*

    1. loop through the edges from lowest cost to highest
    2. for each edge, try and merge the two nodes that are connected by this edge
    3. if the merge fails, then they were already in the same set, don't increment edges done
    4. if the merge succeeds, increment edges done
    5. once edges done equals n-1, we have fully connected the graph with lowest cost

 */