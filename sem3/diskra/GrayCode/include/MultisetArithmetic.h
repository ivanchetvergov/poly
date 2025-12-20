// MultisetArithmetic
#pragma once

#include "Multiset.h"

class MultisetArithmetic {

public:
    MultisetArithmetic(const Multiset& universe) : universe_(universe) {}

    Multiset sum(const Multiset& A, const Multiset& B) const;
    Multiset product(const Multiset& A, const Multiset& B) const;
    Multiset division(const Multiset& A, const Multiset& B) const;
    Multiset difference(const Multiset& A, const Multiset& B) const;

private:
    const Multiset& universe_;
};