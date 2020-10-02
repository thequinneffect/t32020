#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

#define MAX_ENTRIES (262144-1)

vector<int> results;
int numbers[MAX_ENTRIES];

int main() {

    int n,m;
    cin >> n >> m;

    int offset = pow(2, n)-1;
    for (int i=0; i < pow(2, n); i++) {
        int num;
        cin >> num;
        // insert these at the end of the array
        numbers[offset+i] = num;
    }
    vector<pair<int, int>> changes;
    for (int i=0; i < m; i++) {
        pair<int, int> p;
        cin >> p.first >> p.second;
        changes.push_back(p);
    }

    // cout << n << " " << m << endl;
    // for (int i=0; i < pow(2, n); i++) cout << numbers[i] << " "; cout << endl;
    // for (int i=0; i < m; i++) cout << changes[i].first << " " << changes[i].second << endl;

    bool exclusive = false; // start with normal or
    // for each level besides the root level
    for (int i=0; i < n; i++) {

        // calculate the offset for accessing this level in the array
        offset = pow(2, n-i)-1;
        // for each pair of elements in this level of the array
        for (int j=offset; j < offset+pow(2,n-i)-1; j+= 2) {
            pair<int, int> p;
            p.first = numbers[j];
            p.second = numbers[j+1];

            // store the or or xor of them at parent
            if (exclusive) {
                numbers[(j-1)/2] = p.first ^ p.second;
            } else {
                numbers[(j-1)/2] = p.first | p.second;
            }            
        }
        // swap xor/or operation per level
        exclusive = (exclusive) ? false : true; 
    }

    // should have full array now, print it to check
    // for (int i=0; i <= n; i++) {

    //     if (i == n) cout << "(" << numbers[0] << ")\n";
    //     // calculate the offset for accessing this level in the array
    //     offset = pow(2, n-i)-1;
    //     // for each pair of elements in this level of the array
    //     for (int j=offset; j < offset+pow(2,n-i)-1; j+= 2) {
    //         pair<int, int> p;
    //         p.first = numbers[j];
    //         p.second = numbers[j+1];

    //         cout << "(" << p.first << ", " << p.second << ")  ";       
    //     }
    //     cout << "\n";
    // }

    // make changes and recompute
    offset = pow(2, n)-1;
    for (int c=0; c < changes.size(); c++) {
        pair<int, int> change = changes[c];
        // change the requested number
        int change_index = offset + change.first-1; // p is 1 indexed, not 0 indexed
        numbers[change_index] = change.second; // make the change
        exclusive = false;

        // now propagate the change up to the parent
        for (int i=0; i < n; i++) {
            // paired element could be to the left or right
            if (change_index % 2 == 0) change_index--;
            // set the parent
            if (exclusive) numbers[(change_index-1)/2] = numbers[change_index] ^ numbers[change_index+1];
            else numbers[(change_index-1)/2] = numbers[change_index] | numbers[change_index+1];
            // set change index to parent index
            change_index = (change_index-1)/2;
            exclusive = (exclusive) ? false : true;
        }
        cout << numbers[0] << endl;
    }

}

// [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15][16,17,18,19,20,21,22,23][24,25,26,27][28,29][30]

// children are at 2*i+1 and 2*i+2, parent is at (i-1)/2

// access as index / pow(2, level)    ceil(5/pow(2,0)) -> 5, ceil(9/pow(2, 1))
// ceil(17/4) = 5
