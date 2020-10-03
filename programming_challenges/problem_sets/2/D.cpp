#include <iostream>
#include <vector>
#include <stack>
#include <set>

using namespace std;

#define MAXN 500000
struct town { int parent, population; };

struct town tree[MAXN];
int sz[MAXN];
vector<pair<int,int>> roads;
multiset<int> community_pops;

struct town rep(int i) {
    if (tree[i].parent == i) {
        return tree[i];
    }
    tree[i].parent = rep(tree[i].parent).parent;
    return tree[i];
}

void merge(int x, int y) {
    int xrep = rep(x).parent; // find the representative of x
    int yrep = rep(y).parent; // find the representative of y
    if (xrep == yrep) return; // already in the same community
    if (sz[xrep] < sz[yrep]) {
        tree[xrep].parent = yrep; // x is smaller tree, so add it on to y
        tree[yrep].population += tree[xrep].population;
    } else if (sz[yrep] < sz[xrep]) {
        tree[yrep].parent = xrep; // y is smaller tree, so add it on to x
        tree[xrep].population += tree[yrep].population;
    } else { // same size trees, can append either to the other, choosing to add y to x
        tree[yrep].parent = xrep; // make x the parent of y
        tree[xrep].population += tree[yrep].population;
        sz[xrep]++; // increase the size of the joined to tree
    }

}

void add_road(int r1_index, int r2_index) {
     // delete the current populations
    struct town r1 = rep(r1_index);
    auto itr = community_pops.find(r1.population);
    if(itr != community_pops.end()) community_pops.erase(itr);
    struct town r2 = rep(r2_index);
    itr = community_pops.find(r2.population);
    if(itr != community_pops.end()) community_pops.erase(itr);

    // do the merge
    merge(r1_index, r2_index); 
    // add the new community pop
    r1 = rep(r1_index);
    community_pops.insert(r1.population);
}

int main() {

    int n,m,q; // citites, roads, queries respectively
    cin >> n >> m >> q;

    // initialise starting populations
    for (int i=0; i < n; i++) {
        int pop;
        cin >> pop;
        tree[i].population = pop;
    }

    // read in roads
    for (int i=0; i < m; i++) {
        pair<int,int> r;
        cin >> r.first >> r.second;
        r.first--; r.second--; // change from 1 indexing to 0 indexing
        roads.push_back(r);
    }

    // initialise DSU
    for (int i=0; i < n; i++) {
        tree[i].parent = i;
        sz[i] = 1;
    }

    // can't delete from dsu, so start in the state of all roads that will be deleted
    // already being deleted. We also need to have applied all population changes
    // Therefore add starting populations, and all population changes to a stack
    // then process them from the stack.
    stack<pair<int,int>> populations;
    stack<pair<int, int>> deletions;
    stack<bool> is_deletion;
    for (int i=0; i < q; i++) {

        int query;
        cin >> query;
        if (query == 'P') {
            int town, new_pop;
            cin >> town >> new_pop;
            // save current population of town in question to stack
            populations.push({town-1, tree[town-1].population});
            // make the change
            tree[town-1].population = new_pop;
            is_deletion.push(false);
        } else {
            int road;
            cin >> road;
            // add road to deletions
            deletions.push({roads[road-1].first, roads[road-1].second});
            // remove road from initial roads
            roads[road-1].first = roads[road-1].second = -1;
            is_deletion.push(true);
        }

    }

    // init community scores (each town is its own community atm)
    for (int i=0; i < n; i++) community_pops.insert(tree[i].population);

    // loop through remaining initial roads for first setup
    for (int i=0; i < m; i++) {
        if (roads[i].first == -1) continue; // this road was deleted
        add_road(roads[i].first, roads[i].second);   
    }

    // we now have the ending state and can output the first answer
    cout << *(community_pops.rbegin()) << endl; 

    // now we can perform each of the queries in reverse and output the answers
    while (!is_deletion.empty()) {

        pair<int, int> info;

        if (is_deletion.top() == true) {
            info = deletions.top();
            deletions.pop();
            // add the edge in i.e. merge
            add_road(info.first, info.second);

        } else {
            info = populations.top();
            populations.pop();
            // revert the population 
            int difference = abs(tree[info.first].population) - abs(info.second);
            int original_pop = tree[info.first].population;
            tree[info.first].population = info.second;

            // remove current community pop
            auto itr = community_pops.find(rep(info.first).population);
            if(itr != community_pops.end()) community_pops.erase(itr);

            // bubble result up to community representative
            if (info.second < original_pop) {
                // reduce representative population
                tree[rep(info.first).parent].population -= difference;
            } else { // we made the pop greater or equal
                // increase representative population
                tree[rep(info.first).parent].population += difference;
            }
            community_pops.insert(rep(info.first).population);
        }

        cout << *(community_pops.rbegin()) << endl;

        is_deletion.pop();
    }


}

/* 
3 3 6 // n, m, q
1 2 3 // initial populations

1 2 // initial roads
2 3
3 1

P 1 3 -> revert to (1,2)
D 1 -> do a merge (merge(roads[1].first, roads[1].second))
P 2 3
D 2
P 3 10 -> revert to (3, 1)
D 3 -> do a merge

first state needs to be populations 3 3 10 with all road deletes done
then next state we add road 3 back (i.e. merge 3 and 1) -> if command is D, do a merge
next state we need to get the previous population of town 3, hence we pop from pop stack -> if command is P, pop from stack and do population replacement

*/