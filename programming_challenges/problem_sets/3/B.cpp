#include <iostream>
#include <climits>

using namespace std;

#define MAX 101
typedef long long ll;

int trees[MAX];
ll p[MAX][MAX]; // paint costs, (tree, colour)
ll dp[MAX][MAX][MAX];

int main() {

    int n,m,k;
    cin >> n >> m >> k;
    for (int i=1; i <= n; i++) cin >> trees[i];
    for (int i=1; i <= n; i++)
        for (int j=1; j <= m; j++) cin >> p[i][j];

    for (int t=0; t <= n; t++)
        for (int b=0; b <= k; b++)
            for (int c=0; c <= m; c++) 
                dp[t][b][c] = LLONG_MAX; // dp[tree][beauty][colour]

    
    // if the first tree (1) is uncoloured then we have m possibilites for 
    // the first tree and it will have beaty 1 because it is the only tree
    if (trees[1] == 0) {
        for (int c=1; c <= m; c++) dp[1][1][c] = p[1][c];
    } else { // if it was coloured, then we have no choice for the colour and it costs 0
        dp[1][1][trees[1]] = 0;
    }

    // now we have some starting values, we can do the dp

    // for each tree
    for (int t=2; t < n; t++) {

        // for each possible beauty
        for (int b=1; b <= k; b++) {

            // if the tree is already coloured
            if (trees[t] != 0) {
                // if it is the same colour as prev tree
                if (trees[t] == dp[t-1][b][trees[t-1]]) {
                    dp[t][b][trees[t-1]] = dp[t-1][b][trees[t-1]];
                } else { // it is a different colour to the prev tree
                    // set the states for all those different colours
                    for (int c=1; c <= m; c++) {
                        if (c == trees[t-1]) continue; // skip the same colour
                        dp[t][b][c] = dp[t-1][b-1][c]; // was already coloured, different colour though, do for all * where * != trees[i-1]
                    }
                }
            }

            // for each colour
            for (int c=1; c <= m; c++) {
                
                dp[t][b][c] = dp[t-1][b][c] + p[t][trees[t-1]]; // wasn't coloured, coloured same colour
                //dp[t][b][c] = dp[t-1][b][c]; // was already coloured, same colour too

                dp[t][b][c] = dp[t-1][b-1][*] + p[t][c]; // wasn't coloured, coloured different colour i.e. do for all * where * != c
                dp[t][b][c] = dp[t-1][b-1][*]; // was already coloured, different colour though, do for all * where * != trees[i-1]
                

            }

        }

    }    

}