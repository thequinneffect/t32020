#include <bits/stdc++.h>

using namespace std;

#define LIMIT 100000 + 1000
#define MODNUM 1000000007
typedef pair<int, int> pi;
typedef pair<long long, long long> pll;
typedef long long ll;

ll n, q, f1, f2, a, b;
ll seq[LIMIT];
ll propagation[LIMIT];
pll qrs[LIMIT];
ll f[LIMIT];
ll d[LIMIT];

ll troy(ll i) {
    if (d[i]) return f[i];
    f[i] = a*troy(i-2) + b*troy(i-1);
    d[i] = 1;
    return f[i];
}

int main() {

    // handle input
    cin >> n >> q >> f1 >> f2 >> a >> b;
    for (int i=1; i <= n; i++) cin >> seq[i]; 
    for (int i=0; i < q; i++) cin >> qrs[i].first >> qrs[i].second; 

    // cout << n << " " << q << " " << f1 << " " << f2 << " " << a << " " << b << endl;
    // for (int i=1; i <= n; i++) cout << seq[i] << " ";
    // cout << endl;
    // for (int i=0; i < q; i++) cout << qrs[i].first << " " << qrs[i].second << endl; 

    f[1] = f1; f[2] = f2; d[1] = 1; d[2] = 1;

    // now for each query
    for (int i=0; i < q; i++) {
        ll l = qrs[i].first;
        ll r = qrs[i].second;
        if (l == r) {
            // propagation[l-1] = 0 because we don't add anything here and the array is prefilled with 0
            propagation[l] = (propagation[l] + troy(1)) % MODNUM;
            propagation[l+1] = (propagation[l+1] - b*troy(1) + MODNUM) % MODNUM; // and now prop[l+1] += a*prop[l-1] + b*prop[l] i.e. prop[l+1] += 0 + b*troy(1) i.e. net result of prop[l+1] is -b*troy(1)+b*troy(1) == 0
            propagation[l+2] = (propagation[l+2] - a*troy(1) + MODNUM) % MODNUM; // prop[l+2] += a*prop[l] + b*prop[l+1] = a*troy(1) + b*0 (from above) = a*troy(0) i.e. net result of prop[l+2] is -a*troy(1)+a*troy(1) == 0
            // now prop[l+3] and so on will be equal to a*troy(0) + b*troy(0) == 0, and hence the propagation has stopped.
        } else { // need to propagate to troy(2) and so on, not just cancel after adding troy(1)
            // propagation[l-1] = 0, like above
            propagation[l] = (propagation[l] + troy(1)) % MODNUM; // because (i-l+1) = 1 when i=l
            propagation[l+1] = (propagation[l+1] + troy(2) - b*troy(1) + MODNUM) % MODNUM; // -b because prop[l+1] += a*prop[l-1] + b*prop[l] = 0 + b*troy(1), hence we need to -b*troy(1) to not double count it
            // now we have troy(1) and troy(2), troy(3) etc. can be deduced from these if the range is bigger than two
            // but now we need to make sure the propagation stops after r
            // we have some troy(x) where x >= 2, assuming it is 2 then r-l=1, we get, prop[r+1] += a*prop[r-1] + b*prop[r] = a*prop[l] + b*prop[l+1] = a*troy(1) + b*troy(2),
            // which by the definition of troy is the same as troy(3), hence we remove troy(3)
            propagation[r+1] = (propagation[r+1] - 1*troy(r-l+2) + MODNUM) % MODNUM; // in our test case, -troy(3)
            // now for the r+2 position, we need to cancel out a*troy(2), so we do r-1+1 because in our case r-1=1, hence r-1+1=2
            propagation[r+2] = (propagation[r+2] - a*troy(r-l+1) + MODNUM) % MODNUM;
            // now that this works for the case of r = l+1, it should work for all l,r because of how the troy values are generated
        }
    }

    // now perform the propagation
    propagation[2] = (propagation[2] + b*propagation[1]) % MODNUM;
    for (int i=3; i <= n; i++) {
        propagation[i] = ((propagation[i] % MODNUM) + (a*propagation[i-2] % MODNUM) + (b*propagation[i-1] % MODNUM)) % MODNUM;
    }
    // now add the original sequence and print
    for (int i=1; i <= n; i++) {
        seq[i] %= MODNUM;
        seq[i] = seq[i] + propagation[i];
        seq[i] %= MODNUM;
        cout << seq[i] << " ";
    }
    return 0;
}