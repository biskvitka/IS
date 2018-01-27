#include <iostream>
#include <vector>





using namespace std;

const char emptyField = '_'; 

void printBoard(const vector<vector<char> > board) {
    for (int i = 0; i < board.size(); ++i) {
        for (int j = 0; j < board.size(); ++j) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

bool movesLeft(const vector<vector<char> > board) {
    for (int i = 0; i < board.size(); ++i) {
        for (int j = 0; j < board.size(); ++j) {
            if (board[i][j] == emptyField) {
                return true;
            }
        }
    }
    return false;
}

int getResult(const vector<vector<char> > board) {
    for (int i = 0; i < board.size(); ++i) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {  
            if (board[i][0] == 'X') {
                return 10;
            } else if (board[i][0] == 'O') {
                return -10;
            }
        } else if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            if (board[0][i] == 'X') {
                return 10;
            } else if (board[0][i] == 'O') {
                return -10;
            }
        }
    }

    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == 'X') {
            return 10;
        } else if (board[0][0] == 'O') {
            return -10;
        }
    }

    if (board[0][2] == board[2][0] && board[1][1] == board[2][0]) {
        if (board[1][1] == 'X') {
            return 10;
        } else if (board[1][1] == 'O') {
            return -10;
        }
    }
    return 0;
}


int minimax(vector<vector<char> > &board, int depth, int max, int min, bool maxTurn) {
    int result = getResult(board);
    if (result == 10) {
        return result - depth;
    } else if ( result == -10) {
        return result + depth;
    } else if (!movesLeft(board)) {
        return 0;
    }

    if (maxTurn) {
        for (int i = 0; i < board.size(); ++i) {
            for (int j = 0; j < board.size(); ++j) {
                if (board[i][j] != emptyField) {
                    continue;
                }
                board[i][j] = 'X';
                result = minimax(board, depth + 1, max, min, false);
                board[i][j] = emptyField;
                if (max < result) {
                    max = result;
                }
                if (max > min) {
                    return max;
                }
            }
        }
        return max;
    } else {
        for (int i = 0; i < board.size(); ++i) {
            for (int j = 0; j < board.size(); ++j) {
                if (board[i][j] != emptyField) {
                    continue;
                }
                board[i][j] = 'O';
                result = minimax(board, depth + 1, max, min, true);
                board[i][j] = emptyField;
                if (min > result) {
                    min = result;
                }
                if (max > min) {
                    return min;
                }
            }
        }
        return min;
    }
}


int main() {
    vector<vector<char> > board(3, vector<char>(3, emptyField));
    int row, col;
    printBoard(board);
    while(movesLeft(board)) {
        cout << "input a row number: ";
        cin >> row;
        cout << "input a col number: ";
        cin >> col;
        board[row][col] = 'O';
        int result;
        int max = -1000;
        short x, y;
        for (int i = 0; i < board.size(); ++i) {
            for (int j = 0; j < board.size(); ++j) {
                if (board[i][j] != emptyField) {
                    continue;
                }
                int min = 1000; 
                board[i][j] = 'X';
                result = minimax(board, 0, max, min, false);
                board[i][j] = emptyField;
                if (result > max) {
                    max = result;
                    x = i;
                    y = j;
                }
            }
        }
        board[x][y] = 'X';
        printBoard(board);
        result = getResult(board);
        if (result != 0) {
            if (result < 0) {
                cout << "you win!" << endl;
            } else {
                cout << "you can't beat!" << endl;
            }
            return 0;
        }
    }
    return 0;
}


        
