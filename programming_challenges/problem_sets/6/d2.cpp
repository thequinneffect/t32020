#include <iostream>
#include <cstdio>
#include <climits>

#define MAXN 500
#define MAXE MAXN*MAXN
#define DEBUG 0
using namespace std;
typedef long long ll;

int n; // n=1..500 (number of vertices in graph)
ll m[MAXN][MAXN];
int deleted[MAXN];
int added[MAXN];
ll ans[MAXN];
int upto = 0;

void fw(ll via) {

    ll tot = 0;
    for (int u=0; u < n; u++)
        for (int v=0; v < n; v++) {
            // see if using the newly added vertex as the via gives an improvement
            m[u][v] = min(m[u][v], m[u][via] + m[via][v]);
            // regargless of improvement or not, if both u and v have be added back to
            // the graph from the deleted pool, then the sp cost between them needs to be included in sum
            if (added[u] && added[v]) tot += m[u][v];
        }
    ans[n-upto] = tot;
}

int main() {

    cin >> n;
    // read in the matrix
    for (int u=0; u < n; u++) for (int v=0; v < n; v++) cin >> m[u][v];

    // now read in the vertices which will be deleted (store in reverse order)
    for (int i=0; i < n; i++) {
        int d;
        cin >> d;
        d--; // 0-indexing
        deleted[n-1-i] = d;
    }

    if (DEBUG) {
        printf("printing adjacency matrix\n");
        for (int u=0; u < n; u++) {
            for (int v=0; v < n; v++) {
                printf("%lld ", m[u][v]);
            }
            printf("\n");
        }
        printf("printing deletion list\n");
        for (int i=0; i < n; i++) printf("%d ", deleted[i]);
        printf("\n");
    }

    // run FW but not for all i as via, just have the newly added vertex as the via
    // that way we get all shortest paths without that vertex before adding it,
    // and all shortest paths with it after adding it
    added[deleted[0]] = 1; // special case for the first one, as there is no possible edges when there is only 1 vertex 
    upto++;
    for (int i=1; i < n; i++) {
        upto++;
        added[deleted[i]] = 1;
        fw(deleted[i]);
    }
    for (int i=0; i < n; i++) printf("%lld ", ans[i]);
    printf("\n");
}