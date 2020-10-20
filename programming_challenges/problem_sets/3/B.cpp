#include <iostream>
#include <climits>

using namespace std;

#define MAX 101
typedef long long ll;

int trees[MAX];
ll p[MAX][MAX]; // paint costs, (tree, colour)
ll dp[MAX][MAX][MAX]; // tree, beauty, colour = cost

int main() {

    int n,m,k;
    cin >> n >> m >> k;
    for (int i=1; i <= n; i++) cin >> trees[i];
    for (int i=1; i <= n; i++)
        for (int j=1; j <= m; j++) cin >> p[i][j];

    for (int t=0; t <= n; t++)
        for (int b=0; b <= k; b++)
            for (int c=0; c <= m; c++) 
                dp[t][b][c] = LLONG_MAX/2; // dp[tree][beauty][colour]

    
    // if the first tree (1) is uncoloured then we have m possibilites for 
    // the first tree and it will have beaty 1 because it is the only tree
    if (trees[1] == 0) {
        for (int c=1; c <= m; c++) dp[1][1][c] = p[1][c];
    } else { // if it was coloured, then we have no choice for the colour and it costs 0
        dp[1][1][trees[1]] = 0;
    }

    /*
        So now we either have some paint colour costs at dp[1][1][all colours] or we have a 0 at dp[1][1][prev tree colour]
        Now imagine I want to do tree 2, what decisions do i have to make?
        - either tree 2 is coloured or uncoloured
        - if tree 2 is uncoloured then

            - if the previous tree was initially uncoloured, then we have the bunch of paint scores, and we basically have to
            do the same thing again (i.e. dp[2][2][colour a] = dp[1][1][colour b] + cost[2, colour a] but colour of this tree
            and the prev tree cannot be the same because we are assuming the beauty increases here (dp[2][beauty=2])) i.e. colour a != colour b
            for (int c1 = 1; c1 <= m; c1++) 
                for (int c2 = 1; c2 <= m; c2++) {
                    if (c1 == c2) continue;
                    dp[t][b][c1] = dp[t-1][b-1][c2] + p[t][c1];
                }                                                                              t  b                 t  b
            - but we can paint it the same, but we cannot increase the beauty in that case: dp[2][1][trees[1]] = dp[1][1][trees[1]] + cost[2, trees[1]]

            - if the previous tree was already coloured, then we just have a single 0 at dp[1][1][tree[1]], the rest of the c are INF. We can pick
            any colour for the current tree, but the only useful cases are picking the same as the previous predetermined colour or picking different
            to it. It doesn't make sense to consider the previous tree being a different colour because we had no choice in it. i.e. 
            dp[t][b][c] = dp[t-1][b-1][trees[t-1]] 

        - otherwise it is already coloured, in which case 

            - if the previous tree was initially uncoloured, we again have the bunch of paint scores. In this case, it is only going to be a 
            match with one of those different colours. So for the cases where the current colour differs from the previous colours (m-1 of the
            cases because it only matches with one of them) we get;
                dp[t][b][trees[t]] = dp[t-1][b-1][c] where c != trees[t]
    */

    // for each tree
    for (int t=2; t <= n; t++) {

        // for each possible beauty up to t
        for (int b=1; b <= t; b++) { // only need to check to t because that's the highest possible beauty we could have

            
            if (trees[t] != 0) { // if the tree is already coloured
               
                if (trees[t] == trees[t-1]) { // and it is the same colour as prev tree (or use this? dp[t-1][b][trees[t-1]])

                    dp[t][b][trees[t]] = min(dp[t][b][trees[t]], dp[t-1][b][trees[t]]); // then beauty didn't increase, either did cost, only tree count did

                } else { // and is a different colour to the prev tree
                    
                    for (int c=1; c <= m; c++) { // set the states for all those different colours
                        if (c == trees[t]) continue; // skip the same colour
                        dp[t][b][c] = min(dp[t][b][trees[t]], dp[t-1][b-1][c]); // was already coloured (so no cost to us), different colour though (so need to look at prev beauty) for all c where c != trees[t-1]
                    }
                }

            } else { // tree is uncoloured

                if (trees[t-1] != 0) { // and the previous tree was already coloured
                    for (int c=1; c <= m; c++) {
                        if (c == trees[t-1]) dp[t][b][c] = min(dp[t][b][c], dp[t-1][b][c] + p[t][c]);
                        else dp[t][b][c] = min(dp[t][b][c], dp[t-1][b-1][c] + p[t][c]);
                    }
                } else { // previous tree was initially uncoloured
                    for (int c1=1; c1 <= m; c1++) {
                        for (int c2=1; c2 <= m; c2++) {
                            if (c1 == c2) continue;
                            else dp[t][b][c1] = min(dp[t][b][c1], dp[t-1][b-1][c2] + p[t][c1]);
                        }
                    }
                }

                // for (int c1 = 1; c1 <= m; c1++) {
                //     dp[t][b][c1] = min(dp[t][b][c1], dp[t-1][b][c1] + p[t][c1]); // assume we do try colour it the same, either the prev colour
                //     // is valid and we get an actual value, or not and we get infinity
                //     for (int c2 = 1; c2 <= m; c2++) {
                //         if (c1 == c2) continue; // colours cannot be the same because in this condition we assume the beauty increases
                //         dp[t][b][c1] = min(dp[t][b][c1], dp[t-1][b-1][c2] + p[t][c1]);
                //     } 
                // } 
            
            }

        }

    }    

    // only want the best answer for all trees, beaty equals to k, but the ending colour doesn't matter
    ll res = LLONG_MAX/2;
    for (int c=1; c <= m; c++) {
        if (dp[n][k][c] < res) res = dp[n][k][c];
    }
    if (res == LLONG_MAX/2) res = -1;
    cout << res << endl;

}