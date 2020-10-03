#include <iostream>

using namespace std;

int full_fill(int n);
int partial_fill(int n) {
    return (n==0 || n==1) ? n : partial_fill(n-2) + full_fill(n-1);
} 

int full_fill(int n) {
    if (n==0) return 1;
    if (n==1) return 0;
    return full_fill(n-2) + 2*partial_fill(n-1);
}

int main() {

    while (true) {

        int n;
        cin >> n;
        if (n == -1) return 0;

        if (n % 2 != 0 || n == 0) {
            cout << "0\n";
        } else {
            // solve tiling
            cout << full_fill(n) << endl;
        }
    }
}