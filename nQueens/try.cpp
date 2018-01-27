#include <iostream>
using namespace std;
int main() {
    int a = 1;
    int b = 0;
    for (int i = 0; i < 10000; ++i) {
        for (int j = 0; j < 10000; ++j){
            b += a;
        }
    }
    cout << b << endl;

    return 0;
}
