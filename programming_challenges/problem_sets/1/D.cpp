#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int num_currencies;
vector<int> currencies;

int main() {

    // read in the number of cases
    int num_cases;
    cin >> num_cases;

    for (int i=0; i < num_cases; i++) {

        // read in the number of denominations
        cin >> num_currencies;

        // read in the actual denominations
        currencies.clear();
        for (int i=0; i < num_currencies; i++) {
            int currency;
            cin >> currency;
            currencies.push_back(currency);
        }

        // solution here
        int denominations_used = 0;
        for (int i=0; i < num_currencies; i++) {

            int coins_used = 0, sum = 0;
            for (int j=i; j < num_currencies; j++) {
                if (sum < currencies[j] && ((j == num_currencies-1) || (sum+currencies[j] < currencies[j+1]))) {
                    coins_used++;
                    sum += currencies[j];
                }
            }
            if (coins_used > denominations_used) {
                denominations_used = coins_used;
                if (denominations_used == num_currencies) break;
            }

        }
        cout << denominations_used << endl;
    }

}

