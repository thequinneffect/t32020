#include <iostream>
#include <vector>

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

        // if the ferry can fit all cars then you can do it in 1 trip
        if (n >= m) {
            int earliest_leave_time = max(t, car_times[m-1]);
            cout << earliest_leave_time + t << " 1" << endl;
            continue;
        }

        // the only possible way to save time is to get back to the car side
        // earlier, hence leave earler. You must do at least ceil(m/n) trips
        // and so if there is any remainder it is better to do those rem
        // cars first because you want leave earlier.

        // the ferry must cross at the start to get to the cars
        int time_elapsed = 0;
        int cars_done = 0;
        int rtt = 2*t;
        int num_trips = 0;
        
        // whilst we still have cars to take across
        while (cars_done < m) {

            // take rem cars if there are rem, else take n 
            int rem = (m-cars_done) % n;
            int cars_to_take = rem ? rem : n;

            // do we have to wait for the last car of this load, or is it already there?
            int earliest_leave_time = max(time_elapsed, car_times[cars_done + cars_to_take - 1]);

            // take the load
            cars_done += cars_to_take;
            time_elapsed = earliest_leave_time + rtt;
            num_trips++;
        }

        cout << time_elapsed - t << " " << num_trips << endl;

    }

}