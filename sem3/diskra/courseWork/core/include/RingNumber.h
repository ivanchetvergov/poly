// core/include/RingNumber.h
#pragma once
#include <string>
#include <vector>
#include "FiniteRingRules.h"

/*
 * Представляет многосимвольное число в конечном кольце.
 * Хранит цифры в порядке: младший разряд первым (little-endian).
 * 
 * Пример: число "abc" хранится как ['c', 'b', 'a']
 */
class RingNumber {
public:
    // * конструкторы
    explicit RingNumber(const FiniteRingRules& rules);
    RingNumber(const FiniteRingRules& rules, const std::string& value);
    RingNumber(const FiniteRingRules& rules, const std::vector<char>& digits, bool is_negative = false);
    
    // * копирование и присваивание
    RingNumber(const RingNumber& other);
    RingNumber& operator=(const RingNumber& other);
    
    // * доступ к цифрам (младший разряд = индекс 0)
    size_t length() const { return digits_.size(); }
    char operator[](size_t index) const;
    char& operator[](size_t index);
    char getDigit(size_t index) const; 
    
    // * преобразования
    std::string toString() const;
    std::vector<char> toVector() const { return digits_; }
    
    // * модификация
    void normalize();  // удаляет ведущие нули
    void reverse();    // меняет порядок цифр
    
    // * проверки
    bool isZero() const;
    bool isValid() const;
    bool isNegative() const { return is_negative_; }
    void setNegative(bool value);
    void flipSign();
    RingNumber withoutSign() const;

    // * свойства многочлена
    size_t degree() const;
    char leadingCoefficient() const;
    
    // * операторы сравнения
    bool operator==(const RingNumber& other) const;
    bool operator!=(const RingNumber& other) const;
    
    // * получить правила
    const FiniteRingRules& getRules() const { return rules_; }
    
private:
    const FiniteRingRules& rules_;
    std::vector<char> digits_; 
    bool is_negative_ = false;
    
    void validate(); 
};