#include <iostream>
#include <cmath>

using namespace std;

// a sequence of numbers can be defined by the start and end number i.e. 2,3,4,5 can be defined by 2...5
// the sum from 1 to n is (n(n+1))/2, and hence j to k is (k(k+1))/2 - (j(j+1))/2 
int seq_sum(int s, int e) {
    return (e-s+1)*(s+e)/2;
}

int main() {

    // get the input (number of cows)
    int n;
    cin >> n;

    // start count at 1 because n will always be a solution for n
    int cnt = 1;

    // the largest possible number that can be present in the sequence is ceil(n/2) because any
    // number greater than this wouldn't allow the next number (which is bigger by definition)
    // to equal n (it would always give a result > n)
    int max_e = ceil((double)(n/2));
    for (int i=1, j=2; i <= max_e;) {    
        int sum = seq_sum(i, j);
        if (sum == n) {
            cnt++;
            i++;
            j++;
        } else if (sum < n) {
            j++;
        } else { // sum > n
            i++;
        }
    }

    cout << cnt << endl;
}
