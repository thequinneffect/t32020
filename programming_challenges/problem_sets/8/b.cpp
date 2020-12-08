#include <iostream>
#include <cstdio>
#include <set>

using namespace std;
typedef long long ll;

ll n;

ll prod_pf(ll x) {
    ll prod = 1;
    set<ll> factors;
    for (ll f = 2; f*f <= x; f++) {
        while (x % f == 0) {
            if (factors.find(f) == factors.end()) {
                factors.insert(f);
                prod *= f;
            }
            x /= f;
        }
    }
    if (x != 1 && factors.find(x) == factors.end()) prod *= x;
    return prod;
}

int main() {
    cin >> n;
    // get the product of prime factors as this is the lowest you
    // can possibly go
    ll m = prod_pf(n);
    // now go from that minimum up to the given number (or overshoot it)
    // by squaring (have to sqaure because the only way to go less was
    // by using roots, so we must use the inverse operation which is ^2)
    // correction: we want to go until it is an integer multiple, not 
    // just equal or bigger i.e. not remainder.
    ll nops = 0, num = m;
    while (num % n != 0) {
        num *= num;
        nops++;
    }
    // if we overshot it (i.e. num > n), then we can just muliply
    // n by some integer x to get num i.e. n*x = num. Then 
    // x = num/n. But we don't actually need this number, we just
    // need to know whether that one division/multiplication would
    // have needed to be done.
    printf("%lld %lld\n", m, (num > n) ? nops+1 : nops);
}