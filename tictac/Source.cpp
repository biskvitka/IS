#include <iostream>
#include <vector>
#include <utility>

using namespace std;


char emptyField = '_';
vector<vector<char> > board(3, vector<char>(3, emptyField));

void printBoard(vector<vector<char> > board);

bool movesLeft(vector<vector<char> > &board) {
    int s = board.size();
    for (int i = 0; i < s; ++i) {
        for (int j = 0; j < s; ++j) {
            if (board[i][j] == emptyField) {
                return true;
            }
        }
    }
    return false;
}

int getScore(vector<vector<char> > &b) {
    int s = b.size();

    // Checking for Rows for X or O victory.
    for (int row = 0; row<s; row++)
    {
        if (b[row][0] == b[row][1] && b[row][1] == b[row][2]) {
            if (b[row][0] == 'X') {
                return 10;
            } else if (b[row][0] == 'O') {
                return -10;
            }
        }
    }
 
    // Checking for Columns for X or O victory.
    for (int col = 0; col<s; col++) {
        if (b[0][col] == b[1][col] && b[1][col] == b[2][col]) {
            if (b[0][col] == 'X') {
                return 10;
             } else if (b[0][col] == 'O') {
                return -10;
             }
        }
    }
 
    // Checking for Diagonals for X or O victory.
    if (b[0][0] == b[1][1] && b[1][1] == b[2][2]) {
        if (b[0][0] == 'X') {
            return 10;
         } else if (b[0][0] == 'O') {
            return -10;
         }
    }
 
    if (b[0][2] == b[1][1] && b[1][1] == b[2][0]) {
        if (b[0][2] == 'X') {
            return 10;
        } else if (b[0][2] == 'O') {
            return -10;
        }
    }
 
    // Else if none of them have won then return 0
    return 0;
}

int minimax(vector<vector<char> > board, int depth, int &min, int &max, bool maxTurn) {
    int score = getScore(board);
    if (score == 10) {
        return score - depth;
    }
    if (score == -10) {
        return score + depth;
    }

    if (!movesLeft(board)) {
        return score;
    }
    
    int boardSize = board.size();

    if (maxTurn) {
        for (int i = 0; i < boardSize; ++i) {
            for (int j = 0; j < boardSize; ++j) {
                if (board[i][j] != emptyField) {
                    continue;
                }
                board[i][j] = 'X';
                int currentScore = minimax(board, depth + 1, min, max, false);
                cout << "min "<< min << endl;
                board[i][j] = emptyField;
                if (max < currentScore) {
                    max = currentScore;
                }
                if (max > min) {
                    return max;
                }
            }
        }   
        return max;
    } else {
        for (int i = 0; i < boardSize; ++i) {
            for (int j = 0; j < boardSize; ++j) {
                if (board[i][j] != emptyField) {
                    continue;
                }
                board[i][j] = 'O';
                int currentScore = minimax(board, depth + 1, min, max, true);
                cout << "score " << score << " max" << max << endl;
                board[i][j] = emptyField;
                if (min > currentScore) {
                    min = currentScore;
                }
                if (max > min) {
                    return min;
                }
            }
        }
        return min;
    }

}

pair<int, int> bestMove(vector<vector<char> > board) {
    int x, y;
    int max = -100;
    int score;
    for (int i = 0; i < board.size(); ++i) {
        for (int j = 0; j < board.size(); ++j) {
            int min = 100;
            if (board[i][j] != emptyField) {
                continue;
            }
            board[i][j] = 'X';
            score = minimax(board, 0, min, max, false);
            board[i][j] = emptyField;
            if (max < score) {
                max = score;
                x = i;
                y = j;
            }
        }
    }
    return make_pair(x, y);
}

void printBoard(vector<vector<char> > board) {
    for (int i = 0; i < board.size(); ++i) {
        for (int j = 0; j < board.size(); ++j) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    int steps = 0;
    vector<vector<char> > board(3, vector<char>(3, '_'));
    int x , y;
    int score = 0;
    while (score == 0 && steps <= 9) {
        printBoard(board);
        cout << "choose row: ";
        cin >> x;
        cout << "choose column: ";
        cin >> y;
        board[x][y] = 'O';
        pair<int, int> xandy = bestMove(board);
        score = getScore(board);
        if (score == -10 || steps >=7) {
            break;
        }
        board[xandy.first][xandy.second] = 'X';
        steps += 2;
        score = getScore(board);
    }
    
    printBoard(board);

    switch (score) {
        case -10: 
            cout << "You win!" << endl;
            break;
        case 10:
            cout << "You loose" << endl;
            break;
        default: cout << "Equal!" << endl;
    }


    return 0;
}
