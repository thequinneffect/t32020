#include <iostream>
#include <cstdio>

#define MAXN 200
#define debug 1

using namespace std;
typedef long long ll;

ll n, m;
double dist[MAXN][MAXN];

int main() {

    for (int u=0; u < MAXN; u++) {
        for (int v=0; v < MAXN; v++) {
            dist[u][v] = 2e9;
        }
    }

    cin >> n >> m;
    for (int i=0; i < m; i++) {
        ll a, b;
        double p;
        cin >> a >> b >> p;
        a--; b--;
        dist[a][b] = p;
        dist[b][a] = p;
    }

    if (debug) {
        for (int u=0; u < n; u++) {
            for (int v=0; v < n; v++) {
                printf("%f ", dist[u][v]);
            }
            printf("\n");
        }
        printf("\n");
    }

    for (int i=0; i < n; i++) {
        for (int u=0; u < n; u++) {
            for (int v=0; v < n; v++) {
                if (u == v) continue;
                printf("dist[%d][%d] = %f, dist[%d][%d] = %f, dist[%d][%d] = %f\n", u,v,dist[u][v], u,i,dist[u][i], i,v,dist[i][v]);
                dist[u][v] = min(dist[u][v], dist[u][i] * dist[i][v]);
            }
        }
    }
            
    for (int u=0; u < n; u++) {
        for (int v=0; v < n; v++) {
            printf("%f ", dist[u][v]);
        }
        printf("\n");
    }
}