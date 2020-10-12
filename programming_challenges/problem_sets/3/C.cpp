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
    
    if (n == 1) { cout << "1\n"; return 0; }
    if (s.find('1') == string::npos) { cout << (n*(n+1))/2 << "\n"; return 0; }

    // now calculate ans[i] which denotes the cheapest cost up to room i BUT taking every router
    for (int i=1; i<=n; i++) {
        
        bool done = false;
        if (s[i-1] == '1' ) { // router room
            // check if the looked back to room is already covered by prev router
            int e = ((i-(k+1)) < 0) ? 0 : (i-(k+1));
            while (!done && !pq.empty()) {
                int r = pq.front();
                // if it's not in range, pop it
                if (abs(r - e) > k) pq.pop();
                else {
                    ans[i] = min(i + ans[r], i + ans[i-(k+1)]);
                    done = true; 
                }
            }
            if (!done) ans[i] = (i-(k+1) < 0) ? i : i + ans[i-(k+1)];
            pq.push(i);

        } else { // not a router room
            
            while (!done && !pq.empty()) {
                int r = pq.front();
                if (r+k < i) pq.pop();
                else {
                    ans[i] = ans[i-1]; // in range of a prev router
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

    // now how to get answer from this? can i even lol
}