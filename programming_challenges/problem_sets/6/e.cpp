#include <iostream>
#include <cstdio>

#define MAXN 500
#define INF 2e9

using namespace std;
typedef long long ll;

int n, r;
ll m[MAXN][MAXN];
ll p[MAXN][MAXN]; // p[a][b] = next vertex on shortest path from a->b

int get_path_length(int u, int v) {
    int len = 0, at = u;
    while (at != v) {
        at = p[at][v];
        if (at == INF) return -1;
        len++;
    }
    return len;
}

int main() {

    cin >> n >> r;
    for (int u=0; u < n; u++) for (int v=0; v < n; v++) {
        m[u][v] = INF;
        p[u][v] = INF;
    }
    for (int u=0; u < n; u++) m[u][u] = 0;
    for (int i=0; i < r; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        a--; b--;
        m[a][b] = c;
        m[b][a] = c;
        p[a][b] = b; // path is currently just a->b, hence next is b
        p[b][a] = a; // same but reverse case
    }

    for (int i=0; i < n; i++) {
        for (int u=0; u < n; u++) {
            for (int v=0; v < n; v++) {
                if (m[u][v] > m[u][i] + m[i][v]) {
                    m[u][v] = m[u][i] + m[i][v];
                    /* we are now going via i for the shortest path from
                    u->v, hence we need to update the next step vertex
                    on the sp for u->v to be the next step when going 
                    from u->i */
                    p[u][v] = p[u][i];
                }
            }
        }
    }
    for (int u=0; u < n; u++) for (int v=0; v < n; v++) {
        if (u <= v) continue;
        int spl = get_path_length(u, v);
        if (spl == -1) printf("0 ");
        else printf("%d ", spl);
    }
    printf("\n");
}