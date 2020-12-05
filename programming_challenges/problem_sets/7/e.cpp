#include <iostream>
#include <cstdio>
#include <climits>
#include <set>
#include <queue>

#define debug 0
#define MAXV 800+5
#define MAXE 10000+5

using namespace std;
typedef long long ll;

int n,m,a,b;

int start[MAXV], succ[MAXE], cap[MAXE], to[MAXE], seen[MAXV];
int edge_counter = 0;
set<int> ssub;
set<int> tsub;

void add_edge(int u, int v, int c) {
    cap[edge_counter] = c;
    to[edge_counter] = v;
    succ[edge_counter] = start[u];
    start[u] = edge_counter;
    ++edge_counter;
}
int inv(int e) { 
    return e ^ 1; 
}

bool augment(int u, int t, int f) {
    if (debug) printf("calling augment with u=%d, t=%d, f=%d\n", u, t, f);
    if (u == t) return true;
    if (seen[u]) return false;
    seen[u] = true;
    if (debug) printf("u is %d, start[u] is %d\n", u, start[u]);
    for (int e = start[u]; ~e; e=succ[e]) {
        if (debug) printf("examining %d\n", e);
        if (cap[e] >= f && augment(to[e], t, f)) {
            cap[e] -= f;
            cap[inv(e)] += f;
            if (debug) printf("augment returning true\n");
            return true;
        }
    }
    if (debug) printf("augment returning false\n");
    return false;
}

int vis[MAXV];
void bfs(int u, set<int>& sub, bool is_reverse) {
    if (debug) printf("running BFS on %d\n", u);
    fill(vis, vis+MAXV, 0);
    queue<int> q;
    q.push(u);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (vis[u]) return;
        vis[u] = 1;
        sub.insert(u);
        if (debug) printf("inserted %d to subset\n", u);
        for (int e = start[u]; ~e; e = succ[e]) {
            if (debug) printf("considering edge %d->%d with cap %d\n", u, to[e], cap[e]);
            if ((is_reverse) ? cap[inv(e)] > 0 : cap[e] > 0) {
                int nxt = to[e];
                if (!vis[nxt]) q.push(nxt);
            }
        }
    }
}

int main() {
    freopen("attack.in", "r", stdin);
    freopen("attack.out", "w", stdout);
    // construct graph
    cin >> n >> m >> a >> b;
    fill(start, start+MAXV, -1);
    for (int i=0; i < m; i++) {
        int c1, c2, cost;
        cin >> c1 >> c2 >> cost;
        if (debug) printf("%d->%d of cost %d\n", c1, c2, cost);
        add_edge(c1, c2, cost);
        if (debug) printf("%d->%d of cost %d\n", c2, c1, cost);
        add_edge(c2, c1, cost);
    }

    // max flow
    while (augment(a, b, 1)) {
        fill(seen, seen + MAXV, 0);
    }

    // now bfs from source and sink respectively, seeing which
    // nodes are part of the cuts
    bfs(a, ssub, false);
    bfs(b, tsub, true);

    // if the cut subsets contain all nodes then there was 
    // a unique cut, otherwise it was possible to cut elsewhere
    if (ssub.size() + tsub.size() == n) {
        printf("UNIQUE\n");
    } else {
        printf("AMBIGUOUS\n");
    }
}