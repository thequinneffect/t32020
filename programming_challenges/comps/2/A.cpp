#include <iostream>

using namespace std;
typedef pair<int, int> pii;

pii trains[100100];

int main() {
    int n, t;
    cin >> n >> t;
    for (int i=0; i < n; i++) {
        cin >> trains[i].first >> trains[i].second;
    }

    // trip cannot be done
    int sum = 0;
    for (int i=0; i < n; i++) {
        sum += trains[i].first;
        if (sum > t) {
            cout << "-1\n";
            return 0;
        }
    }
    int current_time = 0;
    int curr_reviews = 0;
    int max_reviews = -1;
    for (int i=0; i < n-1; i++) {
        current_time += trains[i].first;
    }
}