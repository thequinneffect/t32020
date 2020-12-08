#include <iostream>
#include <cstdio>
#include <algorithm>
#include <stdint.h>

#define MAXN 100005
#define debug 0
#define m 1000000007

using namespace std;
typedef long long ll;
ll t, n, nums[MAXN], powr[MAXN];

int main() {
    cin >> t;
    powr[0] = 1;
    // need to use a power of 2 array because << can only handle up to
    // 64 bits being shifted
    for (int i=1; i < MAXN; i++) powr[i] = powr[i-1]*2;
    while (t--) {
        cin >> n;
        for (int i=0; i<n; i++) cin >> nums[i];
        sort(nums, nums+n);
        ll res = 0;
        uint64_t one = 1;
        for (int i=n-1; i>=0; i--) res = (res + (nums[i]*powr[i]) % m) % m;
        for (int i=0; i<n; i++) res = (res - ((nums[i]*powr[n-1-i]) % m) + m) % m;
        printf("%lld\n", res);
    }
}