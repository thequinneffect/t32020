#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;
typedef long long ll;

ll n,m;

int main() {

    cin >> n >> m;
    if (m > n) {
        // cannot split any magic gems
        printf("1\n");
    } else {
        ll ub = n/m;
        printf("upper bound was %lld\n", ub);
        for (ll i=ub; i>0; i--) {
            ll magic_space = ub*m;
            ll rem_space = n - magic_space;
        }
    }
}