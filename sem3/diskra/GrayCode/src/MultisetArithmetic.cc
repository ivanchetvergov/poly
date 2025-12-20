#include "MultisetArithmetic.h"
#include "Exceptions.h"

Multiset MultisetArithmetic::sum(const Multiset& A, const Multiset& B) const {
    Multiset result;

    for (const auto& pair_u : universe_.getElements()) {
        const std::string& code = pair_u.first;
        const int universe_cardinality = pair_u.second;
        
        int a_cardinality = 0;
        if (A.getElements().count(code)) {
            a_cardinality = A.getElements().at(code);
        }

        int b_cardinality = 0;
        if (B.getElements().count(code)) {
            b_cardinality = B.getElements().at(code);
        }
        
        int temp_cardinality = a_cardinality + b_cardinality;

        if (temp_cardinality > universe_cardinality) {
            temp_cardinality = universe_cardinality;
        }

        if (temp_cardinality > 0) {
            result.elements_[code] = temp_cardinality;
            result.totalCardinality_ += temp_cardinality;
        }
    }
    
    return result;
}

Multiset MultisetArithmetic::product(const Multiset& A, const Multiset& B) const {
    Multiset result;

    for (const auto& pair_a : A.getElements()) {
        const std::string& code = pair_a.first;
        const int a_cardinality = pair_a.second;
        
        if (B.getElements().count(code)) {
            const int b_cardinality = B.getElements().at(code);
            int universe_cardinality = universe_.getElements().at(code);
            
            int temp_cardinality = a_cardinality * b_cardinality;

            if (temp_cardinality > universe_cardinality) {
                temp_cardinality = universe_cardinality;
            }
            
            if (temp_cardinality > 0) {
                result.elements_[code] = temp_cardinality;
                result.totalCardinality_ += temp_cardinality;
            }
        }
    }
    
    return result;
}

Multiset MultisetArithmetic::division(const Multiset& A, const Multiset& B) const {
    Multiset result;
    
    for (const auto& pair_a : A.getElements()) {
        const std::string& code = pair_a.first;
        const int a_cardinality = pair_a.second;
        
        if (B.getElements().count(code)) {
            const int b_cardinality = B.getElements().at(code);
            
            if (b_cardinality == 0) {
                throw InvalidOperationException("Деление на 0 для элемента " + code);
            }
            
            int universe_cardinality = universe_.getElements().at(code);
            
            int temp_cardinality = a_cardinality / b_cardinality;
            
            // Ограничиваем кратность универсумом
            if (temp_cardinality > universe_cardinality) {
                temp_cardinality = universe_cardinality;
            }
            
            if (temp_cardinality > 0) {
                result.elements_[code] = temp_cardinality;
                result.totalCardinality_ += temp_cardinality;
            }
        }
    }
    
    return result;
}

Multiset MultisetArithmetic::difference(const Multiset& A, const Multiset& B) const {
    Multiset result = A;

    for (const auto& pair : B.getElements()) {
        const std::string& code = pair.first;
        const int other_cardinality = pair.second;
        
        result.elements_[code] -= other_cardinality;
    }

    result.totalCardinality_ = 0;
    for (const auto& pair : result.elements_) {
        result.totalCardinality_ += pair.second;
    }
    return result;
}