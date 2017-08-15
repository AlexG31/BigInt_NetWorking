#include "bigint.h"
#include <iostream>


using namespace std;
using namespace aifuture;


int test_longlong() {
    int L = -1000;
    int R = 1000;
    for (int i = L; i < R; ++i)
        for (int j = L; j < R; ++j) {
            BigInt a(i);
            BigInt b(j);
            auto c = a / b;
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
    BigInt a((long long)100);
    BigInt b((long long)5LL);
    //BigInt a((long long)9223372036854775807LL);
    auto c = a / b;

    a.print();
    b.print();
    c.print();

    test_longlong();
    return 0;
}
