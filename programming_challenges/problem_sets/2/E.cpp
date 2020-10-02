#include <bits/stdc++.h>

using namespace std;

#define LIMIT 100000 + 10
#define MODNUM 1000000007
typedef pair<int, int> pi;
typedef pair<long long, long long> pll;
typedef long long ll;

ll n, q, f1, f2, a, b;
ll seq[LIMIT];
pll qrs[LIMIT];
ll f[LIMIT];
ll d[LIMIT];

ll troynacci(ll i) {
    if (d[i]) return f[i];
    f[i] = a*troynacci(i-2) + b*troynacci(i-1);
    d[i] = 1;
    return f[i];
}

int main() {
    // handle input
    
    cin >> n >> q >> f1 >> f2 >> a >> b;
    for (int i=1; i <= n; i++) { cin >> seq[i]; seq[i] %= MODNUM; }
    for (int i=0; i < q; i++) cin >> qrs[i].first >> qrs[i].second; 

    // cout << n << " " << q << " " << f1 << " " << f2 << " " << a << " " << b << endl;
    // for (int i=0; i < n; i++) cout << seq[i] << " ";
    // cout << endl;
    // for (int i=0; i < q; i++) cout << qrs[i].first << " " << qrs[i].second << endl; 

    f[1] = f1; f[2] = f2; d[1] = 1; d[2] = 1;

    for (int i=0; i < q; i++) {
        for (ll j=qrs[i].first; j <= qrs[i].second; j++) {
            seq[j] += troynacci(j - qrs[i].first + 1) % MODNUM;
        }
    }

    for (int i=1; i <= n; i++) cout << seq[i] % MODNUM << " ";
    cout << endl;
}