#include <iostream>
#include <map>

using namespace std;

int n,m;
string s;

// can we type it in <= this cost?
bool can_do(int cost) {
    map<pair<char, char>, int> pairs;
    char prev_char = s[0];
    int running_cost = 0;
    for (int i=1; i < n; i++) {
        if (s[i] == prev_char) continue;
        pairs[{s[i], prev_char}]++;
        if (running_cost > cost) return false;
        prev_char = s[i];
    }
    return true;
}

int main() {
    cin >> n >> m;
    cin >> s;

    int l = -1; //invariant: f(l) is false
    int r = (m-1)*(n-1)+1; //invariant: f(r) is true
    while (l < r-1) {
        int m = (l+r)/2;
        if (can_do(m)) r = m;
        else l = m;
    }
    return r;
}