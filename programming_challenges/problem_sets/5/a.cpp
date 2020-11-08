#include <iostream>
#include <cstdio>

#define MAXN 100000

using namespace std;

struct node {
    int lazy;
    int mods[3];
};

int n, q;
struct node rt[4*MAXN];

void recalculate(int id, int l, int r) {
    // if this node has children, then update based on childrens mods
    if (r-l != 0) {
        for (int i=0; i < 3; i++) {
            rt[id].mods[i] = rt[id*2+1].mods[i] + rt[id*2+2].mods[i];
        }
    }
}

/* add to the lazy value of this node, update the mods accordingly */
void update_node(int id, int v, int l, int r) {
    //printf("update node called on %d\n", i);
    v %= 3; // there's no point adding 3 (same as 0), and > 3 is the same as 1 or 2
    if (v == 0) return;
    rt[id].lazy += v;
    rt[id].lazy %= 3;
    int old_mods[3] = {0};
    for (int i=0; i < 3; i++) {
        old_mods[i] = rt[id].mods[i];
    }
    for (int i=0; i < 3; i++) {
        rt[id].mods[(i+v)%3] = old_mods[i];
    }
}

void propagate(int i, int l, int r) {
    if (rt[i].lazy == 0) return;
    //printf("propagating %d\n", rt[i].lazy);
    int mid = (l+r)/2;
    update_node(i*2+1, rt[i].lazy, l, mid); // update left child
    update_node(i*2+2, rt[i].lazy, mid, r); // update right child
    rt[i].lazy = 0; // clear the propagated value
}

void update(int l, int r, int i = 0, int lb = 0, int ub = n) {
    //printf("updating on range %d to %d, looking at %d to %d\n", l, r, lb, ub);
    if (l == lb && r == ub) {
        update_node(i, 1, l, r);
        return;
    }
    propagate(i, lb, ub);
    int mid = (lb + ub) / 2;
    if (l < mid) update(l, min(mid, r), i*2+1, lb, mid);
    if (r > mid) update(max(l, mid), r, i*2+2, mid, ub);
    recalculate(i, lb, ub);
}


int query(int l, int r, int i = 0, int lb = 0, int ub = n) {
    // if the query range matches
    if (l == lb && r == ub) {
        //printf("goes to rt node %d\n", i);
        return rt[i].mods[0]; // return the # of #'s that are div by 3
    }
    // otherwise, we are actually visiting this node on our way to 
    // finding the answer, and so we must propagate the lazy value
    propagate(i, lb, ub);
    int mid = (lb + ub) / 2;
    int ans = 0;
    if (l < mid) ans += query(l, min(mid, r), i*2+1, lb, mid);
    if (r > mid) ans += query(max(l, mid), r, i*2+2, mid, ub);
    return ans;
}

void init_node(int l, int r, int i = 0, int lb = 0, int ub = n) {
    if (l == lb && r == ub) {
        rt[i].mods[0] = 1;
        return;
    }
    int mid = (lb + ub) / 2;
    if (l < mid) init_node(l, min(mid, r), i*2+1, lb, mid);
    if (r > mid) init_node(max(l, mid), r, i*2+2, mid, ub);
    rt[i].mods[0] = rt[i*2+1].mods[0] + rt[i*2+2].mods[0];
}

void print(int i=0) {
    for (int i=0; i < n*2-1; i++) printf("%d(%d, %d, %d, %d) ", i, rt[i].mods[0], rt[i].mods[1], rt[i].mods[2], rt[i].lazy);
    printf("\n");
}

int main() {

    cin >> n >> q;
    for (int i=0; i < n; i++) init_node(i, i+1);
    //print();
    for (int i=0; i < q; i++) {
        int t, a, b;
        cin >> t >> a >> b;
        b++;
        if (t==0) { // update
            //printf("updating %d to %d\n", a, b-1);
            update(a, b);
            //print();
        } else { // query
            //printf("querying %d to %d\n", a, b-1);
            printf("%d\n", query(a, b));
            //print();
        }
    }
}