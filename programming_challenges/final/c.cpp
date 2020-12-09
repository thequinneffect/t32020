#include <iostream>
#include <cstdio>
#include <map>
#include <vector>

#define MAXN 100000+5
#define debug 0

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;

int root[MAXN];
int sz[MAXN];
int getRoot(int i) {
    if (root[i] == i) return i;
    root[i] = getRoot(root[i]);
    return root[i];
}
void merge(int x, int y) {
    x = getRoot(x);
    y = getRoot(y);
    if (x == y) return;
    if (sz[x] < sz[y]) root[x] = y;
    else if (sz[x] > sz[y]) root[y] = x;
    else {
        root[y] = x;
        sz[x]++;
    }
}

int n,m,p, max_level = 0;
map<int, vector<pii>> e;
int loc[MAXN];

bool check() {
    if (debug) printf("running check!\n");
    int first = getRoot(loc[0]);
    if (debug) printf("first is %d\n", first);
    for (int i=1; i < p; i++) {
        if (debug) printf("getRoot(%d) is %d\n", i, getRoot(i));
        if (getRoot(loc[i]) != first) return false;
    }
    return true;
}

int main() {

    // input
    cin >> n >> m >> p;
    for (int i=0; i < m; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        e[c].push_back({a,b});
        if (c > max_level) max_level = c;
    }
    for (int i=0; i < p; i++) {
        cin >> loc[i];
    }

    if (debug) {
        printf("locs are;\n");
        for (int i=0; i < p; i++) printf("%d ", loc[i]);
        printf("\n");
    }

    // init dsu size etc.
    for (int i=0; i < MAXN; i++) {
        sz[i] = 1;
        root[i] = i;
    }
    // see if it can be done without any
    if (check()) {
        printf("0\n");
        return 0;
    }

    for (int i=1; i <= max_level; i++) {

        // add edges that current pass i allows
        for (auto edge : e[i]) {
            merge(edge.first, edge.second);
        }

        // now see if done
        if (check()) {
            printf("%d\n", i);
            return 0;
        }
    }
}