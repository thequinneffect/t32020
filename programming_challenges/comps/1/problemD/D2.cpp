#include <iostream>
#include <vector>
#include <set>

using namespace std;

int n, c, m;
string code;
vector<struct qmark_info_s> qmark_info;
int total_forbidden = 0;
// triplets are uniquelly identified by their starting index
set<int> forbidden_triplets;

struct qmark_info_s {
    int index, digit;
    set<int> forbidden_triplets;
};

bool is_forbidden(int x) {
    if ((x ^ c) % m == 0) return true;
    return false;
}

// generates the triplet that starts with i
int triplet(int i) {
    string ss = code.substr(i, 3);
    int triplet = stoi(ss);
    //cout << "triplet is: " << triplet << endl;
    return triplet;
}

void set_triplet_info(int i) {

    // clear current info
    qmark_info[i].forbidden_triplets.clear();

    // now set current information
    int l = qmark_info[i].index;
    string digit = code.substr(l, 1);
    qmark_info[i].digit = stoi(digit);

    // i can only be start of triplet if there are >= 2 chars afterwards
    if (l <= n-3) {
        if (is_forbidden(triplet(l))) {
            //cout << "1. forbidden triplet found: " << triplet(l) << endl;
            forbidden_triplets.insert(l);
            qmark_info[i].forbidden_triplets.insert(l);
        }
    }
    // i can only be middle of triplet if there are >= 1 chars either side
    if (l > 0 && l < n-1) {
        if (is_forbidden(triplet(l-1))) {
            //cout << "2. forbidden triplet found: " << triplet(l-1) << endl;
            forbidden_triplets.insert(l-1);
            qmark_info[i].forbidden_triplets.insert(l-1);
        }
    }
    // i can only be end of triplet if there are >= 2 chars before it
    if (l >= 2) {
        if (is_forbidden(triplet(l-2))) {
            //cout << "3. forbidden triplet found: " << triplet(l-2) << endl;
            forbidden_triplets.insert(l-2);
            qmark_info[i].forbidden_triplets.insert(l-2);
        }
    }
}

// i is qmark_info index
int swap(int swapee_i) {
    qmark_info_s swapee_info = qmark_info[swapee_i];
    int max = 0;
    string best_code;
    set<int> best_forbidden;
    vector<qmark_info_s> best_qmark_states;
    vector<qmark_info_s> old_qmark_states;
    set<int> old_forbidden_triplets;
    // loop through 
    for (int i=0; i < qmark_info.size(); i++) {
        // skip swapping with yourself
        if (i == swapee_i) continue;

        qmark_info_s swapper_info = qmark_info[i];
        // no point doing a swap if they are the same digit
        if (swapee_info.digit == swapper_info.digit) continue;

        // otherwise, do the swap and test whether total nforbidden decreased
        // cout << "attempting to swap: " << swapee_info.digit << " at index " << swapee_info.index
        //     << " with " << swapper_info.digit << " at index " << swapper_info.index << endl;

        // first, save sets so we can restore original state
        old_qmark_states = qmark_info;
        old_forbidden_triplets = forbidden_triplets;

        int original_forbidden = forbidden_triplets.size(); // global forbidden set
        //cout << "SWAP: original forbidden: " << original_forbidden << endl;
        // remove forbidden triplets related to these two
        //for (auto it = swapee_info.forbidden_triplets.begin(); it != swapee_info.forbidden_triplets.end(); it++) forbidden_triplets.erase(*it);
        //for (auto it = swapper_info.forbidden_triplets.begin(); it != swapper_info.forbidden_triplets.end(); it++) forbidden_triplets.erase(*it);

        // do the actual swap
        code.replace(swapper_info.index, 1, to_string(swapee_info.digit));
        code.replace(swapee_info.index, 1, to_string(swapper_info.digit));

        // recalculate the local and global forbidden triplets
        forbidden_triplets.clear();
        for (int i=0; i < qmark_info.size(); i++) {
            set_triplet_info(i);
        }

        //set_triplet_info(swapee_i);
        //set_triplet_info(i);
        int new_forbidden = forbidden_triplets.size();
        int score = original_forbidden - new_forbidden;
        //cout << "SWAP: new forbidden: " << new_forbidden << endl;
        if (score > max) {
            // this is the new best swap, save its state
            best_code = code;
            best_forbidden = forbidden_triplets;
            best_qmark_states = qmark_info;
            max = score;
        }
        // revert
        code.replace(swapper_info.index, 1, to_string(swapper_info.digit));
        code.replace(swapee_info.index, 1, to_string(swapee_info.digit));
        qmark_info = old_qmark_states;
        forbidden_triplets = old_forbidden_triplets;
        //cout << "swap failed" << endl;
        
    }
    // if we found an optimal swap, do it
    if (max > 0) {
        code = best_code;
        qmark_info = best_qmark_states;
        forbidden_triplets = best_forbidden;
    }
    return max;
}

// on second thought, n isn't known until runtime so this wont be compile time evaluated LOL
// constexpr int factorial(int n) {
//     if (n <= 1) return 1;
//     return n * factorial(n-1);
// }

int main() {

    // n is length of code, c and m are constants to determine forbiddenness
    cin >> n >> c >> m;

    int missing[10];
    int total_missing = 0;
    for (int i=0; i < 10; i++) {
        int n;
        cin >> n;
        missing[i] = n;
        total_missing += n;
    }

    cin >> code;

    // cout << "n: " << n << ", c: " << c << ", m: " << m << "num quesion marks: " << total_missing << endl;
    // for (int i=0; i < 10; i++) {
    //     cout << missing[i] << ", ";
    // }
    // cout << endl << code << endl;

    // solution
 
    // for each character in the code
    int number_index = 0;
    for (int i=0; i < n; i++) {
        
        // if we find a ?
        if (code[i] == '?') {
            //cout << "? found at slot " << i << endl;
            struct qmark_info_s info;
            info.index = i;

            // fill it with next available number
            for (; number_index < 10; number_index++) {
                if (missing[number_index] != 0) {
                    code.replace(i, 1, to_string(number_index));
                    missing[number_index]--;
                    //info.digit = number_index;
                    break;
                }
            }
            qmark_info.push_back(info);
        }
    }

    // if (n < 3) {
    //     cout << code << endl;
    //     return 0;
    // }

    //cout << "code after random fill: " << code << endl;

    // set the global forbidden triplets set and the qmark info specific ones
    //cout << "printing qmark info; " << endl;
    for (int i=0; i < qmark_info.size(); i++) {

        set_triplet_info(i);

        //struct qmark_info_s info = qmark_info[i];
        //cout << "index: " << info.index << ", digit: " << info.digit << ", nforbidden: " << qmark_info[i].forbidden_triplets.size() << endl;

    }

    // cout << "n forbidden triplets (global): " << forbidden_triplets.size() << ";" << endl; 
    // for (auto it = forbidden_triplets.begin(); it != forbidden_triplets.end(); it++) {
    //     int index = *it;
    //     cout << "forbidden triplet at index " << index << endl;
    // }
    

    //now perform swaps whilst state improves until a solution is reached or state doesn't improve
    while (forbidden_triplets.size() > 0) {
        int delta_total_forbidden = 0;
        for (int i=0; i < qmark_info.size(); i++) {
            // if we find a forbidden triplet, swap with other digits until better state is reached
            if (qmark_info[i].forbidden_triplets.size() > 0) {
                delta_total_forbidden += swap(i);
            }
        }
        //cout << "MADE IT HERE, delta total forbidden is: " << delta_total_forbidden << endl;
        if (delta_total_forbidden == 0) {
            cout << "-1\n";
            return 0;
        }
    }

    cout << code << endl;
    
}