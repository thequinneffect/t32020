#include <iostream>
#include <cstdio>
#include <vector>
#include <climits>

#define MAXN 100
using namespace std;

struct edge {
    int u,v,w;
    edge(int _u, int _v, int _w) : u(_u), v(_v), w(_w) {} // init list
};

int n;
vector<edge> edges;
vector<int> g[MAXN];
bool seen[MAXN];
vector<int> cycle_rooms;
int energy[MAXN];

void relax() {
    for (auto e : edges) {
        if (energy[e.u] != -INT_MAX && 
            energy[e.v] < energy[e.u] + e.w &&
            energy[e.u] + e.w > 0
        ) {
            energy[e.v] = energy[e.u] + e.w;
        }
    }
}

bool bf() {
    fill(energy, energy+MAXN, -INT_MAX);
    energy[0] = 100;
    for (int i=0; i < n-1; i++) relax();
    bool has_cycle = false;
    for (auto e : edges) {
        if (energy[e.u] != -INT_MAX && 
            energy[e.v] < energy[e.u] + e.w &&
            energy[e.u] + e.w > 0
        ) {
            // e.v would have been updated, hence v is part of a cycle
            cycle_rooms.push_back(e.v);
            has_cycle = true;
        }
    }
    return has_cycle;
}

// dfs to see if we can reach the end from this node
bool can_reach_end(int v) {
    seen[v] = true;
    if (v == n-1) return true;
    for (auto e : g[v]) {
        if (seen[e]) continue;
        if (can_reach_end(e)) return true;
    }
    return false;
}

int main() {

    while (scanf("%d", &n)) {

        if (n == -1) return 0; // reached end of input
        else if (n==1) {
            printf("winnable\n");
            continue;
        }

        for (int i=0; i < n; i++) {
            int e, r;
            cin >> e >> r;
            //printf("read in energy %d and num doors %d\n", e, r);
            energy[i] = e;
            for (int j=0; j < r; j++) {
                int d;
                cin >> d;
                d--; // 0-indexing
                edges.push_back({i, d, -1}); // -1 is temp
                g[i].push_back(d);
            }
        }
        for (int i=0; i < edges.size(); i++) {
            edges[i].w = energy[edges[i].v];
        }
        
        bool has_cycle = bf();

        if (energy[n-1] > 0) printf("winnable\n");
        else if (has_cycle) {
            bool won = false;
            for (int v : cycle_rooms) {
                if (can_reach_end(v)) {
                    printf("winnable\n");
                    won = true;
                    break;
                }
                fill(seen, seen+MAXN, false);
            }
            if (!won) printf("hopeless\n");
        } else {
            printf("hopeless\n");
        }
        // clear stuff
        edges.clear();   
        for (int i=0; i < n; i++) g[i].clear();
        fill(seen, seen+MAXN, false);
        cycle_rooms.clear();
    }

}