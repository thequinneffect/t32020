#include <iostream>

using namespace std;

int main() {

    int n, k;
    cin >> n >> k;

    string squares;
    cin >> squares;

    // cout << "n: " << n << ", k: " << k << endl << "squares: " << squares << endl;

    int current_square = n, jumps = 0;
    while (current_square != 0) {
        int window_start = max(current_square - k, 0);
        for (int i=window_start;; i++) {
            if (i == current_square) {
                cout << "-1" << endl;
                return 0;
            } else if (squares[i] == '1') {
                current_square = i;
                jumps++;
                break;
            }
        }
    }
    cout << jumps << endl;

}