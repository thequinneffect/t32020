#include <iostream>
#include <set>
#include <vector>

using namespace std;

// sort based on profit, then based on cost
struct house_sale {
    int house_number;
    int profit;
    int cost;
};

struct comparator
{
    bool operator()(const house_sale &h1, const house_sale &h2 ) const {
        if (h1.profit > h2.profit) return true;
        else if (h1.profit < h2.profit) return false;
        else if (h1.cost < h2.cost) return true; // at this stage, profits are equal
        else if (h1.cost > h2.cost) return false;
        else if (h1.house_number > h2.house_number) return true;
        else return false;
    }
};

int main() {

    int h, y, m;
    cin >> h >> y >> m;

    int prices[h][y+1];
    for (int i=0; i < h; i++) {
        for (int j=0; j <= y; j++) {
            int p;
            cin >> p;
            prices[i][j] = p;
        }
    }

    // cout << "houses: " << h << ", years: " << y << ", money: " << m << endl;
    // for (int i=0; i < h; i++) {
    //     for (int j=0; j <= y; j++) {
    //         cout << prices[i][j] << ", ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;

    set<house_sale, comparator> sales;
    vector<house_sale> owned_houses;
    
    // for all years except the last one
    for (int curr_y = 0; curr_y <= y; curr_y++) {

        // sell currently owned houses
        //cout << "selling;\n";
        for (auto house : owned_houses) {
            //cout << "house no.: " << house.house_number << ", for: " << prices[house.house_number][curr_y] << "\n";
            m += prices[house.house_number][curr_y];
        }
        // cout << "\n";
        owned_houses.clear();

        // final year, this is money generated
        if (curr_y == y) {
            //cout << "final year, total: ";
            cout << m << endl;
            return 0;
        }

        // calculate profits for each house for the next year
        for (int i=0; i < h; i++) {
            struct house_sale sale = {
                i,
                prices[i][curr_y+1] - prices[i][curr_y],
                prices[i][curr_y]
            };
            //cout << "profit calculation: house no.: " << sale.house_number << ", profit: " << sale.profit << ", cost: " << sale.cost << "\n";
            // only store sales that you make money on!
            if (sale.profit > 0) {
                sales.insert(sale);
            }   
        }

        // cout << "here are the " << sales.size() << " options;\n";
        // for (auto it = sales.begin(); it != sales.end(); it++) {
        //     struct house_sale option = *it;
        //     cout << "profit: " << option.profit << ", cost: " << option.cost << ", house no.: " << option.house_number << "\n";
        // }        
        // cout << "\n";

        // buy as many houses as possible
        int bought = 0;
        for (auto it = sales.begin(); it != sales.end(); it++) {
            struct house_sale sale = *it;
            if (sale.cost <= m) {
                owned_houses.push_back(sale);
                m -= sale.cost;
                if (++bought == 2) break;
            }
        }

        // clear current options
        sales.clear();
    }

    return 0;
}

/* Notes

    - you can pretend the inital buy is as if you just sold all your houses and now have m dollars
    reducing the problem to the same case every time.

    - calculate the profit to be made by each house over the next year by doing next_year_price - this_year_price
    and sort based on this profit. Note that multiple houses could have the same profit margin, but some of those
    could be cheaper than the others. Hence you want to sort based on profit and then based on cost.
    - then you want to buy as many houses are you can afford from this sorted order. 
    - At the start of the next year, sell all houses (even if it is optimal to keep holding it, you will just
    buy it back again)

    EXAMPLE
    h=3 y=2 m=20
    5 9 8
    10 12 16
    12 15 17

    calculate the profit margins;
    h1: 9-5=4, h2: 12-10=2, h3: 15-12=3
    hence profit,cost order of houses is 1,3,2

    buy from this list until we cannot anymore i.e.
    m = 20, 20-5=15, 15-12=3. So we bought house 1 and 3 and have m=3 dollars left

    --- new year ---
    sell all owned houses
    m = 3+9+15 = 27

    calculate profit margins;
    h1: 8-9=-1, h2: 16-12 = 4, h3: 17-15=2
    hence order is h2, h3, h1

    buy from this until we cannot anymore
    m=27, 27-12=15, 15-15=0. So we bought house 2 and 3 and have 0 dollars left

    --- new year ----
    sell all houses, final year so cannot buy again, total is 17+16 = 33

*/