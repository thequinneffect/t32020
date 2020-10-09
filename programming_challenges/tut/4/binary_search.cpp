#include <cstdio>
 
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
    // Find the smallest x in [0,30] such that f(x) is true.
    printf("%d\n", binarysearch());
}