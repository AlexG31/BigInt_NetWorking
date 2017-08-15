#include "bigint.h"
#include <iostream>


using namespace std;


void printBigInt(const BigInt& a) {
    cout << "sign: " << a.sign;
    cout << "len: " << a.len << endl;
    for (int i = 0; i < 21; ++i) {
        cout << a.digits[i] << ", ";
    }
    cout << endl;
}

int main() {
    BigInt a((long long)9223372036854775808LL);
    BigInt b((long long)1LL);
    //BigInt a((long long)9223372036854775807LL);
    //auto c = a + b;

    cout << a.sign << endl;
    for (int i = 0; i < 21; ++i) {
        cout << a.digits[i] << ", ";
    }
    cout << endl;
    return 0;
}
