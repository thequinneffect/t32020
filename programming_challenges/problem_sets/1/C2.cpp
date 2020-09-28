#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>

using namespace std;

int main() {

    // get number of test cases
    int num_test_cases = 0;
    cin >> num_test_cases;

    // for each test case
    for (int i=0; i < num_test_cases; i++) {

        // retrieve n, t, m
        int n, t, m;
        cin >> n >> t >> m;

        // read in car times
        vector<int> car_times;
        for (int i=0; i < m; i++) {
            int car_time;
            cin >> car_time;
            car_times.push_back(car_time);
        }

        // input handled, time to solve.
        
        

    }

}
