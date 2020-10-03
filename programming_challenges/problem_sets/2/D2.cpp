#include <iostream>
#include <vector>
#include <stack>
#include <set>
#include <map>

typedef long long ll;
using namespace std;

#define MAXN 1000000

ll tree[MAXN]; // store sets
ll sz[MAXN]; // for balanacing the trees
ll community_size[MAXN]; // store populations for communities
ll populations[MAXN];
vector<pair<ll, ll>> roads; // store roads
multiset<ll> rep_pops; // for obtaining largest value quickly

ll rep(ll i) {
    if (tree[i] == i) {
        return tree[i];
    }
    tree[i] = rep(tree[i]);
    return tree[i];
}

void merge(ll xrep, ll yrep) {
    if (sz[xrep] < sz[yrep]) {
        tree[xrep] = yrep; // x is smaller tree, so add it on to y
        community_size[yrep] += community_size[xrep];
    } else if (sz[yrep] < sz[xrep]) {
        tree[yrep] = xrep; // y is smaller tree, so add it on to x
        community_size[xrep] += community_size[yrep];
    } else { // same size trees, can append either to the other, choosing to add y to x
        tree[yrep] = xrep; // make x the parent of y
        community_size[xrep] += community_size[yrep];
        sz[xrep]++; // increase the size of the joined to tree
    }

}

void add_road(ll t1, ll t2) { // road goes between t1 and t2
     // delete the current populations
    ll rep1 = rep(t1);  
    ll rep2 = rep(t2);    
    if (rep1 == rep2) return;
    auto itr = rep_pops.find(community_size[rep1]);
    if(itr != rep_pops.end()) rep_pops.erase(itr);
    itr = rep_pops.find(community_size[rep2]);
    if(itr != rep_pops.end()) rep_pops.erase(itr);

    // do the merge
    merge(rep1, rep2); 
    // add the new community pop
    rep1 = rep(t1);
    rep_pops.insert(community_size[rep1]);
}

int main() {

    ll n,m,q; // citites, roads, queries respectively
    cin >> n >> m >> q;

    // initialise starting populations
    for (ll i=0; i < n; i++) cin >> populations[i];

    // read in roads
    for (ll i=0; i < m; i++) {
        pair<ll,ll> r;
        cin >> r.first >> r.second;
        r.first--; r.second--; // change from 1 indexing to 0 indexing
        roads.push_back(r);
    }

    // initialise DSU
    for (ll i=0; i < n; i++) {
        tree[i] = i;
        sz[i] = 1;
    }

    stack<pair<ll,ll>> saved_pops;
    stack<pair<ll, ll>> deletions;
    stack<bool> is_deletion;
    for (ll i=0; i < q; i++) {

        char query;
        cin >> query;
        if (query == 'P') {
            ll town, new_pop;
            cin >> town >> new_pop;
            saved_pops.push({town-1, populations[town-1]});
            // make the change
            populations[town-1] = new_pop;
            is_deletion.push(false);
        } else {
            ll road;
            cin >> road;
            // add road to deletions
            deletions.push({roads[road-1].first, roads[road-1].second});
            // remove road from initial roads
            roads[road-1].first = -1; roads[road-1].second = -1;
            is_deletion.push(true);
        }

    }

    for (int i=0; i < n; i++) {
        rep_pops.insert(populations[i]);
        community_size[i] = populations[i];
    }

    // loop through remaining initial roads for first setup
    for (ll i=0; i < m; i++) {
        if (roads[i].first == -1) continue; // this road was deleted
        add_road(roads[i].first, roads[i].second);   
    }

    stack<ll> answers;
    // we now have the ending state and can output the first answer
    answers.push(*(rep_pops.rbegin()));

    // now we can perform each of the queries in reverse and output the answers
    while (is_deletion.size() > 1)  {
        pair<ll, ll> info;
        if (is_deletion.top() == true) {
            info = deletions.top();
            deletions.pop();
            add_road(info.first, info.second);
        } else {
            info = saved_pops.top();
            saved_pops.pop();
            int comm_rep = rep(info.first);
            auto itr = rep_pops.find(community_size[comm_rep]);
            if(itr != rep_pops.end()) rep_pops.erase(itr);
            community_size[comm_rep] -= populations[info.first];
            populations[info.first] = info.second;
            community_size[comm_rep] += populations[info.first];
            rep_pops.insert(community_size[comm_rep]);
        }
        answers.push(*(rep_pops.rbegin()));
        is_deletion.pop();
    }

    while (!answers.empty()) { 
        cout << answers.top() << endl;
        answers.pop();
    }
}