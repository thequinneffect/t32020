#include <iostream>
#include <cstdio>
#include <vector>

#define debug 0
#define mod 1000000007

using namespace std;
typedef long long ll;

ll n;
vector<ll> k;

vector<ll> rot(vector<ll> a) {
    printf("doing rot\n");
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
    printf("doing mul\n");
    vector<ll> res;
    for (int i=0; i < 5; i++) {
        res.push_back((a[i] * b[i]) % mod);
    }
    return res;
}

vector<ll> combine(vector<ll> a, vector<ll> b) {
    printf("doing combine\n");
    return mul(a, rot(b));
}

vector<ll> combine_n(vector<ll> k, int n) {
    if (n==0) return k;
    return combine(combine_n(k, n-1), k);
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
    auto res = combine_n(k, n);
    for (int i : res) {
        printf("%d ", i);
    }
    printf("\n");
}