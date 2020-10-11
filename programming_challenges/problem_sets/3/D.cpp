#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

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
        char c1, c2;
        c1 = (s[i] < prev_char) ? s[i] : prev_char;
        c2 = (c1 == prev_char) ? s[i] : prev_char;
        pairs[{c1,c2}] += 1;
        prev_char = s[i];
    }
    cout << "showing keyboard requirements for " << cost << ";\n";
    for (auto p : pairs) {
        cout << "(" << p.first.first << "," << p.first.second << ")" << " has num occurences = " << p.second << endl; 
    }

    // get the occurences
    int num_pairs = pairs.size();
    vector<int> counts(num_pairs);
    int index = 0;
    for (const auto &p : pairs) {
        counts[index++] = p.second;
    }
    sort(counts.begin(), counts.end(), greater<int>());

    // can have m-1 direct pairs, m-2 off by 1 pairs, m-3 off by 2 pairs etc.
    int batch = m-1;
    int total_cost = 0;
    for (int i=0, b=0; i < num_pairs; i++, b++) {
        if (b == batch) {
            batch--;
            b=0;
        }
        total_cost += counts[i] * (m-batch);
        if (total_cost > cost) return false;
    }
    return true;
}

int main() {
    cin >> n >> m;
    cin >> s;

    int l = -1; //invariant: f(l) is false
    int r = (m-1)*(n-1)+1; //invariant: f(r) is true
    while (l < r-1) {
        int mid = (l+r)/2;
        if (can_do(mid)) {
            r = mid;
        }
        else {
            l = mid;
        } 
    }
    cout << r << endl;
}