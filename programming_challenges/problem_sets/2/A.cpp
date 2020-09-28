#include <iostream>
#include <vector>
#include <stack>
#include <queue>

using namespace std;

int main() {

    int cases;
    cin >> cases;

    while (cases--) {

        // get input
        string expr;
        cin >> expr;
        int length = expr.size();
        
        // solve
        vector<pair<int, int>> tree;
        stack<int> s;
        for (int i=0; i < length; i++) {
            char ch = expr[i];
            if (isupper(ch)) {
                int r,l;
                r = s.top(); s.pop(); l = s.top(); s.pop();
                tree.push_back(pair<int, int>(l, r));
                s.push(i);
            } else {
                tree.push_back(pair<int, int>(-1,-1));
                s.push(i);
            }
        }

        queue<int> q;
        char solution[length + 1] = {0};
        int i = length-1;
        solution[i] = expr[i];
        q.push(i);
        i--;
        while (i >= 0) {
            int parent = q.front();
            q.pop();
            pair<int, int> children = tree[parent];
            if (children.first == -1) continue;
            solution[i--] = expr[children.first];
            solution[i--] = expr[children.second];
            q.push(children.first);
            q.push(children.second);
        }
        cout << solution << endl;
    }

}