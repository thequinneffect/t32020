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

    // now calculate ans[i] which denotes the cheapest cost up to room i BUT taking each router
    for (int i=1; i<=n; i++) {
        
        bool done = false;
        // if we are at a routerable room
        if (s[i-1] == '1' ) {
            // check if the looked back to room is already covered by prev router
            int e = ((i-(k+1)) < 0) ? 0 : (i-(k+1));
            while (!done && !pq.empty()) {
                int r = pq.front();
                // if it's not in range, pop it
                if (abs(r - e) > k) {
                    pq.pop();
                } else { // it is in range, so add that cost
                    ans[i] = i + ans[r];
                    done = true; 
                }
            }
            if (!done) 
            ans[i] = (i-(k+1) < 0) ? i : i + ans[i-(k+1)];
            pq.push(i);
        } else {
            // if this room is in range of a router, no cost
            while (!done && !pq.empty()) {
                int r = pq.front();
                // if it's not in range, pop it
                if (r+k < i) pq.pop();
                else {
                    ans[i] = ans[i-1];
                    done = true;
                }
            }
            if (!done) ans[i] = ans[i-1] + i;
        }
    }

    cout << "rout: ";
    for (int i=0; i < n; i++) {
        if (ans[i+1] > 9) cout << s[i] << "  ";
        else cout << s[i] << " ";
    } 
    cout << endl;
    cout << "cost: ";
    for (int i=0; i < n; i++) {
        if (ans[i+1] > 9 && i+1 < 10) cout << i+1 << "  ";
        else cout << i+1 << " ";
    } 
    cout << endl;
    cout << "ansr: ";
    for (int i=1; i <= n; i++) cout << ans[i] << " ";
    cout << endl;

    // now pick the router that is earliest and cover to the end
    // int strlen = s.length();
    // for (int i=strlen-k-1; i < strlen; i++) {
    //     if (s[i] == '1') {
    //         // this is our answer
    //         cout << ans[i+1] << endl;
    //         return 0;
    //     }
    // }
    int res = 0;
    for (int i = 1; i <= n; i++) {
        // check if we want to take this router, or if there is a later one that is better
        if (s[i-1] == '1') {
            
        }
    }
    cout << res << endl;
}