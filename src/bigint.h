#ifndef BIG_INT_H_
#define BIG_INT_H_

#include <cstring>
#include <stdexcept>
#include <algorithm>
#include <iostream>

#define MAX_DIGITS 100


namespace aifuture{
class BigInt {
private:
    BigInt absolute_add(const BigInt& a, const BigInt& b) const {
        int carry = 0;
        int max_len = std::max(a.len, b.len);
        BigInt result;
        result.sign = 1;
        int i = 0;
        for (i = 0; i < 100 && i < max_len; ++i) {
            int val = a.digits[i] + b.digits[i] + carry;
            result.digits[i] = val % 10;
            carry = val / 10;
        }
        if (carry > 0) {
            if (i >= MAX_DIGITS) {
                throw std::out_of_range( "Adding result out of range" );
            } else {
                result.digits[i++] = carry;
            }
        }
        result.len = i;
        return result;
    }
    BigInt absolute_sub(const BigInt& a, const BigInt& b) const {
        BigInt result(a);
        result.sign = 1;
        int max_len = std::max(result.len, b.len);
        int diff = 0;
        for (int i = 0; i < max_len; ++i) {
            diff += result.digits[i] - b.digits[i];
            if (diff < 0) {
                result.digits[i] = 10 + diff;
                diff = -1;
            } else {
                result.digits[i] = diff;
                diff = 0;
            }
            
        }
        if (diff < 0) {
            result.sign = -1;
            for (int i = max_len - 1; i > 0; --i) {
                result.digits[i] = 9 - result.digits[i];
            }
            result.digits[0] = 10 - result.digits[0];
        }
        result.len = max_len;
        while (result.len > 1) {
            if (result.digits[result.len - 1] > 0) break;
            --result.len;
        }
        return result;
    }
    BigInt basic_divide(const BigInt& a, const BigInt& b) const {
        BigInt result;
        BigInt current(a);
        for (int i = 0; current.len > 1 || current.digits[0] > 0; ++i) {
            current = current - b;
            result = result + 1;
        }
        return result;
    }
    BigInt basic_divide(const BigInt& a, const BigInt& b) const {
        BigInt result;
        BigInt current(a);
        for (int i = 0; current.len > 1 || current.digits[0] > 0; ++i) {
            current = current - b;
            result = result + 1;
        }
        return result;
    }
    
    
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
    BigInt(long long val):sign(1), len(0) {
        memset(digits, 0, sizeof(digits));
        if (val == -9223372036854775808LL) {
            sign = -1;
            len = 19;
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

    inline long size() const {
        return len;
    }
    // 操作符
    inline int operator [](unsigned int index) const {
        return this->digits[index];
    }
    BigInt operator +(const BigInt& b) const{
        const auto& a = *this;
        if (sign > 0) {
            if (b.sign > 0) return absolute_add(a, b);
            else return absolute_sub(a, b);
        } else {
            if (b.sign > 0) return absolute_sub(b, a);
            else {
                BigInt result = absolute_add(a, b);
                result.sign *= -1;
                return result;
            }
        }
    }
    BigInt operator -(const BigInt& b) const{
        const auto& a = *this;
        if (sign > 0) {
            if (b.sign > 0) return absolute_sub(a, b);
            else return absolute_add(a, b);
        } else {
            if (b.sign > 0) {
                BigInt result = absolute_add(a, b);
                result.sign *= -1;
                return result;
            }
            else return absolute_sub(b, a);
        }
    }
    BigInt operator * (const BigInt& b) const {
        const auto& a = *this;
        BigInt result;
        result.sign = a.sign * b.sign;
        int max_len = std::max(a.len, b.len);
        for (int i = 0; i < a.len; ++i) {
            for (int j = 0; j < b.len; ++j) {
                result.digits[j + i] += a.digits[i] * b.digits[j];
            }
        }
        if (a.len + b.len - 1 > MAX_DIGITS) {
            throw std::out_of_range( "Result out of range" );
        }
        int carry = 0;
        int i = 0;
        for (i = 0; i < a.len + b.len; ++i) {
            int val = carry + result.digits[i];
            result.digits[i] = val % 10;
            carry = val / 10;
        }
        result.len = i;
        if (carry > 0) {
            if (i >= MAX_DIGITS) throw std::out_of_range( "Result out of range" );
            result.digits[i] = 1;
            result.len = i + 1;
        }
        return result;
    }
    BigInt operator /(const BigInt& b) const {
        BigInt result = basic_divide(*this, b);
        return result;
    }

    // YOU have to make sure this not overflows
    long long to_longlong() const{
        long long result = 0;
        for (int i = len - 1; i >= 0; --i) {
            result *= 10LL;
            result += digits[i];
        }
        result *= sign;
        return result;
    }
    void print() {
        const auto& a = *this;
        std::cout << "sign: " << a.sign;
        std::cout << ", ";
        std::cout << "len: " << a.len << std::endl;
        for (int i = 0; i < a.len; ++i) {
            std::cout << a.digits[i] << ", ";
        }
        std::cout << std::endl;
    }
    void print_money() {
        const auto& a = *this;
        if (sign < 0) {
            std::cout << "-";
        }
        for (int i = a.len - 1; i >= 0; --i) {
            std::cout << a.digits[i];
            if (i < a.len && (a.len - i) % 3 == 0) std::cout << ",";
        }
        std::cout << std::endl;
    }

}; //BigInt
} //namespace


#endif  // BIG_INT_H_
