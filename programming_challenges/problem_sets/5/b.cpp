#include <iostream>
#include <cstdio>

#define MAXN 100000
typedef long long ll;

using namespace std;

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

void update_node(int i) {
    //printf("updating rt node %d\n", i);
    rt[i].sum = rt[i*2+1].sum + rt[i*2+2].sum;
    rt[i].max = max(rt[i*2+1].max, rt[i*2+2].max);
    rt[i].min = min(rt[i*2+1].min, rt[i*2+2].min);
    rt[i].increasing = (rt[i*2+1].max <= rt[i*2+2].min);
    rt[i].decreasing = (rt[i*2+1].min >= rt[i*2+2].max);
}

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

ll query_increasing(int l, int r, int i=0, int start=0, int end=np) {
    //printf("query max is at rt index %d which has max %lld\n", i, rt[i].max);
    if (l == start && r == end) {
        // just one number is always considered increasing
        if (r - l == 1) return 1;
        return (rt[i].increasing) ? 1 : 0;
    }
    int mid = (start + end)/2;
    bool il = 0, ir = 0;
    if (l < mid) il = query_increasing(l, min(r, mid), i*2+1, start, mid);
    if (r > mid) ir = query_increasing(max(l, mid), r, i*2+2, mid, end);
    return (
        il == 1 && ir == 1 &&
        (rt[i*2+1].max <= rt[i*2+2].min)
    ) ? 1 : 0;
}

ll query_decreasing(int l, int r, int i=0, int start=0, int end=np) {
    //printf("query max is at rt index %d which has max %lld\n", i, rt[i].max);
    if (l == start && r == end) {
        // just one number is always considered increasing
        if (r - l == 1) return 1;
        return rt[i].decreasing ? 1 : 0;
    }
    int mid = (start + end)/2;
    bool dl = 0, dr = 0;
    if (l < mid) dl = query_decreasing(l, min(r, mid), i*2+1, start, mid);
    if (r > mid) dr = query_decreasing(max(l, mid), r, i*2+2, mid, end);
    return (
        dl == 1 && dr == 1 &&
        (rt[i*2+1].min >= rt[i*2+2].max)
    ) ? 1 : 0;
}

// point update
void update(int n, ll v, int i=0, int start=0, int end=np) {
    // if we have found the node, update its difficulty
    //printf("update: at node %d, trying to insert %d\n", i, v);
    if (end - start == 1) {
        //printf("updating rt index %d with difficulty %lld\n", i, v);
        rt[i].difficulty = rt[i].max = rt[i].min = rt[i].sum = v;
        rt[i].increasing = true; rt[i].decreasing = true;
        return;
    }
    // not at the node yet, so go left or right
    int mid = (start + end)/2;
    if (n < mid) update(n, v, i*2+1, start, mid);
    else update(n, v, i*2+2, mid, end);
    // in the post-order, update the sums back up to the root
    update_node(i);   
}

int main() {
    // TODO: IMPORTANT, ADD IO SPEEDUP SETTINGS HERE

    cin >> np >> nq;

    for (int i=0; i < np; i++) {
        ll d;
        cin >> d;
        //printf("array index %d corresponds with ", i);
        update(i, d);
    }

    for (int i=0; i < nq; i++) {
        char o;
        cin >> o;
        ll x, d;
        cin >> x >> d;
        switch (o) {
            case 'M':
                printf("max query on range [%lld, %lld]\n", x-1, d-1);
                printf("%lld\n", query_max(x-1, d));
                break;
            case 'S':
                printf("sum query on range [%lld, %lld]\n", x-1, d-1);
                printf("%lld\n", query_sum(x-1, d));
                break;
            case 'I':
                printf("increasing on range [%lld, %lld]\n", x-1, d-1);
                printf("%lld\n", query_increasing(x-1, d)); 
                break;
            case 'D':
                printf("decreasing on range [%lld, %lld]\n", x-1, d-1);
                printf("%lld\n", query_decreasing(x-1, d));
                break;
            case 'U':
                printf("updating node %lld to %lld\n", x-1, d);
                update(x-1, d);
        }
    }

}