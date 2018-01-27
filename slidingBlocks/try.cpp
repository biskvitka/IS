#include<iostream>
#include<vector>

using namespace std;


int main() {
    vector<int> a = {1, 2, 3};
    vector<int> b = a;
    b[2] = 10;
    for (int i = 0; i < 3; ++i) {
        cout << a[i] << " " << b[i] << endl;
    }
    return 0;
}
