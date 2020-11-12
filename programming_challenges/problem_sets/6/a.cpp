#include <iostream>
#include <cstdio>
#include <queue>
#include <vector>

#define MAXN 1000
#define MAXV MAXN*MAXN
using namespace std;
typedef pair<int, int> pii;

int n, r, c;
int maze[MAXV];
int seen[MAXV], dist[MAXV];
priority_queue<pii, vector<pii>, greater<pii>> pq;

int main() {

    cin >> n;
    for (int i=0; i < n; i++) {
        cin >> r >> c;
        for (int j=0; j < r*c; j++) cin >> maze[j];

        if (r == 1 && c == 1) {
            printf("%d\n", 0);
            continue;
        }

        pq.push({0, 0}); // cost, index
        while (!pq.empty()) {

            pii curr_e = pq.top();
            pq.pop();
            int v = curr_e.second, d = curr_e.first;
            if (seen[v]) continue;
            seen[v] = true;
            dist[v] = d;

            // add all 4 neighbours to pq
            if (r > 1 && v-c >= 0 && !seen[v-c]) pq.push({maze[v-c]+d, v-c});
            if (r > 1 && v+c < r*c && !seen[v+c]) pq.push({maze[v+c]+d, v+c});
            if (c > 1 && v%c != 0 && !seen[v-1]) pq.push({maze[v-1]+d, v-1});
            if (c > 1 && v%c != c-1 && !seen[v+1]) pq.push({maze[v+1]+d, v+1});
        }
        printf("%d\n", dist[(r*c)-1]);
        fill(seen, seen+MAXV, false);
    }

}

// I think I need to have maze[] start from index 1 and not 0 because
// i am manually passing in 0 as the start and thus it is already seen