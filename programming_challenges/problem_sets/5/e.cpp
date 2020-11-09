#include <cstdio>
#include <iostream>
#include <set>

using namespace std;
typedef pair<int, int> pii;

int n;

int main() {

    const auto compare = [](const pii &lhs, const pii &rhs) {
        return lhs.first < rhs.first || (lhs.first == rhs.first && lhs.second < rhs.second);
    };

    set<pii, decltype(compare)> points(compare);

    cin >> n;
    for (int i=0; i < n; i++) {
        string s;
        int x, y;
        cin >> s >> x >> y;
        switch (s[0]) {
            case 'a':
                //printf("adding point (%d, %d)\n", x, y);
                points.insert({x,y});
                break;
            case 'r':
                //printf("removing point (%d, %d)\n", x, y);
                points.erase({x,y});
                break;
            case 'f':
                //printf("finding from point (%d, %d)\n", x, y);
                set<pii>::iterator it;
                ((it = points.upper_bound({x+1,y})) != points.end()) ? printf("%d %d\n", it->first, it->second) : printf("-1\n");
        } 
    }

}