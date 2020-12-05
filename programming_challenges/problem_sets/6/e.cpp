#include <iostream>
#include <cstdio>
#include <set>

#define MAXN 500
#define INF 2e9
#define DEBUG 1

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;

int n, r;
ll m[MAXN][MAXN];
set<ll> sp[MAXN][MAXN]; // p[a][b] = all possible next vertices on shortest path from a->b
int nop[MAXN][MAXN]; // next vertex on shortest path from a to b

// int get_path_length(int u, int v) {
//     int len = 0, at = u;
//     while (at != v) {
//         at = sp[at][v];
//         if (at == INF) return -1;
//         len++;
//     }
//     return len;
// }

int main() {

    cin >> n >> r;
    for (int u=0; u < n; u++) for (int v=0; v < n; v++) {
        m[u][v] = INF;
        nop[u][v] = -1;
    }
    for (int u=0; u < n; u++) {
        m[u][u] = 0;
        nop[u][u] = u;
    }
    for (int i=0; i < r; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        a--; b--;
        m[a][b] = c;
        m[b][a] = c;
        nop[a][b] = b; // shortest path is currently just a->b, hence next is b
        nop[b][a] = a; // same but reverse case
    }

    for (int i=0; i < n; i++) {
        for (int u=0; u < n; u++) {
            for (int v=0; v < n; v++) {
                if (m[u][v] == m[u][i] + m[i][v]) {
                    if (nop[u][i] != u) sp[u][v].insert(nop[u][i]);
                } else if (m[u][v] > m[u][i] + m[i][v]) {
                    m[u][v] = m[u][i] + m[i][v];
                    /* we are now going via i for the shortest path from
                    u->v, hence we need to update the next step vertex
                    on the sp for u->v to be the next step when going 
                    from u->i */
                    sp[u][v].clear();
                    nop[u][v] = nop[u][i];
                    if (nop[u][v] != u) sp[u][v].insert(nop[u][i]);
                }
            }
        }
    }
    // for (int u=0; u < n; u++) for (int v=0; v < n; v++) {
    //     if (u <= v) continue;
    //     int spl = get_path_length(u, v);
    //     if (spl == -1) printf("0 ");
    //     else printf("%d ", spl);
    // }
    for (int u=0; u < n; u++) for (int v=0; v < n; v++) {
        if (u >= v) continue;
        if (sp[u][v].empty()) printf("0 ");
        else {
            if (DEBUG) {
                printf("from %d to %d, the next step verts are; \n", u, v);
                for (auto v : sp[u][v]) {
                    printf("%lld ", v);
                }
                printf("\n");
            }
        }
    }
    printf("\n");
}