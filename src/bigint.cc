#include "bigint.h"
//#define MAX_DIGITS 500


aifuture::BigInt aifuture::BigInt::absolute_add(const aifuture::BigInt& a, const aifuture::BigInt& b) const {
    int carry = 0;
    int max_len = std::max(a.len, b.len);
    aifuture::BigInt result;
    result.sign = 1;
    int i = 0;
    for (i = 0; i < max_len; ++i) {
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
aifuture::BigInt aifuture::BigInt::absolute_sub(const aifuture::BigInt& a, const aifuture::BigInt& b) const {
    aifuture::BigInt result(a);
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

aifuture::BigInt aifuture::BigInt::basic_divide(const aifuture::BigInt& a, const aifuture::BigInt& b) const {
    aifuture::BigInt result;
    aifuture::BigInt current(a);
    for (int i = 0; current.len > 1 || current.digits[0] > 0; ++i) {
        current = current - b;
        result = result + 1;
    }
    return result;
}

void aifuture::BigInt::shift10_down(int K) {
    for (int i = K; i < len; ++i) {
        digits[i - K] = digits[i];
    }
    for (int i = std::max(0, len - K); i < len; ++i) digits[i] = 0;
    len = std::max(len - K, 1);
    return;
}
void aifuture::BigInt::shift10_up(int K) {
    if (K + len >= MAX_DIGITS) 
        throw std::out_of_range( "Result out of range" );
    for (int i = K + len - 1; i >= 0; --i) {
        int val = i >= K? digits[i - K]:0;
        digits[i] = val;
    }
    len += K;
    return;
}

// Newton–Raphson division
aifuture::BigInt aifuture::BigInt::NR_divide(const aifuture::BigInt& a, const aifuture::BigInt& b) const {
    int sign = a.sign * b.sign;
    int precision_len = std::max(a.len, b.len) * 2;
    aifuture::BigInt X(1);
    X.shift10_up(precision_len);
    int M = MAX_DIGITS;
    while (M-- > 0) {
        aifuture::BigInt pX(X);
        pX.shift10_down(precision_len);
        aifuture::BigInt newX(1);
        newX.shift10_up(precision_len);
        newX = absolute_sub(newX, b * pX);
        newX = newX * pX;
        X = X + newX;
    }
    X = X * a;
    X.shift10_down(2 * precision_len);
    X.sign = a.sign * b.sign;
    // Remove zeros
    while (X.len > 1) {
        if (X.digits[X.len - 1] > 0) break;
        --X.len;
    }
        
    return X;
}
    
    
// 构造函数
aifuture::BigInt::BigInt(long long val):sign(1), len(0) {
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

aifuture::BigInt aifuture::BigInt::operator +(const aifuture::BigInt& b) const{
    const auto& a = *this;
    if (sign > 0) {
        if (b.sign > 0) return absolute_add(a, b);
        else return absolute_sub(a, b);
    } else {
        if (b.sign > 0) return absolute_sub(b, a);
        else {
            aifuture::BigInt result = absolute_add(a, b);
            result.sign *= -1;
            return result;
        }
    }
}
aifuture::BigInt aifuture::BigInt::operator -(const aifuture::BigInt& b) const{
    const auto& a = *this;
    if (sign > 0) {
        if (b.sign > 0) return absolute_sub(a, b);
        else return absolute_add(a, b);
    } else {
        if (b.sign > 0) {
            aifuture::BigInt result = absolute_add(a, b);
            result.sign *= -1;
            return result;
        }
        else return absolute_sub(b, a);
    }
}
aifuture::BigInt aifuture::BigInt::operator * (const aifuture::BigInt& b) const {
    const auto& a = *this;
    aifuture::BigInt result;
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
aifuture::BigInt aifuture::BigInt::operator /(const aifuture::BigInt& b) const {
    // BigInt result = basic_divide(*this, b);
    aifuture::BigInt result = NR_divide(*this, b);
    return result;
}

// YOU have to make sure this not overflows
long long aifuture::BigInt::to_longlong() const{
    long long result = 0;
    for (int i = len - 1; i >= 0; --i) {
        result *= 10LL;
        result += digits[i];
    }
    result *= sign;
    return result;
}
void aifuture::BigInt::print() {
    const auto& a = *this;
    std::cout << "sign: " << a.sign;
    std::cout << ", ";
    std::cout << "len: " << a.len << std::endl;
    for (int i = 0; i < a.len; ++i) {
        std::cout << a.digits[i] << ", ";
    }
    std::cout << std::endl;
}
