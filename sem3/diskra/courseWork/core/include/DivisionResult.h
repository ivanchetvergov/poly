// core/include/DivisionResult.h 
#pragma once

#include "RingNumber.h"
#include <sstream>

// * структура для возврата результата деления
struct DivisionResult {
    RingNumber quotient;  // частное (Q)
    RingNumber remainder; // остаток (R)
    
    // ! конструктор для удобного возврата из divide
    DivisionResult(const RingNumber& q, const RingNumber& r) 
        : quotient(q), remainder(r) {}

    std::string toString() const {
        std::stringstream ss;
        ss << "Q: " << quotient.toString() 
           << " | R: " << remainder.toString();
        return ss.str();
    }
};