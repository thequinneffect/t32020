#include <iostream>
#include <cstdio>
#include <set>
#include <queue>

#define MAXV 800+5
#define MAXE 10000+5

using namespace std;
typedef long long ll;

ll n,m,a,b;
ll start[MAXV], succ[MAXE], cap[MAXE], to[MAXE], seen[MAXV];
ll edge_counter = 0;
set<ll> ssub;
set<ll> tsub;

void add_edge(ll u, ll v, ll c) {
    cap[edge_counter] = c;
    to[edge_counter] = v;
    succ[edge_counter] = start[u];
    start[u] = edge_counter;
    ++edge_counter;
}
ll inv(ll e) { 
    return e ^ 1; 
}

bool augment(ll u, ll t, ll f) {
    if (u == t) return true;
    if (seen[u]) return false;
    seen[u] = true;
    for (ll e = start[u]; ~e; e=succ[e]) {
        if (cap[e] >= f && augment(to[e], t, f)) {
            cap[e] -= f;
            cap[inv(e)] += f;
            return true;
        }
    }
    return false;
}

void bfs(ll u, set<ll>& sub, bool is_reverse) {
    fill(seen, seen+MAXV, 0);
    queue<ll> q;
    q.push(u);
    while (!q.empty()) {
        ll u = q.front(); q.pop();
        if (seen[u]) return;
        seen[u] = 1;
        sub.insert(u);
        for (ll e = start[u]; ~e; e = succ[e]) {
            if ((is_reverse) ? cap[inv(e)] > 0 : cap[e] > 0) {
                ll nxt = to[e];
                if (!seen[nxt]) q.push(nxt);
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
    for (ll i=0; i < m; i++) {
        ll c1, c2, cost;
        cin >> c1 >> c2 >> cost;
        add_edge(c1, c2, cost);
        add_edge(c2, c1, cost);
    }

    // max flow
    while (augment(a, b, 1)) fill(seen, seen + MAXV, 0);

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