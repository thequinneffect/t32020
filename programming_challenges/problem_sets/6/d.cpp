#include <iostream>
#include <cstdio>
#include <climits>

#define MAXN 500
#define MAXE MAXN*MAXN
#define DEBUG 0
using namespace std;
typedef long long ll;

int n; // n=1..500 (number of vertices in graph)
ll all[MAXN][MAXN];
ll sub[MAXN][MAXN];
int deleted[MAXN];
ll ans[MAXN];
int added = 0;

void add_vertex(int v) {
    if (added == 0) {
        added++;
        return;
    }
    for (int i=0; i < added; i++) {
        sub[deleted[i]][v] = all[deleted[i]][v];
        sub[v][deleted[i]] = all[v][deleted[i]];
    }
    added++;
    if (DEBUG) {
        printf("printing sub matrix\n");
        for (int u=0; u < n; u++) {
            for (int v=0; v < n; v++) {
                if (sub[u][v] == LONG_LONG_MAX) printf("inf ");
                else printf("%lld ", sub[u][v]);
            }
            printf("\n");
        }
    }
}

void fw() {

    ll fw[MAXN][MAXN];
    for (int u=0; u < n; u++) for (int v=0; v < n; v++) {
        if (sub[u][v] == -1) fw[u][v] = 2e9;
        else fw[u][v] = sub[u][v];
    }
    for (int u=0; u < n; u++) fw[u][u] = 0;
    for (int i=0; i < n; i++)
        for (int u=0; u < n; u++)
            for (int v=0; v < n; v++) {
                fw[u][v] = min(fw[u][v], fw[u][i] + fw[i][v]);
            }
    // sum shortest paths and add to answer
    ll tot = 0;
    for (int u=0; u < n; u++) for (int v=0; v < n; v++) {
        if (fw[u][v] != 2e9) {
            tot += fw[u][v];
        }
    }
    ans[n-added] = tot;
}

int main() {

    cin >> n;

    // there should be no infinity values and all edges u->u are guaranteed to have a 0 already
    //for (int u=0; u < n; u++) for (int v=0; v < n; v++) dist[u][v] = LONG_LONG_MAX;
    for (int u=0; u < n; u++) for (int v=0; v < n; v++) cin >> all[u][v];

    // now read in the vertices which will be deleted (in reverse order)
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
                printf("%lld ", all[u][v]);
            }
            printf("\n");
        }
        printf("printing deletion list\n");
        for (int i=0; i < n; i++) printf("%d ", deleted[i]);
        printf("\n");
    }

    for (int u=0; u < n; u++) for (int v=0; v < n; v++) sub[u][v] = -1;
    add_vertex(deleted[0]);
    // run FW on each sub-graph
    for (int i=1; i < n; i++) {
        // add the current deleted node and all it's edges to the sub-graph
        add_vertex(deleted[i]);
        fw();
    }
    for (int i=0; i < n; i++) printf("%lld ", ans[i]);
    printf("\n");
}