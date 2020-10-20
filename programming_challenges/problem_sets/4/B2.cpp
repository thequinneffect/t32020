#include <cstdio>
#include <iostream>
#include <map>
#include <set>

using namespace std;

#define MAXN 2005
#define INF 1000000000 + 1
typedef pair<int, int> pii;

multimap<int, pii> edge_mins; 
set<int> nodes_done;

int main() {

    int n, m;
    cin >> n >> m;

    for (int i=0; i < m; i++) {
        int f1, f2, cost;
        cin >> f1 >> f2 >> cost;
        //if (f2 < f1) swap(f1, f2);
        edge_mins.insert({cost, {f1, f2}});
    }

    int max_cost = 0;
    for (multimap<int, pii>::iterator it = edge_mins.begin(); it != edge_mins.end(); it++) {
        if (nodes_done.size() == n) break;
        max_cost = it->first;
        nodes_done.insert(it->second.first);
        nodes_done.insert(it->second.second);
    }
    printf("%d\n", max_cost);
}