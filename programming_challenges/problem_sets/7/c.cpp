#include <iostream>
#include <cstdio>
#include <climits>
#include <vector>
#include <set>

#define MAXV 50+5
#define MAXE 500+5

using namespace std;
typedef long long ll;
typedef pair<int, double> pii;
typedef pair<double, int> weight_nbears;

ll ncities, nedges, nbears, edge_counter;
ll start[MAXV], succ[MAXE], cap[MAXE], to[MAXE], path[MAXE];
bool seen[MAXV];
vector<int> final_edges;
vector<pii> final_flows;
ll bears_at[MAXE];
multiset<weight_nbears, greater<pair<double, int>>> flows;

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

int main() {

    cin >> ncities >> nedges >> nbears;
    fill(start, start + MAXV, -1);
    for (int i=0; i < nedges; i++) {
        ll a, b, c;
        cin >> a >> b >> c;
        add_edge(a, b, c);
        add_edge(b, a, 0);
    }
    int res = 0;
    fill(seen, seen + MAXV, 0);
    while (augment(1, ncities, 1)) {
        fill(seen, seen + MAXV, 0);
        res += 1;
    }
    printf("%d\n", res);
}