#include <cstdio>
#include <iostream>
#include <climits>
#include <queue>

#define MAXV (100*2+2) // 100 original verts, *2 for in and out versions, +2 for source and sink
#define MAXE MAXV*MAXV
#define DEBUG 0
typedef long long ll;

using namespace std;
ll n, edge_counter;
ll start[MAXV], succ[MAXE], cap[MAXE], to[MAXE], path[MAXE];
bool seen[MAXV];

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
    //printf("calling augment with u=%lld, t=%lld, f=%lld\n", u, t, f);
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

ll augment_ek(int s, int t) {
  // This is a BFS, shortest path means by edge count not capacity
  queue<int> q;
  // path[v] = which edge we used from to reach v
  fill(path, path + MAXE, -1);
  for (q.push(s), path[s] = -2; !q.empty(); q.pop()) {
    int u = q.front();
    for (int e = start[u]; ~e; e = succ[e]) {
      // if we can use e and we haven't already visited v, do it
      if (cap[e] <= 0) continue;
      int v = to[e];
      if (path[v] == -1) {
        path[v] = e;
        q.push(v);
      }
    }
  }
  if (path[t] == -1) return 0; // can't reach the sink
  ll res = INT_MAX;
  // walk over the path backwards to find the minimum edge
  for (int e = path[t]; e != -2; e = path[to[inv(e)]])
    res = min(res, cap[e]);
  // do it again to subtract that from the capacities
  for (int e = path[t]; e != -2; e = path[to[inv(e)]]) {
    cap[e] -= res;
    cap[inv(e)] += res;
  }
  //printf("res is: %lld\n", res);
  return res;
}


ll out(ll i) {
    return i+n;
}


int main() {
    
    while (cin >> n) {

        fill(start, start+MAXV, -1);
        edge_counter = 0;

        // vertices have edges between themeselves to account for
        // vertex limit condition
        for (ll i=1; i <= n; i++) {
            ll v;
            cin >> v;
            add_edge(i, out(i), v);
            add_edge(out(i), i, 0);
        }
        ll e;
        cin >> e;
        for (ll i=1; i <= e; i++) {
            ll u, v, c;
            cin >> u >> v >> c;
            add_edge(out(u), v, c);
            add_edge(v, out(u), 0);
        }
        ll s, t;
        cin >> s >> t;
        for (ll i=1; i <= s; i++) {
            ll u;
            cin >> u;
            add_edge(0, u, INT_MAX);
            add_edge(u, 0, 0);
        }
        for (ll i=1; i <= t; i++) {
            ll u;
            cin >> u;
            add_edge(out(u), 2*n+1, INT_MAX);
            add_edge(2*n+1, out(u), 0);
            // might also need reverse edge here?
        }

        // debug, try print rough graph
        if (DEBUG) {
            for (int i=0; i <= 2*n+1; i++) {
                printf("outgoing edges of vertex %d are: ", i);
                for (ll e = start[i]; ~e; e=succ[e]) {
                    printf("%lld ", e);
                }
                printf("\n");
            }
            printf("FINISHED FOR THIS GRAPH!\n");
        }
        ll res = 0;
        while (true) {
            fill(seen, seen+MAXV, 0);
            ll temp = augment_ek(0, 2*n+1);
            if (temp == 0) break;
            res += temp;
        }
        printf("%lld\n", res);
    }

}