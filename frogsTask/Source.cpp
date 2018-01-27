#include <iostream>
#include <cstring>

using namespace std;

char* changeString(const char* str, int i, int j, int size){
    char* tmp = new char[size];
    strcpy(tmp, str);
    tmp[i] = str[j];
    tmp[j] = str[i];
    return tmp;
}

bool DFSrecursion(const char* current, const char* searched, const int size, int blank){
    if (strcmp(current, searched) == 0) {
        return true;
    }


    int mostLeft = blank - 2;
    if (mostLeft >= 0 && current[mostLeft] == '<') {
        char* tmpStr = changeString(current, mostLeft, blank, size);
        if (DFSrecursion(tmpStr, searched, size, mostLeft)) {
            cout << tmpStr << endl;
            delete[] tmpStr;
            return true;
        }
        delete[] tmpStr;
    }

    int mostRight = blank + 2;
    if (mostRight <= (size - 1) && current[mostRight] == '>') {
        char* tmpStr= changeString(current, mostRight, blank, size);
        if (DFSrecursion(tmpStr, searched, size, mostRight)) {
            cout << tmpStr << endl;
            delete[] tmpStr;
            return true;
        }
        delete[] tmpStr;
    }

    int right = blank + 1;
    if (right <= (size - 1) && current[right] == '>') {
        char* tmpStr= changeString(current, right, blank, size);
        if (DFSrecursion(tmpStr, searched, size, right)) {
            cout << tmpStr << endl;
            delete[] tmpStr;
            return true;
        }
        delete[] tmpStr;
    }

    int left = blank - 1;
    if (left >= 0 && current[left] == '<') {
        char* tmpStr= changeString(current, left, blank, size);
        if (DFSrecursion(tmpStr, searched, size, left)) {
            cout << tmpStr << endl;
            delete[] tmpStr;
            return true;
        }
        delete[] tmpStr;
    }

    return false;
}

int main() {
    cout << "Input left frogs count" << endl;
    int n;
    cin >> n;
    int size = 2 * n + 1;
    char* initialState = new char[size];
    for (int i = 0; i < n; ++i) {
        initialState[i] = '<';
        initialState[size - i - 1] = '>';
    }
    initialState[n] = '_';
    
    char* finalState = new char[size];
    for (int i = 0; i < n; ++i) {
        finalState[i] = '>';
        finalState[size - i - 1] = '<';
    }
    finalState[n] = '_';
    DFSrecursion(initialState, finalState, size, n);
    cout << initialState << endl;
    delete[] finalState;
    delete[] initialState;
}
