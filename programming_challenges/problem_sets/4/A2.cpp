#include <vector>
#include <map>
#include <cstring>
#include <cstdio>

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;
#define x first
#define y second
#define pb push_back
#define all(x) x.begin(), x.end()
#define MAXN 200005

vector<pii> edges;
 
int compress(vector<pii> &v) {
    map<int, int> m;
    for (int i = 0; i < v.size(); i++) {
            m.insert({v[i].x, 0});
            m.insert({v[i].y, 0});
    }

    int upto = 0;
    map<int, int>::iterator it = m.begin();
    while (it != m.end()) {
            m[it->x] = upto;
            upto++;
            it++;
    }

    for (int i = 0; i < v.size(); i++) {
            v[i].x = m[v[i].x];
            v[i].y = m[v[i].y];
    }

    return upto;
}

// DSU
int root[MAXN]; // this tracks the representatives of the sets i.e. the roots
int sz[MAXN]; // this tracks the size of a the sets

// this function finds the representative of a set
// you can use it to test if two elements are in the same set or not i.e. get_rep(i) == get_rep(j)
int get_rep(int i) {
    // if an element is the representative of itself, then it must be the representative of the set it is in
    // note that this could be a set of size 1 (just itself), which is initially true for all elements
    if (root[i] == i) return i;

    // otherwise, the parent of i is not itself, and so we must find the parent of i's parent
    // note that we could just do get_rep(root[i]) to recurse until we find the representative, however, we 
    // can add an optimisation in.
    // return get_rep(root[i]);
    
    // this optimisation is called path compression. Basically, if root[i] isn't i, then we want to set root[i] to
    // be the toppest level parent (i.e. the representative), and don't actually store the inbetween parents
    root[i] = get_rep(root[i]); // all elements in this set will have the same parent now, the representative
    return root[i];
}

// this function merges two sets, specifically the smaller set is made a direct child of the larger set
// i.e. the representative of the smaller set is no longer a representative, it is a direct child of the larger representative
// you can picture this as the smaller tree being attached to the root of the larger tree
int merge(int x, int y) {
    int repx = get_rep(x);
    int repy = get_rep(y);
    if (repx == repy) return repx; // can return either rep because they are already in the same set

    if (sz[repx] < sz[repy]) root[repx] = repy;
    else if (sz[repy] < sz[repx]) root[repy] = repx;
    else { // same size
        // can join either to the other, choose x as parent
        root[repy] = repx;
        sz[repx]++;
    }
    return root[repx]; // is either self-referential or points to repy, either way it correctly return the new rep
}

void yes(int i) { printf("Case %d is a tree.\n", i); }
void no(int i) { printf("Case %d is not a tree.\n", i); }

int pointed_to[MAXN];

int main () {
    //ios_base::sync_with_stdio(0); cin.tie(0);

    int num = 0;
    main_loop: while (++num) {

        edges.clear();
        memset(pointed_to, 0, MAXN*sizeof(int));

        int a, b;
        while(scanf("%d %d\n", &a, &b), a != 0) {
            if (a == -1) return 0;
            edges.pb({a,b});
        }

        // compress the input to edges in range 0..n
        int N = compress(edges);

        // initalise the DSU for the compressed input range
        for (int i=0; i < N; i++) {
            root[i] = i;
            sz[i] = 1;
        }

        //printf("tree has %d edges.\n", N);

        if (edges.size() == 0) {
            // an empty tree is considered a tree
            yes(num);
        } else if (edges.size() != N-1) {
            // if there are edges, there must be n-1 because all N nodes
            // except the root (N-1) have exactly one incoming edge
            no(num);
        } else {
            // check that each node only has 1 incoming edge at maximum
            for (vector<pii>::iterator it = edges.begin(); it != edges.end(); it++) {
                merge(it->x, it->y);
                if (pointed_to[it->y] == 1) {
                    no(num);
                    goto main_loop;
                } else {
                    pointed_to[it->y] = 1;
                }
            }
            // edges check out, now see if we have a root
            int root = -1;
            for (int i=0; i < N; i++) {
                if (pointed_to[i] == 0) root = i;
            }
            if (root == -1) {
                no(num);
                goto main_loop;
            }
            // check if graph is connected
            for (int i=0; i < N; i++) {
                if (get_rep(i) != root) {
                    no(num);
                    goto main_loop;
                }
            }
            // it's a graph!!!
            yes(num);
        }
    }
}
 