#include <iostream>
#include <cstdio>
#include <algorithm>

#define debug 0

using namespace std;
typedef long long ll;

ll n, k;
ll g[100];

int f(int x) {
    return x >= 12;
}
 
int binarysearch() {
    int l = -1; //invariant: f(l) is false
    int r = 31; //invariant: f(r) is true
    while (l < r-1) {
        int m = (l+r)/2;
        if (f(m)) r = m;
        else l = m;
    }
    return r;
}
 
int main () {
    //printf("%d\n", binarysearch());

    cin >> n >> k;
    for (int i=0; i < k; i++) {
        cin >> g[i];
    }
    sort(g, g+k);

    if (debug) {
        for (int i=0; i < k; i++) {
            printf("%lld ", g[i]);
        }
        printf("\n");
    }
    ll b1 = n, b2 = n;
    for (int i=k-1; i >= 0; i--) {
        if (b1 > b2) {
            b1 -= g[i];
        } else {
            b2 -= g[i];
        }
    }
    ll res = min(b1, b2);
    if (res < 0) res = -1;
    printf("%lld\n", res);
}