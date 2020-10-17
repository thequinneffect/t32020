#include <iostream>

using namespace std;
typedef long long ll;

#define MAXN 5005

ll values[MAXN];
ll best[MAXN];

int main() {

    int n;
    cin >> n;
    for (int i=1; i <= n; i++) cin >> values[i];

    // for (int i=1; i <= n; i++) cout << values[i] << ", ";
    // cout << endl;

    // for size 1, can only take the first box
    best[1] = values[1];

    // loop through the rest of the boxes
    for (int i=2; i < n; i++) {

        // for each box, you can either use it or not

        // case 1: use the box, in which case you want to also
        // fill it with the highest possible size that will
        // fit in it
        ll case1  = values[i] + best[i-1];

        // case 2: don't use the box. In this case you want to
        // make up the size with the best possible way
        ll case2 = best[i-1] + best[1];

        best[i] = max(case1, case2);

    }

    cout << best[n-1] + values[n] << endl;
}