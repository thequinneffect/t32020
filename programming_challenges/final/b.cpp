#include <iostream>
#include <cstdio>
#include <stack>

#define MAXN 100000+5

using namespace std;
typedef long long ll;

ll n;
ll nums[4*MAXN];
int seen[2*MAXN];
stack<ll> wires;

int main() {

    cin >> n;
    for (int i=0; i < 4*n; i++) {
        cin >> nums[i];
    }
    for (int i=0; i < 4*n; i++) {
        if (!seen[nums[i]]) {
            wires.push(nums[i]);
            seen[nums[i]] = 1;
        } else {
            if (wires.top() == nums[i]) {
                wires.pop();
            } else {
                printf("NO\n");
                return 0;
            }
        }
    }
    printf("YES\n");
}