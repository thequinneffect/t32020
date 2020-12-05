#include <iostream>
#include <cstdio>

#define MAXV 600
#define MAXE MAXV*MAXV
#define DEBUG 0

using namespace std;
typedef long long ll;
int n;
ll start[MAXV], succ[MAXE], cap[MAXE], to[MAXE];
bool seen[MAXV];
int edge_counter = 0;
char m[MAXV][MAXV];

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
    if (DEBUG) printf("calling augment with u=%lld, t=%lld, f=%lld\n", u, t, f);
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

bool is_valid(int r, int c) {
    if (r >= 0 && r < n && c >= 0 && c < n) return true;
    return false;
}
bool is_oil_spill(int r, int c) {
    if (m[r][c] == '#') return true;
    return false;
}

void add_both(int a, int b) {
    if (DEBUG) printf("adding edge between nodes %d and %d\n", a, b);
    add_edge(a, b, 1);
    add_edge(b, a, 0);
}

int cart_to_index(int r, int c) {
    return r*n+c+1;
}

void add_bipartite_edges(int r, int c) {
    int from = cart_to_index(r, c);
    if (is_valid(r-1, c) && is_oil_spill(r-1, c)) add_both(from, cart_to_index(r-1, c));
    if (is_valid(r+1, c) && is_oil_spill(r+1, c)) add_both(from, cart_to_index(r+1, c));
    if (is_valid(r, c-1) && is_oil_spill(r, c-1)) add_both(from, cart_to_index(r, c-1));
    if (is_valid(r, c+1) && is_oil_spill(r, c+1)) add_both(from, cart_to_index(r, c+1));
}

int main() {

    int ncases, curr = 0;
    cin >> ncases;
    while (ncases--) {
        fill(start, start+MAXV, -1);
        fill(succ, succ+MAXE, 0);
        fill(cap, cap+MAXE, 0);
        fill(to, to+MAXE, 0);
        edge_counter = 0;
        cin >> n;
        int sink = n*n+1;
        
        for (int r=0; r < n; r++) {
            for (int c=0; c < n; c++) {
                int node = r*n+c + 1;
                if ((r+c)%2==0) {
                    // this node is in the even set
                    // make an edge from source to here
                    add_both(0, node);
                } else {
                    // this node is in the odd set
                    // make an edge from here to the sink
                    add_both(node, sink);
                }
                cin >> m[r][c];
            }
        }
        // now add edges between adjacent oil spills
        for (int r=0; r < n; r++) {
            for (int c=0; c < n; c++) {
                if ((r+c)%2!=0 || m[r][c] == '.') continue;
                // oil spill on an even square, add edges
                add_bipartite_edges(r, c);
            }
        }
        if (DEBUG) {
            for (int r=0; r < n; r++) {
                for (int c=0; c < n; c++)
                    printf("%c ", m[r][c]);
                printf("\n");
            }
        }
        int res = 0;
        fill(seen, seen + MAXV, 0);
        while (augment(0, sink, 1)) {
            fill(seen, seen + MAXV, 0);
            res += 1;
        }
        printf("Case %d: %d\n", ++curr, res);
    }
    
}

/*
    src node is index 0
    nodes are 1..n*n
    index n*n+1 is the sink
*/