#include <bits/stdc++.h>

using namespace std;

int main() {
    int n,k;
    string s;
    cin >> n >> k >> s;
    //cout << n << " " << k << " " << s << endl;

    int wire_all_cost = n(n+1)/2;

    for (int i=1; i<=n; i++) {
        
    }
}

/*
    - let f(n) denote the min cost for getting internet for n rooms
    - base case f(1) = 1 (whether you place a router or wire it, it costs 1)
    - recursion: f(n) = min()

    - choices for each room are;
    1. do nothing if it is in wifi range of a router (cost 0) i.e. f(n) = f(n-1) if in wifi range
    2. wire it (cost i)
    3. router it (cost i)
    - note that a later router won't always save you more money (router on room n only gives half of its benefit because it only
    has rooms less than it)

 */