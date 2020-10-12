#include <iostream>
#include <queue>
#include <map>

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

    map<int, int> bought;
    // now calculate ans[i] which denotes the cheapest cost up to room i BUT taking each router
    for (int i=1; i<=n; i++) {
        
        bool done = false;
        // if we are at a routerable room
        if (s[i-1] == '1' ) {
            
            while (!done && !pq.empty()) {
                int r = pq.front();
                if (r + k >= i) {
                    ans[i] = ans[r];
                    done = true;
                //} //else if (abs(r - e) > k) {
                   // pq.pop();
                } else { // it is in range, so add that cost
                    pq.pop();
                }
            }
            if (!done) {
                ans[i] = (i-(k+1) < 0) ? i : i + ans[i-(k+1)];
                bought[i]= 1;
            }
            pq.push(i);
        } else {
            
            while (!done && !pq.empty()) {
                
                int r = pq.front();
                if (r+k < i) pq.pop(); // router doesn't reach, useless
                else { // router does reach, see if it is optimal to buy

                    

                    // if (bought[r] == 1) {
                    //     ans[i] = ans[i-1];
                    // } else {
                    //     if ((ans[i-1] + r) < (ans[i-1] + i)) {
                    //         bought[r] = 1; 
                    //     }
                    //     ans[i] = min(ans[i-1] + i, ans[i-1] + r);
                    // }
                    // done = true;
                }
            }
            if (!done) ans[i] = ans[i-1] + i;
        }
    }
}