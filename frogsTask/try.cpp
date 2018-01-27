#include <iostream>
#include <string>

using namespace std;
int main() {
    string a = "hello";
    string b = a;
    b[2] = 'a';
    cout << a << endl;
    cout << b << endl;
    return 0;
}
