#include <cstdio>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

#define MAXN 2005
#define INF 1000000000 + 1
typedef pair<int,int> pii;

vector<pii> graph[MAXN]; // (to, weight)
int seen[MAXN];
map<int, int> tree_edges;
multimap<int, int> back_edges;

void dfs(int f) {
    seen[f] = 1;
    for (vector<pii>::iterator nb = graph[f].begin(); nb != graph[f].end(); nb++) {
        int s = f, d = nb->first, c = nb->second;
        if (!seen[nb->first]) {
            tree_edges[d] = c;
            dfs(nb->first);
        } else {
            back_edges.insert({d, c});
        }
    }
}

int main() {

    int n, m;
    cin >> n >> m;

    for (int i=0; i < m; i++) {
        int index, neighbour, cost;
        cin >> index >> neighbour >> cost;
        graph[index].push_back({neighbour, cost});
        //graph[neighbour].push_back({index, cost});
    }

    // for (int i=1; i <= n; i++) {
    //     for (pii e : graph[i]) {
    //         printf("farm %d is connected to farm %d with a road of cost %d\n", i, e.first, e.second);
    //     }
    // }

    dfs(1);

    // printf("Tree edges are:\n");
    // for (auto edge : tree_edges) {
    //     printf("--(%d)--> %d\n", edge.second, edge.first);
    // }

    // printf("Back edges are:\n");
    // for (auto edge : back_edges) {
    //     printf("--(%d)--> %d\n", edge.second, edge.first);
    // }

    for (map<int, int>::iterator be_it = back_edges.begin(); be_it != back_edges.end(); be_it++) {
        // if the cost of this back edge is better than the currently used edge, swap them
        if (be_it->second < tree_edges[be_it->first]) {
            tree_edges[be_it->first] = be_it->second;
        }
    }

    int ans = -1;
    for (map<int, int>::iterator it = tree_edges.begin(); it != tree_edges.end(); it++) {
        ans = max(ans, it->second);
    }
    printf("%d\n", ans);
}