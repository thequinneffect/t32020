#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

int main() {

    // handle input
    int n, k;
    cin >> n >> k;

    vector<pair<int, int>> times;
    for (int i=0; i < n; i++) {
        pair<int, int> e;
        cin >> e.first >> e.second;
        times.push_back(e);
    }

    // solve

    // 1. sort events based on earliest end time
    sort(times.begin(), times.end(), [](pair<int, int> &left, pair<int, int> &right) {
        return left.second < right.second;
    });

    // 2. loop through all event times, and maintain up to k end times
    // each representing the end time of the latest event for a room
    multiset<int> end_times;
    end_times.insert(times[0].second); // init with first event
    int rooms_used = 1, events_booked = 1; // first event uses 1 room

    for (int i=1; i < n; i++) {
        pair<int, int> e = times[i];
        // check if the current event can be booked in a room
        // that already has events in it (i.e. starts after
        // the last event ends i.e. after one of the stored end times)
        auto it = end_times.lower_bound(e.first);
        if (it != end_times.begin()) it--;
        //if (it != end_times.end() && *it < e.first) cout << "first k less than is: " << *it << " vs " << e.first << endl;
        if (it != end_times.end() && *it < e.first) {
            // it does, so this event can be added to an already used room
            end_times.erase(it);
            end_times.insert(e.second);
            events_booked++;
            //cout << "using already booked room\n";
        } else if (rooms_used < k) {
            // we can put this event in a new room
            events_booked++;
            rooms_used++;
            end_times.insert(e.second);
            //cout << "using new room\n";
        }
    }

    cout << events_booked << endl;

}