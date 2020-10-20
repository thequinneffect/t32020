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
 
vector<int> G[MAXN];

int parents[MAXN];
 
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

int pointed_to[MAXN];

int main () {
    //ios_base::sync_with_stdio(0); cin.tie(0);

    int num = 0;
    while (++num) {

        memset(pointed_to, 0, MAXN*sizeof(int));
        edges.clear();

        int a, b;
        while(scanf("%d %d\n", &a, &b), a != 0) {
            if (a == -1) return 0;
            edges.pb({a,b});
        }

        int N = compress(edges);

        for (int i=0; i < N; i++) parents[i] = i;
        
        if (edges.size() != N-1) {
            // this cannot be a tree because there are too many edges
            printf("Case %d is not a tree.\n", num);
        } else if (edges.size() == 0) {
            // an empty tree is considered a tree
            printf("Case %d is a tree.\n", num);
        } else {
            // check for duplicate incoming edges or reflexive edges
            bool edges_okay = true;
            for (vector<pii>::iterator it = edges.begin(); it != edges.end(); it++) {
                // adding an edge, so change the parent
                parents[it->y] = it->x;
                if (pointed_to[it->y] == 1 || it->x == it->y) {
                    printf("Case %d is not a tree.\n", num);
                    edges_okay = false; break;
                } else {
                    pointed_to[it->y] = 1;
                }
            }
            if (!edges_okay) continue;
        
            // at this point the edges are okay, so check we have a root
            bool has_root = false;
            for (int i=0; i < N; i++) {
                if (pointed_to[i] == 0 && outgoing[i] == N-1) {
                    has_root = true; break;
                }
            }
            if (!has_root) {
                printf("Case %d is not a tree.\n", num);
            } else {
                printf("Case %d is a tree.\n", num);
            }
        }
    }
}
 