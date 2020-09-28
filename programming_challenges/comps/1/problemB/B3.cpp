#include <iostream>
#include <map>

using namespace std;

typedef pair<int, int> pi;

int n;

bool is_valid(pi tile) {
    if (tile.first >= 0 
        && tile.first < n
        && tile.second >= 0
        && tile.second < n) {
            return true;
        }
        return false;
}

int main() {

    // handle input
    cin.tie(nullptr);
    cin.sync_with_stdio(false);

    cin >> n;

    pi start;
    pi receiver;
    char start_direction;

    pair<char, map<char, bool>> board[n][n];
    for (int r=0; r < n; r++) {
        for (int c=0; c < n; c++) {
            char tile;
            cin >> tile;
            if (tile == '*') {
                receiver.first = r;
                receiver.second = c;
            }
            if (tile == '>' || tile == '<' || tile == '^' || tile == 'v') {
                start.first = r;
                start.second = c;
                start_direction = tile;
            }
            board[r][c].first = tile;
            board[r][c].second['>'] = false;
            board[r][c].second['<'] = false;
            board[r][c].second['^'] = false;
            board[r][c].second['v'] = false;
        }
    }

    /* 
    
    init: at start facing in start direction
    
    while current tile valid;
    - mark seen for the current direction you are facing in
    - do move in direction you are facing in
    - check if at destination, if so -> YES
    - check if visited in this orientation before, if so -> NO

    loop ended -> NO
    */

    pi current_tile = start;
    char current_direction = start_direction;
    while (is_valid(current_tile)) {

        // check if we are on the winning tile or have already been here before
        if (current_tile == receiver) {
            cout << "YES\n";
            return 0;
        } else if (board[current_tile.first][current_tile.second].second[current_direction] == true) {
            cout << "NO\n";
            return 0;
        }

        // mark seen
        board[current_tile.first][current_tile.second].second[current_direction] = true;
        
        // move
        // if we are on a mirror then change direction appropriately
        if (board[current_tile.first][current_tile.second].first == '/') {
            switch(current_direction) {
                case '<':
                    current_direction = 'v';
                break;
                case '>':
                    current_direction = '^';
                break;
                case '^':
                    current_direction = '>';
                break;
                case 'v':
                    current_direction = '<';
                break;
            }
        } else if (board[current_tile.first][current_tile.second].first == '\\') {
            switch(current_direction) {
                case '<':
                    current_direction = '^';
                break;
                case '>':
                    current_direction = 'v';
                break;
                case '^':
                    current_direction = '<';
                break;
                case 'v':
                    current_direction = '>';
                break;
            }
        }

        // left
        switch(current_direction) {
            case '<':
                current_tile.second--;
            break;
            case '>':
                current_tile.second++; 
            break;
            case '^':
                current_tile.first--;
            break;
            case 'v':
                current_tile.first++;
            break;
        }
        //cout << "current tile:" << current_tile.first << ", " << current_tile.second << endl;
        //cout << "current direction: " << current_direction << ", inverse start direction: " << inverse_start_vector << endl;
    }

    cout << "NO\n";
}