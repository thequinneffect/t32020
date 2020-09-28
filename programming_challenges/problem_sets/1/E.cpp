#include <iostream>

using namespace std;

int main() {

    unsigned long long n, k;
    cin >> n >> k;

    string s, t;
    cin >> s >> t;

    cout << "n: " << n << ", k: " << k << endl << s << endl << t << endl;
    
}

/* Notes

    - k strings, all of length n, all consisting of 'a' and 'b' were written down then lost. 
    Where does k come into play?
    - c is the number of other strings that prefix >= 1 of the k strings
    - the k strings are not known, but you do know a lexicographically smaller and larger
    string than every k, respecitvely s and t. i.e. s serves as a lower bound for each char,
    t serves as an upper bound for every char.


    Example
    n=2 k=4 (i.e. strings that nut wrote were of length 2, as are the the s and t we are given)
    s=aa (i.e. the lower bound for char 0 is a, same with char 1)
    t=bb (i.e. the upper bound for char 1 is b, same with char 2)

    4 strings, where the min string is aa and the max is bb
    hence they were;
    1. aa
    2. ab
    3. ba
    4. bb
    these are the only 4 possibilities
    hence the string 'a' prefixes 1 and 2, 'aa' -> 1, 'ab' -> 2, 'ba' -> 3, 'bb' -> 4, 'b' -> 3, 4
    hence c = 6, a aa ab ba bb b

    note that the above scenario is somewhat easy because there are k=4 missing strings and
    there are only 4 possible strings, hence we know exactly what strings to test.

    3 3
    aba
    bba

    in this example, k=3, so there were only 3 missing strings

    aba
    baa (because b > a in the first slot so this is alreay lexicographically larger after the first slot)
    bba

    again, we generated all of them
    'a' is a prefix, so is 'b', 'ab', 'ba', 'bb', 'aba', 'baa' and 'bba' = 8
    - here we looped through and added to a set all strings of len 1, then len 2, etc for all missing strings

    4 5
    abbb
    baaa

    k=5, so 5 missing strings

    abbb
    aaaa
    baaa

    here the s and t don't allow us to generate all 5 missing strings, so some are truly lost
    so k serves as a maximum for amount of strings you are allowed to generate, not the 

 */