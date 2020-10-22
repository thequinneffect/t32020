#include <cstdio>
#include <iostream>
#include <vector>
#include <map>

using namespace std;
#define MAXN 5005
typedef pair<int, int> pii;

vector<int> graph[MAXN];
map<pii, int> edge_colours;
vector<pii> output_order;
int active[MAXN];
bool seen[MAXN];
bool has_cycle;

void dfs(int u) {
    if (seen[u]) return;
    seen[u] = true;
    active[u] = true;
    for (int v : graph[u]) {
        if (active[v]) {
            has_cycle = true;
            edge_colours[{u, v}] = 2;
        } else {
            edge_colours[{u, v}] = 1;
            dfs(v);
        }
    }
    active[u] = false;
}

int main() {
    // input
    int n, m;
    cin >> n >> m;
    for (int i=0; i < m; i++) {
        int n1, n2;
        cin >> n1 >> n2;
        graph[n1].push_back(n2);
        output_order.push_back({n1, n2});
    }

    // solve
    has_cycle = false;
    for (auto e : output_order) {
        dfs(e.first);
    }

    (has_cycle) ? printf("2\n") : printf("1\n");
    for (auto e : output_order) {
        printf("%d ", edge_colours[{e.first, e.second}]);
    }
    printf("\n");
}