#include <cstdio>
#include <iostream>
#include <queue>

#define N (2*500+3) // 500 heros, 500 monsters, source, sink, potion
#define E (504*500)
#define INF 1e8
#define DEBUG 0

using namespace std;

int nheroes, nmonsters, npotions, source, sink;

/*
 * dinitz 1
 */

// assumes the residual graph is constructed as in the previous section
// stores dist from source.
int lvl[N];
// stores first non-useless child.
int nextchld[N];
int start[N];
// if e is an outgoing edge from u, succ[e] is another one, or -1
// cap[e] is the capacity/weight of the e
// to[e] is the destination vertex of e
int succ[E], cap[E], to[E], seen[N];

int edge_counter = 0;
void add_edge(int u, int v, int c) {
  // set the properties of the new edge
  cap[edge_counter] = c, to[edge_counter] = v;
  // insert this edge at the start of u'source linked list of edges
  succ[edge_counter] = start[u];
  start[u] = edge_counter;
  ++edge_counter;
}

// constructs the BFS tree.
// Returns if the sink is still reachable.
bool bfs() {
  for (int i = 0; i <= nheroes+nmonsters+3; i++) lvl[i] = -1;
  queue<int> q;
  q.push(source); lvl[source] = 0;
  while (!q.empty()) {
    int u = q.front(); q.pop();
    nextchld[u] = start[u]; // reset nextchld
    for (int e = start[u]; ~e; e = succ[e]) {
      if (cap[e] > 0) {
        int nxt = to[e];
        if (lvl[nxt] != -1) continue; // already seen
        lvl[nxt] = lvl[u] + 1;
        q.push(nxt);
      }
    }
  }
  return lvl[sink] != -1;
}

/*
 * dinitz 2
 */

// Finds an augmenting path with up to cflow flow.
int aug(int u, int cflow) {
  if (u == sink) return cflow; // base case.
  // Note the reference here! We will keep decreasing nextchld[u]
  // Till we find a child that we can flow through.
  for (int &i = nextchld[u]; i >= 0; i = succ[i]) {
    if (cap[i] > 0) {
      int nxt = to[i];
      // Ignore edges not in the BFS tree.
      if (lvl[nxt] != lvl[u] + 1) continue;
      int rf = aug(nxt, min(cflow, cap[i]));
      // Found a child we can flow through!
      if (rf > 0) {
        cap[i] -= rf;
        cap[i^1] += rf;
        return rf;
      }
    }
  }
  lvl[u]=-1;
  return 0;
}

int inv(int e) { return e ^ 1; }

bool augment(int u, int t, int f) {
  if (u == t) return true;                     // the path is empty!
  if (seen[u]) return false;
  seen[u] = true;
  for (int e = start[u]; ~e; e = succ[e])
    if (cap[e] >= f && augment(to[e], t, f)) { // if we can reach the end,
      cap[e] -= f;                             // use this edge
      cap[inv(e)] += f;                        // allow "cancelling out"
      //printf("using edge %d->%d\n", u, e);
      return true;
    }
  return false;
}

int mindex(int monster) {
    return monster + nheroes;
}

int main() {

    // read in inital input
    fill(start, start+N, -1);
    cin >> nheroes >> nmonsters >> npotions;
    // derive fixed indexes
    source = 0, sink = nheroes+nmonsters+1;
    int potion = sink+1;
    //printf("sink is %d\n", sink);

    // add edge from source to potion
    add_edge(source, potion, min(npotions, nheroes));
    add_edge(potion, source, 0);

    // read in the per hero input
    for (int hero=1; hero <= nheroes; hero++) {

        // add edge from source to this hero
        add_edge(source, hero, 1);
        add_edge(hero, source, 0);
        // add edge from potion to this hero
        add_edge(potion, hero, 1);
        add_edge(hero, potion, 0);

        // get how many monsters in this heroes kill list
        int noptions;
        cin >> noptions;
        for (int i=0; i < noptions; i++) {
            // read in the actual kill options
            int monster;
            cin >> monster;
            add_edge(hero, mindex(monster), 1);
            add_edge(mindex(monster), hero, 0);
        }
    }

    // add edges between monsters and sink
    for (int i=mindex(1); i <= mindex(nmonsters); i++) {
        add_edge(i, sink, 1);
        add_edge(sink, i, 0);
    }

    if (DEBUG) {
        for (int i=0; i <= nheroes + nmonsters + 3; i++) {
            printf("edges coming out of vertex %d;\n", i);
            for (int e = start[i]; ~e; e = succ[e]) {
                printf("%d(%d) ", to[e], cap[e]);
            }
            printf("\n");
        }
    }

    // int tot = 0;
    // while (bfs())
    //     for (int x = aug(source,INF); x; x = aug(source,INF)) tot+=x;
    // printf("%d\n", tot);
    int res = 0;
    fill(seen, seen + N, 0);
    while (augment(source, sink, 1)) {
        fill(seen, seen + N, 0);
        res += 1;
    }
    printf("%d\n", res);
}

/*
    node 0 is the source
    nodes 1..nheroes are the heroes
    nodes nheroes+1..nheroes+nmonsters are the monsters
    node nheroes+nmonsters+1 is the sink
    node nheroes+1+nmonsters+1+1 is the potion

 */