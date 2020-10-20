/* Restructing Company
https://codeforces.com/problemset/problem/566/D
 
Keep a set of ranges, represented by their leader.
On top of this, keep a DSU.
- If type 1, merge two leaders using DSU, and leave the set alone.
    - O(logN + alpha(N))
- If type 2, merge the range from [leader(x)+1, leader(y)] into leader(x), and then delete them.
    - amortised O(NlogN)
- If type 3, find the leaders of x and y, and then check DSU to see if they are in the same team.
    - O(logN + alpha(N))
final complexity: O(QlogN + NlogN)
*/
#include <cstdio>
#include <iostream>
#include <set>
using namespace std;
 
#define MAXN 200005
 
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
    if (sz[x] < sz[y]) root[x] = y;
    else if (sz[x] > sz[y]) root[y] = x;
    else {
        root[y] = x;
        sz[x]++;
    }
}
 
int main () {
    ios_base::sync_with_stdio(0); cin.tie(0);
 
    int N, Q;
    cin >> N >> Q;
 
    set<int> s;
 
    for (int i = 0; i < MAXN; i++) root[i] = i;
    for (int i = 0; i < MAXN; i++) sz[i] = 1;
    for (int i = 1; i <= N; i++) s.insert(i);
 
    for (int i = 0; i < Q; i++) {
        int t, x, y;
        cin >> t >> x >> y;
        if (t == 1) {
            // merge the two ranges in dsu
            int lx = *(--s.upper_bound(x));
            int ly = *(--s.upper_bound(y));
            merge(lx, ly);
        } else if (t == 2) {
            // merge and erase all leaders from x+1 to y.
            auto lx = --s.upper_bound(x);
            auto ly = --s.upper_bound(y);
            if (lx == ly) continue;
            auto it = next(lx);
            while (it != ly) {
                merge(*it, *lx);
                s.erase(it++);
            }
            merge(*ly, *lx);
            s.erase(ly);
 
        } else if (t == 3) {
            // check to see if leaders are merged in dsu
            int lx = *(--s.upper_bound(x));
            int ly = *(--s.upper_bound(y));
            if (getRoot(lx) == getRoot(ly)) printf("YES\n");
            else printf("NO\n");
        }
    }
 
}