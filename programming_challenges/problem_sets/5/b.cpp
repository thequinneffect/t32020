#include <iostream>
#include <cstdio>

#define MAXN 100000
using namespace std;
typedef long long ll;
typedef pair<ll, ll> pll;

struct node {
    ll difficulty;
    ll sum;
    ll max;
    ll min;
    bool increasing;
    bool decreasing;
};

int np,nq;
struct node rt[4*MAXN];

ll query_max(int l, int r, int i=0, int start=0, int end=np) {
    //printf("query max is at rt index %d which has max %lld\n", i, rt[i].max);
    if (l == start && r == end) {
        return rt[i].max;
    }
    int mid = (start + end)/2;
    ll ml = 0, mr = 0;
    if (l < mid) ml = query_max(l, min(r, mid), i*2+1, start, mid);
    if (r > mid) mr = query_max(max(l, mid), r, i*2+2, mid, end);
    return max(ml, mr);
}

ll query_sum(int l, int r, int i=0, int start=0, int end=np) {
    //printf("query max is at rt index %d which has max %lld\n", i, rt[i].max);
    if (l == start && r == end) {
        //printf("sum being returned is %lld\n", rt[i].sum);
        return rt[i].sum;
    }
    int mid = (start + end)/2;
    ll sl = 0, sr = 0;
    if (l < mid) sl = query_sum(l, min(r, mid), i*2+1, start, mid);
    if (r > mid) sr = query_sum(max(l, mid), r, i*2+2, mid, end);
    return sl + sr;
}

pll query_increasing(int l, int r, int i=0, int start=0, int end=np) {
    //printf("query max is at rt index %d which has max %lld\n", i, rt[i].max);
    if (l == start && r == end) {
        if (rt[i].increasing) {
            return {rt[i].min, rt[i].max};
        } else return {0, 0};
    }
    int mid = (start + end)/2;
    pll il = {-1,-1}, ir = {-1,-1};
    if (l < mid) {
        il = query_increasing(l, min(r, mid), i*2+1, start, mid);
    }
    if (r > mid) {
        ir = query_increasing(max(l, mid), r, i*2+2, mid, end);
    }
    if (il.first == 0 && ir.first == 0) return {0,0};
    if (il.first == -1) {
        return ir;
    } else if (ir.first == -1) {
        return il;
    } else if (il.second <= ir.first) {
        return {il.first, ir.second};
    } else {
        return {0,0};
    }
}

pll query_decreasing(int l, int r, int i=0, int start=0, int end=np) {
    //printf("query max is at rt index %d which has max %lld\n", i, rt[i].max);
    //printf("query max is at rt index %d which has max %lld\n", i, rt[i].max);
    if (l == start && r == end) {
        if (rt[i].decreasing) {
            //printf("parent node %d is decreasing, returning {%lld, %lld}\n", i, rt[i].min, rt[i].max);
            return {rt[i].min, rt[i].max};
        } else return {0, 0};
    }
    int mid = (start + end)/2;
    pll il = {-1,-1}, ir = {-1,-1};
    if (l < mid) {
        il = query_decreasing(l, min(r, mid), i*2+1, start, mid);
    }
    if (r > mid) {
        ir = query_decreasing(max(l, mid), r, i*2+2, mid, end);
    }
    if (il.first == 0 && ir.first == 0) return {0,0};
    if (il.first == -1) { // if we didn't visit the left path
        return ir; // return only the result for the right path
    } else if (ir.first == -1) { // likewise for not visiting right
        return il;
    } else if (il.first >= ir.second) { // if we visited both, check the subtrees are decreasing
        //printf("parent node %d is decreasing, returning {%lld, %lld}\n", i, ir.first, il.second);
        return {ir.first, il.second};
    } else {
        return {0,0};
    }
}


void update_node(int i) {
    //printf("updating rt node %d\n", i);
    rt[i].sum = rt[i*2+1].sum + rt[i*2+2].sum;
    rt[i].max = max(rt[i*2+1].max, rt[i*2+2].max);
    rt[i].min = min(rt[i*2+1].min, rt[i*2+2].min);
    rt[i].increasing = (rt[i*2+1].max <= rt[i*2+2].min);
    rt[i].decreasing = (rt[i*2+1].min >= rt[i*2+2].max);
}

// point update
void update(int n, ll v, int i=0, int start=0, int end=np) {
    // if we have found the node, update its difficulty etc.
    //printf("update: at node %d, trying to insert %d\n", i, v);
    if (end - start == 1) {
        //printf("updating rt index %d with difficulty %lld\n", i, v);
        rt[i].difficulty = rt[i].max = rt[i].min = rt[i].sum = v;
        rt[i].increasing = rt[i].decreasing = true;
        return;
    }
    // not at the node yet, so go left or right
    int mid = (start + end)/2;
    if (n < mid) update(n, v, i*2+1, start, mid);
    else update(n, v, i*2+2, mid, end);
    // in the post-order, update the sums etc. back up to the root
    update_node(i);   
}

int main() {

    cin >> np >> nq;

    // set intial values
    for (int i=0; i < np; i++) {
        ll d;
        cin >> d;
        update(i, d);
    }

    for (int i=0; i < nq; i++) {
        char o;
        cin >> o;
        ll x, d;
        cin >> x >> d;
        switch (o) {
            case 'M':
                //printf("max query on range [%lld, %lld]\n", x-1, d-1);
                printf("%lld\n", query_max(x-1, d));
                break;
            case 'S':
                //printf("sum query on range [%lld, %lld]\n", x-1, d-1);
                printf("%lld\n", query_sum(x-1, d));
                break;
            case 'I':
                //printf("increasing on range [%lld, %lld]\n", x-1, d-1);
                printf("%d\n", query_increasing(x-1, d).first > 0 ? 1 : 0); 
                break;
            case 'D':
                //printf("decreasing on range [%lld, %lld]\n", x-1, d-1);
                printf("%d\n", query_decreasing(x-1, d).first > 0 ? 1 : 0);
                break;
            case 'U':
                //printf("updating node %lld to %lld\n", x-1, d);
                update(x-1, d);
        }
    }

}