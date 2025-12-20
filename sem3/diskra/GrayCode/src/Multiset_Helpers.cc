// Multiset_Helpers.cc
#include "Multiset.h"
#include <iostream>

void Multiset::print() const {
    std::cout << "элементы мультимножества:" << std::endl;


    if (this->isEmpty()) {
        std::cout << "(пустое множество)" << std::endl;
    } else {
        for (const auto& pair : this->elements_) {
            std::cout << "  " << pair.first << ": " << pair.second << std::endl;
        }
    }

    std::cout << "общая мощность: " << this->totalCardinality_ << std::endl;
}

bool Multiset::isEmpty() const {
    return elements_.empty();
}