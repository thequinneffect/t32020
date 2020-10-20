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

    // start with tree 1 and hence beauty 1
    if (trees[1] == 0) {
        // if it is uncoloured we can paint it any colour with that colours respective cost
        for (int c=1; c <= m; c++) dp[1][1][c] = p[1][c];
    } else {
        // the tree was already coloured, hence we pay nothing but are stuck with that single colour
        dp[1][1][trees[1]] = p[1][trees[1]];
    }

    for (int t=2; t <= n; t++) {
        for (int b=1; b <= t; b++) {
            // it's not possible to have a beauty > t because even if every t was a different colour that
            // is only t distinct groups at most.
            // if (trees[t] == 0 && trees[t-1] == 0) {
            //     // if we are choosing the current colour, and the previous one was chosen too
            //     // then we need to compare all possible pairs
            //     for (int c1=1; c1 <= m; c1++) {
            //         for (int c2=1; c2 <= m; c2++) {
            //             if (c1 == c2) dp[t][b][c1] = min(dp[t][b][c1], dp[t-1][b][c1] + p[t][c1]);
            //             else dp[t][b][c1] = min(dp[t][b][c1], dp[t-1][b-1][c2] + p[t][c1]);
            //         }
            //     }
            // } else if (trees[t] == 0 && trees[t-1] != 0) {
            //     // if we are choosing the current colour but the previous one was already painted
            //     // then we compare all pairs where the previous is fixed
            //     for (int c=1; c <= m; c++) {
            //         if (c == trees[t-1]) dp[t][b][c] = min(dp[t][b][c], dp[t-1][b][c] + p[t][c]);
            //         else dp[t][b][c] = min(dp[t][b][c], dp[t-1][b-1][c] + p[t][c]);
            //     }
            if (trees[t] != 0 && trees[t-1] == 0) {
                // if the current tree is already painted but the previous one wasn't
                // then we need to compare these pairs where the current tree colour is fixed
                for (int c=1; c <= m; c++) {
                    if (c == trees[t-1]) dp[t][b][trees[t]] = min(dp[t][b][trees[t]], dp[t-1][b][c]);
                    else dp[t][b][trees[t]] = min(dp[t][b][trees[t]], dp[t-1][b-1][c]);
                }
            } else if (trees[t] != 0 && trees[t-1] != 0) { // trees[t] != 0 && trees[t-1] != 0 
                // if the current tree is already painted and the previous one was too
                // then we need to compare this single pair
                if (trees[t] == trees[t-1]) dp[t][b][trees[t]] = min(dp[t][b][trees[t]], dp[t-1][b][trees[t-1]]);
                else dp[t][b][trees[t]] = min(dp[t][b][trees[t]], dp[t][b-1][trees[t-1]]);
            } else {
                for (int c1 = 1; c1 <= m; c1++) {
                    dp[t][b][c1] = min(dp[t][b][c1], dp[t-1][b][c1] + p[t][c1]); // assume we do try colour it the same, either the prev colour
                    // is valid and we get an actual value, or not and we get infinity
                    for (int c2 = 1; c2 <= m; c2++) {
                        if (c1 == c2) continue; // colours cannot be the same because in this condition we assume the beauty increases
                        dp[t][b][c1] = min(dp[t][b][c1], dp[t-1][b-1][c2] + p[t][c1]);
                    } 
                } 
            }
        }
    }
    
    // because we only want answers for all trees and beauty k, we fix t=n and b=k
    // and find the lowest there
    for (int b=1; b <= k; b++)
    for (int c=1; c <= m; c++) {
        cout << dp[n][k][c] << ", ";
    }
    cout << endl;
}