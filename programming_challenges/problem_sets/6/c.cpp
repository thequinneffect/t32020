#include <iostream>
#include <cstdio>
#include <vector>
#include <queue>
#include <tuple>
#include <set>

#define MAXN 200000 + 5
#define DEBUG 0

using namespace std;
typedef tuple<int, int, int, int> edge; // cost, is_train, at, from

// num cities (2..200000), num roads (1..300000), num trains (1..100000)
int n, r, t;
vector<edge> g[MAXN];
priority_queue<edge, vector<edge>, greater<edge>> pq;
int seen[MAXN];
multiset<pair<int, int>> trains;

int main() {

    cin >> n >> r >> t;
    for (int i=0; i < r; i++) { // read in roads
        int a, b, c;
        cin >> a >> b >> c;
        g[a].push_back({c, 0, b, a});
        g[b].push_back({c, 0, a, b});
        if (DEBUG) pq.push({c, 0, b, a});
        if (DEBUG) pq.push({c, 0, a, b});
    }
    for (int i=0; i < t; i++) {
        int a, b;
        cin >> a >> b;
        g[1].push_back({b, 1, a, 1});
        g[a].push_back({b, 1, 1, a});
        if (DEBUG) pq.push({b, 1, a, 1});
        if (DEBUG) pq.push({b, 1, 1, a});
        trains.insert({min(1, a), max(1, a)});
    }

    if (DEBUG) {
        printf("printing out pq order\n");
        while (!pq.empty()) {
            edge e = pq.top();
            pq.pop();
            int e_cost = get<0>(e), e_train = get<1>(e), e_index = get<2>(e);
            printf("cost: %d, is_train: %d, id: %d\n", e_cost, e_train, e_index);
        }
        return 0;
    }
    
    //printf("trains.size is %ld\n", trains.size());
    pq.push({0, 0, 1, 1}); // cost, is_train, index (i.e. start at capital with 0 cost)
    while (!pq.empty()) {

        edge curr = pq.top();
        pq.pop();
        int at_cost = get<0>(curr), is_train = get<1>(curr), at = get<2>(curr), from = get<3>(curr);
        if (seen[at]) continue;
        if (is_train) {
            auto it = trains.find({min(at, from), max(at, from)});
            if (it != trains.end()) trains.erase(it);
        }
        //printf("at index %d with cost %d (train? %d)\n", at, at_cost, is_train);
        seen[at] = 1;
        for (auto e : g[at]) {
            int e_cost = get<0>(e), e_train = get<1>(e), e_index = get<2>(e);
            //if (!seen[e_index]) printf("----options are: to %d on a %s with point cost %d and total cost %d\n", e_index, (e_train) ? "train" : "road", e_cost, e_cost + at_cost);
            if (!seen[e_index]) pq.push({e_cost + at_cost, e_train, e_index, at});
        }
    }
    printf("%ld\n", trains.size());
}