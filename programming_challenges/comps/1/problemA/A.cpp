#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {

    // handle input
    int n;
    cin >> n;

    // handle the trivial case of 1
    if (n==1) {
        int num;
        cin >> num;
        cout << num;
        return 0;
    }

    vector<int> numbies;
    for (int i=0; i < n*n; i++) {
        int new_numby;
        cin >> new_numby;
        numbies.push_back(new_numby);
    }

    sort(numbies.begin(), numbies.end());
    int solution[n][n] = {0};
    // solution[0][0] = numbies[0];
    // solution[n-1][n-1] = numbies[n-1];

    // start off at [1,0]
    int r = 0, c = 0, i = 0;
    int orig_r = r, orig_c = c;

    while (i < n*n) {
    
            // insert lowest number
            solution[r][c] = numbies[i++];
            // decrement row, increment column until mirrored.
            if (c == orig_r && r == orig_c) {
                if (orig_r < n-1) {
                    r = ++orig_r;
                    c = orig_c;
                } else {
                    c = ++orig_c;
                    r = orig_r;
                }
            } else {
                r--; c++;
            }
    }
    
    for (int i=0; i < n; i++) {
        for (int j=0; j < n; j++) {
            cout << solution[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

}