#include <iostream>
#include <cstdio>
#include <vector>

#define debug 1
#define mod 1000000007

using namespace std;
typedef long long ll;

ll n;
vector<ll> k;

vector<ll> rot(vector<ll> a) {
    vector<ll> res;
    for (int i=0; i < 5; i++) {
        res.push_back(a[i]);
    }
    for (int i=0; i < 5; i++) {
        res[(i+1)%5] = a[i];
    }
    return res;
}

vector<ll> mul(vector<ll> a, vector<ll> b) {
    vector<ll> res;
    for (int i=0; i < 5; i++) {
        res.push_back((a[i] * b[i]) % mod);
    }
    return res;
}

vector<ll> combine(vector<ll> a, vector<ll> b) {
    return mul(a, rot(b));
}

vector<ll> solve(int n, vector<ll> k) {
    if (n==0) return k;
    solve(n-1, solve(n-1, k));
}

int main() {

    cin >> n;
    for (int i=0; i < 5; i++) {
        ll temp;
        cin >> temp;
        k.push_back(temp);
    }
    if (debug) {
        for (int i=0; i < 5; i++) {
            printf("%lld ", k[i]);
        }
        printf("\n");
    }


}