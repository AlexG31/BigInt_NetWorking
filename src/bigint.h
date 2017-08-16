#ifndef BIG_INT_H_
#define BIG_INT_H_

#include <cstring>
#include <stdexcept>
#include <algorithm>
#include <iostream>

#define MAX_DIGITS 900

namespace aifuture{
class BigInt {
private:
    BigInt absolute_add(const BigInt& a, const BigInt& b) const;
    BigInt absolute_sub(const BigInt& a, const BigInt& b) const;
    BigInt basic_divide(const BigInt& a, const BigInt& b) const;
    void shift10_down(int K);
    void shift10_up(int K);
    // Newton–Raphson division
    BigInt NR_divide(const BigInt& a, const BigInt& b) const;
    
    
    // Private variables
    int digits[MAX_DIGITS];
    int sign;
    int len = 0;
public:
    // 构造函数
    BigInt():sign(1), len(0) {
        memset(digits, 0, sizeof(digits));
    }
    BigInt(const BigInt& a):sign(a.sign), len(a.len) {
        memset(digits, 0, sizeof(digits));
        for (int i = 0; i < len; ++i) digits[i] = a.digits[i];
    }
    BigInt(long long val);

    inline long size() const {
        return len;
    }
    // 操作符
    inline int operator [](unsigned int index) const {
        return this->digits[index];
    }
    BigInt operator +(const BigInt& b) const;
    BigInt operator -(const BigInt& b) const;
    BigInt operator * (const BigInt& b) const;
    BigInt operator /(const BigInt& b) const;

    // YOU have to make sure this not overflows
    long long to_longlong() const;
    void print();

}; //BigInt
} //namespace


#endif  // BIG_INT_H_
