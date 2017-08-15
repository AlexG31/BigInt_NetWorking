#ifndef BIG_INT_H_
#define BIG_INT_H_

#include <cstring>
#include <stdexcept>
#include <algorithm>

#define MAX_DIGITS 100
class BigInt {
private:
    BigInt absolute_add(const BigInt& a, const BigInt& b) const {
        int carry = 0;
        int max_len = std::max(a.len, b.len);
        BigInt result;
        int i = 0;
        for (i = 0; i < 100 && i <= max_len; ++i) {
            int val = a.digits[i] + b.digits[i] + carry;
            result.digits[i] = val % 10;
            carry = val / 10;
        }
        if (carry > 0) {
            if (i >= 100) {
                throw std::out_of_range( "Adding result out of range" );
            } else {
                result.digits[i] = carry;
            }
        }
        return result;
    }
    BigInt absolute_sub(const BigInt& a, const BigInt& b) const {
        BigInt result;
        return result;
    }
    
public:
    int digits[MAX_DIGITS];
    int sign;
    int len = 0;
    // 构造函数
    BigInt():sign(1), len(0) {
        memset(digits, 0, sizeof(digits));
    }
    BigInt(const BigInt& a):sign(a.sign), len(a.len) {
        memset(digits, 0, sizeof(digits));
    }
    BigInt(long long val):sign(1), len(0) {
        memset(digits, 0, sizeof(digits));
        if (val == -9223372036854775808LL) {
            sign = -1;
            len = 18;
            int newarr [] = {8,0,8,5,7,7,4,5,8,6,3,0,2,7,3,3,2,2,9};
            std::copy(newarr, newarr + 19, digits);
        } else {
            if (val < 0) {
                sign = -1;
                val *= -1;
            }
            for (int i = 0; i < MAX_DIGITS && val > 0; ++i) {
                digits[i] = val % 10LL;
                val /= 10LL;
                ++len;
            }
        }
    }

    // 操作符
    BigInt operator +(const BigInt& b) const{
        const auto& a = *this;
        if (sign > 0) {
            if (b.sign > 0) return absolute_add(a, b);
            else return absolute_sub(a, b);
        } else {
            if (b.sign > 0) return absolute_sub(b, a);
            else {
                BigInt result = absolute_add(a, b);
                result.sign = -1;
                return result;
            }
        }
    }
    BigInt operator -(const BigInt& b) const{
        const auto& a = *this;
        if (sign > 0) {
            if (b.sign > 0) return absolute_add(a, b);
            else return absolute_sub(a, b);
        } else {
            if (b.sign > 0) return absolute_sub(b, a);
            else {
                BigInt result = absolute_add(a, b);
                result.sign = -1;
                return result;
            }
        }
    }
    void print() {
        const auto& a = *this;
        cout << "sign: " << a.sign;
        cout << "len: " << a.len << endl;
        for (int i = 0; i < 21; ++i) {
            cout << a.digits[i] << ", ";
        }
        cout << endl;
    }
};


#endif  // BIG_INT_H_
