#include <iostream>
#include <queue>

using namespace std;

#define MAXN 200000 + 100
int ans[MAXN];
queue<int> pq;

int main() {
    int n,k;
    string s;
    cin >> n >> k >> s;
    //cout << n << " " << k << " " << s << endl;
    
    if (n == 1) { cout << "1" << "\n"; return 0; } // (whether you place a router or wire it, it costs 1)
    if (s.find('1') == string::npos) { cout << (n*(n+1))/2 << "\n"; return 0; }
    for (int i=1; i<=n; i++) {
        
        bool done = false;
        // if we are at a routerable room
        if (s[i-1] == '1' ) {
            // check if the looked back to room is already covered by a router
            while (!done && !pq.empty()) {
                int r = pq.front();
                // if it's not in range, pop it
                if (abs((i-(k+1))-r) > k) pq.pop();
                else { 
                    ans[i] = i + r; 
                    done = true; 
                }
            }
            if (!done) ans[i] = i + ans[i-(k+1)];
            pq.push(i);
        } else {
            // if this room is in range of a router, no cost
            while (!done && !pq.empty()) {
                int r = pq.front();
                // if it's not in range, pop it
                if (i-r > k) pq.pop();
                else {
                    ans[i] = ans[i-1];
                    done = true;
                }
            }
            if (!done) ans[i] = ans[i-1] + i;
        }
    }

    // now pick the router that is earliest and cover to the end
    int strlen = s.length();
    for (int i=strlen-k-1; i < strlen; i++) {
        if (s[i] == '1') {
            // this is our answer
            cout << ans[i+1] << endl;
            return 0;
        }
    }
    cout << ans[n] << endl;
}