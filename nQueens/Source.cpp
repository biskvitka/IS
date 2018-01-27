#include <iostream>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <vector>
#include <memory>
#include <list>

using namespace std;


int myRandom(int i) { return rand() % i; }

void printBoard(const vector<int> &v) {
    for (int i = 0; i < v.size(); ++i) {
        for (int j = 0; j < v.size(); ++j) {
            char symbol = ((v[j] == i) ? '*' : '_'); 
            cout << symbol << " ";
        }
        cout << endl;
    }
}

int findConflicts(const vector<int> &v, vector<int> &conflicts) {
    int countMaxConflicts = 0;
    int size = v.size() - 1;
    int maxConflictsInd = 0;
    for (int i = 0; i < size; ++i) {
        for (int j = i + 1; j < (size + 1); ++j) {
            if ((v[i] == v[j]) || ((j - i) == abs(v[i] - v[j]))) {
                conflicts[j] = conflicts[j] + 1;
                conflicts[i] = conflicts[i] + 1;
            }
        }
        if (conflicts[i] > countMaxConflicts) {
            countMaxConflicts = conflicts[i];
            maxConflictsInd = i;
        } else if (conflicts[i] == countMaxConflicts && (rand() % 2)) {
            maxConflictsInd = i;
        }
    }
    if (conflicts[size] > countMaxConflicts || (conflicts[size] == countMaxConflicts && (rand() % 2))) {
        maxConflictsInd = size;
    }
    return maxConflictsInd;
}


int findMinConflicts(vector<int> &v, vector<int> &conflicts, int maxConflictsInd) {
    int maxConflicts = conflicts[maxConflictsInd];
    int size = conflicts.size();
    
    int minConflicts = size + 2;
    int minConflictsInd = maxConflictsInd;
    for (int j = 0; j < size; ++j) {
        //find minConflicts
        if (j != v[maxConflictsInd]) {
            int conflicts = 0;
            for (int i = 0; i < size; ++i) {
                if (i == maxConflictsInd) {
                    continue;
                }
                if ((v[i] == j) || (abs(j - v[i]) == abs(maxConflictsInd - i))) {
                    conflicts++;
                }
                if (conflicts > minConflicts) {
                    break;
                }
            }
            if (conflicts < minConflicts) {
                minConflicts = conflicts;
                minConflictsInd = j;
            } else if (conflicts == minConflicts && (rand() % 2)) {
                minConflictsInd = j;
            }
        } else {
            //remove previous conflicts for that queen
            for (int i = 0; i < size; ++i) {
                if (i == maxConflictsInd) {
                    continue;
                }
                
                if ((v[i] == v[maxConflictsInd]) || (abs(v[maxConflictsInd] - v[i]) == abs(i - maxConflictsInd))) {
                    conflicts[i] = conflicts[i] - 1;
                }
            }
        }
    }
    v[maxConflictsInd] = minConflictsInd;
    conflicts[maxConflictsInd] = minConflicts;
    int newMaxConflictsInd = 0;
    maxConflicts = 0;
    for (int i = 0; i < size; ++i) {
        
        if (i == maxConflictsInd) {
            continue;
        }
        if (v[i] == minConflictsInd || (abs(maxConflictsInd - i) == abs(v[i] - minConflictsInd))) {
            conflicts[i] += 1;
        }
        if (maxConflicts < conflicts[i]) {
            maxConflicts = conflicts[i];
            newMaxConflictsInd = i;
        } else if (maxConflicts == conflicts[i] && (rand() % 2)) {
            newMaxConflictsInd = i;
        }
    }
    return newMaxConflictsInd;
}

int createInitialBoard(vector<int> &board, vector<int> &conflicts) {
    int size = board.size();
    for (int i = 0; i < size; ++i) {
        board[i] = i;
        conflicts[i] = 0;
    }
    random_shuffle(board.begin(), board.end(), myRandom);

    int maxConflictsInd = findConflicts(board, conflicts);
    return maxConflictsInd;
}


void solve(vector<int> &v, vector<int> &conflictsVector, int maxConflictsInd) {
    int moves = 0;
    int conflictsInd = maxConflictsInd;
    while (true) {
        ++moves;
        conflictsInd = findMinConflicts(v, conflictsVector, conflictsInd);
        if (conflictsVector[conflictsInd] == 0) {
            break;
        }
    

        if (moves == (v.size() * 2)) {
            conflictsInd = createInitialBoard(v, conflictsVector);
            if(conflictsVector[conflictsInd] == 0) {
                break;
            }
            moves = 0;
        }
    }
}



int main() {
    srand(time(0));
    int N;
    cin >> N;
    vector<int> board(N);
    vector<int> conflicts(N);
    int maxConflictsInd = createInitialBoard(board, conflicts);
    if (conflicts[maxConflictsInd] != 0){
        solve(board, conflicts, maxConflictsInd);
    }
    printBoard(board);

    return 0;
}
