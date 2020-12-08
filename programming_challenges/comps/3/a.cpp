#include <iostream>
#include <cstdio>
#include <vector>
#include <set>
#include <queue>
#include <climits>

#define MAXN 100000

using namespace std;
typedef pair<int, int> edge; // cost, index
typedef priority_queue<edge, vector<edge>, greater<edge>> PQE;

int n, m, k; // comps, wires, certs
vector<edge> g[MAXN];
set<int> p[MAXN]; 
set<int> r[MAXN]; 
PQE pq;
int seen[MAXN];
int dist[MAXN];
set<int> certs;
int collection_cost[8];

bool has_certs(int v) {
    if (v==0) return true;
    for (auto c : r[v]) {
        auto it = certs.find(c);
        if (it == certs.end()) return false;
    }
    return true;
}

bool dj(int run) {
    fill(seen, seen+MAXN, 0);
    fill(dist, dist+MAXN, -1);
    pq = PQE();
    pq.push({0, 0}); // cost, index : starting at comp 0 with cost 0
    while (!pq.empty()) {
        edge curr = pq.top();
        pq.pop();
        int v = curr.second, v_cost = curr.first;
        if (seen[v]) continue;
        // attach certs it gets at this step
        for (auto c : p[v]) {
            certs.insert(c);
            collection_cost[c] = v_cost;
        }
        //printf("at %d!!\n", v);
        seen[v] = 1;
        dist[v] = v_cost;

        for (auto e : g[v]) {
            int u = e.second, u_cost = e.first;
            bool allowed = has_certs(u);
            if (!seen[u] && allowed) {
                pq.push({u_cost + v_cost, u});
            }
            // else if (!has_certs(u)) {
            //     printf("cant move from %d to %d because of cert\n", v, u);
            // } else {
            //     printf("can't move from %d to %d beacuse of seen\n", v, u);
            // }
        }
    }
    return allowed;
}

int main() {

    cin >> n >> m >> k;
    for (int i=0; i < m; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        a--; b--; // 0-indexing
        g[a].push_back({c, b});
        g[b].push_back({c, a});
    }
    for (int i=0; i < k; i++) {
        int f;
        cin >> f;
        for (int j=0; j < f; j++) {
            int comp;
            cin >> comp;
            comp--; // 0-indexing
            p[comp].insert(i);
        }
    }
    for (int i=0; i < k; i++) {
        int f;
        cin >> f;
        for (int j=0; j < f; j++) {
            int comp;
            cin >> comp;
            comp--;
            r[comp].insert(i);
        }
    }
    for (int i=0; i < k; i++) collection_cost[i] = INT_MAX;
    for (int i=0; i < k; i++) {
        dj(i);
        if (dist[n-1] != -1) {
            printf("%d\n", dist[n-1]);
            break;
        }
        
    }
    
    
}