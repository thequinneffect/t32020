#include <iostream>
#include <cstdio>
#include <queue>
#include <vector>

#define MAXN 1000
#define MAXV MAXN*MAXN
using namespace std;
typedef pair<int, int> pii;
typedef pair<int, pii> edge;

int n, r, c;
int maze[MAXN][MAXN];
int seen[MAXN][MAXN], dist[MAXN][MAXN];
priority_queue<edge, vector<edge>, greater<edge>> pq;

bool is_valid(pii e) {
    if (e.first >= 0 && e.first < r && e.second >= 0 && e.second < c) return true;
    return false;
}

void add_edge(pii e, int c) {
    if (is_valid(e) && !seen[e.first][e.second]) pq.push({c + maze[e.first][e.second], e});
}

int main() {

    cin >> n;
    for (int i=0; i < n; i++) {
        cin >> r >> c;
        for (int j=0; j < r; j++) for (int k=0; k < c; k++) cin >> maze[j][k];

        if (r == 1 && c == 1) {
            printf("0\n");
            continue;
        }

        pq.push({0, {0, 0}}); // cost, {r, c}
        while (!pq.empty()) {

            edge e = pq.top();
            pq.pop();
            pii e_dest = e.second;
            int e_cost = e.first;
            if (seen[e_dest.first][e_dest.second]) continue;
            seen[e_dest.first][e_dest.second] = true;
            dist[e_dest.first][e_dest.second] = e_cost;

            // add all 4 neighbours to pq, up, down, left, right
            pii up, down, left, right;
            up = down = left = right = e.second;
            up.first--; down.first++; left.second--; right.second++;
            add_edge(up, e_cost);
            add_edge(down, e_cost);
            add_edge(left, e_cost);
            add_edge(right, e_cost);
        }
        printf("%d\n", dist[r-1][c-1]);
        for (int u=0; u < r; u++) for (int v=0; v < c; v++) seen[u][v] = 0;
    }

}