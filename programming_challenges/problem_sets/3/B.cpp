#include <iostream>

using namespace std;

int c[100];
int p[100][100];

int main() {

    int n,m,k;
    cin >> n >> m >> k;
    for (int i=0; i < n; i++) cin >> c[i];
    for (int i=0; i < n; i++)
        for (int j=0; j < m; j++) cin >> p[i][j];

    /*
    cout << n << " " << m << " " << k << endl;
    for (int i=0; i < n; i++) cout << c[i] << " ";
    cout << endl;
    for (int i=0; i < n; i++) {
        for (int j=0; j < m; j++) cout << p[i][j] << " ";
        cout << "\n";
    }
    */
        

}