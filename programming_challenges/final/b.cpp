#include <iostream>
#include <cstdio>
#include <stack>

#define MAXN (100000 + 5)
#define debug 0

using namespace std;
typedef long long ll;

ll n;
ll nums[MAXN], seen[MAXN];

int main() {

    cin >> n;
    for (int i=0; i < 4*n; i++) {
        cin >> nums[i];
    }
    if (debug) {
        for (int i=0; i < 4*n; i++) {
            printf("%lld ", nums[i]);
        }
        printf("\n");
    }
    // solve
    stack<ll> wires;
    bool possible = true;
    for (int i=0; i < 4*n; i++) {
        if (!seen[nums[i]]) {
            wires.push(nums[i]);
            seen[nums[i]] = 1;
        } else {
            if (wires.top() == nums[i]) {
                wires.pop();
            } else {
                possible = false;
                break;
            }
        }
    }
    if (possible) printf("YES\n");
    else printf("NO\n");
}