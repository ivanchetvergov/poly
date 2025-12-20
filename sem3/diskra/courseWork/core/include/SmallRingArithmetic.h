#pragma once
#include "FiniteRingRules.h"

class SmallRingArithmetic {
public:
    explicit SmallRingArithmetic(const FiniteRingRules& r) : rules_(r) {}

    // * арифметические операции в поле
    char add(char a, char b) const;
    char subtract(char a, char b) const;
    char multiply(char a, char b) const;
    char divide(char a, char b) const;

    // * получить правила поля
    const FiniteRingRules& getRules() const { return rules_; }
    // * методы поиска обратных элементов
    char findMultiplicativeInverse(char element) const;
    char findAdditiveInverse(char element) const;

    char plusOne(char c) const;
private:
    // ссылка на правила поля
    const FiniteRingRules& rules_;
};
