#include <iostream>
#include <cstdio>

using namespace std;
typedef long long ll;

int f(int x) {
    return x >= 12;
}
 
int binarysearch() {
    int l = -1; //invariant: f(l) is false
    int r = 31; //invariant: f(r) is true
    while (l < r-1) {
        int m = (l+r)/2;
        if (f(m)) r = m;
        else l = m;
    }
    return r;
}
 
int main () {
    printf("%d\n", binarysearch());
}