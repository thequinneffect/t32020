#include <iostream>
#include <queue>
#include <climits>

using namespace std;
typedef long long ll;

#define MAXN 200000 + 100
ll dp[MAXN];
queue<int> pq;

int main() {
    int n,k;
    string s;
    cin >> n >> k >> s;
    //cout << n << " " << k << " " << s << endl;
    
    for (int i = 1; i <= n; i++) {
        // case 1: connect directly
        ll case1 = dp[i-1] + i;

        // case 2: leech of prev router
        ll case2 = LLONG_MAX;
        while (!pq.empty()) {
            int r = pq.front();
            if (r+k < i) pq.pop();
            else {
                int index = (r-k-1 >= 0) ? (r-k-1) : 0; 
                case2 = dp[index] + r;
                break;
            }
        }

        // case 3: put router in this room
        ll case3 = LLONG_MAX;
        if (s[i-1] == '1') {
            int index = (i-k-1 >= 0) ? (i-k-1) : 0;
            case3 = i + dp[index];
        }

        // min of all 3 cases
        dp[i] = min(min(case1, case2), case3);

        // push current router if so
        if (s[i-1] == '1') {
            pq.push(i);
            // check if we need to make a correction
            int prev = -1;
            for (int j=i-1; j > 0; j--) { 
                dp[j] = min(dp[j], dp[i]);
                if (s[j-1] == '1') break;
            }
        }
    }
    cout << dp[n] << endl;
}