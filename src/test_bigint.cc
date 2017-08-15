#include "bigint.h"
#include <iostream>


using namespace std;
using namespace aifuture;


int test_longlong() {
    int L = -100;
    int R = 100;
    for (int i = L; i < L + 2; ++i)
        for (int j = L; j < R; ++j) {
            BigInt a(i);
            BigInt b(j);
            BigInt c;
            if (j == 0) continue;
            try {
                c = a / b;
            } catch (...) {
                cout << "Out of range ERROR!";
                cout << i;
                cout << " + ";
                cout << j << endl;
                continue;
            }
            long long result = i / j;
            if (c.to_longlong() != result) {
                cout << "ERROR!";
                cout << i;
                cout << " + ";
                cout << j << endl;
                cout << c.to_longlong() << endl;
                return -1;
            }
        
        }

    cout << "SUCCESS!" << endl;
    return 0;
}
int main() {
    BigInt a((long long)-100023);
    BigInt b((long long)-100023LL);
    //BigInt a((long long)9223372036854775807LL);

    a.print();
    b.print();

    auto c = a / b;
    c.print();

    test_longlong();
    return 0;
}
