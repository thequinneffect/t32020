#include <iostream>

using namespace std;

int main() {

    // read in the input strings
    string s1, s2;
    cin >> s1 >> s2;

    int i = 0;
    // loop through the second string (super string)
    for (int j=0; j < s2.length(); j++) {
        // if we find a match for the current char then move on
        if (s2[j] == s1[i]) {
            i++;
            // if we found a match and it was the last char needed, then s2 is a superset
            if (i == s1.length()) {
                cout << "Yes\n";
                return 0;
            }
        }
    }
    cout << "No\n";

}