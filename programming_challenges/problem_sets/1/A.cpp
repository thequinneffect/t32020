#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

int main() {

    // read in the lines
    string line;
    while (getline(cin, line)) {

        // split the lines into the two words
        istringstream stream(line);
        string s1, s2;
        stream >> s1 >> s2;

        // loop through the second string (bigger string)
        int i = 0, s1_len = s1.length(), s2_len = s2.length();
        for (int j=0; j < s2_len; j++) {
            // if we find a match for the current char then move on
            if (s2[j] == s1[i]) {
                i++;
                // if we found a match and it was the last char needed, then s1 is a subset of s2
                if (i == s1_len) {
                    cout << "Yes" << endl;
                    break;
                }
            } else if (j == s2_len-1) { // if we make it to the end without a match, then no match
                cout << "No" << endl;
            }
        }
        
    }

}