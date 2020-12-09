#include <iostream>
#include <cstdio>
#include <map>
#include <vector>
#include <set>

#define MAXN 100000+5
#define debug 0

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;


int n,m,p, max_level = 0, nsubsets = 0;
map<int, vector<pii>> e;
int loc[MAXN], seen[MAXN];
map<int, set<int>> sets;

int root[MAXN];
int sz[MAXN];
int getRoot(int i) {
    if (root[i] == i) return i;
    root[i] = getRoot(root[i]);
    return root[i];
}
void merge(int x, int y) {
    // if x and y are part cities
    int sx = x, sy = y;
    bool useful_subset = false;
    if (seen[x] && seen[y]) useful_subset = true;
    x = getRoot(x);
    y = getRoot(y);
    if (x == y) return;
    if (useful_subset) nsubsets--;
    if (useful_subset && debug) printf("nsubsets changed from %d to %d\n", nsubsets+1, nsubsets);
    if (sz[x] < sz[y]) {
        root[x] = y;
        sets[y].insert(x);
    } else if (sz[x] > sz[y]) {
        root[y] = x;
        sets[x].insert(y);
    } else {
        root[y] = x;
        sz[x]++;
        sets[x].insert(y);
    }
    // possibly just insert to both?
}


bool check() {
    if (debug) printf("running check! nsubsets currently is %d\n", nsubsets);
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
    int unique_locs = 0;
    for (int i=0, j=0; i < p; i++) {
        int c;
        cin >> c;
        if (!seen[c]) {
            loc[j] = c;
            j++, unique_locs++;
            seen[c] = 1;
        }
    }
    p = unique_locs;
    nsubsets = p;
    for (int i=0; i < p; i++) {
        sets[loc[i]].insert(loc[i]);
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
        if (check()) {
            printf("%d\n", i);
            return 0;
        }
    }
}