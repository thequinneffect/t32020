#include <iostream>
#include <cstdio>
#include <vector>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

#define MAXN 200000

using namespace __gnu_pbds;
using namespace std;

typedef tree<int, null_type, less<int>, rb_tree_tag,
    tree_order_statistics_node_update> ordered_set;

vector<int> graph[MAXN + 5];
int seen[MAXN + 5];
int rt_index[MAXN+5], rt_begin[MAXN+5], rt_end[MAXN+5];
ordered_set rt[4*MAXN + 5];
int totId;
int n, q;

void dfs(int n = 0) {
    if (seen[n]) return;
    seen[n] = 1;
    //printf("%d ", n);
    rt_index[n] = rt_begin[n] = totId++;
    for (auto c : graph[n]) {
        dfs(c);
    }
    rt_end[n] = totId;
}

// find the range, then simply ask for order_of_key among
// the range(s), summing them on return
int query(int l, int r, int berry, int i=0, int lb=0, int ub=n) {
    // found the range, return the order of key
    if (l == lb && r == ub) {
        return rt[i].order_of_key(berry);
    }
    int mid = (lb + ub)/2;
    int ans = 0;
    if (l < mid) ans += query(l, min(r, mid), berry, i*2+1, lb, mid);
    if (r > mid) ans += query(max(mid, l), r, berry, i*2+2, mid, ub);
    return ans;
}

// update ost at index i in rt with berry i, also all ancestors
// in the postorder
void update(int l, int r, int berry, int i=0, int lb=0, int ub=n) {
    // if we have found the range to update, then add the berry
    if (l == lb && r == ub) {
        rt[i].insert(berry);
        return;
    }
    // otherwise continue pushing down the range
    int mid = (lb + ub)/2;
    if (l < mid) update(l, min(r, mid), berry, i*2+1, lb, mid); 
    else if (r > mid) update(max(l, mid), r, berry, i*2+2, mid, ub);
    // in the postorder, also add the berry to all ancestors
    rt[i].insert(berry);

}

void print() {
    // print out the current range tree
    vector<int> rt_berries[MAXN];
    for (int i=0; i < n; i++) {
        for (auto berry : rt[rt_index[i]]) {
            rt_berries[rt_index[i]].push_back(berry);
        }
    }
    for (int i=0; i < n; i++) {
        printf("%d: ", i);
        for (int berry : rt_berries[i]) {
            printf("%d ", berry);
        }
        printf("\n");
    }       
    printf("\n");
}

int main() {

    cin >> n >> q;

    vector<int> initial_berries;
    for (int i=0; i < n; i++) {
        int s;
        cin >> s;
        initial_berries.push_back(s);
    }

    for (int i=0; i < n-1; i++) {
        int a, b;
        cin >> a >> b;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }
    dfs();
    // printf("\n0 1 2 3 4 5 6\n");
    // for (int i=0; i < n; i++) {
    //     printf("range tree index of node %d is %d, begin and end are %d and %d\n", i, rt_index[i], rt_begin[i], rt_end[i]);
    // }

    for (int i=0; i < n; i++) {
        update(rt_begin[i], rt_end[i], initial_berries[i]);
    }

    //print();

    for (int i=0; i < q; i++) {
        int a, b;
        cin >> a >> b;
        update(rt_begin[a], rt_end[a], b);
        printf("%d\n", query(rt_begin[a], rt_end[a], b));
        //print();
    }

}

/*

    Update: find the node n, add the mulberry to its set, then upon
    returning back up the rt, also add that mulberry to every 
    ancestor along the way. We can afford to do this because insertion
    into the ost is logn

    Query: it is enough to find the range and return order_of_key(k)
    as we are adding all new mulberries to all ancestors as well
    as the node they grow at, meaning for a query at node n we have
    stored all mulberries at this node that are underneath us as well.
    order_of_key(k) is logn, hence we can do it in time.

 */