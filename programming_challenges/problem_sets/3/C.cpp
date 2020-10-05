#include <iostream>

using namespace std;

#define MAXN 200000 + 100
int ans[MAXN];

int main() {
    int n,k;
    string s;
    cin >> n >> k >> s;
    //cout << n << " " << k << " " << s << endl;

    int prev_router = -1;
    if (n == 1) return 1; // (whether you place a router or wire it, it costs 1)
    for (int i=1; i<=n; i++) {
        // if we are at a routerable room
        if (s[i-1] == '1' ) {
            // if this router isn't in the previous routers signal, take it
            if (prev_router != -1 && prev_router + k < i) {
                ans[i] = ans[i-1] + i;
                prev_router = i;
            // if this router is in the previous routers signal
            } else if (prev_router != -1 && prev_router + k >= i) {
                
            }
        }
    }
}

/*
    - choices for each room are;
    1. if in wifi range of router -> do nothing (cost 0) i.e. ans[i] = ans[i-1]
    else
    2. if no room for router -> wire it (cost i) i.e. ans[i] = ans[i-1] + i
    3. else room for router -> router it (cost i) 
    - routering is better because it covers other rooms
    - note that a later router won't always save you more money (router on room n only gives half of its benefit because it only
    has rooms less than it)

    ans[0] = 0, when you get to a router that you want to use, you can set ans[i] = ans[i-k]
    when you get to a router you want to check whether
 */