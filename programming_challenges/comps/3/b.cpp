#include <iostream>
#include <cstdio>
#include <set>

#define MAXN 100000
#define DEBUG 1

using namespace std;
typedef pair<int, int> pii;

int n, m;
pii p[MAXN];
set<int> groups[MAXN];
int best;

int root[MAXN];
int sz[MAXN];
int get_rep(int i) {
    if (root[i] == i) return i;
    return root[i] = get_rep(root[i]);
}

int merge(int a, int b) {
    int x = get_rep(a);
    int y = get_rep(b);
    if (x == y) return 0;
    if (sz[x] < sz[y]) root[x] = y;
    else if (sz[x] > sz[y]) root[y] = x;
    else {
        sz[y]++;
        root[x] = y;
    }
    set<int> res = groups[b];
    for (auto w : groups[a]) {
        res.insert(w);
    }
    groups[a] = res;
    groups[b] = res;
    if (res.find(0) != res.end() && res.size() > best) {
        best = res.size();
    }
    return root[x];
}

void partial_dsu(int from) {
    for (int i = 0; i < n; i++) {
        root[i] = i;
        sz[i] = 1;
        groups[i].clear();
        groups[i].insert(i);
        best = 0;
    }

    for (int i = from; i < m; i++) {
        merge(p[i].first, p[i].second);
    }
}

int main() {

    cin >> n >> m;
    for (int i=0; i<m; i++) {
        int a, b;
        cin >> a >> b;
        a--; b--;
        p[i].first = a; p[i].second = b;
    }

    if (DEBUG) {
        for (int i=0; i<m; i++) {
            int a, b;
            a= p[i].first; b=p[i].second;
            printf("%d %d\n", a, b);
        }
    }

    for (int i=0; i < m; i++) {
        partial_dsu(i);
        printf("ANSWER: %d\n", max(best, 1));
    }
}