// Shape.cc
#include "../include/Shape.h"
#include <iostream>

int Shape::Count = 0;
Shape* Shape::head_ = nullptr;
Shape* Shape::tail_ = nullptr;

Shape::Shape() : next_shape(nullptr) {
    if (head_ == nullptr) head_ = this;
    else tail_->next_shape = this;
    tail_ = this;
    Count++;
    // std::cout << "вызван Shape()" << std::endl;

}

Shape::~Shape() {
    if (head_ == this && tail_ == this) { 
        head_ = nullptr;
        tail_ = nullptr;
    } else if (head_ == this) { // 
        head_ = next_shape;
    } else { 
        Shape* current = head_;
        while (current && current->next_shape != this) {
            current = current->next_shape;
        }
        if (current) {
            current->next_shape = next_shape;
            if (this == tail_) {
                tail_ = current;
            }
        }
    }
    // std::cout << "вызван ~Shape" << std::endl;
    Count--;
}

void Shape::PrintCount() {
    std::cout << "\nNow there are " << Count << " shapes.";
}

void Shape::PrintShapes() {
    Shape* current = head_;
    while (current) {
        current->Out();
        current = current->next_shape;
    }
}