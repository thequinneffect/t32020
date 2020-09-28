#include <iostream>
#include <queue>

using namespace std;
typedef pair<int, int> pi;
#define x first
#define y second

int main() {

    cin.tie(nullptr);
    cin.sync_with_stdio(false);

    // handle input
    int n;
    cin >> n;

    char board[n][n] = {0};
    pi start;
    pi receiver;
    char start_vector;
    char inverse_start_vector;
    for (int r=0; r < n; r++) {
        for (int c=0; c < n; c++) {
            char tile;
            cin >> tile;
            if (tile == '*') {
                receiver.x = r;
                receiver.y = c;
            }
            if (tile == '>' || tile == '<' || tile == '^' || tile == 'v') {
                start.x = r;
                start.y = c;
                start_vector = tile;
                switch (start_vector) {
                    case '<':
                        inverse_start_vector = '>';
                    break;
                    case '>':
                        inverse_start_vector = '<';
                    break;
                    case '^':
                        inverse_start_vector = 'v';
                    break;
                    case 'v':
                        inverse_start_vector = '^';
                    break;
                }
            }
            board[r][c] = tile;
        }
    }

    // input handled, solve

    queue<pair<pi, char>> moves;
    moves.push(pair<pi, char>(receiver, '<'));
    moves.push(pair<pi, char>(receiver, '>'));
    moves.push(pair<pi, char>(receiver, '^'));
    moves.push(pair<pi, char>(receiver, 'v'));
    pi curr_tile;
    while (!moves.empty()) {

        // pop current pos from queue
        curr_tile = moves.front().first;
        char curr_vector = moves.front().second;
        moves.pop();
        if (curr_tile == start && curr_vector == inverse_start_vector) {
            // win
            cout << "YES" << endl;
            return 0;
        }

        // push possible moves from current tile to queue
        int r = curr_tile.x, c = curr_tile.y;

        // check if we are on a mirror because in this case
        // we dont get to move freely
        if (board[r][c] == '/') {
            switch(curr_tile.second) {
                case '<':
                    curr_tile.second = 'v';
                break;
                case '>':
                    curr_tile.second = '^';
                break;
                case '^':
                    curr_tile.second = '>';
                break;
                case 'v':
                    curr_tile.second = '<';
                break;
            }
        } else if (board[r][c] == '\\') {
            switch(curr_tile.second) {
                case '<':
                    curr_tile.second = '^';
                break;
                case '>':
                    curr_tile.second = 'v';
                break;
                case '^':
                    curr_tile.second = '<';
                break;
                case 'v':
                    curr_tile.second = '>';
                break;
            }
        }

        // left
        if (curr_tile.second == '<' && c-1 >= 0) { // check if left is still on board
            moves.push(pair<pi, char>(pi(r, c-1), '<'));
        }

        // right
        if (curr_tile.second == '>' && c+1 < n) { // check if right is still on board
            moves.push(pair<pi, char>(pi(r, c+1), '>'));
        }

        // up
        if (curr_tile.second == '^' && r-1 >= 0) { // check if up is still on board
            moves.push(pair<pi, char>(pi(r-1, c), '^'));
        }

        // down
        if (curr_tile.second == 'v' && r+1 < n) { // check if down is still on board
            moves.push(pair<pi, char>(pi(r+1, c), 'v'));
        }

    }
     
    cout << "NO" << endl;
    return 0;

}