// core/src/RingNumber.cc
#include "RingNumber.h"
#include <stdexcept>
#include <algorithm>
#include <sstream>

using std::string;
using std::vector;
using std::runtime_error;

// создает ноль
// конструктор ноль просто создает ноль без знака
RingNumber::RingNumber(const FiniteRingRules& rules)
    : rules_(rules), is_negative_(false) {
        digits_.push_back(rules_.getZeroElement());
}

// из строки читаем число слева направо потом переворачиваем короче
RingNumber::RingNumber(const FiniteRingRules& rules, const string& value)
    : rules_(rules), is_negative_(false) {
        
    if (value.empty()) {
        throw runtime_error("Cannot create RingNumber from empty string");
    }

    // обработка унарного минуса короче без фокусов
    string new_value = value;
    if (value.length() > 0 && value[0] == '-') {
        new_value = value.substr(1);
        if (new_value.empty()) {
            throw runtime_error("Invalid negative literal");
        }
        is_negative_ = true;
    }

    // копируем в обратном порядке то есть младшие сначала
    digits_.reserve(value.size());
    for (auto it = new_value.rbegin(); it != new_value.rend(); ++it) {
        char c = *it;
        if (!rules_.isValidChar(c)) {
            throw runtime_error("Invalid symbol in RingNumber constructor: " + string(1, c));
        }
        digits_.push_back(c);
    }
    normalize();
}   

// этот конструктор принимает вектор уже в формате младшие сначала
RingNumber::RingNumber(const FiniteRingRules& rules, const vector<char>& value, bool is_negative)
    : rules_(rules), is_negative_(is_negative) {
    if (value.empty()) {
        throw runtime_error("Cannot create RingNumber from empty vector");
    }
    digits_ = value;
    // std::reverse(digits_.begin(), digits_.end());

    normalize();
    validate();
}  

// копирования
RingNumber::RingNumber(const RingNumber& other)
    :rules_(other.rules_), digits_(other.digits_), is_negative_(other.is_negative_) {}

// оператор присваивания
RingNumber& RingNumber::operator=(const RingNumber& other) {
    if (this != &other) {
        if (&rules_ != &other.rules_){
            throw runtime_error("Cannot assign RingNumbers with different rules");
        }
        digits_ = other.digits_;
        is_negative_ = other.is_negative_;
    }
    return *this;
}

// доступ по индексу
char& RingNumber::operator[](size_t index) {
    if (index >= digits_.size()) {
        throw std::out_of_range("RingNumber index out of range");
    }
    return digits_[index];
}

// доступ по индексу
char RingNumber::operator[](size_t index) const {
    if (index >= digits_.size()) {
        throw std::out_of_range("RingNumber index out of range");
    }
    return digits_[index];
}

char RingNumber::getDigit(size_t index) const {
    if (index >= digits_.size()) {
        return rules_.getZeroElement();  // за пределами = ноль
    }
    return digits_[index];
}

// преобразование в строку делаем простую печаль без доп символов
string RingNumber::toString() const {
    if (digits_.empty()){
        return string(1, rules_.getZeroElement());
    }

    string result;
    result.reserve(digits_.size() + 1);

    if (is_negative_) {
        result.push_back('-');
    }

    // записываем в обратном порядке
    for (auto it = digits_.rbegin(); it != digits_.rend(); ++it){
        result.push_back(*it);
    }

    return result;
}

// нормализация удаляем ведущие нули и обрубаем выше 8 разрядов
void RingNumber::normalize() {
    const char zero = rules_.getZeroElement();
    
    // --- 1. Удаление ведущих нулей ---
    while (digits_.size() > 1 && digits_.back() == zero) {
        digits_.pop_back();
    }

    // гарантируем хотя бы одну цифру (ноль)
    if (digits_.empty()) {
        digits_.push_back(zero);
        return; 
    }
    
    // --- 2. Обрезка до MAX_DIGITS ---
    // const size_t PROJECT_MAX_DIGITS = 8; 

    // if (digits_.size() > PROJECT_MAX_DIGITS) {
    //     digits_.resize(PROJECT_MAX_DIGITS);
    //     while (digits_.size() > 1 && digits_.back() == zero) {
    //         digits_.pop_back();
    //     }
    // }

    if (isZero()) {
        is_negative_ = false;
    }
}

// разворот массива цифр если нужно
void RingNumber::reverse() {
    std::reverse(digits_.begin(), digits_.end());
}

// проверка на ноль
bool RingNumber::isZero() const {
    return digits_.size() == 1 && digits_[0] == rules_.getZeroElement();
}

// проверка валидности символов типа все ок или нет
bool RingNumber::isValid() const {
    for (auto c : digits_) {
        if (!rules_.isValidChar(c)) {
            return false;
        }
    }
    return true;
}

// возвращает степень многочлена 
size_t RingNumber::degree() const {
    if (isZero()) {
        return 0; 
    }
    return digits_.size() - 1; 
}

// возвращает старший коэффициент
char RingNumber::leadingCoefficient() const {
    if (isZero()) {
        return rules_.getZeroElement();
    }
    return digits_.back();
}

bool RingNumber::operator==(const RingNumber& other) const {
        if (&rules_ != &other.rules_) {
        return false;
    }
    
    // cравниваем нормализованные версии
    RingNumber a(*this), b(other);
    a.normalize();
    b.normalize();
    
    if (a.is_negative_ != b.is_negative_) {
        return false;
    }
    return a.digits_ == b.digits_;
}

bool RingNumber::operator!=(const RingNumber& other) const {
    return !(*this == other);
}

void RingNumber::validate() {
    for (auto c : digits_) {
        if (!rules_.isValidChar(c)) {
            throw std::runtime_error("RingNumber is in an invalid state: invalid character " + std::string(1, c));
        }
    }
}

void RingNumber::setNegative(bool value) {
    if (isZero()) {
        is_negative_ = false;
        return;
    }
    is_negative_ = value;
}

void RingNumber::flipSign() {
    setNegative(!is_negative_);
}

RingNumber RingNumber::withoutSign() const {
    RingNumber copy(*this);
    copy.is_negative_ = false;
    return copy;
}
