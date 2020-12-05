#include <cstdio>
#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <climits>

#define MAXN 200000

using namespace std;
typedef pair<int, int> pii;

int n;
vector<string> orig_s;
vector<pii> orig_p;
vector<pii> compressed_p;
set<int> rt[4*MAXN];
int x_from_rt_index[MAXN];

// we need to find the range it is in, then find the actual leaf node (x) it is in
pii query(int l, int y, int r=n, int i=0, int lb=0, int ub=n) {
    if (ub - lb == 1) {
        auto it = rt[i].lower_bound(y);
        if (it != rt[i].end()) return {x_from_rt_index[i], *it};
        else return {INT_MAX, INT_MAX};
    }
    int mid = (lb + ub)/2;
    pii al, ar;
    al = ar = {INT_MAX, INT_MAX};
    if (l < mid) al = query(l, y, min(mid, r), i*2+1, lb, mid);
    if (r > mid) ar = query(max(mid, l), y, r, i*2+2, mid, ub);
    return min(al, ar);
}

// add to sets x and onwards
void update(int l, int y, int r=n, int i=0, int lb=0, int ub=n, bool add=true) {
    if (l == lb && r == ub) {
        if (add) rt[i].insert(y);
        else rt[i].erase(y);
        return;
    }
    // otherwise continue pushing down the range
    int mid = (lb + ub)/2;
    if (l < mid) update(l, y, min(mid, r), i*2+1, lb, mid, add); 
    else if (r > mid) update(max(l, mid), y, r, i*2+2, mid, ub, add);
    // in the postorder, also add the berry to all ancestors
    if (add) rt[i].insert(y);
    else rt[i].erase(y);
}

int query_leaf(int node, int i=0, int start=0, int end=n) {
    if (end - start == 1) return i;
    // not at the node yet, so go left or right
    int mid = (start + end)/2;
    int res;
    if (n < mid) res = query_leaf(node, i*2+1, start, mid);
    else res = query_leaf(node, i*2+2, mid, end);
    return res;
}

void compress() {
    map<int, int> m;
    for (int i = 0; i < orig_p.size(); i++) {
            m.insert({orig_p[i].first, 0});
            m.insert({orig_p[i].second, 0});
    }

    int upto = 0;
    map<int, int>::iterator it = m.begin();
    while (it != m.end()) {
            m[it->first] = upto;
            upto++;
            it++;
    }

    compressed_p.resize(orig_p.size());
    for (int i = 0; i < orig_p.size(); i++) {
            compressed_p[i].first = m[orig_p[i].first];
            compressed_p[i].second = m[orig_p[i].second];
    }
}

int main() {

    cin >> n;
    for (int i=0; i < n; i++) {
        string s;
        int x, y;
        cin >> s >> x >> y;
        orig_p.push_back({x, y});
        orig_s.push_back(s);

        // switch (s[0]) {
        //     case 'a':
        //         //printf("adding point (%d, %d)\n", x, y);
        //         points.insert({x,y});
        //         break;
        //     case 'r':
        //         //printf("removing point (%d, %d)\n", x, y);
        //         points.erase({x,y});
        //         break;
        //     case 'f':
        //         //printf("finding from point (%d, %d)\n", x, y);
        //         set<pii>::iterator it = points.find_if(points.begin(), points.end(), {x+1,y+1}, [](Pair const& a, Pair const& b) { 
        //             return a.second < b.second; });
        //         (it != points.end()) ? printf("%d %d\n", it->first, it->second) : printf("-1\n");
        // } 
    }
    compress();
    // for (int i=0; i < n; i++) {
    //     printf("(%d, %d) generated from (%d, %d)\n", compressed_p[i].first, compressed_p[i].second, orig_p[i].first, orig_p[i].second);
    // }

    // init x_from_rt_index
    for (int i=0; i < n; i++) {
        int rt_index = query_leaf(i);
        x_from_rt_index[rt_index] = i;
    }

    for (int i=0; i < n; i++) {
        int x = compressed_p[i].first, y = compressed_p[i].second;
        switch (orig_s[i][0]) {
            case 'a':
                printf("adding point (%d, %d)\n", x, y);
                update(x, y, true);
                break;
            case 'r':
                printf("removing point (%d, %d)\n", x, y);
                update(x, y, false);
                break;
            case 'f':
                printf("finding from point (%d, %d)\n", x, y);
                pii res = query(x+1, y+1);
                if (res.first == INT_MAX) printf("-1\n");
                else printf("%d %d\n", res.first, res.second);
        } 
    }
}