#include <iostream>

using namespace std;

typedef pair<int, int> pi;

int n;
pi current_tile;
char current_direction;
pair<char, bool> board[100][100];
pi start;
pi receiver;
char start_vector;
char inverse_start_vector;

bool is_valid(pi tile) {
    if (current_tile.first >= 0 
        && current_tile.first < n
        && current_tile.second >= 0
        && current_tile.second < n) {
            return true;
        }
        return false;
}

bool shoot_laser() {        
    while (is_valid(current_tile)) {

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

        // check if we are on the winning tile
        //cout << "current tile:" << current_tile.first << ", " << current_tile.second << endl;
        //cout << "current direction: " << current_direction << ", inverse start direction: " << inverse_start_vector << endl;

        if (current_tile == start && current_direction == inverse_start_vector) {
            cout << "YES\n";
            return true;
        }
    }
}


int main() {

    // handle input
    cin.tie(nullptr);
    cin.sync_with_stdio(false);
    
    cin >> n;

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
            board[r][c].first = tile;
        }
    }

    // moving left, right, up, or down from the receiver will either
    // lead you to the start or off the board

    //cout << "receiver tile: " << receiver.first << ", " << receiver.second << endl;
    //cout << "start tile: " << start.first << ", " << start.second << endl;

    current_tile = receiver;
    current_direction = '<';
    if (shoot_laser()) return 0;
    //cout << "< was invalid" << endl;

    current_tile = receiver;
    current_direction = '>';
    if (shoot_laser()) return 0;
    //cout << "> was invalid" << endl;

    current_tile = receiver;
    current_direction = '^';
    if (shoot_laser()) return 0;
    //cout << "^ was invalid" << endl;

    current_tile = receiver;
    current_direction = 'v';
    if (shoot_laser()) return 0;
    //cout << "v was invalid" << endl;


    cout << "NO\n";
}