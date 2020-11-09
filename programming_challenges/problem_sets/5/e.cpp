#include <cstdio>
#include <iostream>
#include <set>

using namespace std;
typedef pair<int, int> pii;

int n;
set<pii> points;

int main() {

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
                set<pii>::iterator it = points.lower_bound({x+1,y+1});
                (it != points.end()) ? printf("%d %d\n", it->first, it->second) : printf("-1\n");
        } 
    }

}