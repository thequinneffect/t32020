#include <iostream>
#include <vector>
#include <set>

using namespace std;

unsigned long long n, k;
string s, t;

int main() {

    cin >> n >> k >> s >> t;
    //cout << "n: " << n << ", k: " << k << endl << s << endl << t << endl;

    // if only 1 string was written down, then there are by definition only len(string)
    // prefixes possible (and n is the length of the string)
    if (k == 1) {
        cout << n << endl;
        return 0;
    }   

    int nstrings = 0;
    
    
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

    k=5, so 5 missing strings, but we can only make 2;
    abbb
    baaa

    here the s and t don't allow us to generate all 5 missing strings, so some are truly lost
    so k serves as a maximum for amount of strings you are allowed to generate, not the 
    actual amount to generate

    prefixes are: a, b, ab, ba, abb, baa, abbb, baaa

    - the last remainging problem is: if i can generate > k strings, which generated strings
    do i pick? the obvious answer is the strings that maximise c, but how do you decide that?

    revisit problem 1;
    we could generate these 4 strings from s=aa and t=bb, but say k=1,2,or 3 (what do we pick)    

    1. aa -> a, aa
    2. ab -> a, ab
    3. ba -> b, ba
    4. bb -> b, bb

    all 4 strings have 2 prefixes, but some of these are shared! 

    if k=1, we can pick any string because 1 string has len prefixes no matter what string it is
    if k>1, we **want to pick strings that differ asap**, because then they wont share prefixes
    and so will generate more, thus increasing our c.

    YOU WANT STRINGS THAT DIFFER AS SOON AS POSSIBLE, BECAUSE WHILST THEY DON'T DIFFER, THEY
    SHARE PREFIXES. AS SOON AS THEY DIFFER IN 1 SLOT, THE PREFIX IS UNIQUE

    General method;

    - loop through s and t simultaneously. Whilst they are the same, you have no choice on
    what character to use. IF they differ, then you can pick either the lower bound S, or
    the upper bound T. Note that if they do differ, only choose the differing one.

    - if you pick the lower bound (s[i]), then from here on out you have to make sure that you
    are >= the lower bound (you can ignore the upper bound string t now because the
    character you chose when they differed means you are already lexico < than t)
        - basically, if you get an 'b' then you only have 1 choice (b), if you get a 'a', you have
        2 choices (a or b)

    - if you pick the higher bound (t[i]), then from here on out you have to make sure that
    you are <= the upper bound. Likwise you can now ignore the lower bound string s because
    you are guaranteed lexico > than it. 
        - if you get an 'a', then you have 1 choice (a), if you get a b, then you have 2 choices (a or b)

    If they differ, add 2 strings of i length, representing
    the current substrings of s and t.
 */


