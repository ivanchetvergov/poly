// core/src/BigRingArithmetic.cc
#include "BigRingArithmetic.h"
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <fstream>

// #define DEBUG_ARITHMETIC

#ifdef DEBUG_ARITHMETIC
#define DEBUG_LOG(msg) std::cout << "[DEBUG] " << msg << std::endl; 
#else
#define DEBUG_LOG(msg)
#endif

BigRingArithmetic::BigRingArithmetic(const FiniteRingRules& rules,
                                     const SmallRingArithmetic& small)
    : rules_(rules), small_(small) {}


// тупо тут складываем по цифрам без инт короче
// * --- СЛОЖЕНИЕ ---
RingNumber BigRingArithmetic::add(const RingNumber& a, const RingNumber& b) const {
    DEBUG_LOG("=== ADD START ===");
    DEBUG_LOG("  a = " << a.toString() << " (neg=" << a.isNegative() << ", len=" << a.length() << ")");
    DEBUG_LOG("  b = " << b.toString() << " (neg=" << b.isNegative() << ", len=" << b.length() << ")");
    
    bool same_sign = (a.isNegative() == b.isNegative());
    DEBUG_LOG("  same_sign = " << same_sign);

    RingNumber uns_a = a.withoutSign();
    RingNumber uns_b = b.withoutSign();

    if (same_sign) {
        DEBUG_LOG("  Branch: same sign -> addUnsigned");
        RingNumber sum = addUnsigned(uns_a, uns_b);
        sum.setNegative(a.isNegative());
        DEBUG_LOG("  result = " << sum.toString());
        DEBUG_LOG("=== ADD END ===");
        return sum;
    }

    if (isGreaterOrEqual(uns_a, uns_b)) {
        DEBUG_LOG("  Branch: |a| >= |b| -> subtractPositional(a, b)");
        RingNumber diff = subtractPositional(uns_a, uns_b);
        diff.setNegative(a.isNegative());
        DEBUG_LOG("  result = " << diff.toString());
        DEBUG_LOG("=== ADD END ===");
        return diff;
    }

    DEBUG_LOG("  Branch: |a| < |b| -> subtractPositional(b, a)");
    RingNumber diff = subtractPositional(uns_b, uns_a);
    diff.setNegative(b.isNegative());
    DEBUG_LOG("  result = " << diff.toString());
    DEBUG_LOG("=== ADD END ===");
    return diff;
}

RingNumber BigRingArithmetic::addUnsigned(const RingNumber& a, const RingNumber& b) const {
    // тут чисто модульное сложение по разрядам не переводим в числа
    DEBUG_LOG("  --> addUnsigned: a=" << a.toString() << ", b=" << b.toString());
    size_t max_len = std::max(a.length(), b.length());
    DEBUG_LOG("      max_len=" << max_len);
    
    std::vector<char> result_digits;
    result_digits.reserve(max_len + 1);
    
    char carry_out = zero_;
    
    for (size_t i = 0; i < max_len || carry_out != zero_; ++i) {
        DEBUG_LOG("      iter " << i << ": carry_in=" << carry_out << ", result.size=" << result_digits.size());
        char digit_a = a.getDigit(i);
        char digit_b = b.getDigit(i);
        DEBUG_LOG("        digit_a=" << digit_a << ", digit_b=" << digit_b);
        char carry_in = carry_out;
        
        char sum_mid = small_.add(digit_a, digit_b);
        // проверка на перенос из-за переполнения
        char carry_1 = zero_;
        if (isLessThan(sum_mid, digit_a)) {
            carry_1 = one_;
        }
        
        char sum_final = small_.add(sum_mid, carry_in);
        // проверка на перенос из-за добавления переноса
        char carry_2 = zero_;
        if (carry_in != zero_ && isLessThan(sum_final, sum_mid)) {
            carry_2 = one_;
        }
        
        carry_out = (carry_1 != zero_ || carry_2 != zero_) ? one_ : zero_;
        DEBUG_LOG("        sum_final=" << sum_final << ", carry_out=" << carry_out);
        result_digits.push_back(sum_final);
    }

    RingNumber result(rules_, result_digits);
    result.normalize();
    DEBUG_LOG("      result: " << result.toString() << ", len=" << result.length());
    DEBUG_LOG("  <-- addUnsigned done");
    return result;
}

// * --- ВЫЧИТАНИЕ (через аддитивную инверсию) ---
RingNumber BigRingArithmetic::subtract(const RingNumber& a, const RingNumber& b) const {
    // вычитаем через прибавление инверсии экономно и без заморочек
    DEBUG_LOG("=== SUBTRACT START ===");
    DEBUG_LOG("  a = " << a.toString() << ", b = " << b.toString());
    RingNumber neg_b = negate(b);
    DEBUG_LOG("  neg_b = " << neg_b.toString());
    RingNumber result = add(a, neg_b);
    DEBUG_LOG("=== SUBTRACT END ===");
    return result;
}

// * --- ПОЗИЦИОННОЕ ВЫЧИТАНИЕ (для деления) ---
RingNumber BigRingArithmetic::subtractPositional(const RingNumber& a, const RingNumber& b) const {
    // позиционное вычитание
    DEBUG_LOG("  --> subtractPositional: a=" << a.toString() << ", b=" << b.toString());
    
    size_t max_len = std::max(a.length(), b.length());
    DEBUG_LOG("      max_len=" << max_len);
    std::vector<char> result_digits;
    result_digits.reserve(max_len);
    
    char borrow = zero_; // заём
    
    for (size_t i = 0; i < max_len; ++i) {
        char digit_a = a.getDigit(i);
        char digit_b = b.getDigit(i);
        DEBUG_LOG("      iter " << i << ": digit_a=" << digit_a << ", digit_b=" << digit_b << ", borrow=" << borrow);
        // учитываем заём
        bool borrow_out = false;

        if (borrow != zero_) {
            // нужно занять у следующего разряда
            bool wrapped = (digit_a == zero_);
            digit_a = small_.subtract(digit_a, one_);
            DEBUG_LOG("        after borrow: digit_a=" << digit_a << "; wrapped=" << wrapped);
            if (wrapped) {
                // одолжили у следующего разряда, сохраняем этот долг
                borrow_out = true;
            }
        }
        // проверка необходимости займа
        bool need_borrow = isLessThan(digit_a, digit_b);
        char diff = small_.subtract(digit_a, digit_b);
        DEBUG_LOG("        diff=" << diff << ", need_borrow=" << need_borrow);

        if (need_borrow) {
            borrow_out = true;
        }
        // сохраняем результат
        result_digits.push_back(diff);
        borrow = borrow_out ? one_ : zero_;
    }

    if (borrow != zero_) {
        DEBUG_LOG("      ERROR: final borrow != zero (negative result)");
        throw std::runtime_error("subtractPositional produced a negative result");
    }

    RingNumber result(rules_, result_digits);
    result.normalize();
    DEBUG_LOG("      result: " << result.toString() << ", len=" << result.length());
    DEBUG_LOG("  <-- subtractPositional done");
    return result;
}

// * --- УМНОЖЕНИЕ ---
RingNumber BigRingArithmetic::multiply(const RingNumber& a, const RingNumber& b) const {
    // тут столбик умножения по цифрам без перевода в числа
    RingNumber uns_a = a.withoutSign();
    RingNumber uns_b = b.withoutSign();
    
    if (uns_a.isZero() || uns_b.isZero()) {
        return RingNumber(rules_);
    }
      
    RingNumber result(rules_);
    
    for (size_t i = 0; i < uns_b.length(); ++i) {
        char digit_b = uns_b.getDigit(i);
        RingNumber partial = multiplyByDigit(uns_a, digit_b);
        partial = shiftLeft(partial, static_cast<int>(i));
        result = addUnsigned(result, partial);
    }
    
    bool negative = (a.isNegative() != b.isNegative());
    result.setNegative(negative);
    result.normalize();
    return result;
}

// * --- ДЕЛЕНИЕ С ОСТАТКОМ (Деление Столбиком) ---
DivisionResult BigRingArithmetic::divide(const RingNumber& a, const RingNumber& b) const {
    // деление столбиком короче сперва по модулю потом правим знак
    DEBUG_LOG("=== DIVIDE START ===");
    DEBUG_LOG("  a = " << a.toString() << ", b = " << b.toString());
    
    if (a.isZero() && b.isZero()) {
        throw std::runtime_error("Err: 0 / 0 -> [-hhhhhhhh(min): hhhhhhhh(max)]");
    }
    
    if (b.isZero()) {
        throw std::runtime_error("Err: Division by zero! Empty set");
    }

    RingNumber dividend = a.withoutSign();
    RingNumber divisor = b.withoutSign();
    RingNumber quotient(rules_);
    RingNumber remainder = dividend;

    DEBUG_LOG("  dividend (|a|) = " << dividend.toString());
    DEBUG_LOG("  divisor (|b|) = " << divisor.toString());

    if (isGreaterOrEqual(dividend, divisor)) {
        int shift_amount = static_cast<int>(dividend.length()) - static_cast<int>(divisor.length());
        RingNumber shifted_divisor = shiftLeft(divisor, shift_amount);
        DEBUG_LOG("  shift_amount = " << shift_amount);

        for (int i = shift_amount; i >= 0; --i) {
            char q_digit = findQuotientDigit(remainder, shifted_divisor);
            DEBUG_LOG("    position " << i << ": q_digit = " << q_digit);

            if (q_digit != zero_) {
                RingNumber q_digit_num(rules_, std::string(1, q_digit));
                RingNumber partial_quotient = shiftLeft(q_digit_num, i);
                quotient = addUnsigned(quotient, partial_quotient);

                RingNumber subtraction_amount = multiplyByDigit(shifted_divisor, q_digit);
                remainder = subtractPositional(remainder, subtraction_amount);
                DEBUG_LOG("    remainder after subtraction: " << remainder.toString());
            }

            if (i > 0) {
                shifted_divisor = shiftRight(shifted_divisor, 1);
            }
        }
    }

    remainder.normalize();
    quotient.normalize();

    DEBUG_LOG("  Before sign adjustment: q=" << quotient.toString() << ", r=" << remainder.toString());

    bool dividend_negative = a.isNegative();
    bool divisor_negative = b.isNegative();

    DEBUG_LOG("  dividend_negative=" << dividend_negative << ", divisor_negative=" << divisor_negative);
    DEBUG_LOG("  remainder.isZero()=" << remainder.isZero());

    if (dividend_negative && !divisor_negative && !remainder.isZero()) {
        DEBUG_LOG("  Applying correction for (-a) / (+b) with non-zero remainder");
        RingNumber one_num(rules_, std::string(1, one_));
        quotient = addUnsigned(quotient, one_num);
        remainder = subtractPositional(divisor, remainder);
        DEBUG_LOG("  After correction: q=" << quotient.toString() << ", r=" << remainder.toString());
    }

    // знак частного: XOR знаков
    bool quotient_negative = (dividend_negative != divisor_negative);
    quotient.setNegative(quotient_negative);

    // остаток всегда неотрицательный
    remainder.setNegative(false);

    remainder.normalize();
    quotient.normalize();

    DEBUG_LOG("  Final result: quotient = " << quotient.toString() << ", remainder = " << remainder.toString());
    DEBUG_LOG("=== DIVIDE END ===");

    return DivisionResult(quotient, remainder);
}

// * --- АДДИТИВНАЯ ИНВЕРСИЯ (отрицание) ---
RingNumber BigRingArithmetic::negate(const RingNumber& a) const {
    // просто меняем знак и нормализуем
    RingNumber result = a;
    result.flipSign();
    result.normalize();
    return result;
}

// * --- ВСПОМОГАТЕЛЬНЫЕ МЕТОДЫ ---
RingNumber BigRingArithmetic::multiplyByDigit(const RingNumber& num, char digit) const {
    // умножаем на одну цифру через последовательное сложение
    DEBUG_LOG("  --> multiplyByDigit: num=" << num.toString() << ", digit=" << digit);

    if (digit == zero_ || num.isZero()) {
        DEBUG_LOG("      result: 0");
        return RingNumber(rules_);
    }

    RingNumber positive = num.withoutSign();

    if (digit == one_) {
        DEBUG_LOG("      result: " << positive.toString());
        return positive;
    }

    RingNumber result(rules_);
    char counter = zero_;
    size_t iterations = 0;
    size_t base_size = static_cast<size_t>(rules_.getSize());

    while (counter != digit) {
        result = addUnsigned(result, positive);
        counter = small_.plusOne(counter);
        iterations++;
        
        if (iterations >= base_size) {
            throw std::runtime_error("multiplyByDigit: infinite loop detected or invalid digit");
        }
    }

    DEBUG_LOG("      result: " << result.toString() << " (" << iterations << " iterations)");
    DEBUG_LOG("  <-- multiplyByDigit done");
    return result;
}

RingNumber BigRingArithmetic::shiftLeft(const RingNumber& num, int positions) const {
    if (positions == 0 || num.isZero()) {
        return num;
    }
    std::vector<char> result_digits;
    result_digits.reserve(num.length() + positions);
    // добавляем нули в младшие разряды тупо сдвиг назад
    for (int i = 0; i < positions; ++i) {
        result_digits.push_back(zero_);
    }

    // копируем цифры числа
    for (size_t i = 0; i < num.length(); ++i) {
        result_digits.push_back(num.getDigit(i));
    }

    RingNumber result(rules_, result_digits);
    result.setNegative(num.isNegative());
    return result;
}

RingNumber BigRingArithmetic::shiftRight(const RingNumber& num, int positions) const {
    if (positions <= 0 || num.isZero() || positions >= num.length()) {
        return RingNumber(rules_); // возвращаем ноль
    }

    std::vector<char> original_digits = num.toVector();
    std::vector<char> result_digits;
    
    // копируем цифры, начиная с positions (отбрасывая младшие разряды)
    for (size_t i = positions; i < original_digits.size(); ++i) {
        result_digits.push_back(original_digits[i]);
    }

    RingNumber result(rules_, result_digits);
    result.setNegative(num.isNegative());
    result.normalize(); 
    return result;
}

char BigRingArithmetic::findQuotientDigit(const RingNumber& remainder, const RingNumber& shifted_divisor) const {

    char q_digit = zero_;
    RingNumber temp_remainder = remainder;
    while (isGreaterOrEqual(temp_remainder, shifted_divisor)) {
        temp_remainder = subtractPositional(temp_remainder, shifted_divisor);
        q_digit = small_.plusOne(q_digit);
    }
    return q_digit;
}