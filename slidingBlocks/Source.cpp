#include <iostream>
#include <queue>
#include <list>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <math.h>
#include <utility>
#include <vector>
#include <memory>

using namespace std;

struct Node{
    vector<vector<int> > board;
    shared_ptr<Node> parentNode;
    vector<char> path;
    int f;
};

void printBoard(vector<vector<int> > board) {
    int size = board.size();
    for (int i = 0; i < size; ++i){
        for (int j = 0; j < size; ++j) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

int manhattanDistance(const vector<vector<int> > board) {
    int distance = 0;
    int size = board.size();
    for (int x = 0; x < size; ++x) {
        for(int y = 0; y < size; ++y) {
            int value = board[x][y];
			if (value) {
                --value;
                int origRow = floor(value / size);
                int origCol = value % size;
				distance += abs(x - origRow) + abs(y - origCol);
			}
        }
    }
    return distance;
}

list<vector<vector<int> > > closedBoards; 
auto cmp = [](Node left, Node right){
    return left.f > right.f;};
priority_queue<Node, vector<Node>, decltype(cmp)> possibleNodes(cmp);

int myrandom(int i) { return rand() % i; }

pair<int, int> findInversionsZeroPosition(const vector<int> boardArr) {
    int size = boardArr.size();
    int inversions = 0;
    int zeroPosition = 0;
    for (int i = 0; i < size - 1; ++i) {
        for (int j = i + 1; j < size; ++j) {
            if (!boardArr[j]) {
                zeroPosition = j;
            } else if (boardArr[i] > boardArr[j]) {
                ++inversions;
            }
        }
    }
    return make_pair(inversions, zeroPosition);
}

bool isSolvable(int size, pair<int, int> inversionZeroPosition) {
    int inversionsCount = inversionZeroPosition.first;
    int zeroPosition = inversionZeroPosition.second;
    int rowFromBottom = (!zeroPosition) ? size : (size - zeroPosition / size);
    bool oddSize = size % 2;
    bool validCase1 = oddSize && !(inversionsCount % 2);
    bool validCase2 = !oddSize && !(rowFromBottom % 2) && (inversionsCount % 2);
    bool validCase3 = !oddSize && rowFromBottom % 2 && !(inversionsCount % 2);
    return validCase1 || validCase2 || validCase3;
}

vector<vector<int> > createBoard(int size) {
    int N = size*size;
    vector<int> randArr(N);
    for (int i = 0; i < N; ++i) {
        randArr[i] = i;
    }
    bool solvable = false;
    while (!solvable) {
        random_shuffle(randArr.begin(), randArr.end(), myrandom);
        solvable = isSolvable(size, findInversionsZeroPosition(randArr));
    }
    int num = 0;
    vector<vector<int> > board(size, vector<int>(size));
    for (int i = 0; i < size; ++i){
        for (int j = 0; j < size; ++j, ++num) {
            board[i][j] = randArr[num];
        }
    }
    return board;
}

pair<int, int> returnZeroPosition(const vector<vector<int> >board) {
    int size = board.size();
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (!board[i][j]) {
                return make_pair(i, j);
            }
        }
    }
}

bool areEqual(const vector<vector<int> > board1, const vector<vector<int> > board2) {
    int boardSize = board1.size();
    for (int i = 0; i < boardSize; ++i){
        for (int j = 0; j < boardSize; ++j){
            if (board1[i][j] != board2[i][j]){
                return false;
            }
        }
    }
    return true;
}


bool isSolution(const vector<vector<int> >board) {
    int size = board.size();
    int num = 1;
    int maxNum = size * size;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (maxNum == num) {
                break;
            }
            if (board[i][j] != num) {
                return false;
            }
            ++num;
        }
    }
    return true;
}


void findChildren(const Node parent) {
    pair<int,int> zeroPosition = returnZeroPosition(parent.board);
    int i = zeroPosition.first;
    int j = zeroPosition.second;
    int size = parent.board.size();
    if (j < (size - 1)) {
        Node rChild;
        rChild.board = parent.board;
        rChild.board[i][j] = parent.board[i][j + 1];
        rChild.board[i][j + 1] = 0;
        bool equalBoards = false;
        for (auto it = closedBoards.begin(); it != closedBoards.end(); ++it ) {
            if (equalBoards = areEqual(*it, rChild.board)) {
                break;
            }
        }
        if (!equalBoards){
            rChild.path = parent.path;
            rChild.path.push_back('r');
            rChild.parentNode = make_shared<Node>(parent);
            rChild.f = manhattanDistance(rChild.board) + rChild.path.size();
            possibleNodes.push(move(rChild));
        }

    }

    if (j > 0) {
        Node lChild;
        lChild.board = parent.board;
        lChild.board[i][j] = parent.board[i][j - 1];
        lChild.board[i][j - 1] = 0;
        bool equalBoards = false;
        for (auto it = closedBoards.begin(); it != closedBoards.end(); ++it ) {
            if (equalBoards = areEqual(*it, lChild.board)) {
                break;
            }
        }
        if (!equalBoards){
            lChild.path = parent.path;
            lChild.path.push_back('l');
            lChild.parentNode = make_shared<Node>(parent);
            lChild.f = manhattanDistance(lChild.board) + lChild.path.size();
            possibleNodes.push(move(lChild));
        }

    }

    if (i > 0) {
        Node uChild;
        uChild.board = parent.board;
        uChild.board[i][j] = parent.board[i-1][j];
        uChild.board[i - 1][j] = 0;
        bool equalBoards = false;
        for (auto it = closedBoards.begin(); it != closedBoards.end(); ++it ) {
            if (equalBoards = areEqual(*it, uChild.board)) {
                break;
            }
        }
        if (!equalBoards) {
            uChild.path = parent.path;
            uChild.path.push_back('u');
            uChild.parentNode = make_shared<Node>(parent);
            uChild.f = manhattanDistance(uChild.board) + uChild.path.size();
            possibleNodes.push(move(uChild));
        }

    }

    if (i < (size - 1)) {
        Node dChild;
        dChild.board = parent.board;
        dChild.board[i][j] = parent.board[i+1][j];
        dChild.board[i + 1][j] = 0;
        bool equalBoards = false;
        for (auto it = closedBoards.begin(); it != closedBoards.end(); ++it ) {
            if (equalBoards = areEqual(*it, dChild.board)) {
                break;
            }
        }
        if (!equalBoards) {
            dChild.path = parent.path;
            dChild.path.push_back('d');
            dChild.parentNode = make_shared<Node>(parent);
            dChild.f = manhattanDistance(dChild.board) + dChild.path.size();
            possibleNodes.push(move(dChild));
        }

    }

}


void findSolution(Node parent){
    if (isSolution(parent.board)) {
        for (auto i = parent.path.begin(); i != parent.path.end(); ++i) {
                switch(*i) {
                    case 'l': cout << "left" << endl; break;
                    case 'r': cout << "right" << endl; break;
                    case 'u': cout << "up" << endl; break;
                    default: cout << "down" << endl; break;
            }
        }
    } else {
        closedBoards.push_front(parent.board);
        findChildren(parent);
        Node current = possibleNodes.top();
        possibleNodes.pop();
        findSolution(current);
    }
}

int main() {
    cout << "input a number of blocks" << endl;
    int N;
    cin >> N;
    ++N;
    int n = sqrt(N);
    if (n*n != N) {
        cout << "Incorrect input!" << endl;
        return -1;
    }
    srand(time(NULL));
    //vector<vector<int> > board = {{6, 5, 3}, {2, 4 ,8}, {7, 0, 1}};
    vector<vector<int> > board = {{2, 3, 6}, {1, 5 ,8}, {4, 7, 0}};
    //vector<vector<int> > board = createBoard(n);
    closedBoards.push_back(board);
    printBoard(board);
    Node root;
    root.board = board;
    root.path = {};
    findSolution(root);
    return 0;
}


