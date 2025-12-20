// core/src/utils.cc
#include "BigRingArithmetic.h"
#include <stdexcept>
#include <algorithm>
#include <iostream>




// * --- СРАВНЕНИЕ СИМВОЛОВ: a < b? ---
bool BigRingArithmetic::isLessThan(char a, char b) const {
    if (a == b) {
        return false;
    }

    char cursor = zero_;
    do {
        if (cursor == a) {
            return true;
        }
        if (cursor == b) {
            return false;
        }
        cursor = small_.plusOne(cursor);
    } while (cursor != zero_);

    throw std::runtime_error("Ring traversal failed while comparing digits");
}

// * --- СРАВНЕНИЕ ЧИСЕЛ: a >= b? ---
bool BigRingArithmetic::isGreaterOrEqual(const RingNumber& a, const RingNumber& b) const {
    
    // нормализуем оба числа
    RingNumber a_norm = a;
    RingNumber b_norm = b;
    a_norm.setNegative(false);
    b_norm.setNegative(false);
    a_norm.normalize();
    b_norm.normalize();
    
    size_t len_a = a_norm.length();
    size_t len_b = b_norm.length();
                        
    // 1. сравнение по длине
    if (len_a != len_b) {
        return len_a > len_b;
    }
    
    // 2. сравниваем поразрядно от СТАРШЕГО к младшему
    for (int i = static_cast<int>(len_a) - 1; i >= 0; --i) {
        char digit_a = a_norm[i];
        char digit_b = b_norm[i];

        if (digit_a == digit_b) {
            continue;
        }
        return !isLessThan(digit_a, digit_b);
    }
    
    return true; // числа равны
}