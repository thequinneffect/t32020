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
    int index, digit, ntriplets, nforbidden;
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

    // start by resetting existing qmark info
    qmark_info[i].ntriplets = 0;
    qmark_info[i].nforbidden = 0;

    // now set current information
    int l = qmark_info[i].index;

    // i can only be start of triplet if there are >= 2 chars afterwards
    if (l <= n-3) {
        qmark_info[i].ntriplets++;
        if (is_forbidden(triplet(l))) {
            cout << "1. forbidden triplet found: " << triplet(l) << endl;
            qmark_info[i].nforbidden++;
            forbidden_triplets.insert(l);
        }
    }
    // i can only be middle of triplet if there are >= 1 chars either side
    if (l > 0 && l < n-1) {
        qmark_info[i].ntriplets++;
        if (is_forbidden(triplet(l-1))) {
            cout << "2. forbidden triplet found: " << triplet(l-1) << endl;
            qmark_info[i].nforbidden++;
            forbidden_triplets.insert(l-1);
        }
    }
    // i can only be end of triplet if there are >= 2 chars before it
    if (l >= 2) {
        qmark_info[i].ntriplets++;
        if (is_forbidden(triplet(l-2))) {
            cout << "3. forbidden triplet found: " << triplet(l-2) << endl;
            qmark_info[i].nforbidden++;
            forbidden_triplets.insert(l-2);
        }
    }
}

// i is qmark_info index
int swap(int swapee_i) {
    qmark_info_s swapee_info = qmark_info[swapee_i];
    // loop through 
    for (int i=0; i < qmark_info.size(); i++) {
        // skip swapping with yourself
        if (i == swapee_i) continue;

        qmark_info_s swapper_info = qmark_info[i];
        // no point doing a swap if they are the same digit
        if (swapee_info.digit == swapper_info.digit) continue;

        // otherwise, do the swap and test whether total nforbidden decreased
        cout << "attempting to swap: " << swapee_info.digit << " at index " << swapee_info.index
            << " with " << swapper_info.digit << " at index " << swapper_info.index << endl;
        int swapper_nforbidden = swapper_info.nforbidden;
        int swapee_nforbidden = swapee_info.nforbidden;
        int original_forbidden = swapper_nforbidden + swapee_nforbidden;

        code.replace(swapper_info.index, 1, to_string(swapee_info.digit));
        code.replace(swapee_info.index, 1, to_string(swapper_info.digit));

        set_triplet_info(swapee_i);
        set_triplet_info(i);
        int new_forbidden = qmark_info[swapee_i].nforbidden + qmark_info[i].nforbidden;

        if (new_forbidden < original_forbidden) {
            // this much change in total forbidden
            cout << "swap granted" << endl;
            return original_forbidden - new_forbidden;
        } else {
            // revert
            code.replace(swapper_info.index, 1, to_string(swapper_info.digit));
            code.replace(swapee_info.index, 1, to_string(swapee_info.digit));
            qmark_info[swapee_i].nforbidden = swapee_nforbidden;
            qmark_info[i].nforbidden = swapper_nforbidden; 
            cout << "swap failed" << endl;
        }
    }
    // didn't find any swap, hence no change in total forbidden
    return 0;
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
    int used[10] = {0};
    int total_missing = 0;
    for (int i=0; i < 10; i++) {
        int n;
        cin >> n;
        missing[i] = n;
        total_missing += n;
    }

    cin >> code;

    cout << "n: " << n << ", c: " << c << ", m: " << m << endl;
    for (int i=0; i < 10; i++) {
        cout << missing[i] << ", ";
    }
    cout << endl << code << endl;

    cout << "total missing: " << total_missing << endl; 

    // solution
 
    // for each character in the code
    int number_index = 0;
    for (int i=0; i < n; i++) {
        
        // if we find a ?
        if (code[i] == '?') {
            cout << "? found at slot " << i << endl;
            struct qmark_info_s info;
            info.index = i;
            info.ntriplets = 0;
            info.nforbidden = 0;

            // fill it with next available number
            for (number_index; number_index < 10; number_index++) {
                if (missing[number_index] != 0) {
                    code.replace(i, 1, to_string(number_index));
                    missing[number_index]--;
                    used[number_index]++;
                    info.digit = number_index;
                    break;
                }
            }
            qmark_info.push_back(info);
        }
    }

    cout << "code after random fill: " << code << endl;

    // set up num triplets for each ? slot and how many are allowed (ALSO DEBUG PRINTING HERE, REMOVE IT LATER)
    cout << "printing qmark info; " << endl;
    for (int i=0; i < qmark_info.size(); i++) {

        set_triplet_info(i);
        total_forbidden += qmark_info[i].nforbidden;

        struct qmark_info_s info = qmark_info[i];
        cout << "index: " << info.index << ", digit: " << info.digit << ", triplets: " << info.ntriplets << ", nforbidden: " << info.nforbidden << endl;

    }

    cout << "n forbidden triplets: " << forbidden_triplets.size() << ";" << endl; 
    for (auto it = forbidden_triplets.begin(); it != forbidden_triplets.end(); it++) {
        int index = *it;
        cout << "forbidden triplet at index " << index << endl;
    }
    

    //now perform swaps whilst state improves until a solution is reached or state doesn't improve
    int delta_total_forbidden = 0;
    while (forbidden_triplets.size() > 0) {
        for (int i=0; i < qmark_info.size(); i++) {
            // if we find a forbidden triplet, swap with other digits until better state is reached
            if (qmark_info[i].nforbidden > 0) {
                delta_total_forbidden += swap(i);
            }
        }
        cout << "MADE IT HERE, delta total forbidden is: " << delta_total_forbidden << endl;
        if (delta_total_forbidden == 0) {
            cout << "-1\n";
            return 0;
        }
    }

    cout << code << endl;
    
}

/* Notes

    1. assign the digits first come first serve to the ? marks
    2. now you have a code with no ?'s, now look for forbidden triplets
    3. if you find a forbidden triplet, store the start index in a set
        - this is because a triplet is uniquelly determined by its start index
        - and a set means you wont double store a triplet
    4. if your set is empty, you are done
    5. 
*/