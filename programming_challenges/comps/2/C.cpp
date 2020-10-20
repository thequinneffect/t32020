#include <iostream>
#include <climits>

using namespace std;
typedef pair<int, int> pii;

#define MAXM 100100
pii reactions[MAXM];
int destinations[MAXM];
int state[MAXM]; // basic -1. neutral 0, acidic 1
int empty[MAXM];

int main() {

    int n,m;
    cin >> n >> m;
    string s;
    cin >> s;

    for (int i=1; i <= m; i++) cin >> reactions[i].first >> reactions[i].second;
    

    cout << n << ", " << m << ", " << s << "\n";
    for (int i=1; i <= m; i++) cout << reactions[i].first << ", " << reactions[i].second << "\n";

    // set the states (acid, base, neutral)
    for (int i=0; i < n; i++) {
        if (s[i] == 'A') state[i+1] = 1;
        else if (s[i] == 'B') state[i+1] = -1;
    }

    // set the destinations
    for (int i=1; i <= n; i++) {
        destinations[i] = i;
        empty[i] = 0;
    }

    // now do the queries
    for (int i=1; i <= m; i++) {

        // pour the mixture and update the states
        
        // if acid and base, then they go poof (destinations are 0)
        if (
            (state[reactions[i].first] == 1 && state[reactions[i].second] == -1) ||  
            (state[reactions[i].first] == -1 && state[reactions[i].second] == 1)
        ) {
            destinations[reactions[i].first] = 0; // poof them
            destinations[reactions[i].second] = 0;
            state[reactions[i].first] = 0; // set neutral (even tho empty)
            state[reactions[i].second] = 0;
            empty[reactions[i].first] = 1; // set empty
            empty[reactions[i].second] = 1;
        } else {
            // no poofing to be done, just updating states and destinations
            if (empty[reactions[i].first] == 0) destinations[reactions[i].first] = reactions[i].second; // can only update destination if not poofed
            // solution state may need to be updated
            int new_state = state[reactions[i].first] + state[reactions[i].second];
            state[reactions[i].first] = new_state;
            state[reactions[i].second] = new_state;
        }

    }

    // print out the destinations
    for (int i=1; i <= n; i++) {
        //if (destinations[i] == INT_MAX) destinations[i] = 0;
        cout << destinations[i] << endl;
    }
}

/* 

    cases;
    - mix acid and base (liquid destinations end up being 0)
    - mix acid and neutral (liquid is acidic and in one beaker, other beaker empty)
    - mix base and neutral (liquid is basic and in the one beaker, other beaker empty)
    - mix neitral and netral (resuilt is neutral, one beaker empty)

    - pouring into empty beaker is fine

*/