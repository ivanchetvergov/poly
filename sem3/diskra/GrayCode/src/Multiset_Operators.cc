// Multiset_Operators.cc
#include "Multiset.h"
#include "Exceptions.h"
#include <algorithm>

// (базовый метод) A or B (max)
Multiset Multiset::unionWith(const Multiset& other) const {
    Multiset result;

    // собираем объединение всех ключей
    for (const auto& [code, a_card] : this->elements_) {
        int b_card = 0;
        if (other.elements_.count(code)) {
            b_card = other.elements_.at(code);
        }
        int res_card = std::max(a_card, b_card);
        if (res_card > 0) {
            result.elements_[code] = res_card;
            result.totalCardinality_ += res_card;
        }
    }

    // добавляем ключи, которых не было в this
    for (const auto& [code, b_card] : other.elements_) {
        if (!this->elements_.count(code)) {
            if (b_card > 0) {
                result.elements_[code] = b_card;
                result.totalCardinality_ += b_card;
            }
        }
    }

    return result;
}


// (базовый метод) A \ B (Правильная разность мультимножеств)
// (A∖B)(x)=max(0,A(x)−B(x)) в мультим-ах
// TODO: A and not B
Multiset Multiset::difference(const Multiset& other) const {
    Multiset result = *this; // cоздаем копию A
    for (const auto& pair : other.elements_) { // перебираем B
        const std::string& code = pair.first;
        const int other_cardinality = pair.second;
        if (result.elements_.count(code)) { // если этот эл есть и в A
            int current_cardinality = result.elements_[code];
            int new_cardinality = current_cardinality - other_cardinality;
            if (new_cardinality > 0) { // если разность > 0
                result.elements_[code] = new_cardinality;
            } else { // если меньше 0
                result.elements_.erase(code); 
            }
        }
    }
    result.totalCardinality_ = 0;
    for (const auto& pair : result.elements_) {
        result.totalCardinality_ += pair.second;
    }
    return result;
}

// пересечение: A and B = A \ (A \ B)
Multiset Multiset::intersectWith(const Multiset& other) const {
    Multiset a_minus_b = this->difference(other);
    return this->difference(a_minus_b);
}

// сим разность: (A \ B) or (B \ A)
Multiset Multiset::symmetricDifference(const Multiset& other) const {
    Multiset a_minus_b = this->difference(other);
    Multiset b_minus_a = other.difference(*this);
    return a_minus_b.unionWith(b_minus_a);
}

// дополнение: U \ A
Multiset Multiset::complement(const Multiset& universe) const {
    return universe.difference(*this);
}

// -- (обертки) ---

Multiset Multiset::operator|(const Multiset& other) const {
    return this->unionWith(other);
}

Multiset Multiset::operator&(const Multiset& other) const {
    return this->intersectWith(other);
}

Multiset Multiset::operator-(const Multiset& other) const {
    return this->difference(other);
}

Multiset Multiset::operator^(const Multiset& other) const {
    return this->symmetricDifference(other);
}



