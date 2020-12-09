#include <iostream>
#include <cstdio>
#include <vector>

#define debug 0
#define mod 1000000007

using namespace std;
typedef long long ll;

ll n;
vector<ll> k, rhs;

vector<ll> rot(vector<ll> a) {
    //printf("doing rot\n");
    vector<ll> res;
    for (int i=0; i < 5; i++) {
        res.push_back(a[i]);
    }
    for (int i=0; i < 5; i++) {
        res[(i+1)%5] = a[i];
    }
    return res;
}

vector<ll> mul(vector<ll>& a) {
    //printf("doing mul\n");
    vector<ll> res;
    for (int i=0; i < 5; i++) {
        res.push_back((a[i] * rhs[i]) % mod);
    }
    return res;
}

int main() {

    cin >> n;
    for (int i=0; i < 5; i++) {
        ll temp;
        cin >> temp;
        k.push_back(temp);
    }
    rhs = rot(k);
    if (debug) {
        for (int i=0; i < 5; i++) {
            printf("%lld ", k[i]);
        }
        printf("\n");
    }

    vector<ll> res = k;
    for (int i=0; i < n; i++) {
        res = mul(res);
    }
    for (int i : res) {
        printf("%d ", i);
    }
    printf("\n");
}