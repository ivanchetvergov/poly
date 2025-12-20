#pragma once
#include "FiniteRingRules.h"
#include "SmallRingArithmetic.h"
#include "RingNumber.h"
#include "DivisionResult.h"

class BigRingArithmetic {
public:
    BigRingArithmetic(const FiniteRingRules& rules, 
                      const SmallRingArithmetic& small);
    
    // * операции: строка × строка -> строка
    RingNumber add(const RingNumber& a, const RingNumber& b) const;
    RingNumber subtract(const RingNumber& a, const RingNumber& b) const;
    RingNumber multiply(const RingNumber& a, const RingNumber& b) const;
    DivisionResult divide(const RingNumber& a, const RingNumber& b) const;

    RingNumber negate(const RingNumber& a) const;
    RingNumber subtractPositional(const RingNumber& a, const RingNumber& b) const;

    // * константа: максимальное количество разрядов
    static const size_t MAX_DIGITS = 8;

private:
    const FiniteRingRules& rules_;
    const SmallRingArithmetic& small_;

    const char zero_ = rules_.getZeroElement();
    const char one_ = rules_.getOneElement();
    
    // * вспомогательные методы
    RingNumber multiplyByDigit(const RingNumber& num, char digit) const;
    char findQuotientDigit(const RingNumber& remainder, const RingNumber& shifted_divisor) const;
    RingNumber addUnsigned(const RingNumber& a, const RingNumber& b) const;
    // * сдвиги
    RingNumber shiftLeft(const RingNumber& num, int positions) const;
    RingNumber shiftRight(const RingNumber& num, int positions) const;
    // * сравнение 
    bool isGreaterOrEqual(const RingNumber& a, const RingNumber& b) const;
    bool isLessThan(char a, char b) const;
};